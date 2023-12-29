#pragma once
#include <Windows.h>
#include "MBL_CLASS_BUMP7D.h"
#include <stdlib.h>
#include <time.h>

MBL_CLASS_BUMP7D::MBL_CLASS_BUMP7D(void)
{
	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	cStringPointer = 0;
	iCStringPointerLen = 0;
}

MBL_CLASS_BUMP7D::~MBL_CLASS_BUMP7D(void)
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

void MBL_CLASS_BUMP7D::Scramble(char *cString, int iNumOfChars, char cXorKey)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		cString[i] += cXorKey;
}

void MBL_CLASS_BUMP7D::Scramble(wchar_t *wcString, int iNumOfChars, wchar_t wcXorKey)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		wcString[i] += wcXorKey;
}

