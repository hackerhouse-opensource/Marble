#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MBL_CLASS_XOR10.h"


MBL_CLASS_XOR10::MBL_CLASS_XOR10(void)
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

MBL_CLASS_XOR10::~MBL_CLASS_XOR10(void)
{
	//free key literals
	if (cKeyLiteral)
		free(cKeyLiteral);

	if (cWKeyLiteral)
		free(cWKeyLiteral);
}

int MBL_CLASS_XOR10::ScrambleW(wchar_t *wcToScramble, unsigned int iNumOfChars)
{
	if (wcToScramble == NULL) return 0;
	for (int i = 0; i < iNumOfChars; i++)
		wcToScramble[i] ^= wcKey[i % 8];

	return 1;
}

int MBL_CLASS_XOR10::ScrambleA(char *cToScramble, unsigned int iNumOfChars)
{
	if (cToScramble == NULL) return 0;
	for (int i = 0; i < iNumOfChars; i++)
		cToScramble[i] ^= cKey[i % 8];

	return 1;
}

int MBL_CLASS_XOR10::GenerateInsertA(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "char %s[] = %s;\r\n"
		"char c%sMarbleKey[] = %s;\r\n"
		"MBL_CLASS_XOR10D m%sMarbleXorClass(%s, %d, c%sMarbleKey);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + (strlen(cVarName) * 5) + strlen(cStringLiteral) + strlen(cKeyLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cKeyLiteral, cVarName, cVarName, iNumOfChars, cVarName);

	return 1;
}

int MBL_CLASS_XOR10::GenerateInsertW(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "wchar_t %s[] = %s;\r\n"
		"wchar_t c%sMarbleKey[] = %s;\r\n"
		"MBL_CLASS_XOR10D m%sMarbleXorClass(%s, %d, c%sMarbleKey);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + (strlen(cVarName) * 5) + strlen(cStringLiteral) + strlen(cKeyLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, cWKeyLiteral, cVarName, cVarName, iNumOfChars, cVarName);

	return 1;
}