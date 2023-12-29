#pragma once
#include "MBL_FORLOOP_FUNC_RXOR2.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


MBL_FORLOOP_FUNC_RXOR2::MBL_FORLOOP_FUNC_RXOR2(void)
{
	srand(time(NULL));
	cKey = (unsigned char)(rand() % 255 + 1); //+1 so we don't XOR with 0
	wcKey = (wchar_t)(rand() % 65534 + 1); //+1 so we don't XOR with 0

}

MBL_FORLOOP_FUNC_RXOR2::~MBL_FORLOOP_FUNC_RXOR2(void)
{

}


int MBL_FORLOOP_FUNC_RXOR2::ScrambleW(wchar_t *wcToScramble, unsigned int iNumOfChars)
{
	if (wcToScramble == NULL || iNumOfChars <= 0) return 0;

	wcToScramble[0] ^= wcKey;
	for (int i = 1; i < iNumOfChars; i++)
		wcToScramble[i] ^= wcToScramble[i - 1];

	return 1;
}

int MBL_FORLOOP_FUNC_RXOR2::ScrambleA(char *cToScramble, unsigned int iNumOfChars)
{
	if (cToScramble == NULL || iNumOfChars <= 0) return 0;

	cToScramble[0] ^= cKey;
	for (int i = 1; i < iNumOfChars; i++)
		cToScramble[i] ^= cToScramble[i - 1];

	return 1;
}

int MBL_FORLOOP_FUNC_RXOR2::GenerateInsertA(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "char %s[] = %s;\r\n"
		"MBL_FORLOOP_FUNC_RXOR2D(%s, %d, 0x%02X);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + (strlen(cVarName) * 2) + strlen(cStringLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, iNumOfChars, cKey);

	return 1;
}

int MBL_FORLOOP_FUNC_RXOR2::GenerateInsertW(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "wchar_t %s[] = %s;\r\n"
		"MBL_FORLOOP_FUNC_RXOR2D(%s, %d, 0x%04X);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + (strlen(cVarName) * 2) + strlen(cStringLiteral) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, iNumOfChars, wcKey);

	return 1;
}