#pragma once
#include <Windows.h>
#include "MBL_CLASS_BUMP1D.h"

MBL_CLASS_BUMP1D::MBL_CLASS_BUMP1D(char cXorKey, int iNumOfChars, char *cString)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		cString[i] -= cXorKey;
}

MBL_CLASS_BUMP1D::MBL_CLASS_BUMP1D(wchar_t wcXorKey, int iNumOfChars, wchar_t *wcString)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		wcString[i] -= wcXorKey;
}

MBL_CLASS_BUMP1D::~MBL_CLASS_BUMP1D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}