/*
 * Filename:		Marbler.cpp
 *
 * Classification:	
 * Classified By:	
 *
 * Tool Name:		Marbler
 * Requirement #:	2015-XXXX
 *
 * Author:			???
 * Date Created:	01/15/2015
 * Version 1.0:		01/15/2015 (???)
 *
 * This will implement the actual string scrambling, copy originals and replace
 * code.
 *
 * Arguments: Root path of solution (looks through files below the root to modify strings)
 *
 *
 */
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <Shlwapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <WinCrypt.h>
#include <ObjBase.h>
#include <MsXml.h>
#include <atlstr.h>
#include <comutil.h>
#include <vector>
#include <iterator>
#import <msxml6.dll>

using namespace std;

// User-defined Header Files:
#include "IScramble.h"
#include "Mibster.h"
#include "Misc\FileTyping\MISCTextFileTyper_ENC.h"
#include "Misc\MemoryScan\MISCMemorySearch_NSS.h"

#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "misc.lib")
#pragma comment (lib, "shlwapi.lib")

static enum StringType : unsigned int
{
	stWCHAR = 1,	//String is a WCHAR
	stCHAR = 2		//String is a CHAR
};

//Linked List of Scramble Nodes
//typedef struct _MARBLER_NODE
//{
//	WCHAR		*wcOriginalFilePath;	//Make copy and edit at original path
//	ULONGLONG	ullInsertIndex;			//Index of inserted code in the file
//	ULONGLONG	ullLength;				//Length of entry
//	WCHAR		*wcFileTempCopyPath;	//Temporary copy path
//	WCHAR		*wcVariableName;		//The name of the variable being used
//	CHAR		*stStringToScramble;	//The string that needs to be scrambled (could be WCHAR or CHAR)
//	StringType  eStringType;			//The type of string (WCHAR or CHAR)
//
//	_MARBLER_NODE *pNextNode;			//Next node in the linked list
//
//}MARBLER_NODE, *PMARBLER_NODE, MARBLER_LIST, *PMARBLER_LIST;

typedef struct _MARBLER_NODE
{
	WCHAR		*wcFilePath;			//Make copy and edit at original path

	INT64		iLineNumber;			//The line number of the string being obfuscated
	INT64		iStartIndex;			//Index of inserted code in the file
	INT64		iLength;				//Length of entry
	
	StringType  eStringType;			//The type of string (WCHAR or CHAR)
	CHAR		*cString;				//String to be scrambled
	INT64		iBinLength;				//Length of the actual binary string

	_MARBLER_NODE *pNextNode;			//Next node in the linked list

}MARBLER_NODE, *PMARBLER_NODE, MARBLER_LIST, *PMARBLER_LIST;

typedef struct _OPTIONS_LIST
{
	WCHAR *wcName;			//The name of the choice
	ULONGLONG ullIndex;		//Index of the line
	DWORD dwLineLength;		//Length of the line
	
	_OPTIONS_LIST *pNext;	//Next node in the options list
}OPTION_NODE, *POPTION_NODE, OPTION_LIST, *POPTION_LIST;

BOOL g_bModificationError = FALSE;
IScramble *g_pScram = NULL;
WCHAR *g_wcRootDir = NULL;
WCHAR *g_wcOutDir = NULL;
#define MAX_CONSECUTIVE 3

BOOL GenerateModifiedFiles(WCHAR *wcRoot, PMARBLER_LIST &pMarblerList);

BOOL ProcessFile(WCHAR *wcPath, PMARBLER_LIST &pMarblerList);
BOOL ParseLine(StringType stType, char *cStart, INT64 iLength, LPBYTE &lpbOutput, INT32 &iOutputLen, char *&cVarName, INT32 &iVarNameLen);
BOOL GenerateLineNumberList(WCHAR *wcFilePath, DWORD *&dwLineStartList, DWORD &dwNumLines);
BOOL VerifyScramRatio(StringType stType, LPBYTE lpbString1, LPBYTE lpbString2, DWORD dwStringLen);

BOOL ChooseMarble(WCHAR *&wcChoice);
BOOL SetScrambler(WCHAR *&wcChoice);

BOOL ConvertQuoteToBytes(WCHAR *wcStringToConvert, int iStringToConvertLen, LPBYTE &lpbBytes, INT32 &iBytesLen);
BOOL ConvertQuoteToBytes(CHAR *cStringToConvert, int iStringToConvertLen, LPBYTE &lpbBytes, INT32 &iBytesLen);
BOOL ConvertBraceToBytesW(CHAR *cStringToConvert, int iStringToConvertLen, LPBYTE &lpbBytes, INT32 &iBytesLen);
BOOL ConvertBraceToBytes(CHAR *cStringToConvert, int iStringToConvertLen, LPBYTE &lpbBytes, INT32 &iBytesLen);

BOOL GenerateReceipt(WCHAR *wcChoice, PMARBLER_LIST pMarblerList);


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpCmdLine*/, int nCmdShow)
{
	WCHAR wcUsage[] = L"\nUsage: \n\tRoot Path to Project or Solution\n"
		L"\tPath to directory containing Marble.h (most often Shared) used in Project/Solution\n"
		L"\tPath to directory where the generated receipt is placed\n\n";

	INT32 iRet = -1;

	// Attach to the parent console for output.
	/*if (AttachConsole(ATTACH_PARENT_PROCESS))
	{
		// Redirect stdout to the attached console
		FILE* stream;
		_wfreopen_s(&stream, L"CONOUT$", L"w", stdout);
	}*/

	//Validate Args
	if (__argc < 4)
	{
		wprintf(wcUsage);
		return iRet;
	}
	
	g_wcRootDir = __wargv[1];
	wprintf(L"\n\nObfuscating contents of %s\n", g_wcRootDir);
	
	SetCurrentDirectory(__wargv[2]);
	WCHAR wcDir[MAX_PATH + 2] = { 0 };
	GetCurrentDirectory(MAX_PATH + 2, wcDir);
	wprintf(L"Marble.h directory is: %s\n", wcDir);

	g_wcOutDir = __wargv[3];
	wprintf(L"Output Directory: %s\n", g_wcOutDir);

	wprintf(L"\n\nStarting Marbler....\n");


	//Choose from list of available techniques by searching include file list (Marble.h)
	
	PMARBLER_LIST pMarbleList = NULL;
	WCHAR *wcChoice = NULL;
	BOOL bFoundMarble = ChooseMarble(wcChoice);
	if (!bFoundMarble || wcChoice == NULL)
	{
		wprintf(L"Failed to pick Obfuscation Method from list\n");
		goto cleanup;
	}

	//------------------------------------------------------------------------------------



	//Set Scrambler to the right function

	BOOL bScramSet = SetScrambler(wcChoice);
	if (!bScramSet || !g_pScram)
	{
		wprintf(L"Failed to find Obfuscation Method by name\n");
		goto cleanup;
	}

	//---------------------------------------------------------------------------------------


	//Generate linked list of modifications for WARBLE, CARBLE, and BARBLE, cache files ---- ALL FAILURES FROM THIS POINT ON RESULT IN A RESET OF FILES

	GenerateModifiedFiles(g_wcRootDir, pMarbleList);
	if (g_bModificationError)
	{
		wprintf(L"Error in modifying files...Resetting to original files\n");
		goto reset;
	}
	else if (pMarbleList == NULL)
	{
		iRet = 0;
		wprintf(L"Failed to find data to obfuscate\n");

		//Restore Marble to original state...
		goto nostrings;
	}

	//----------------------------------------------------------------------------------------


	//Generate Receipt

	GenerateReceipt(wcChoice, pMarbleList);

	//-----------------------------------------------------------------------------------------

	iRet = 0;
	if (!g_bModificationError) //If no errors, free linked list and return 1
		goto cleanup;

reset:
	iRet = -1;
	//Call post-build to clean up mistakes
	wprintf(L"Failure In Obfuscating Strings, Restoring Files");

nostrings:
	{
		iRet = 0;
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		SecureZeroMemory(&si, sizeof(si));
		SecureZeroMemory(&pi, sizeof(pi));
		si.cb = sizeof(si);
		WCHAR wcApp[MAX_PATH] = {0};
		swprintf(wcApp, L"Mender.exe \"%s\"", __wargv[1]);

		if (CreateProcess(NULL, wcApp, NULL, NULL, FALSE, CREATE_UNICODE_ENVIRONMENT|CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
		{
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
		else
		{
			wprintf(L"Failed to run Mender Application");
			//MessageBox(NULL, L"Error Running Mender Application", L"INFO", MB_OK);
		}
	}

cleanup:
	//Free linked list of Marbler Nodes
	PMARBLER_NODE pCurrentNode = pMarbleList;
	while (pCurrentNode != NULL)
	{
		PMARBLER_NODE pTempNode = pCurrentNode;
		pCurrentNode = pCurrentNode->pNextNode;
		

		if (pTempNode->wcFilePath)
			free(pTempNode->wcFilePath);
		if (pTempNode->cString)
			free(pTempNode->cString);

		free(pTempNode);
	}

	//Delete Scrambler Object
	if(g_pScram)
		delete g_pScram;

	//Free Choice
	if (wcChoice)
		free(wcChoice);

	wprintf(L"\n\n");

	return iRet;
}

BOOL GenerateModifiedFiles(WCHAR *pszRoot, PMARBLER_LIST &pMarblerList)
{
	if (pszRoot == NULL)
		return FALSE;

	WIN32_FIND_DATA FindFileData;			//Our FindFirst FindNext file data
	HANDLE hFind = INVALID_HANDLE_VALUE;	//Handle to the found file.

	DWORD dwRootLength = lstrlenW(pszRoot);
	BOOL bHasBackSlash = FALSE;

	if (pszRoot[dwRootLength - 1] == '\\')
		bHasBackSlash = TRUE;

	LPWSTR pszSearchPath;
	pszSearchPath = (LPWSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (dwRootLength + 10) * sizeof(WCHAR));
	if (pszSearchPath == NULL)
	{
		return FALSE; //Failed to allocate...
	}

	LPWSTR pszFullPath;
	pszFullPath = (LPWSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (dwRootLength + (MAX_PATH * 2) + 2) * sizeof(WCHAR));
	if (pszFullPath == NULL)
	{
		HeapFree(GetProcessHeap(), 0, pszSearchPath);
		return FALSE; //Failed to allocate...
	}

	if (bHasBackSlash)
		wsprintfW(pszSearchPath, L"%s*.*", pszRoot);	//Copy our path over and append *.*
	else
		wsprintfW(pszSearchPath, L"%s\\*.*", pszRoot);	//Copy our path over and append *.*

	if ((hFind = FindFirstFileW(pszSearchPath, &FindFileData)) != INVALID_HANDLE_VALUE)	//Find the first file.
	{
		do	//Go until there are no more.
		{
			if (lstrcmp(L".", FindFileData.cFileName) == 0 || lstrcmp(L"..", FindFileData.cFileName) == 0 || (lstrcmp(L"Marble", FindFileData.cFileName) == 0 && FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))	//The dot and dotdot folders, ignore these.
				continue;
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)			//If we are a directory.
			{
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
				{
					continue;
				}
				else
				{//This is a folder we want to look through
					if (bHasBackSlash)
						wsprintf(pszFullPath, L"%s%s", pszRoot, FindFileData.cFileName);
					else
						wsprintf(pszFullPath, L"%s\\%s", pszRoot, FindFileData.cFileName);
					GenerateModifiedFiles(pszFullPath, pMarblerList);
				}
			}
			else
			{
				if (bHasBackSlash)
					wsprintf(pszFullPath, L"%s%s", pszRoot, FindFileData.cFileName);
				else
					wsprintf(pszFullPath, L"%s\\%s", pszRoot, FindFileData.cFileName);
				
				//Process File
				if (PathMatchSpec(pszFullPath, L"*.c") || PathMatchSpec(pszFullPath, L"*.cpp") || PathMatchSpec(pszFullPath, L"*.h"))
				{
					if (!PathMatchSpec(FindFileData.cFileName, L"Marble.*"))
					{				
						BOOL bProcessed = ProcessFile(pszFullPath, pMarblerList);

						//Global Flag for error
						if (!bProcessed)
						{
							g_bModificationError = TRUE;
							wprintf(L"Error modifying file\n");
						}				
					}
				}

			}
		} while (FindNextFile(hFind, &FindFileData) && !g_bModificationError);	//Keep going until we are out of files, passed the threshold, or stop mutex is present
	}
	if (hFind != INVALID_HANDLE_VALUE) FindClose(hFind);	//We found everything, close the handle
	HeapFree(GetProcessHeap(), 0, pszSearchPath);
	HeapFree(GetProcessHeap(), 0, pszFullPath);

	return TRUE;
}

BOOL ProcessFile(WCHAR *wcPath, PMARBLER_LIST &pMarblerList)
{
	if (wcPath == NULL)
		return FALSE;

	BOOL bRet = FALSE;

	//Figure out encoding
	MISCTextFileTyper_ENC mTyper;
	TextEncoding teEncoding = mTyper.GetTextEncoding(wcPath);
	if (teEncoding == UnicodeBigEndian || teEncoding == UnkownEncoding) // No support for Big Endian
	{
		wprintf(L"Unsupported file encoding\n");
		return FALSE;
	}

	//Read in entire file
	HANDLE hSrcFile = CreateFile(wcPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSrcFile == INVALID_HANDLE_VALUE)
	{
		wprintf(L"Failed to open file for read\n");
		return FALSE;
	}

	DWORD dwRWBytes = 0;
	DWORD dwFileSize = GetFileSize(hSrcFile, NULL);
	LPBYTE lpbFile = NULL;
	if (teEncoding == ANSI) //Convert to UTF-8 if not
	{
		lpbFile = (LPBYTE)calloc(1, dwFileSize + 3);
		memcpy(lpbFile, "\xEF\xBB\xBF", 3);
		ReadFile(hSrcFile, lpbFile + 3, dwFileSize, &dwRWBytes, NULL);
		dwFileSize += 3;
	}
	else
	{
		lpbFile = (LPBYTE)calloc(1, dwFileSize);
		ReadFile(hSrcFile, lpbFile, dwFileSize, &dwRWBytes, NULL);
	}

	CloseHandle(hSrcFile);

	//If Unicode convert to UTF-8
	if (teEncoding == Unicode)
	{
		LPBYTE lpbTemp = lpbFile;
		DWORD dwTempLen = dwFileSize;
		
		dwFileSize = WideCharToMultiByte(CP_UTF8, 0, (WCHAR *)(lpbTemp + 2), (dwTempLen/2) - 1, NULL, 0, 0, 0); // - 2 for Unicode BOM
		dwFileSize += 3; //+ 3 for UTF8 BOM

		lpbFile = (LPBYTE)calloc(1, dwFileSize);
		memcpy(lpbFile, "\xEF\xBB\xBF", 3);			//Prepend UTF-8 Encoding
		WideCharToMultiByte(CP_UTF8, 0, (WCHAR *)(lpbTemp + 2), (dwTempLen/2) - 1, (CHAR *)lpbFile + 3, dwFileSize - 3, 0, 0); // - 3 for Unicode BOM
		free(lpbTemp);
	}

	//Scan file in loop and build nodes
	PMARBLER_LIST pList = NULL;
	INT64 iCurrentIndex = 0;
	BOOL bError = FALSE;
	INT64 iPosit = MISCMemorySearch_NSS::FindIndexOfSequenceInMemory((CHAR *)lpbFile, dwFileSize, "ARBLE", strlen("ARBLE"));
	while (iPosit >= 0 && !bError)
	{
		//Create Node containing info about string to scramble
		PMARBLER_NODE pNode = (PMARBLER_NODE)calloc(sizeof(MARBLER_NODE), 1);
		pNode->iStartIndex = iCurrentIndex + iPosit - 1;

		//Make copy of file name - we'll want to keep a gold copy before modifying anything
		pNode->wcFilePath = (WCHAR *) calloc(sizeof(WCHAR), wcslen(wcPath) + 1);
		memcpy(pNode->wcFilePath, wcPath, wcslen(wcPath) * sizeof(WCHAR));

		//Get type of string (WCHAR or CHAR)
		if (lpbFile[pNode->iStartIndex] == 'C')
			pNode->eStringType = stCHAR;
		else if (lpbFile[pNode->iStartIndex] == 'W')
			pNode->eStringType = stWCHAR;
		else bError = TRUE;
	
		//Determine length of statement
		INT64 iStart = pNode->iStartIndex;
		BOOL bFoundEnd = FALSE;
		BOOL bInQuotes = FALSE;
		while( iStart < dwFileSize && !bFoundEnd)
		{
			if (lpbFile[iStart] == '\"')
			{
				if (iStart > 0)
				{
					//Go backwards until not a backslash counting backslashes (if odd then we're escaped - even we're not)
					INT64 iTempCount = iStart - 1;
					BOOL bIsBackSlash = TRUE;
					DWORD dwBackCount = 0;
					while (iTempCount > pNode->iStartIndex && bIsBackSlash)
					{
						if (lpbFile[iTempCount] != '\\')
							bIsBackSlash = FALSE;
						else
						{
							iTempCount--;
							dwBackCount++;
						}
					}

					DWORD dwIsEscaped = dwBackCount % 2;
					if (dwIsEscaped == 0)
						bInQuotes ^= 0x1;
					
				}	
			}

			if (lpbFile[iStart] == ';' && !bInQuotes)
				bFoundEnd = TRUE;
			iStart++;
		}

		if (!bFoundEnd)
			bError = TRUE;
		else
		{
			pNode->iLength = iStart - pNode->iStartIndex;
		}
		
		if (!bError)
		{
			iCurrentIndex = pNode->iStartIndex + pNode->iLength;
			pNode->pNextNode = pList;
			pList = pNode;
		}
		else
		{
			 //Free node
			if (pNode->wcFilePath)
				free(pNode->wcFilePath);
			free(pNode);
		}

		iPosit = MISCMemorySearch_NSS::FindIndexOfSequenceInMemory((CHAR *)lpbFile + iCurrentIndex, dwFileSize - iCurrentIndex, "ARBLE", strlen("ARBLE"));
	}

	if (bError)
	{
		//Add to list
		pMarblerList = pList;
		bRet = FALSE;
		goto freelist;
	}
	if (pList == NULL)
	{
		bRet = TRUE;
		goto cleanup;
	}

	wprintf(L"Processing File ");
	wprintf(wcPath);
	wprintf(L"\n");

	//Copy off original
	WCHAR *wcGoldPath = (WCHAR *)calloc(sizeof(WCHAR), wcslen(wcPath) + wcslen(L".marble") + 1);
	swprintf(wcGoldPath, L"%s.marble", wcPath);
	BOOL bCopySuccess = CopyFile(wcPath, wcGoldPath, TRUE);
	DWORD dwAttribs = GetFileAttributes(wcGoldPath);
	free(wcGoldPath);

	if (!bCopySuccess || dwAttribs == INVALID_FILE_ATTRIBUTES)
	{
		//Add to list
		pMarblerList = pList;
		bRet = FALSE;
		goto freelist;
	}

	//Build Line Numbers List
	DWORD *dwLineStartList = NULL;
	DWORD dwNumLines = 0;
	GenerateLineNumberList(wcPath, dwLineStartList, dwNumLines);

	//Loop through, get string, scramble and write file
	PMARBLER_NODE pNode = pList;
	while (pNode != NULL)
	{
		//Translate string to byte array - get variable name
		LPBYTE lpbLine = NULL;
		INT32 iLineLen = 0;
		CHAR *cVarName = NULL;
		INT32 iVarNameLen = 0;
		BOOL bParsed = ParseLine(pNode->eStringType, (CHAR *)lpbFile + pNode->iStartIndex, pNode->iLength, lpbLine, iLineLen, cVarName, iVarNameLen);

		if (!bParsed || cVarName == NULL || iVarNameLen <= 0 || lpbLine == NULL || iLineLen <= 0) //If there is an error in parsing, go ahead and bail
		{
			//Cleanup
			if (cVarName)
				free(cVarName);
			if (lpbLine)
				free(lpbLine);

			//Add to list
			pMarblerList = pList;
			bRet = FALSE;
			wprintf(L"Failure parsing string at offset %d in %s!!\n", pNode->iStartIndex, wcPath);
			goto freelist;
		}

		//Determine Line Number
		if (dwLineStartList != NULL)
		{
			BOOL bFoundLineNum = FALSE;
			DWORD dwIndex = 0;
			while (dwIndex < dwNumLines && !bFoundLineNum)
			{
				if (pNode->iStartIndex == dwLineStartList[dwIndex])
				{
					pNode->iLineNumber = dwIndex + 1;
					bFoundLineNum = TRUE;
				}
				else if (pNode->iStartIndex < dwLineStartList[dwIndex])
				{
					pNode->iLineNumber = dwIndex;
					bFoundLineNum = TRUE;
				}

				dwIndex++;
			}
		}

		//Print out line numbers
		{
			WCHAR wcLineNum[MAX_PATH] = { 0 };
			swprintf(wcLineNum, L"Scrambling Line %d\n", pNode->iLineNumber);
			wprintf(wcLineNum);
		}

		//Make copies, store original hex for review
		DWORD dwCharSize = sizeof(CHAR);
		if (pNode->eStringType == stWCHAR)
			dwCharSize = sizeof(WCHAR);

		pNode->cString = (CHAR *)calloc(dwCharSize, iLineLen);
		memcpy(pNode->cString, lpbLine, iLineLen * dwCharSize);
		pNode->iBinLength = iLineLen * dwCharSize;

		//Scramble String, Create Literal, Generate Insert
		CHAR *cLiteral = NULL;
		CHAR *cInsert = NULL;
		BOOL bModError = FALSE;
		//TODO: Error reporting
		if (pNode->eStringType == stCHAR)
		{
			int iResult = g_pScram->ScrambleA((CHAR *)lpbLine, iLineLen);
			if (iResult > 0)
			{
				if (VerifyScramRatio(pNode->eStringType, (LPBYTE)pNode->cString, lpbLine, iLineLen))
				{
					iResult = g_pScram->CreateStringLiteralA(lpbLine, iLineLen, cLiteral);
					if (iResult > 0)
					{
						iResult = g_pScram->GenerateInsertA(cVarName, cLiteral, iLineLen, cInsert);
						if (iResult <= 0)
							bModError = TRUE;
					}
					else
						bModError = TRUE;
				}
				else bModError = TRUE;
			}
			else
				bModError = TRUE;
		}
		else
		{
			int iResult = g_pScram->ScrambleW((WCHAR *)lpbLine, iLineLen);
			if (iResult > 0)
			{
				if (VerifyScramRatio(pNode->eStringType, (LPBYTE)pNode->cString, lpbLine, iLineLen))
				{
					iResult = g_pScram->CreateStringLiteralW((WCHAR *)lpbLine, iLineLen, cLiteral);
					if (iResult > 0)
					{
						g_pScram->GenerateInsertW(cVarName, cLiteral, iLineLen, cInsert);
						if (iResult <= 0)
							bModError = TRUE;
					}
					else
						bModError = TRUE;
				}
				else
					bModError = TRUE;
			}
			else
				bModError = TRUE;
		}

		//Done with literal, line and variable
		if (cVarName)
			free(cVarName);
		if (lpbLine)
			free(lpbLine);
		if (cLiteral)
			free(cLiteral);


		if (bModError || cInsert == NULL)
		{
			//Free and release
			wprintf(L"Failed to obfuscate line %d\n", pNode->iLineNumber);
			if (cInsert)
				free(cInsert);

			//Add to list
			pMarblerList = pList;
			bRet = FALSE;
			goto freelist;
		}

		//Write File With Modifications
		LPBYTE lpbTemp = lpbFile;
		DWORD dwTempLen = dwFileSize;

		//New Length of file
		dwFileSize = dwTempLen + strlen(cInsert) - pNode->iLength;
		lpbFile = (LPBYTE)calloc(sizeof(CHAR), dwFileSize);
		
		memcpy(lpbFile, lpbTemp, pNode->iStartIndex);
		memcpy(lpbFile + pNode->iStartIndex, cInsert, strlen(cInsert));
		memcpy(lpbFile + pNode->iStartIndex + strlen(cInsert), lpbTemp + pNode->iStartIndex + pNode->iLength, dwTempLen - pNode->iStartIndex - pNode->iLength);

		free(cInsert);
		free(lpbTemp);
		lpbTemp = NULL;
		dwTempLen = 0;

		if (pNode->pNextNode == NULL)
		{
			pNode->pNextNode = pMarblerList;	//Set Last Node To Head Of Marbler List
			pNode = NULL;
		}
		else pNode = pNode->pNextNode;
	}

	pMarblerList = pList;
	if (dwLineStartList)
		free(dwLineStartList);

	//Flush File if no errors
	hSrcFile = CreateFile(wcPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSrcFile == INVALID_HANDLE_VALUE)
	{
		bRet = FALSE;
		goto freelist;
	}
	
	WriteFile(hSrcFile, lpbFile, dwFileSize, &dwRWBytes, NULL);
	SetEndOfFile(hSrcFile);
	CloseHandle(hSrcFile);
	bRet = TRUE;
	wprintf(L"Successfully modified\n");
	goto cleanup;

freelist:
	{
		//Freeing list of nodes
		wprintf(L"Freeing list of nodes\n");
		PMARBLER_NODE pCurrent = (PMARBLER_NODE)pMarblerList;
		while (pCurrent != NULL)
		{
			PMARBLER_NODE pTemp = (PMARBLER_NODE)pCurrent;
			pCurrent = pCurrent->pNextNode;

			if (pTemp->wcFilePath)
				free(pTemp->wcFilePath);
			if (pTemp->cString)
				free(pTemp->cString);
			free(pTemp);
		}
		pMarblerList = NULL;
	}

cleanup:
	free(lpbFile);
	return bRet;
}

BOOL ParseLine(StringType stType, char *cStart, INT64 iLength, LPBYTE &lpbOutput, INT32 &iOutputLen, char *&cVarName, INT32 &iVarNameLen)
{
	//Validate Args
	if (cStart == NULL || iLength <= 0)
		return FALSE;

	lpbOutput = NULL;
	iOutputLen = 0;
	cVarName = NULL;
	iVarNameLen = 0;

	//Get Variable Name
	BOOL bFoundVarName = FALSE;
	DWORD dwVarStart = 0;
	DWORD dwVarEnd = 0;
	DWORD dwIndex = strlen("CARBLE");
	while (dwIndex < iLength && !bFoundVarName)
	{
		if (dwVarStart == 0 && cStart[dwIndex] != 0x20)
			dwVarStart = dwIndex;
		if (dwVarEnd <= dwVarStart && cStart[dwIndex] == '[') //If its not the right format, error out
		{
			dwVarEnd = dwIndex;
			bFoundVarName = TRUE;
		}

		dwIndex++;
	}

	if (dwVarEnd <= dwVarStart || !bFoundVarName)
		return FALSE;

	iVarNameLen = dwVarEnd - dwVarStart;
	cVarName = (CHAR *)calloc(sizeof(CHAR), iVarNameLen + 1);
	memcpy(cVarName, cStart + dwVarStart, iVarNameLen);

	// Parse string to get bytes - handle char cstring[] = "\x22\x33"; and char cstring[] = { 0x22, 0x33 };
	BOOL bFoundString = FALSE;
	BOOL bFoundEquals = FALSE;
	DWORD dwStringStart = 0;
	DWORD dwStringEnd = 0;
	BOOL isQuotedString = TRUE;
	while (dwIndex < iLength && !bFoundString)
	{
		if (!bFoundEquals && cStart[dwIndex] == '=')
			bFoundEquals = TRUE;

		if (bFoundEquals && dwStringStart == 0 && (cStart[dwIndex] == '\"' || cStart[dwIndex] == '{'))
		{
				dwStringStart = dwIndex + 1;
				if (cStart[dwIndex] == '{')
					isQuotedString = FALSE;
		}
		else if (bFoundEquals && dwStringStart != 0 && (cStart[dwIndex] == '\"' || cStart[dwIndex] == '}'))
		{
			bFoundString = TRUE;
			if (isQuotedString && cStart[dwIndex] == '\"')
			{
				//Go backwards until not a backslash counting backslashes (if odd then we're escaped - even we're not)
				DWORD dwCount = dwIndex - 1;
				BOOL bIsBackSlash = TRUE;
				DWORD dwBackCount = 0;
				while (dwCount >= dwStringStart && bIsBackSlash)
				{
					if (cStart[dwCount] != '\\')
						bIsBackSlash = FALSE;
					else
					{
						dwCount--;
						dwBackCount++;
					}
				}

				DWORD dwIsEscaped = dwBackCount % 2;
				if (dwIsEscaped == 0)
					dwStringEnd = dwIndex;
				else				
					bFoundString = FALSE;				
			}
			else if (!isQuotedString && cStart[dwIndex] == '}')
			{
				dwStringEnd = dwIndex;
			} 
			else
			{
				bFoundString = FALSE;
			}
		}

		dwIndex++;
	}

	if (!bFoundString || dwStringStart >= dwStringEnd)
	{
		//Cleanup and return False
		free(cVarName);
		cVarName = NULL;
		return FALSE;
	}

	//Convert to bytes - WCHAR vs CHAR
	BOOL bConverted = FALSE;
	if (isQuotedString)
	{
		if (stType == stCHAR) //If WCHAR convert to WCHAR from UTF-8 and pass along, convert returns back to UTF-8
			bConverted = ConvertQuoteToBytes(cStart + dwStringStart, dwStringEnd - dwStringStart, lpbOutput, iOutputLen);
		else
		{
			WCHAR *wcWideString = NULL;
			DWORD dwSizeNeeded = MultiByteToWideChar(CP_UTF8, 0, cStart + dwStringStart, dwStringEnd - dwStringStart, NULL, 0);
			wcWideString = (WCHAR *)calloc(sizeof(WCHAR), dwSizeNeeded);
			MultiByteToWideChar(CP_UTF8, 0, cStart + dwStringStart, dwStringEnd - dwStringStart, wcWideString, dwSizeNeeded);
			bConverted = ConvertQuoteToBytes(wcWideString, dwSizeNeeded, lpbOutput, iOutputLen);
			free(wcWideString);
		}
	}
	else
	{
		if (stType == stCHAR)
			bConverted = ConvertBraceToBytes(cStart + dwStringStart, dwStringEnd - dwStringStart, lpbOutput, iOutputLen);
		else
			bConverted = ConvertBraceToBytesW(cStart + dwStringStart, dwStringEnd - dwStringStart, lpbOutput, iOutputLen);
	}

	if (!bConverted)
	{
		//cleanup return false
		free(cVarName);
		cVarName = NULL;

		free(lpbOutput);
		lpbOutput = NULL;
	}

	return TRUE;
}

BOOL GenerateLineNumberList(WCHAR *wcFilePath, DWORD *&dwLineStartList, DWORD &dwNumLines)
{
	if (wcFilePath == NULL)
		return FALSE;

	dwLineStartList = NULL;
	dwNumLines = 0;
	string line;
	
	ifstream fsParseFile(wcFilePath, ifstream::in);
	if (fsParseFile.is_open())
	{
		//Roll through once counting how many lines there are
		while (getline(fsParseFile, line))
			dwNumLines++;

		//Alloc necessary mem
		dwLineStartList = (DWORD *)calloc(sizeof(DWORD), dwNumLines);

		//Roll through and set start indexes
		fsParseFile.clear();
		fsParseFile.seekg(0);
		DWORD dwIndex = 1;
		while (getline(fsParseFile, line) && dwIndex < dwNumLines)
		{
			dwLineStartList[dwIndex] = fsParseFile.tellg();
			dwIndex++;
		}

		fsParseFile.close();
	}

	return TRUE;
}

BOOL VerifyScramRatio(StringType stType, LPBYTE lpbString1, LPBYTE lpbString2, DWORD dwStringLen)
{
	INT32 iNumSame = 0;
	INT32 iConsecutive = 0;
	for (int i = 0; i < dwStringLen; i++)
	{
		if (stType == stCHAR)
		{
			if ((CHAR *)lpbString1[i] == (CHAR *)lpbString2[i])
			{
				iNumSame++;
				iConsecutive++;
			}
			else iConsecutive = 0;
		}
		else
		{
			if ((WCHAR *)lpbString1[i] == (WCHAR *)lpbString2[i])
			{
				iNumSame++;
				iConsecutive++;
			}
			else iConsecutive = 0;
		}

		if (iConsecutive == MAX_CONSECUTIVE)
			break;
	}

	if (iConsecutive == MAX_CONSECUTIVE)
	{
		wprintf(L"Too many consecutive characters remained the same\n");
		return FALSE;
	}

	WCHAR wcMessage[MAX_PATH] = { 0 };
	swprintf(wcMessage, L"%d characters are the same\n", iNumSame);
	wprintf(wcMessage);

	return TRUE;
}



BOOL ChooseMarble(WCHAR *&wcChoice)
{
	//Open Marble.h header file
	DWORD dwStart = 0;
	DWORD dwEnd = 0;
	DWORD dwCount = 0; //Number of entries
	string line;
	BOOL bFoundChoice = FALSE;
	BOOL bRet = FALSE;
	WCHAR wcMessage[1024] = { 0 };
	wcChoice = NULL;

	POPTION_LIST pList = NULL;
	POPTION_NODE pChosenNode = NULL; //There can only be one...
	

	//Copy file before making modifications
	CopyFile(L"Marble.h", L"Marble.h.marble", TRUE);

	//Open File
	ifstream fsMarble(L"Marble.h", ifstream::in);
	if (fsMarble.is_open())
	{
		//Build list of possible choices
		while (getline(fsMarble, line) && !bFoundChoice)
		{
			dwEnd = fsMarble.tellg();
			//Load List
			DWORD dwCharsNeeded = MultiByteToWideChar(CP_UTF8, 0, line.c_str(), strlen(line.c_str()), NULL, 0);
			WCHAR *wcTemp = (WCHAR *)calloc(sizeof(WCHAR), dwCharsNeeded + 1);
			MultiByteToWideChar(CP_UTF8, 0, line.c_str(), strlen(line.c_str()), wcTemp, dwCharsNeeded);

			if (PathMatchSpec(wcTemp, L"//#include*MBL*"))
			{
				//Got a match add to list
				POPTION_NODE pNode = (POPTION_NODE)calloc(sizeof(OPTION_NODE), 1);
				pNode->ullIndex = dwStart;
				pNode->dwLineLength = dwEnd - dwStart;
				pNode->wcName = wcTemp;

				pNode->pNext = pList;
				pList = pNode;
				dwCount++;
			}
			else if (PathMatchSpec(wcTemp, L"#include*MBL*"))
			{
				//User specified a specific algorithm to use - make this our chosen node
				pChosenNode = (POPTION_NODE)calloc(sizeof(OPTION_NODE), 1);
				pChosenNode->wcName = wcTemp;
				pChosenNode->pNext = pList;
				pList = pChosenNode;
				bFoundChoice = TRUE;
				dwCount++;
			}
			else
				free(wcTemp);
			dwStart = dwEnd;
		}
		fsMarble.close();
	}
	else
	{
		wprintf(L"Failed to open Marble.h\n");
		return FALSE;
	}


	if (pList == NULL || dwCount == 0)
	{
		wprintf(L"No Choices Found\n");
		goto freelist;
	}
	else if (pChosenNode == NULL)
	{
		//Choose Node From List Randomly
		swprintf(wcMessage, L"Choosing from %d algorithms\n", dwCount);
		wprintf(wcMessage);

		srand(time(NULL));
		DWORD dwChosenIndex = rand() % dwCount;
		DWORD dwCurrentIndex = 0;

		pChosenNode = pList;
		while (pChosenNode != NULL && dwCurrentIndex < dwChosenIndex)
		{
			pChosenNode = pChosenNode->pNext;
			dwCurrentIndex++;
		}
	}
	//else; //User specifically defined a module\technique to use

	if (pChosenNode == NULL)
	{
		wprintf(L"Failed to choose algorithm\n");
		goto freelist;
	}

	//We have found the algorithm we'll use - generate name for return
	wcChoice = (WCHAR *)calloc(sizeof(WCHAR), wcslen(pChosenNode->wcName) + 1);
	swprintf(wcChoice, pChosenNode->wcName);

	//Remove comments to include module
	if (!bFoundChoice)
	{
		//A choice wasn't explicitly defined so include the one we chose
		//UTF-8 encode and remove slashes for entry
		WCHAR *wcNewEntry = pChosenNode->wcName + 2; //hop the preceding comment slashes
		DWORD dwSizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wcNewEntry, wcslen(wcNewEntry), NULL, 0, NULL, NULL);
		CHAR *cUTFEntry = (CHAR *)calloc(sizeof(CHAR), dwSizeNeeded + 1);
		WideCharToMultiByte(CP_UTF8, 0, wcNewEntry, wcslen(wcNewEntry), cUTFEntry, dwSizeNeeded, NULL, NULL);

		HANDLE hMarble = CreateFile(L"Marble.h", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hMarble == INVALID_HANDLE_VALUE)
		{
			free(cUTFEntry);
			wprintf(L"Unable to open Marble.h for writing\n");
			goto freelist;
		}

		//Read in everything after entry
		DWORD dwRWBytes = 0;
		DWORD dwFileLen = GetFileSize(hMarble, NULL);
		DWORD dwTailLen = dwFileLen - (pChosenNode->ullIndex + pChosenNode->dwLineLength);
		LPBYTE lpbTail = (LPBYTE)malloc(dwTailLen);

		SetFilePointer(hMarble, pChosenNode->ullIndex + pChosenNode->dwLineLength, 0, FILE_BEGIN);
		ReadFile(hMarble, lpbTail, dwTailLen, &dwRWBytes, NULL);

		//Write new entry
		SetFilePointer(hMarble, pChosenNode->ullIndex, 0, FILE_BEGIN);
		WriteFile(hMarble, cUTFEntry, strlen(cUTFEntry), &dwRWBytes, NULL);
		WriteFile(hMarble, "\r\n", 2, &dwRWBytes, NULL);

		//Write tail
		WriteFile(hMarble, lpbTail, dwTailLen, &dwRWBytes, NULL);
		SetEndOfFile(hMarble);

		CloseHandle(hMarble);
		free(lpbTail);
		free(cUTFEntry);
	}

	bRet = TRUE;

	//free list
freelist:
	POPTION_NODE pCurrentNode = pList;
	while (pCurrentNode != NULL)
	{
		POPTION_NODE pTempNode = pCurrentNode;
		pCurrentNode = pCurrentNode->pNext;

		if (pTempNode->wcName)
			free(pTempNode->wcName);
		free(pTempNode);
	}

	return bRet;
}

BOOL SetScrambler(WCHAR *&wcChoice)
{
	//Validate args
	if (wcChoice == NULL)
		return FALSE;

	BOOL bRet = TRUE;
	WCHAR *wcCopy = _wcsdup(wcChoice);
	WCHAR wcSeps[] = L"\".";

	// account for full paths
	WCHAR *fuckme = wcsrchr(wcCopy, '\\'), *wcName = NULL;
	if( fuckme == NULL )
	{
		wcName = wcstok(wcCopy, wcSeps);
		if( !wcName )
		{
			free(wcCopy);
			return FALSE;
		}
		wcName = wcstok(NULL, wcSeps);
	}
	else
	{
		wcName = fuckme + 1;
		wcstok(wcName, wcSeps);
	}

	//Reset Choice For Name Formatting
	WCHAR *wcTemp = wcChoice;
	wcChoice = (WCHAR *)calloc(sizeof(WCHAR), wcslen(wcName) + 1);
	swprintf(wcChoice, wcName);
	free(wcTemp);

	//Use string to identify scrambler needed -- Print which algorithm is being used
	if (_wcsicmp(wcName, L"MBL_FORLOOP_XOR1D") == 0)
		g_pScram = new MBL_FORLOOP_XOR1();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_XOR2D") == 0)
		g_pScram = new MBL_FORLOOP_XOR2();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_XOR3D") == 0)
		g_pScram = new MBL_FORLOOP_XOR3();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_XOR4D") == 0)
		g_pScram = new MBL_FORLOOP_XOR4();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_XOR1D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_XOR1();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_XOR2D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_XOR2();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_XOR3D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_XOR3();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_XOR4D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_XOR4();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_XOR5D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_XOR5();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_XOR6D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_XOR6();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_RXOR1D") == 0)
		g_pScram = new MBL_FORLOOP_RXOR1();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_RXOR2D") == 0)
		g_pScram = new MBL_FORLOOP_RXOR2();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_RXOR3D") == 0)
		g_pScram = new MBL_FORLOOP_RXOR3();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_RXOR4D") == 0)
		g_pScram = new MBL_FORLOOP_RXOR4();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_RXOR1D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_RXOR1();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_RXOR2D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_RXOR2();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_RXOR3D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_RXOR3();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_RXOR4D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_RXOR4();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_BUMP1D") == 0)
		g_pScram = new MBL_FORLOOP_BUMP1();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_BUMP2D") == 0)
		g_pScram = new MBL_FORLOOP_BUMP2();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_BUMP3D") == 0)
		g_pScram = new MBL_FORLOOP_BUMP3();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_BUMP4D") == 0)
		g_pScram = new MBL_FORLOOP_BUMP4();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_BUMP5D") == 0)
		g_pScram = new MBL_FORLOOP_BUMP5();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_BUMP6D") == 0)
		g_pScram = new MBL_FORLOOP_BUMP6();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_BUMP7D") == 0)
		g_pScram = new MBL_FORLOOP_BUMP7();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_BUMP8D") == 0)
		g_pScram = new MBL_FORLOOP_BUMP8();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_BUMP9D") == 0)
		g_pScram = new MBL_FORLOOP_BUMP9();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_BUMP10D") == 0)
		g_pScram = new MBL_FORLOOP_BUMP10();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_BUMP11D") == 0)
		g_pScram = new MBL_FORLOOP_BUMP11();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_BUMP12D") == 0)
		g_pScram = new MBL_FORLOOP_BUMP12();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_BUMP1D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_BUMP1();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_BUMP2D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_BUMP2();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_BUMP3D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_BUMP3();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_BUMP4D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_BUMP4();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_BUMP5D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_BUMP5();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_BUMP6D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_BUMP6();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_BUMP7D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_BUMP7();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_BUMP8D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_BUMP8();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_BUMP9D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_BUMP9();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_BUMP10D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_BUMP10();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_BUMP11D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_BUMP11();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_BUMP12D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_BUMP12();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_RBUMP1D") == 0)
		g_pScram = new MBL_FORLOOP_RBUMP1();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_RBUMP2D") == 0)
		g_pScram = new MBL_FORLOOP_RBUMP2();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_RBUMP3D") == 0)
		g_pScram = new MBL_FORLOOP_RBUMP3();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_RBUMP4D") == 0)
		g_pScram = new MBL_FORLOOP_RBUMP4();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_RBUMP5D") == 0)
		g_pScram = new MBL_FORLOOP_RBUMP5();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_RBUMP6D") == 0)
		g_pScram = new MBL_FORLOOP_RBUMP6();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_RBUMP7D") == 0)
		g_pScram = new MBL_FORLOOP_RBUMP7();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_RBUMP8D") == 0)
		g_pScram = new MBL_FORLOOP_RBUMP8();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_RBUMP1D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_RBUMP1();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_RBUMP2D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_RBUMP2();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_RBUMP3D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_RBUMP3();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_RBUMP4D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_RBUMP4();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_RBUMP5D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_RBUMP5();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_RBUMP6D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_RBUMP6();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_RBUMP7D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_RBUMP7();
	else if (_wcsicmp(wcName, L"MBL_FORLOOP_FUNC_RBUMP8D") == 0)
		g_pScram = new MBL_FORLOOP_FUNC_RBUMP8();
	else if (_wcsicmp(wcName, L"MBL_CLASS_XOR1D") == 0)
		g_pScram = new MBL_CLASS_XOR1();
	else if (_wcsicmp(wcName, L"MBL_CLASS_XOR2D") == 0)
		g_pScram = new MBL_CLASS_XOR2();
	else if (_wcsicmp(wcName, L"MBL_CLASS_XOR3D") == 0)
		g_pScram = new MBL_CLASS_XOR3();
	else if (_wcsicmp(wcName, L"MBL_CLASS_XOR4D") == 0)
		g_pScram = new MBL_CLASS_XOR4();
	else if (_wcsicmp(wcName, L"MBL_CLASS_XOR5D") == 0)
		g_pScram = new MBL_CLASS_XOR5();
	else if (_wcsicmp(wcName, L"MBL_CLASS_XOR6D") == 0)
		g_pScram = new MBL_CLASS_XOR6();
	else if (_wcsicmp(wcName, L"MBL_CLASS_XOR7D") == 0)
		g_pScram = new MBL_CLASS_XOR7();
	else if (_wcsicmp(wcName, L"MBL_CLASS_XOR8D") == 0)
		g_pScram = new MBL_CLASS_XOR8();
	else if (_wcsicmp(wcName, L"MBL_CLASS_XOR9D") == 0)
		g_pScram = new MBL_CLASS_XOR9();
	else if (_wcsicmp(wcName, L"MBL_CLASS_XOR10D") == 0)
		g_pScram = new MBL_CLASS_XOR10();
	else if (_wcsicmp(wcName, L"MBL_CLASS_XOR11D") == 0)
		g_pScram = new MBL_CLASS_XOR11();
	else if (_wcsicmp(wcName, L"MBL_CLASS_XOR12D") == 0)
		g_pScram = new MBL_CLASS_XOR12();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RXOR1D") == 0)
		g_pScram = new MBL_CLASS_RXOR1();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RXOR2D") == 0)
		g_pScram = new MBL_CLASS_RXOR2();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RXOR3D") == 0)
		g_pScram = new MBL_CLASS_RXOR3();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RXOR4D") == 0)
		g_pScram = new MBL_CLASS_RXOR4();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RXOR5D") == 0)
		g_pScram = new MBL_CLASS_RXOR5();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RXOR6D") == 0)
		g_pScram = new MBL_CLASS_RXOR6();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RXOR7D") == 0)
		g_pScram = new MBL_CLASS_RXOR7();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RXOR8D") == 0)
		g_pScram = new MBL_CLASS_RXOR8();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RXOR9D") == 0)
		g_pScram = new MBL_CLASS_RXOR9();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RXOR10D") == 0)
		g_pScram = new MBL_CLASS_RXOR10();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RXOR11D") == 0)
		g_pScram = new MBL_CLASS_RXOR11();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RXOR12D") == 0)
		g_pScram = new MBL_CLASS_RXOR12();
	else if (_wcsicmp(wcName, L"MBL_CLASS_BUMP1D") == 0)
		g_pScram = new MBL_CLASS_BUMP1();
	else if (_wcsicmp(wcName, L"MBL_CLASS_BUMP2D") == 0)
		g_pScram = new MBL_CLASS_BUMP2();
	else if (_wcsicmp(wcName, L"MBL_CLASS_BUMP3D") == 0)
		g_pScram = new MBL_CLASS_BUMP3();
	else if (_wcsicmp(wcName, L"MBL_CLASS_BUMP4D") == 0)
		g_pScram = new MBL_CLASS_BUMP4();
	else if (_wcsicmp(wcName, L"MBL_CLASS_BUMP5D") == 0)
		g_pScram = new MBL_CLASS_BUMP5();
	else if (_wcsicmp(wcName, L"MBL_CLASS_BUMP6D") == 0)
		g_pScram = new MBL_CLASS_BUMP6();
	else if (_wcsicmp(wcName, L"MBL_CLASS_BUMP7D") == 0)
		g_pScram = new MBL_CLASS_BUMP7();
	else if (_wcsicmp(wcName, L"MBL_CLASS_BUMP8D") == 0)
		g_pScram = new MBL_CLASS_BUMP8();
	else if (_wcsicmp(wcName, L"MBL_CLASS_BUMP9D") == 0)
		g_pScram = new MBL_CLASS_BUMP9();
	else if (_wcsicmp(wcName, L"MBL_CLASS_BUMP10D") == 0)
		g_pScram = new MBL_CLASS_BUMP10();
	else if (_wcsicmp(wcName, L"MBL_CLASS_BUMP11D") == 0)
		g_pScram = new MBL_CLASS_BUMP11();
	else if (_wcsicmp(wcName, L"MBL_CLASS_BUMP12D") == 0)
		g_pScram = new MBL_CLASS_BUMP12();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RBUMP1D") == 0)
		g_pScram = new MBL_CLASS_RBUMP1();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RBUMP2D") == 0)
		g_pScram = new MBL_CLASS_RBUMP2();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RBUMP3D") == 0)
		g_pScram = new MBL_CLASS_RBUMP3();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RBUMP4D") == 0)
		g_pScram = new MBL_CLASS_RBUMP4();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RBUMP5D") == 0)
		g_pScram = new MBL_CLASS_RBUMP5();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RBUMP6D") == 0)
		g_pScram = new MBL_CLASS_RBUMP6();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RBUMP7D") == 0)
		g_pScram = new MBL_CLASS_RBUMP7();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RBUMP8D") == 0)
		g_pScram = new MBL_CLASS_RBUMP8();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RBUMP9D") == 0)
		g_pScram = new MBL_CLASS_RBUMP9();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RBUMP10D") == 0)
		g_pScram = new MBL_CLASS_RBUMP10();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RBUMP11D") == 0)
		g_pScram = new MBL_CLASS_RBUMP11();
	else if (_wcsicmp(wcName, L"MBL_CLASS_RBUMP12D") == 0)
		g_pScram = new MBL_CLASS_RBUMP12();
	else
		bRet = FALSE;

	WCHAR wcMessage[1024] = { 0 };
	swprintf(wcMessage, L"Using module: %s\n", wcName);
	wprintf(wcMessage);
	free(wcCopy);

	return bRet;
}



BOOL ConvertQuoteToBytes(CHAR *cStringToConvert, int iStringToConvertLen, LPBYTE &lpbBytes, INT32 &iBytesLen)
{
	if (cStringToConvert == NULL || iStringToConvertLen <= 0)
		return FALSE;

	lpbBytes = (LPBYTE) calloc(sizeof(CHAR), iStringToConvertLen); //it won't be bigger than this
	iBytesLen = 0;
	INT32 iBytesIndex = 0;

	BOOL bRet = TRUE;

	//Char by char converting the string to bytes
	for (int i = 0; i < iStringToConvertLen; i++, iBytesIndex++)
	{
		if (cStringToConvert[i] == '\\' && i < iStringToConvertLen - 1)
		{
			switch (cStringToConvert[i + 1])
			{
			case 'n':
				lpbBytes[iBytesIndex] = '\n';
				break;
			case 't':
				lpbBytes[iBytesIndex] = '\t';
				break;
			case 'v':
				lpbBytes[iBytesIndex] = '\v';
				break;
			case 'b':
				lpbBytes[iBytesIndex] = '\b';
				break;
			case 'r':
				lpbBytes[iBytesIndex] = '\r';
				break;
			case 'f':
				lpbBytes[iBytesIndex] = '\f';
				break;
			case 'a':
				lpbBytes[iBytesIndex] = '\a';
				break;
			case '\\':
				lpbBytes[iBytesIndex] = '\\';
				break;
			case '?':
				lpbBytes[iBytesIndex] = '\?';
				break;
			case '\'':
				lpbBytes[iBytesIndex] = '\'';
				break;
			case '\"':
				lpbBytes[iBytesIndex] = '\"';
				break;
			case '\0':
				lpbBytes[iBytesIndex] = '\"';
				break;
			case 'x':
				{
					if (!(i < iStringToConvertLen - 3))
						bRet = FALSE;
					else
					{
						//Convert from hexascii
						CHAR cTemp[2] = { 0 };
						CHAR cTemp2[2] = { 0 };
						cTemp[0] = cStringToConvert[i + 2];
						cTemp[1] = cStringToConvert[i + 3];
						DWORD dwTempLen = 2;
						CryptStringToBinaryA(cTemp, 2, CRYPT_STRING_HEXASCII, (BYTE *)cTemp2, &dwTempLen, NULL, NULL);

						//Convert to hex
						lpbBytes[iBytesIndex] = cTemp2[0];
						i += 2;
					}
				}
				break;
			default:
				{
					//Octal - no support
					bRet = FALSE;
				}
				break;
			}
			i++;
		}
		else
			lpbBytes[iBytesIndex] = cStringToConvert[i];
	}
	iBytesLen = iBytesIndex;


	if (!bRet)
	{
		if (lpbBytes)
			free(lpbBytes);
		lpbBytes = NULL;
		iBytesLen = 0;
	}

	return bRet;
}

BOOL ConvertQuoteToBytes(WCHAR *wcStringToConvert, int iStringToConvertLen, LPBYTE &lpbBytes, INT32 &iBytesLen)
{
	if (wcStringToConvert == NULL || iStringToConvertLen <= 0)
		return FALSE;

	lpbBytes = (LPBYTE)calloc(sizeof(WCHAR), iStringToConvertLen);
	WCHAR *wcBytes = (WCHAR *)lpbBytes;
	iBytesLen = 0;
	INT32 iBytesIndex = 0;
	BOOL bRet = TRUE;

	//Char by char converting the string to bytes
	for (int i = 0; i < iStringToConvertLen; i++, iBytesIndex++)
	{
		if (wcStringToConvert[i] == L'\\' && i < iStringToConvertLen - 1)
		{
			switch (wcStringToConvert[i + 1])
			{
			case L'n':
				wcBytes[iBytesIndex] = L'\n';
				break;
			case L't':
				wcBytes[iBytesIndex] = L'\t';
				break;
			case L'v':
				wcBytes[iBytesIndex] = L'\v';
				break;
			case L'b':
				wcBytes[iBytesIndex] = L'\b';
				break;
			case L'r':
				wcBytes[iBytesIndex] = L'\r';
				break;
			case L'f':
				wcBytes[iBytesIndex] = L'\f';
				break;
			case L'a':
				wcBytes[iBytesIndex] = L'\a';
				break;
			case L'\\':
				wcBytes[iBytesIndex] = L'\\';
				break;
			case L'?':
				wcBytes[iBytesIndex] = L'\?';
				break;
			case L'\'':
				wcBytes[iBytesIndex] = L'\'';
				break;
			case L'\"':
				wcBytes[iBytesIndex] = L'\"';
				break;
			case L'\0':
				wcBytes[iBytesIndex] = L'\"';
				break;
			case L'x':
			{
				if (!(i < iStringToConvertLen - 5))
					bRet = FALSE;
				else
				{
					WCHAR wcTemp[5] = { 0 };
					WCHAR wcTemp2[3] = { 0 };
					CHAR cTemp3[5] = { 0 };
					wcTemp[0] = wcStringToConvert[i + 2];
					wcTemp[1] = wcStringToConvert[i + 3];
					wcTemp[2] = wcStringToConvert[i + 4];
					wcTemp[3] = wcStringToConvert[i + 5];

					//Convert to UTF-8
					WideCharToMultiByte(CP_UTF8, 0, wcTemp, 4, cTemp3, 4, NULL, NULL);

					DWORD dwTempLen = 2;
					
					BOOL bTemp = CryptStringToBinaryA(cTemp3 + 2, 2, CRYPT_STRING_HEXASCII, ((LPBYTE)wcTemp2), &dwTempLen, NULL, NULL);
					if (bTemp)
						bTemp = CryptStringToBinaryA(cTemp3, 2, CRYPT_STRING_HEXASCII, ((LPBYTE)wcTemp2) + 1, &dwTempLen, NULL, NULL);
					
					if (!bTemp) //Fail if unable to convert
						bRet = FALSE;
					else
						wcBytes[iBytesIndex] = wcTemp2[0];

					i += 4;
				}
			}
			break;
			default:
			{
				//Octal - Unsupported
				bRet = FALSE;
			}
			break;
			}

			i++;
		}
		else
			wcBytes[iBytesIndex] = wcStringToConvert[i];
	}

	iBytesLen = iBytesIndex;

	if (!bRet)
	{
		if (lpbBytes)
			free(lpbBytes);
		lpbBytes = NULL;
		iBytesLen = 0;
	}

	return bRet;
}

BOOL ConvertBraceToBytes(CHAR *cStringToConvert, int iStringToConvertLen, LPBYTE &lpbBytes, INT32 &iBytesLen)
{
	if (cStringToConvert == NULL || iStringToConvertLen <= 0)
		return FALSE;

	lpbBytes = (LPBYTE)calloc(sizeof(CHAR), iStringToConvertLen);
	iBytesLen = 0;
	INT32 iBytesIndex = 0;

	BOOL bRet = TRUE;

	//Char by char converting the string to bytes
	for (int i = 0; i < iStringToConvertLen; i++)
	{
		if ((cStringToConvert[i] == 'x' || cStringToConvert[i] == 'X') && i < iStringToConvertLen - 2)
		{
			CHAR cTemp[2] = { 0 };
			CHAR cTemp2[2] = { 0 };
			cTemp[0] = cStringToConvert[i + 1];
			cTemp[1] = cStringToConvert[i + 2];
			DWORD dwTempLen = 2;
			CryptStringToBinaryA(cTemp, 2, CRYPT_STRING_HEXASCII, (BYTE *)cTemp2, &dwTempLen, NULL, NULL);

			//Convert to hex
			lpbBytes[iBytesIndex] = cTemp2[0];
			i += 2;
			iBytesIndex++;
		}
	}

	iBytesLen = iBytesIndex;

	if (!bRet)
	{
		if (lpbBytes)
			free(lpbBytes);
		lpbBytes = NULL;
		iBytesLen = 0;
	}

	return bRet;
}

BOOL ConvertBraceToBytesW(CHAR *cStringToConvert, int iStringToConvertLen, LPBYTE &lpbBytes, INT32 &iBytesLen)
{
	if (cStringToConvert == NULL || iStringToConvertLen <= 0)
		return FALSE;

	lpbBytes = (LPBYTE)calloc(sizeof(WCHAR), iStringToConvertLen);
	iBytesLen = 0;
	INT32 iBytesIndex = 0;

	BOOL bRet = TRUE;

	//Char by char converting the string to bytes
	for (int i = 0; i < iStringToConvertLen; i++)
	{
		if ((cStringToConvert[i] == 'x' || cStringToConvert[i] == 'X') && i < iStringToConvertLen - 4)
		{
			CHAR cTemp[4] = { 0 };
			CHAR cTemp2[2] = { 0 };
			cTemp[0] = cStringToConvert[i + 1];
			cTemp[1] = cStringToConvert[i + 2];
			cTemp[2] = cStringToConvert[i + 3];
			cTemp[3] = cStringToConvert[i + 4];

			DWORD dwTempLen = 2;

			CryptStringToBinaryA(cTemp + 2, 2, CRYPT_STRING_HEXASCII, (BYTE *)cTemp2, &dwTempLen, NULL, NULL);
			lpbBytes[iBytesIndex] = cTemp2[0];
			iBytesIndex++;

			CryptStringToBinaryA(cTemp, 2, CRYPT_STRING_HEXASCII, (BYTE *)cTemp2, &dwTempLen, NULL, NULL);
			lpbBytes[iBytesIndex] = cTemp2[0];
			iBytesIndex++;

			i += 2;
		}
	}

	iBytesLen = iBytesIndex / sizeof(WCHAR);

	if (!bRet)
	{
		if (lpbBytes)
			free(lpbBytes);
		lpbBytes = NULL;
		iBytesLen = 0;
	}

	return bRet;
}



BOOL GenerateReceipt(WCHAR *wcChoice, PMARBLER_LIST pMarblerList)
{
	//Validate Args
	if (wcChoice == NULL || g_wcOutDir == NULL)
		return FALSE;

	if (wcslen(g_wcOutDir) == 0)
		return FALSE;

	if (g_wcOutDir[wcslen(g_wcOutDir) - 1] == L'\\')
		g_wcOutDir[wcslen(g_wcOutDir) - 1] = L'\0';

	WCHAR wcRec[] = L"%s\\MarbleReceipt.xml";
	WCHAR *wcReceiptFull = (WCHAR *)calloc(sizeof(WCHAR), wcslen(g_wcOutDir) + wcslen(wcRec) + 2);
	swprintf(wcReceiptFull, wcRec, g_wcOutDir);
	DeleteFile(wcReceiptFull);

	//Write XML File Documenting Changes
	IXMLDOMDocument* doc = NULL;
	IXMLDOMElement *pElem = NULL;
	IXMLDOMNode *pNewNode = NULL;
	BOOL bRet = FALSE;

	if (FAILED(CoInitialize(NULL)))
	{
		free(wcReceiptFull);
		return FALSE;
	}

	if (FAILED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&doc)))
		goto uninit;
	
	doc->put_async(VARIANT_FALSE);
	doc->put_validateOnParse(VARIANT_FALSE);
	doc->put_resolveExternals(VARIANT_FALSE);
	doc->put_preserveWhiteSpace(VARIANT_TRUE);

	//Create Processing Instruction
	{
		IXMLDOMProcessingInstruction *pi = NULL;
		BSTR bstrTarget = SysAllocString(L"xml");
		BSTR bstrData = SysAllocString(L"version='1.0'");

		if (SUCCEEDED(doc->createProcessingInstruction(bstrTarget, bstrData, &pi)))
		{
			if (SUCCEEDED(doc->appendChild(pi, &pNewNode)))
				pNewNode->Release();
			pi->Release();
		}
	}

	//Create Root Element
	BSTR bstrFramework = SysAllocString(L"MarbleFramework");
	HRESULT hr = doc->createElement(bstrFramework, &pElem);
	SysFreeString(bstrFramework);
	if (FAILED(hr))
		goto reldoc;//bail

	hr = doc->appendChild(pElem, &pNewNode);
	if (SUCCEEDED(hr))
		pNewNode->Release();

	//Add Version Element
	{
		IXMLDOMElement *pVersion = NULL;
		BSTR bstrVersion= SysAllocString(L"Version");
		BSTR bstrVerNum = SysAllocString(L"1.0");
		hr = doc->createElement(bstrVersion, &pVersion);
		if (SUCCEEDED(hr))
		{
			pVersion->put_text(bstrVerNum);
			hr = pElem->appendChild(pVersion, &pNewNode);
			if (SUCCEEDED(hr))
				pNewNode->Release();

			pVersion->Release();
		}

		SysFreeString(bstrVersion);
		SysFreeString(bstrVerNum);
	}

	if (FAILED(hr))
		goto relelem;//bail

	//Add Choice Element
	{
		IXMLDOMElement *pChoice = NULL;
		BSTR bstrChoice = SysAllocString(L"ObfuscationAlgorithm");
		hr = doc->createElement(bstrChoice, &pChoice);
		if (SUCCEEDED(hr))
		{
			BSTR bstrData = SysAllocString(wcChoice);
			pChoice->put_text(bstrData);
			hr = pElem->appendChild(pChoice, &pNewNode);
			if (SUCCEEDED(hr))
				pNewNode->Release();

			pChoice->Release();
			SysFreeString(bstrData);
		}

		SysFreeString(bstrChoice);
	}

	
	
	PMARBLER_NODE pNode = pMarblerList;
	IXMLDOMElement *pCurrentFile = NULL;
	IXMLDOMNode *pCurrentStrings = NULL;
	WCHAR *wcCurrentFile = NULL;

	while (pNode != NULL)
	{
		//Check to see if file has changed
		BOOL bCreateNew = FALSE;
		if (wcCurrentFile == NULL || pCurrentFile == NULL || pCurrentStrings == NULL)
			bCreateNew = TRUE;

		if (!bCreateNew)
		{
			if (_wcsicmp(wcCurrentFile, pNode->wcFilePath) != 0)
				bCreateNew = TRUE;
		}

		if (bCreateNew)
		{
			if(pCurrentFile)
				pCurrentFile->Release();
			pCurrentFile = NULL;

			if (pCurrentStrings)
				pCurrentStrings->Release();
			pCurrentStrings = NULL;

			wcCurrentFile = NULL;

			BSTR bstrFileTag = SysAllocString(L"File");
			hr = doc->createElement(bstrFileTag, &pCurrentFile);
			if (SUCCEEDED(hr))
			{
				hr = pElem->appendChild(pCurrentFile, &pNewNode);
				if (SUCCEEDED(hr))
					pNewNode->Release();
			}
			SysFreeString(bstrFileTag);

			if (SUCCEEDED(hr))
			{	
				//Add File Name Element
				IXMLDOMElement *pFileName;
				BSTR bstrFileNameTag = SysAllocString(L"FileName");
				BSTR bstrFileName = SysAllocString(pNode->wcFilePath);
				hr = doc->createElement(bstrFileNameTag, &pFileName);
				if (SUCCEEDED(hr))
				{
					//Put File Path - Append To File
					pFileName->put_text(bstrFileName);
					hr = pCurrentFile->appendChild(pFileName, &pNewNode);
					if (SUCCEEDED(hr))
						pNewNode->Release();

					pFileName->Release();

					//Add Strings Element
					BSTR bstrStrings = SysAllocString(L"Strings");
					hr = pCurrentFile->selectSingleNode(bstrStrings, &pCurrentStrings);
					if (FAILED(hr) || hr == S_FALSE)
					{
						//Doesn't exist - create it
						hr = doc->createElement(bstrStrings, (IXMLDOMElement **)&pCurrentStrings);
						if (SUCCEEDED(hr))
						{
							hr = pCurrentFile->appendChild(pCurrentStrings, &pNewNode);
							if (SUCCEEDED(hr))
								pNewNode->Release();
						}
					}
					SysFreeString(bstrStrings);
				}
				SysFreeString(bstrFileName);
				SysFreeString(bstrFileNameTag);
			}
		}
		else
			hr = S_OK;


		
		if (SUCCEEDED(hr))
		{
			//Add String Nodes to Strings Top Level Element
			wcCurrentFile = pNode->wcFilePath;

			IXMLDOMElement *pStringObject = NULL;
			BSTR bstrString = SysAllocString(L"String");

			hr = doc->createElement(bstrString, &pStringObject);
			if (SUCCEEDED(hr))
			{
				hr = pCurrentStrings->appendChild(pStringObject, &pNewNode);
				if (SUCCEEDED(hr))
					pNewNode->Release();

				//String Type - If CHAR = hexascii
				IXMLDOMElement *pStringType = NULL;
				BSTR bstrTypeTag = SysAllocString(L"StringType");
				hr = doc->createElement(bstrTypeTag, &pStringType);
				if (SUCCEEDED(hr))
				{
					if (pNode->eStringType == stWCHAR)
					{
						BSTR bstrWCHAR = SysAllocString(L"WCHAR");
						pStringType->put_text(bstrWCHAR);
						SysFreeString(bstrWCHAR);
					}
					else
					{
						BSTR bstrCHAR = SysAllocString(L"CHAR");
						pStringType->put_text(bstrCHAR);
						SysFreeString(bstrCHAR);
					}

					hr = pStringObject->appendChild(pStringType, &pNewNode);
					if (SUCCEEDED(hr))
						pNewNode->Release();
				}
				SysFreeString(bstrTypeTag);

				//if WCHAR convert to HEXASCII and print string
				WCHAR *wcHexAscii = NULL;
				if (pNode->eStringType == stWCHAR)
				{

					WCHAR *wcWideString = (WCHAR *)calloc(1, pNode->iBinLength + sizeof(WCHAR));
					memcpy(wcWideString, pNode->cString, pNode->iBinLength);
					IXMLDOMElement *pOriginalString = NULL;
					BSTR bstrOrgStringTag = SysAllocString(L"OriginalString");
					BSTR bstrOrgString = SysAllocString(wcWideString);
					hr = doc->createElement(bstrOrgStringTag, &pOriginalString);
					if (SUCCEEDED(hr))
					{
						pOriginalString->put_text(bstrOrgString);
						
						hr = pStringObject->appendChild(pOriginalString, &pNewNode);
						if (SUCCEEDED(hr))
							pNewNode->Release();
						pOriginalString->Release();
					}
					SysFreeString(bstrOrgStringTag);
					SysFreeString(bstrOrgString);
					free(wcWideString);
				}

				CHAR *cHexAscii = (CHAR *)calloc(2, pNode->iBinLength + 1);
				DWORD dwSize = 3;
				INT32 iIndex = 0;
				for (int i = 0; i < pNode->iBinLength; i++, iIndex += 2)
				{
					CryptBinaryToStringA((BYTE *)pNode->cString + i, 1, CRYPT_STRING_HEXASCII, cHexAscii + iIndex, &dwSize);
				}

				DWORD dwSizeNeeded = MultiByteToWideChar(CP_UTF8, 0, cHexAscii, strlen(cHexAscii), wcHexAscii, 0);
				wcHexAscii = (WCHAR *)calloc(sizeof(WCHAR), dwSizeNeeded + 1);
				MultiByteToWideChar(CP_UTF8, 0, cHexAscii, strlen(cHexAscii), wcHexAscii, dwSizeNeeded);
				free(cHexAscii);				

				IXMLDOMElement *pOriginalHex = NULL;
				BSTR bstrHexTag = SysAllocString(L"OriginalHex");
				BSTR bstrOrigHex = SysAllocString(wcHexAscii);
				hr = doc->createElement(bstrHexTag, &pOriginalHex);
				if (SUCCEEDED(hr))
				{
					pOriginalHex->put_text(bstrOrigHex);

					hr = pStringObject->appendChild(pOriginalHex, &pNewNode);
					if (SUCCEEDED(hr))
						pNewNode->Release();

					pOriginalHex->Release();
				}

				//Line Number
				WCHAR wcLineNum[MAX_PATH] = { 0 };
				swprintf(wcLineNum, L"%d", pNode->iLineNumber);
				IXMLDOMElement *pLineNumber = NULL;
				BSTR bstrLineTag = SysAllocString(L"LineNumber");
				BSTR bstrLineNum = SysAllocString(wcLineNum);
				hr = doc->createElement(bstrLineTag, &pLineNumber);
				if (SUCCEEDED(hr))
				{
					pLineNumber->put_text(bstrLineNum);

					hr = pStringObject->appendChild(pLineNumber, &pNewNode);
					if (SUCCEEDED(hr))
						pNewNode->Release();

					pLineNumber->Release();
				}

				if (wcHexAscii)
					free(wcHexAscii);
				SysFreeString(bstrLineTag);
				SysFreeString(bstrLineNum);
				SysFreeString(bstrHexTag);
				SysFreeString(bstrOrigHex);
				pStringObject->Release();
			}
			SysFreeString(bstrString);
		}

		pNode = pNode->pNextNode;
	}

	if (pCurrentFile)
		pCurrentFile->Release();
	if (pCurrentStrings)
		pCurrentStrings->Release();

	VARIANT vtFile;
	BSTR bstrFile = SysAllocString(wcReceiptFull);

	V_VT(&vtFile) = VT_BSTR;
	V_BSTR(&vtFile) = bstrFile;
	doc->save(vtFile);
	VariantClear(&vtFile);
	
relelem:
	pElem->Release();
reldoc:
	doc->Release();
uninit:
	CoUninitialize();
	
	if (wcReceiptFull)
		free(wcReceiptFull);

	return bRet;
}



