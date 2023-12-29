#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MBL_FORLOOP_BUMP3.h"

MBL_FORLOOP_BUMP3::MBL_FORLOOP_BUMP3(void)
{
	srand(time(NULL));
	cKey = (unsigned char)(rand() % 225 + 15); //+1 so we don't XOR with 0
	wcKey = (wchar_t)(rand() % 65505 + 15); //+1 so we don't XOR with 0
}

MBL_FORLOOP_BUMP3::~MBL_FORLOOP_BUMP3(void)
{

}

int MBL_FORLOOP_BUMP3::ScrambleW(wchar_t *wcToScramble, unsigned int iNumOfChars)
{
	if (wcToScramble == NULL) return 0;
	for (int i = 0; i < iNumOfChars; i++)
		wcToScramble[i] -= wcKey;

	return 1;
}

int MBL_FORLOOP_BUMP3::ScrambleA(char *cToScramble, unsigned int iNumOfChars)
{
	if (cToScramble == NULL) return 0;
	for (int i = 0; i < iNumOfChars; i++)
		cToScramble[i] -= cKey;

	return 1;
}

int MBL_FORLOOP_BUMP3::GenerateInsertA(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "char %s[] = %s;\r\n"
		"for(int i = 0; i < %d; i++)\r\n"
		"\t%s[i] += 0x%02X;\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + strlen(cStringLiteral) + (strlen(cVarName) * 2) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, iNumOfChars, cVarName, cKey);

	return 1;
}

int MBL_FORLOOP_BUMP3::GenerateInsertW(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert)
{
	if (cVarName == NULL || cStringLiteral == NULL)
		return 0;
	cInsert = NULL;

	char cInsertFormat[] = "wchar_t %s[] = %s;\r\n"
		"for(int i = 0; i < %d; i++)\r\n"
		"\t%s[i] += 0x%04X;\r\n";

	cInsert = (char *)calloc(sizeof(char), strlen(cInsertFormat) + strlen(cStringLiteral) + (strlen(cVarName) * 2) + 50);
	sprintf(cInsert, cInsertFormat, cVarName, cStringLiteral, iNumOfChars, cVarName, wcKey);

	return 1;
}