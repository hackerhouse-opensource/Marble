#pragma once
#include <Windows.h>
#include "MBL_CLASS_RXOR2D.h"

MBL_CLASS_RXOR2D::MBL_CLASS_RXOR2D(char cXorKey, int iNumOfChars, char *cString)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	char cXor =  cString[0];
	for (int i = 1; i < iNumOfChars; i++)
	{
		char cTemp = cString[i];
		cString[i] ^= cXor;
		cXor = cTemp;
	}
	cString[0] ^= cXorKey;
}

MBL_CLASS_RXOR2D::MBL_CLASS_RXOR2D(wchar_t wcXorKey, int iNumOfChars, wchar_t *wcString)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

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

MBL_CLASS_RXOR2D::~MBL_CLASS_RXOR2D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}