#pragma once
#include <Windows.h>
#include "MBL_CLASS_RXOR11D.h"
#include <stdlib.h>
#include <time.h>

MBL_CLASS_RXOR11D::MBL_CLASS_RXOR11D(char cXorKey, char *cString, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	for (int i = 0; i < iNumOfChars - 1; i++)
	{
		cString[i] ^= cString[i + 1];
	}
	cString[iNumOfChars - 1] ^= cXorKey;
}

MBL_CLASS_RXOR11D::MBL_CLASS_RXOR11D(wchar_t wcXorKey, wchar_t *wcString, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Xor
	for (int i = 0; i < iNumOfChars - 1; i++)
	{
		wcString[i] ^= wcString[i + 1];
	}
	wcString[iNumOfChars - 1] ^= wcXorKey;
}

MBL_CLASS_RXOR11D::~MBL_CLASS_RXOR11D(void)
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