#pragma once
#include <Windows.h>
#include "MBL_CLASS_XOR1D.h"

MBL_CLASS_XOR1D::MBL_CLASS_XOR1D(char cXorKey, char *cString, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		cString[i] ^= cXorKey;
}

MBL_CLASS_XOR1D::MBL_CLASS_XOR1D(wchar_t wcXorKey, wchar_t *wcString, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		wcString[i] ^= wcXorKey;
}

MBL_CLASS_XOR1D::~MBL_CLASS_XOR1D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}