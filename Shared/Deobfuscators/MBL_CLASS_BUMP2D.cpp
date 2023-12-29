#pragma once
#include <Windows.h>
#include "MBL_CLASS_BUMP2D.h"

MBL_CLASS_BUMP2D::MBL_CLASS_BUMP2D(char cXorKey, int iNumOfChars, char *cString)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		cString[i] -= cXorKey;
}

MBL_CLASS_BUMP2D::MBL_CLASS_BUMP2D(wchar_t wcXorKey, int iNumOfChars, wchar_t *wcString)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		wcString[i] -= wcXorKey;
}

MBL_CLASS_BUMP2D::~MBL_CLASS_BUMP2D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}