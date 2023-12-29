#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MBL_CLASS_RBUMP8.h"


MBL_CLASS_RBUMP8::MBL_CLASS_RBUMP8(void)
{
	srand(time(NULL));
	cKey = (unsigned char)(rand() % 225 + 15); //+1 so we don't XOR with 0
	wcKey = (wchar_t)(rand() % 65505 + 15); //+1 so we don't XOR with 0

}

MBL_CLASS_RBUMP8::~MBL_CLASS_RBUMP8(void)
{

}

int MBL_CLASS_RBUMP8::ScrambleW(wchar_t *wcToScramble, unsigned int iNumOfChars)
{
	if (wcToScramble == NULL) return 0;

	wcToScramble[iNumOfChars - 1] -= wcKey;
	for (int i = iNumOfChars - 2; i >= 0; i--)
		wcToScramble[i] -= wcToScramble[i + 1];

	return 1;
}

int MBL_CLASS_RBUMP8::ScrambleA(char *cToScramble, unsigned int iNumOfChars)
{
	if (cToScramble == NULL) return 0;

	cToScramble[iNumOfChars - 1] -= cKey;
	for (int i = iNumOfChars - 2; i >= 0; i--)
		cToScramble[i] -= cToScramble[i + 1];

	return 1;
}

int MBL_CLASS_RBUMP8::GenerateInsertA(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "char %s[] = %s;\r\n"
		"MBL_CLASS_RBUMP8D m%sMarbleXorClass;\r\n"
		"m%sMarbleXorClass.Scramble(0x%02X, %s, %d);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + (strlen(cVarName) * 4) + strlen(cStringLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cVarName, cKey, cVarName, iNumOfChars);

	return 1;
}

int MBL_CLASS_RBUMP8::GenerateInsertW(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "wchar_t %s[] = %s;\r\n"
		"MBL_CLASS_RBUMP8D m%sMarbleXorClass;\r\n"
		"m%sMarbleXorClass.Scramble(0x%04X, %s, %d);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + (strlen(cVarName) * 4) + strlen(cStringLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cVarName, wcKey, cVarName, iNumOfChars);

	return 1;
}