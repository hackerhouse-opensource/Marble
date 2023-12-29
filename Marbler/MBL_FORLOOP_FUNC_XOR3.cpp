#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MBL_FORLOOP_FUNC_XOR3.h"

MBL_FORLOOP_FUNC_XOR3::MBL_FORLOOP_FUNC_XOR3(void)
{
	cKeyLiteral = NULL;
	cWKeyLiteral = NULL;

	srand(time(NULL));
	for (int i = 0; i < 8; i++)
		cKey[i] = (unsigned char)(rand() % 255 + 1); //+1 so we don't XOR with 0

	for (int i = 0; i < 8; i++)
		wcKey[i] = (wchar_t)(rand() % 65534 + 1); //+1 so we don't XOR with 0

	//create key literals
	CreateStringLiteralA(cKey, 8, cKeyLiteral);
	CreateStringLiteralW(wcKey, 8, cWKeyLiteral);

}

MBL_FORLOOP_FUNC_XOR3::~MBL_FORLOOP_FUNC_XOR3(void)
{
	//free key literals
	if (cKeyLiteral)
		free(cKeyLiteral);

	if (cWKeyLiteral)
		free(cWKeyLiteral);
}

int MBL_FORLOOP_FUNC_XOR3::ScrambleW(wchar_t *wcToScramble, unsigned int iNumOfChars)
{
	if (wcToScramble == NULL) return 0;
	for (int i = 0; i < iNumOfChars; i++)
		wcToScramble[i] ^= wcKey[i % 8];
	return 1;
}

int MBL_FORLOOP_FUNC_XOR3::ScrambleA(char *cToScramble, unsigned int iNumOfChars)
{
	if (cToScramble == NULL) return 0;
	for (int i = 0; i < iNumOfChars; i++)
		cToScramble[i] ^= cKey[i % 8];
	return 1;
}

int MBL_FORLOOP_FUNC_XOR3::GenerateInsertA(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "char c%sMarbleXorKey[] = %s;\r\n"
		"char %s[] = %s;\r\n"
		"MBL_FORLOOP_FUNC_XOR3D(%s, %d, c%sMarbleXorKey);\r\n";

	int iLen = (strlen(cVarName) * 4) + strlen(cStringLiteral) + strlen(cInsertFormat) + strlen(cKeyLiteral) + 50;

	cInsert = (char *)calloc(sizeof(char), iLen);
	sprintf(cInsert, cInsertFormat, cVarName, cKeyLiteral, cVarName, cStringLiteral, cVarName, iNumOfChars, cVarName);

	return 1;
}

int MBL_FORLOOP_FUNC_XOR3::GenerateInsertW(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "wchar_t c%sMarbleXorKey[] = %s;\r\n"
		"wchar_t %s[] = %s;\r\n"
		"MBL_FORLOOP_FUNC_XOR3D(%s, %d, c%sMarbleXorKey);\r\n";

	int iLen = (strlen(cVarName) * 4) + strlen(cStringLiteral) + strlen(cInsertFormat) + strlen(cWKeyLiteral) + 50;

	cInsert = (char *)calloc(sizeof(char), iLen);
	sprintf(cInsert, cInsertFormat, cVarName, cWKeyLiteral, cVarName, cStringLiteral, cVarName, iNumOfChars, cVarName);

	return 1;
}