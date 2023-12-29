#pragma once
#include <Windows.h>
#include "MBL_CLASS_XOR7D.h"
#include <stdlib.h>
#include <time.h>

MBL_CLASS_XOR7D::MBL_CLASS_XOR7D(void)
{
	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	cStringPointer = 0;
	iCStringPointerLen = 0;
}

MBL_CLASS_XOR7D::~MBL_CLASS_XOR7D(void)
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

void MBL_CLASS_XOR7D::Scramble(char cXorKey, int iNumOfChars, char *cString)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		cString[i] ^= cXorKey;
}

void MBL_CLASS_XOR7D::Scramble(wchar_t wcXorKey, int iNumOfChars, wchar_t *wcString)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		wcString[i] ^= wcXorKey;
}

