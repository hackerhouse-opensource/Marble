#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MBL_FORLOOP_FUNC_BUMP12.h"

MBL_FORLOOP_FUNC_BUMP12::MBL_FORLOOP_FUNC_BUMP12(void)
{
	cKeyLiteral = NULL;
	cWKeyLiteral = NULL;

	srand(time(NULL));
	for (int i = 0; i < 16; i++)
		cKey[i] = (unsigned char)(rand() % 225 + 15); //+1 so we don't XOR with 0

	for (int i = 0; i < 16; i++)
		wcKey[i] = (wchar_t)(rand() % 65505 + 15); //+1 so we don't XOR with 0

	//create key literals
	CreateStringLiteralA(cKey, 16, cKeyLiteral);
	CreateStringLiteralW(wcKey, 16, cWKeyLiteral);
}

MBL_FORLOOP_FUNC_BUMP12::~MBL_FORLOOP_FUNC_BUMP12(void)
{
	//free key literals
	if (cKeyLiteral)
		free(cKeyLiteral);

	if (cWKeyLiteral)
		free(cWKeyLiteral);
}

int MBL_FORLOOP_FUNC_BUMP12::ScrambleW(wchar_t *wcToScramble, unsigned int iNumOfChars)
{
	if (wcToScramble == NULL) return 0;
	for (int i = 0; i < iNumOfChars; i++)
		wcToScramble[i] -= wcKey[i % 16];

	return 1;
}

int MBL_FORLOOP_FUNC_BUMP12::ScrambleA(char *cToScramble, unsigned int iNumOfChars)
{
	if (cToScramble == NULL) return 0;
	for (int i = 0; i < iNumOfChars; i++)
		cToScramble[i] -= cKey[i % 16];

	return 1;
}

int MBL_FORLOOP_FUNC_BUMP12::GenerateInsertA(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "char %s[] = %s;\r\n"
		"char c%sMarbleBumpKey[] = %s;\r\n"
		"MBL_FORLOOP_FUNC_BUMP12D(%s, %d, c%sMarbleBumpKey);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + strlen(cStringLiteral) + (strlen(cVarName) * 4) + strlen(cKeyLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cKeyLiteral, cVarName, iNumOfChars, cVarName);

	return 1;
}

int MBL_FORLOOP_FUNC_BUMP12::GenerateInsertW(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "wchar_t %s[] = %s;\r\n"
		"wchar_t wc%sMarbleBumpKey[] = %s;\r\n"
		"MBL_FORLOOP_FUNC_BUMP12D(%s, %d, wc%sMarbleBumpKey);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + strlen(cStringLiteral) + (strlen(cVarName) * 4) + strlen(cWKeyLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cWKeyLiteral, cVarName, iNumOfChars, cVarName);

	return 1;
}