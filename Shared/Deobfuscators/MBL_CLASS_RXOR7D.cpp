#pragma once
#include <Windows.h>
#include "MBL_CLASS_RXOR7D.h"
#include <stdlib.h>
#include <time.h>

MBL_CLASS_RXOR7D::MBL_CLASS_RXOR7D(void)
{
	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;
}

MBL_CLASS_RXOR7D::~MBL_CLASS_RXOR7D(void)
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

void MBL_CLASS_RXOR7D::Scramble(char *cString, char cXorKey, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	//Xor
	for (int i = iNumOfChars - 1; i > 0; i--)
		cString[i] ^= cString[i - 1];
	cString[0] ^= cXorKey;
}

void MBL_CLASS_RXOR7D::Scramble(wchar_t *wcString, wchar_t wcXorKey, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	//Xor
	for (int i = iNumOfChars - 1; i > 0; i--)
		wcString[i] ^= wcString[i - 1];
	wcString[0] ^= wcXorKey;
}

