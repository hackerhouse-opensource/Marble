#pragma once
#include <Windows.h>
#include "MBL_CLASS_RXOR8D.h"
#include <stdlib.h>
#include <time.h>


MBL_CLASS_RXOR8D::MBL_CLASS_RXOR8D(void)
{
	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;
}

MBL_CLASS_RXOR8D::~MBL_CLASS_RXOR8D(void)
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

void MBL_CLASS_RXOR8D::Scramble(char *cString, char cXorKey, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	//Xor
	char cXor = cString[0];
	for (int i = 1; i < iNumOfChars; i++)
	{
		char cTemp = cString[i];
		cString[i] ^= cXor;
		cXor = cTemp;
	}
	cString[0] ^= cXorKey;
}

void MBL_CLASS_RXOR8D::Scramble(wchar_t *wcString, wchar_t wcXorKey, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	//Xor
	wchar_t wcXor = wcString[0];
	for (int i = 1; i < iNumOfChars; i++)
	{
		wchar_t wcTemp = wcString[i];
		wcString[i] ^= wcXor;
		wcXor = wcTemp;
	}
	wcString[0] ^= wcXorKey;
}