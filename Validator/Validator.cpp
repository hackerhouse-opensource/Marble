/*
 * Filename:		Validator.cpp
 *
 * Classification:	SECRET//NOFORN
 * Classified By:	
 *
 * Tool Name:		Validator
 * Requirement #:	2015-XXXX
 *
 * Author:			???
 * Date Created:	02/11/2015
 * Version 1.0:		02/11/2015 (???)
 *
 * This tool takes a list of dirty strings/data (in the form of a Marble Receipt)
 * and verifies that no strings/data are in the specified binary.

 *
 */

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <ObjBase.h>
#include <MsXml.h>
#include <atlstr.h>
#include <comutil.h>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#import <msxml6.dll>

#include "Misc\MemoryScan\MISCMemorySearch_NSS.h"

using namespace std;

#pragma comment(lib, "misc.lib")
#pragma comment(lib, "Crypt32.lib")

typedef struct _STRING_LIST
{
	WCHAR *wcFile;
	INT64 iLineNumber;
	LPBYTE lpbData;
	DWORD dwDataLen;

	_STRING_LIST *pNextNode;
}STRING_LIST, *PSTRING_LIST, STRING_NODE, *PSTRING_NODE;

BOOL LoadReceipt(WCHAR *wcPath, PSTRING_LIST &pList);
BOOL ScanFile(WCHAR *wcPath, PSTRING_LIST pList);

template<class T>
bool getString(/*IXMLDOMDocument*/T* doc, const char* xpath, std::wstring& res) {
	IXMLDOMNode* node;
	BSTR xmlString;
	doc->selectSingleNode(_bstr_t(xpath), &node);
	if (!node) {
		return false;
	}
	if (node->get_text(&xmlString) != S_OK) {
		return false;
	}
	res = xmlString;
	return true;
}

template<class T>
bool fillProperty(T* doc, const char* xpath, std::wstring& value) {
	if (!getString(doc, xpath, value)) return false;
	return true;
}

template<class T>
bool fillProperty(T* doc, const char* xpath, INT32* value) {
	std::wstring str;
	if (!getString(doc, xpath, str)) return false;
	*value = _wtoi(str.c_str());
	return true;
}

template<class T>
bool fillProperty(T* doc, const char* xpath, DWORD* value) {
	std::wstring str;
	if (!getString(doc, xpath, str)) return false;

	DWORD temp = _wtoi(str.c_str());
	*value = temp;
	//*value = 1;
	//*value = *value << temp;
	return true;
}


int wmain(int argc, wchar_t* argv[])
{
	WCHAR wcUsage[] = L"Usage:\n\tPath to Marbler Receipt\n\tPath to File to Scan\n\n*Arguments can go in any order\n\n";
	
	//Usage - Validate Args
	if (argc < 3)
		goto printusage;


	int iRet = -1;

	//Determine which file is which
	WCHAR *wcReceipt = NULL;
	WCHAR *wcScanFile = NULL;
	PSTRING_LIST pList = NULL;

	WCHAR *wcFile1 = __wargv[1];
	WCHAR *wcExt = wcFile1 + wcslen(wcFile1) - 4;
	
	if (wcsicmp(wcExt, L".xml") == 0)
	{
		wcReceipt = wcFile1;
		wcScanFile = __wargv[2];
	}
	else
	{
		wcReceipt = __wargv[2];
		wcScanFile = wcFile1;
	}


	//Parse Marble Receipt File
	printf("\n\nValidating No Strings In Binary\n");
	if (!LoadReceipt(wcReceipt, pList))
	{
		printf("Failed to parse receipt file\n");
		goto cleanup;
	}

	//Scan File
	if (!ScanFile(wcScanFile, pList))
	{
		printf("Failure in scanning file\n");
		goto cleanup;
	}

	iRet = 0;

cleanup:
	//Cleanup List
	PSTRING_NODE pNode = pList;
	while (pNode != NULL)
	{
		if (pNode->wcFile)
			free(pNode->wcFile);
		if (pNode->lpbData)
			free(pNode->lpbData);

		pNode = pNode->pNextNode;
	}

	if (iRet == -1)
		goto printusage;
	printf("\n");
	return 0;


printusage:
	wprintf(wcUsage);
	return iRet;
}

BOOL LoadReceipt(WCHAR *wcPath, PSTRING_LIST &pList)
{
	//Validate Args
	if (wcPath == NULL)
		return FALSE;

	//load the xml document
	IXMLDOMDocument* doc;
	IXMLDOMNode *fileNode, *stringNode;
	IXMLDOMNodeList* fileList, *stringList;
	VARIANT_BOOL success;
	BOOL bRet = FALSE;

	if (FAILED(CoInitialize(NULL)))
		return FALSE;

	if (FAILED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&doc)))
		goto uninit;

	if (FAILED(doc->load(_variant_t(wcPath), &success)))
		goto reldoc;

	//Create List
	BSTR bstrFileTag = SysAllocString(L"/MarbleFramework/File");
	HRESULT hr = doc->selectNodes(bstrFileTag, &fileList);
	long fileListLength;
	if (SUCCEEDED(hr))
	{
		fileList->get_length(&fileListLength);
		for (long i = 0; i < fileListLength; i++)
		{
			hr = fileList->get_item(i, &fileNode);
			if (SUCCEEDED(hr))
			{
				//Set File Name 
				wstring pszFileName;
				WCHAR *wcFileName;

				fillProperty(fileNode, "FileName", pszFileName);
				wcFileName = (WCHAR *)calloc(sizeof(WCHAR), pszFileName.length() + 1);
				memcpy(wcFileName, pszFileName.c_str(), pszFileName.length() * sizeof(WCHAR));

				//Get All Strings for File
				BSTR bstrStringTag = SysAllocString(L"Strings/String");
				hr = fileNode->selectNodes(bstrStringTag, &stringList);
				if (SUCCEEDED(hr))
				{
					long lStringListLength;
					stringList->get_length(&lStringListLength);

					for (long j = 0; j < lStringListLength; j++)
					{
						hr = stringList->get_item(j, &stringNode);
						if (SUCCEEDED(hr))
						{
							//Create Node
							PSTRING_NODE pNewString = (PSTRING_NODE)calloc(sizeof(STRING_NODE), 1);

							//Make Copy of FileName
							pNewString->wcFile = (WCHAR *)calloc(sizeof(WCHAR), wcslen(wcFileName) + 1);
							memcpy(pNewString->wcFile, wcFileName, wcslen(wcFileName) * sizeof(WCHAR));

							//Get Type
							/*wstring pszStringType;
							fillProperty(stringNode, "StringType", pszStringType);*/

							//Get Line Number
							fillProperty(stringNode, "LineNumber", (DWORD *)&pNewString->iLineNumber);

							wstring pszString;
							fillProperty(stringNode, "OriginalHex", pszString);
							WCHAR *wcString = (WCHAR *)pszString.c_str();

							//Convert to MultiByte
							CHAR *cHexAscii = NULL;
							DWORD dwSizeNeeded = WideCharToMultiByte(CP_UTF8, 0, pszString.c_str(), pszString.length(), NULL, 0, 0, 0);
							cHexAscii = (CHAR *)calloc(1, dwSizeNeeded + 2);
							WideCharToMultiByte(CP_UTF8, 0, pszString.c_str(), pszString.length(), cHexAscii, dwSizeNeeded, 0, 0);

							//Convert from HEXASCII to hex
							DWORD dwSize = 2;
							pNewString->dwDataLen = dwSizeNeeded / 2;
							pNewString->lpbData = (LPBYTE)calloc(1, pNewString->dwDataLen);
							for (int k = 0; k < dwSizeNeeded / 2; k++)
							{
								CryptStringToBinaryA(cHexAscii + (k * 2), 2, CRYPT_STRING_HEXASCII, pNewString->lpbData + k, &dwSize, NULL, NULL);
							}

							free(cHexAscii);

							pNewString->pNextNode = pList;
							pList = pNewString;
							stringNode->Release();
						}
					}
					stringList->Release();
				}
				SysFreeString(bstrStringTag);


				if (wcFileName)
					free(wcFileName);
				fileNode->Release();
			}
		}
		fileList->Release();
	}

	bRet = TRUE;

freebstr:
	SysFreeString(bstrFileTag);
reldoc:
	doc->Release();
uninit:
	CoUninitialize();

	return bRet;
}

BOOL ScanFile(WCHAR *wcPath, PSTRING_LIST pList)
{
	if (wcPath == NULL)
		return FALSE;

	//Alert User If Any Sequences Found
	DWORD dwStringsFound = 0;

	HANDLE hSearch = CreateFile(wcPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSearch == INVALID_HANDLE_VALUE)
	{
		wprintf(L"Failure opening file to scan\n");
		return FALSE;
	}

	DWORD dwFileLen = GetFileSize(hSearch, NULL);
	DWORD dwReadBytes = 0;
	CHAR *cFile = (CHAR *)malloc(dwFileLen);
	ReadFile(hSearch, cFile, dwFileLen, &dwReadBytes, NULL);
	CloseHandle(hSearch);

	PSTRING_NODE pCurrentNode = pList;
	while (pCurrentNode != NULL)
	{
		if (pCurrentNode->lpbData && pCurrentNode->dwDataLen > 0)
		{
			INT64 iPosit = MISCMemorySearch_NSS::FindIndexOfSequenceInMemory(cFile, dwFileLen, (CHAR *)pCurrentNode->lpbData, pCurrentNode->dwDataLen);
			if (iPosit >= 0)
			{
				wprintf(L"Found string in binary (File: %s    Line %d)\n", pCurrentNode->wcFile, pCurrentNode->iLineNumber);
				dwStringsFound++;
			}
		}

		pCurrentNode = pCurrentNode->pNextNode;
	}
	free(cFile);
	wprintf(L"%d Strings Found\n", dwStringsFound);

	return TRUE;
}
