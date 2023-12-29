#pragma once
#include <Windows.h>
#include "MBL_CLASS_BUMP9D.h"

MBL_CLASS_BUMP9D::MBL_CLASS_BUMP9D(char *cXorKey, char *cString, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		cString[i] -= cXorKey[i % 8];
}

MBL_CLASS_BUMP9D::MBL_CLASS_BUMP9D(wchar_t *wcXorKey, wchar_t *wcString, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		wcString[i] -= wcXorKey[i % 8];
}

MBL_CLASS_BUMP9D::~MBL_CLASS_BUMP9D(void)
{
	
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);


	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}