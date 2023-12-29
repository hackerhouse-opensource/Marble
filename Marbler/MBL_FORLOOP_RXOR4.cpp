#pragma once
#include "MBL_FORLOOP_RXOR4.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


MBL_FORLOOP_RXOR4::MBL_FORLOOP_RXOR4(void)
{
	srand(time(NULL));
	cKey = (unsigned char)(rand() % 255 + 1); //+1 so we don't XOR with 0
	wcKey = (wchar_t)(rand() % 65534 + 1); //+1 so we don't XOR with 0

}

MBL_FORLOOP_RXOR4::~MBL_FORLOOP_RXOR4(void)
{

}


int MBL_FORLOOP_RXOR4::ScrambleW(wchar_t *wcToScramble, unsigned int iNumOfChars)
{
	if (wcToScramble == NULL || iNumOfChars <= 0) return 0;

	wcToScramble[iNumOfChars - 1] ^= wcKey;
	for (int i = iNumOfChars - 2; i >= 0; i--)
		wcToScramble[i] ^= wcToScramble[i + 1];

	return 1;
}

int MBL_FORLOOP_RXOR4::ScrambleA(char *cToScramble, unsigned int iNumOfChars)
{
	if (cToScramble == NULL || iNumOfChars <= 0) return 0;

	cToScramble[iNumOfChars - 1] ^= cKey;
	for (int i = iNumOfChars - 2; i >= 0; i--)
		cToScramble[i] ^= cToScramble[i + 1];

	return 1;
}

int MBL_FORLOOP_RXOR4::GenerateInsertA(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "char %s[] = %s;\r\n"
		"char c%sXor = %s[%d - 1];\r\n"
		"%s[%d - 1] ^= 0x%02X;\r\n"
		"for(int i = %d - 2; i >= 0; i--)\r\n"
		"{\r\n\tchar cTemp = %s[i];\r\n"
		"\t%s[i] ^= c%sXor;\r\n"
		"\tc%sXor = cTemp;\r\n}\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + (strlen(cVarName) * 8) + strlen(cStringLiteral) + 75);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cVarName, iNumOfChars, cVarName, iNumOfChars, cKey, iNumOfChars, cVarName, cVarName, cVarName, cVarName);

	return 1;
}

int MBL_FORLOOP_RXOR4::GenerateInsertW(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "wchar_t %s[] = %s;\r\n"
		"wchar_t c%sXor = %s[%d - 1];\r\n"
		"%s[%d - 1] ^= 0x%02X;\r\n"
		"for(int i = %d - 2; i >= 0; i--)\r\n"
		"{\r\n\twchar_t cTemp = %s[i];\r\n"
		"\t%s[i] ^= c%sXor;\r\n"
		"\tc%sXor = cTemp;\r\n}\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + (strlen(cVarName) * 8) + strlen(cStringLiteral) + 75);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cVarName, iNumOfChars, cVarName, iNumOfChars, wcKey, iNumOfChars, cVarName, cVarName, cVarName, cVarName);

	return 1;
}