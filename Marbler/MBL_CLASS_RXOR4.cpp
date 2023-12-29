#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MBL_CLASS_RXOR4.h"


MBL_CLASS_RXOR4::MBL_CLASS_RXOR4(void)
{
	srand(time(NULL));
	cKey = (unsigned char)(rand() % 255 + 1); //+1 so we don't XOR with 0
	wcKey = (wchar_t)(rand() % 65534 + 1); //+1 so we don't XOR with 0

}

MBL_CLASS_RXOR4::~MBL_CLASS_RXOR4(void)
{

}

int MBL_CLASS_RXOR4::ScrambleW(wchar_t *wcToScramble, unsigned int iNumOfChars)
{
	if (wcToScramble == NULL) return 0;

	wcToScramble[0] ^= wcKey;
	for (int i = 1; i < iNumOfChars; i++)
		wcToScramble[i] ^= wcToScramble[i - 1];

	return 1;
}

int MBL_CLASS_RXOR4::ScrambleA(char *cToScramble, unsigned int iNumOfChars)
{
	if (cToScramble == NULL) return 0;

	cToScramble[0] ^= cKey;
	for (int i = 1; i < iNumOfChars; i++)
		cToScramble[i] ^= cToScramble[i - 1];

	return 1;
}

int MBL_CLASS_RXOR4::GenerateInsertA(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "char %s[] = %s;\r\n"
		"MBL_CLASS_RXOR4D m%sMarbleXorClass(0x%02X, %d, %s);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + (strlen(cVarName) * 3) + strlen(cStringLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cKey, iNumOfChars, cVarName);

	return 1;
}

int MBL_CLASS_RXOR4::GenerateInsertW(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "wchar_t %s[] = %s;\r\n"
		"MBL_CLASS_RXOR4D m%sMarbleXorClass(0x%04X, %d, %s);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + (strlen(cVarName) * 3) + strlen(cStringLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, wcKey, iNumOfChars, cVarName);

	return 1;
}