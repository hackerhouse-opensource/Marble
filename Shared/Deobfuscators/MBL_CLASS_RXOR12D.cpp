#pragma once
#include <Windows.h>
#include "MBL_CLASS_RXOR12D.h"
#include <stdlib.h>
#include <time.h>

MBL_CLASS_RXOR12D::MBL_CLASS_RXOR12D(char cXorKey, char *cString, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	CHAR cXorByte = cString[iNumOfChars - 1];
	cString[iNumOfChars - 1] ^= cXorKey;
	for (int i = iNumOfChars - 2; i >= 0; i--)
	{
		CHAR cTemp = cString[i];
		cString[i] ^= cXorByte;
		cXorByte = cTemp;
	}
}

MBL_CLASS_RXOR12D::MBL_CLASS_RXOR12D(wchar_t wcXorKey, wchar_t *wcString, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Xor
	WCHAR wcXorByte = wcString[iNumOfChars - 1];
	wcString[iNumOfChars - 1] ^= wcXorKey;
	for (int i = iNumOfChars - 2; i >= 0; i--)
	{
		WCHAR wcTemp = wcString[i];
		wcString[i] ^= wcXorByte;
		wcXorByte = wcTemp;
	}
}

MBL_CLASS_RXOR12D::~MBL_CLASS_RXOR12D(void)
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