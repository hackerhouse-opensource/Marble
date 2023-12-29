#pragma once
#include <Windows.h>
#include "MBL_CLASS_BUMP12D.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

MBL_CLASS_BUMP12D::MBL_CLASS_BUMP12D(char *cXorKey, char *cString, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		cString[i] += cXorKey[i % 8];
}

MBL_CLASS_BUMP12D::MBL_CLASS_BUMP12D(wchar_t *wcXorKey, wchar_t *wcString, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		wcString[i] += wcXorKey[i % 8];
}

MBL_CLASS_BUMP12D::~MBL_CLASS_BUMP12D(void)
{
	srand((DWORD) time(NULL));

	if (cStringPointer)
	{
		for (int i = iCStringPointerLen - 1; i >= 0; i--)
			cStringPointer[i] = (unsigned char)rand() % 255;
	}


	if (wcStringPointer)
	{
		for (int i = iWStringPointerLen - 1; i >= 0; i--)
			wcStringPointer[i] = rand() % 65534;
	}
}