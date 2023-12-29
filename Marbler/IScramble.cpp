#pragma once
#include <Windows.h>
#include <stdio.h>
#include "IScramble.h"


int IScramble::CreateStringLiteralA(unsigned char *cBuffer, int iNumOfChars, char *&cOutput)
{
	if (cBuffer == NULL || iNumOfChars <= 0)
		return 0;

	cOutput = NULL;

	//Generate string literals in the form \x00\x01\x02 - newlines every 30 entries
	int iIndex = 1;
	int iNumOfNewLines = (iNumOfChars / 30) + 1;
	int iOutLen = (iNumOfChars * 4) + (iNumOfNewLines * 12) + 10;
	cOutput = (char *)calloc(sizeof(char), iOutLen);
	cOutput[0] = '\"';

	for (int i = 0; i < iNumOfChars; i++, iIndex += 4)
	{
		if (i != 0 && i % 30 == 0)
		{
			memcpy(cOutput + iIndex, "\"\r\n\"", 4);
			iIndex += 4;
		}

		
		char cByte[10] = { 0 };
		sprintf(cByte, "\\x%02X", cBuffer[i]);
		memcpy(cOutput + iIndex, cByte, 4);
	}
	cOutput[iIndex] = '\"';

	return iIndex;
}


int IScramble::CreateStringLiteralW(wchar_t *wcBuffer, int iNumOfChars, char *&cOutput)
{
	if (wcBuffer == NULL || iNumOfChars <= 0)
		return 0;

	cOutput = NULL;

	//Generate string literals in the form \x0000, \x0001, \x0002 - newlines every 30 entries
	int iIndex = 2;
	int iNumOfNewLines = (iNumOfChars / 30) + 1;
	int iOutLen = (iNumOfChars * 6) + (iNumOfNewLines * 16) + 10;
	cOutput = (char *)calloc(sizeof(char), iOutLen);
	cOutput[0] = 'L';
	cOutput[1] = '\"';

	for (int i = 0; i < iNumOfChars; i++, iIndex += 6)
	{
		if (i != 0 && i % 30 == 0)
		{
			memcpy(cOutput + iIndex, "\"\r\nL\"", 5);
			iIndex += 5;
		}


		char cByte[10] = { 0 };
		sprintf(cByte, "\\x%04X", wcBuffer[i]);
		memcpy(cOutput + iIndex, cByte, 6);
	}
	cOutput[iIndex] = '\"';
	
	return iIndex;
}
