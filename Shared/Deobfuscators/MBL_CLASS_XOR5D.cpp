#pragma once
#include <Windows.h>
#include "MBL_CLASS_XOR5D.h"

MBL_CLASS_XOR5D::MBL_CLASS_XOR5D(void)
{
	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;
}

MBL_CLASS_XOR5D::~MBL_CLASS_XOR5D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}

void MBL_CLASS_XOR5D::Scramble(char cXorKey, int iNumOfChars, char *cString)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		cString[i] ^= cXorKey;
}

void MBL_CLASS_XOR5D::Scramble(wchar_t wcXorKey, int iNumOfChars, wchar_t *wcString)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		wcString[i] ^= wcXorKey;
}