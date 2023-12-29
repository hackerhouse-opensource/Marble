#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MBL_FORLOOP_FUNC_RBUMP3.h"

MBL_FORLOOP_FUNC_RBUMP3::MBL_FORLOOP_FUNC_RBUMP3(void)
{
	srand(time(NULL));
	cKey = (unsigned char)(rand() % 225 + 15); //+1 so we don't XOR with 0
	wcKey = (wchar_t)(rand() % 65505 + 15); //+1 so we don't XOR with 0
}

MBL_FORLOOP_FUNC_RBUMP3::~MBL_FORLOOP_FUNC_RBUMP3(void)
{

}

int MBL_FORLOOP_FUNC_RBUMP3::ScrambleW(wchar_t *wcToScramble, unsigned int iNumOfChars)
{
	if (wcToScramble == NULL) return 0;

	wcToScramble[iNumOfChars - 1] += wcKey;
	for (int i = iNumOfChars - 2; i >= 0; i--)
		wcToScramble[i] += wcToScramble[i + 1];

	return 1;
}

int MBL_FORLOOP_FUNC_RBUMP3::ScrambleA(char *cToScramble, unsigned int iNumOfChars)
{
	if (cToScramble == NULL) return 0;

	cToScramble[iNumOfChars - 1] += cKey;
	for (int i = iNumOfChars - 2; i >= 0; i--)
		cToScramble[i] += cToScramble[i + 1];

	return 1;
}

int MBL_FORLOOP_FUNC_RBUMP3::GenerateInsertA(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "char %s[] = %s;\r\n"
		"MBL_FORLOOP_FUNC_RBUMP3D(%s, %d, 0x%02X);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + strlen(cStringLiteral) + (strlen(cVarName) * 2) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, iNumOfChars, cKey);

	return 1;
}

int MBL_FORLOOP_FUNC_RBUMP3::GenerateInsertW(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "wchar_t %s[] = %s;\r\n"
		"MBL_FORLOOP_FUNC_RBUMP3D(%s, %d, 0x%04X);\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + strlen(cStringLiteral) + (strlen(cVarName) * 2) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, cVarName, iNumOfChars, wcKey);

	return 1;
}