#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MBL_CLASS_RBUMP10.h"


MBL_CLASS_RBUMP10::MBL_CLASS_RBUMP10(void)
{
	srand(time(NULL));
	cKey = (unsigned char)(rand() % 225 + 15); //+1 so we don't XOR with 0
	wcKey = (wchar_t)(rand() % 65505 + 15); //+1 so we don't XOR with 0

}

MBL_CLASS_RBUMP10::~MBL_CLASS_RBUMP10(void)
{

}

int MBL_CLASS_RBUMP10::ScrambleW(wchar_t *wcToScramble, unsigned int iNumOfChars)
{
	if (wcToScramble == NULL) return 0;

	wcToScramble[0] -= wcKey;
	for (int i = 1; i < iNumOfChars; i++)
		wcToScramble[i] += wcToScramble[i - 1];

	return 1;
}

int MBL_CLASS_RBUMP10::ScrambleA(char *cToScramble, unsigned int iNumOfChars)
{
	if (cToScramble == NULL) return 0;

	cToScramble[0] -= cKey;
	for (int i = 1; i < iNumOfChars; i++)
		cToScramble[i] += cToScramble[i - 1];

	return 1;
}

int MBL_CLASS_RBUMP10::GenerateInsertA(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "char %s[] = %s;\r\n"
		"MBL_CLASS_RBUMP10D m%sMarbleXorClass(%s, %d, 0x%02X);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + (strlen(cVarName) * 3) + strlen(cStringLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cVarName, iNumOfChars, cKey);

	return 1;
}

int MBL_CLASS_RBUMP10::GenerateInsertW(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "wchar_t %s[] = %s;\r\n"
		"MBL_CLASS_RBUMP10D m%sMarbleXorClass(%s, %d, 0x%04X);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + (strlen(cVarName) * 3) + strlen(cStringLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cVarName, iNumOfChars, wcKey);

	return 1;
}