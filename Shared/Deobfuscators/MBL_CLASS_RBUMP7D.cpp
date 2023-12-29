#pragma once
#include <Windows.h>
#include "MBL_CLASS_RBUMP7D.h"
#include <stdlib.h>
#include <time.h>

MBL_CLASS_RBUMP7D::MBL_CLASS_RBUMP7D(void)
{
	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	cStringPointer = 0;
	iCStringPointerLen = 0;
}

MBL_CLASS_RBUMP7D::~MBL_CLASS_RBUMP7D(void)
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

void MBL_CLASS_RBUMP7D::Scramble(char cXorKey, char *cString, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	//Bump
	for (int i = 0; i < iNumOfChars - 1; i++)
		cString[i] += cString[i + 1];
	cString[iNumOfChars - 1] += cXorKey;
}

void MBL_CLASS_RBUMP7D::Scramble(wchar_t wcXorKey, wchar_t *wcString, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	//Bump
	for (int i = 0; i < iNumOfChars - 1; i++)
		wcString[i] += wcString[i + 1];
	wcString[iNumOfChars - 1] += wcXorKey;
}

