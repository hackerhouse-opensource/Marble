#pragma once
#include <Windows.h>
#include "MBL_CLASS_XOR11D.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

MBL_CLASS_XOR11D::MBL_CLASS_XOR11D(char *cString, int iNumOfChars, char *cXorKey)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		cString[i] ^= cXorKey[i % 8];
}

MBL_CLASS_XOR11D::MBL_CLASS_XOR11D(wchar_t *wcString, int iNumOfChars, wchar_t *wcXorKey)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		wcString[i] ^= wcXorKey[i % 8];
}

MBL_CLASS_XOR11D::~MBL_CLASS_XOR11D(void)
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