#pragma once
#include <Windows.h>
#include "MBL_CLASS_BUMP10D.h"

MBL_CLASS_BUMP10D::MBL_CLASS_BUMP10D(char *cXorKey, char *cString, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		cString[i] -= cXorKey[i % 8];
}

MBL_CLASS_BUMP10D::MBL_CLASS_BUMP10D(wchar_t *wcXorKey, wchar_t *wcString, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		wcString[i] -= wcXorKey[i % 8];
}

MBL_CLASS_BUMP10D::~MBL_CLASS_BUMP10D(void)
{

	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);


	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}