#pragma once
#include <Windows.h>
#include "MBL_CLASS_BUMP11D.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

MBL_CLASS_BUMP11D::MBL_CLASS_BUMP11D(char *cXorKey, char *cString, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		cString[i] += cXorKey[i % 8];
}

MBL_CLASS_BUMP11D::MBL_CLASS_BUMP11D(wchar_t *wcXorKey, wchar_t *wcString, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		wcString[i] += wcXorKey[i % 8];
}

MBL_CLASS_BUMP11D::~MBL_CLASS_BUMP11D(void)
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