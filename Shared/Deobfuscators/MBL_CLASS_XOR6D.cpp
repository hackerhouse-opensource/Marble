#pragma once
#include <Windows.h>
#include "MBL_CLASS_XOR6D.h"

MBL_CLASS_XOR6D::MBL_CLASS_XOR6D(void)
{
	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;
}

MBL_CLASS_XOR6D::~MBL_CLASS_XOR6D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}

void MBL_CLASS_XOR6D::Scramble(char cXorKey, int iNumOfChars, char *cString)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		cString[i] ^= cXorKey;
}

void MBL_CLASS_XOR6D::Scramble(wchar_t wcXorKey, int iNumOfChars, wchar_t *wcString)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		wcString[i] ^= wcXorKey;
}

