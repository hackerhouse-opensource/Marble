#pragma once
#include <Windows.h>
#include "MBL_CLASS_RXOR3D.h"
#include <stdlib.h>
#include <time.h>

MBL_CLASS_RXOR3D::MBL_CLASS_RXOR3D(char cXorKey, int iNumOfChars, char *cString)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	for (int i = iNumOfChars - 1; i > 0; i--)
		cString[i] ^= cString[i - 1];
	cString[0] ^= cXorKey;
}

MBL_CLASS_RXOR3D::MBL_CLASS_RXOR3D(wchar_t wcXorKey, int iNumOfChars, wchar_t *wcString)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Xor
	for (int i = iNumOfChars - 1; i > 0; i--)
		wcString[i] ^= wcString[i - 1];
	wcString[0] ^= wcXorKey;
}

MBL_CLASS_RXOR3D::~MBL_CLASS_RXOR3D(void)
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