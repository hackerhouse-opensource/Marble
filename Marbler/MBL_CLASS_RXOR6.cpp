#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MBL_CLASS_RXOR6.h"


MBL_CLASS_RXOR6::MBL_CLASS_RXOR6(void)
{
	srand(time(NULL));
	cKey = (unsigned char)(rand() % 255 + 1); //+1 so we don't XOR with 0
	wcKey = (wchar_t)(rand() % 65534 + 1); //+1 so we don't XOR with 0

}

MBL_CLASS_RXOR6::~MBL_CLASS_RXOR6(void)
{

}

int MBL_CLASS_RXOR6::ScrambleW(wchar_t *wcToScramble, unsigned int iNumOfChars)
{
	if (wcToScramble == NULL) return 0;

	wcToScramble[0] ^= wcKey;
	for (int i = 1; i < iNumOfChars; i++)
		wcToScramble[i] ^= wcToScramble[i - 1];

	return 1;
}

int MBL_CLASS_RXOR6::ScrambleA(char *cToScramble, unsigned int iNumOfChars)
{
	if (cToScramble == NULL) return 0;

	cToScramble[0] ^= cKey;
	for (int i = 1; i < iNumOfChars; i++)
		cToScramble[i] ^= cToScramble[i - 1];

	return 1;
}

int MBL_CLASS_RXOR6::GenerateInsertA(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "char %s[] = %s;\r\n"
		"MBL_CLASS_RXOR6D m%sMarbleXorClass;\r\n"
		"m%sMarbleXorClass.Scramble(%s, 0x%02X, %d);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + (strlen(cVarName) * 4) + strlen(cStringLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cVarName, cVarName, cKey, iNumOfChars);

	return 1;
}

int MBL_CLASS_RXOR6::GenerateInsertW(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "wchar_t %s[] = %s;\r\n"
		"MBL_CLASS_RXOR6D m%sMarbleXorClass;\r\n"
		"m%sMarbleXorClass.Scramble(%s, 0x%04X, %d);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + (strlen(cVarName) * 4) + strlen(cStringLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cVarName, cVarName, wcKey, iNumOfChars);

	return 1;
}