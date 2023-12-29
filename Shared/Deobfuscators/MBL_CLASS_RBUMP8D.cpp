#pragma once
#include <Windows.h>
#include "MBL_CLASS_RBUMP8D.h"
#include <stdlib.h>
#include <time.h>

MBL_CLASS_RBUMP8D::MBL_CLASS_RBUMP8D(void)
{
	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	cStringPointer = 0;
	iCStringPointerLen = 0;
}

MBL_CLASS_RBUMP8D::~MBL_CLASS_RBUMP8D(void)
{
	srand((DWORD) time(NULL));

	if (cStringPointer)
	{
		for (int i = 0; i < iCStringPointerLen; i++)
			cStringPointer[i] = (unsigned char)rand() % 255;
	}


	if (wcStringPointer)
	{
		for (int i = 0; i < iWStringPointerLen; i++)
			wcStringPointer[i] = rand() % 65534;
	}
}

void MBL_CLASS_RBUMP8D::Scramble(char cXorKey, char *cString, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	//Bump
	char cBump = cString[iNumOfChars - 1];
	for (int i = iNumOfChars - 2; i >= 0; i--)
	{
		char cTempChar = cString[i];
		cString[i] += cBump;
		cBump = cTempChar;

	}
	cString[iNumOfChars - 1] += cXorKey;
}

void MBL_CLASS_RBUMP8D::Scramble(wchar_t wcXorKey, wchar_t *wcString, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	//Bump
	wchar_t wcBump = wcString[iNumOfChars - 1];
	for (int i = iNumOfChars - 2; i >= 0; i--)
	{
		wchar_t wcTempChar = wcString[i];
		wcString[i] += wcBump;
		wcBump = wcTempChar;

	}
	wcString[iNumOfChars - 1] += wcXorKey;
}

