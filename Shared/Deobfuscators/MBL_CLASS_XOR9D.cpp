#pragma once
#include <Windows.h>
#include "MBL_CLASS_XOR9D.h"

MBL_CLASS_XOR9D::MBL_CLASS_XOR9D(char *cString, int iNumOfChars, char *cXorKey)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		cString[i] ^= cXorKey[i % 8];
}

MBL_CLASS_XOR9D::MBL_CLASS_XOR9D(wchar_t *wcString, int iNumOfChars, wchar_t *wcXorKey)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		wcString[i] ^= wcXorKey[i % 8];
}

MBL_CLASS_XOR9D::~MBL_CLASS_XOR9D(void)
{
	
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);


	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}