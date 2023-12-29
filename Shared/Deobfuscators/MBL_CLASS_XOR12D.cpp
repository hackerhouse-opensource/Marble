#pragma once
#include <Windows.h>
#include "MBL_CLASS_XOR12D.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

MBL_CLASS_XOR12D::MBL_CLASS_XOR12D(char *cString, int iNumOfChars, char *cXorKey)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		cString[i] ^= cXorKey[i % 8];
}

MBL_CLASS_XOR12D::MBL_CLASS_XOR12D(wchar_t *wcString, int iNumOfChars, wchar_t *wcXorKey)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		wcString[i] ^= wcXorKey[i % 8];
}

MBL_CLASS_XOR12D::~MBL_CLASS_XOR12D(void)
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