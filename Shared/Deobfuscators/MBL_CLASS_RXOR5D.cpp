#pragma once
#include <Windows.h>
#include "MBL_CLASS_RXOR5D.h"

MBL_CLASS_RXOR5D::MBL_CLASS_RXOR5D(void)
{
	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;
}

MBL_CLASS_RXOR5D::~MBL_CLASS_RXOR5D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}

void MBL_CLASS_RXOR5D::Scramble(char *cString, char cXorKey, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	//Xor
	for (int i = iNumOfChars - 1; i > 0; i--)
		cString[i] ^= cString[i - 1];
	cString[0] ^= cXorKey;
}

void MBL_CLASS_RXOR5D::Scramble(wchar_t *wcString, wchar_t wcXorKey, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	//Xor
	for (int i = iNumOfChars - 1; i > 0; i--)
		wcString[i] ^= wcString[i - 1];
	wcString[0] ^= wcXorKey;
}

