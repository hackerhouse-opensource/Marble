#pragma once
#include <Windows.h>
#include "MBL_CLASS_RXOR1D.h"

MBL_CLASS_RXOR1D::MBL_CLASS_RXOR1D(char cXorKey, int iNumOfChars, char *cString)
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

MBL_CLASS_RXOR1D::MBL_CLASS_RXOR1D(wchar_t wcXorKey, int iNumOfChars, wchar_t *wcString)
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

MBL_CLASS_RXOR1D::~MBL_CLASS_RXOR1D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}