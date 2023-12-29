#pragma once
#include <Windows.h>
#include "MBL_CLASS_XOR10D.h"

MBL_CLASS_XOR10D::MBL_CLASS_XOR10D(char *cString, int iNumOfChars, char *cXorKey)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		cString[i] ^= cXorKey[i % 8];
}

MBL_CLASS_XOR10D::MBL_CLASS_XOR10D(wchar_t *wcString, int iNumOfChars, wchar_t *wcXorKey)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		wcString[i] ^= wcXorKey[i % 8];
}

MBL_CLASS_XOR10D::~MBL_CLASS_XOR10D(void)
{

	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);


	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}