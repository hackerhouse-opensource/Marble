#pragma once
#include <Windows.h>
#include "MBL_CLASS_XOR8D.h"
#include <stdlib.h>
#include <time.h>

MBL_CLASS_XOR8D::MBL_CLASS_XOR8D(void)
{
	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;
}

MBL_CLASS_XOR8D::~MBL_CLASS_XOR8D(void)
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

void MBL_CLASS_XOR8D::Scramble(char cXorKey, int iNumOfChars, char *cString)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		cString[i] ^= cXorKey;
}

void MBL_CLASS_XOR8D::Scramble(wchar_t wcXorKey, int iNumOfChars, wchar_t *wcString)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		wcString[i] ^= wcXorKey;
}

