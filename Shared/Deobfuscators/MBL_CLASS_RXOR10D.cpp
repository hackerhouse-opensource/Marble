#pragma once
#include <Windows.h>
#include "MBL_CLASS_RXOR10D.h"

MBL_CLASS_RXOR10D::MBL_CLASS_RXOR10D(char cXorKey, char *cString, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	CHAR cXorByte = cString[iNumOfChars - 1];
	cString[iNumOfChars - 1] ^= cXorKey;
	for (int i = iNumOfChars - 2; i >= 0; i--)
	{
		CHAR cTemp = cString[i];
		cString[i] ^= cXorByte;
		cXorByte = cTemp;
	}
	
}

MBL_CLASS_RXOR10D::MBL_CLASS_RXOR10D(wchar_t wcXorKey, wchar_t *wcString, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Xor
	WCHAR wcXorByte = wcString[iNumOfChars - 1];
	wcString[iNumOfChars - 1] ^= wcXorKey;
	for (int i = iNumOfChars - 2; i >= 0; i--)
	{
		WCHAR wcTemp = wcString[i];
		wcString[i] ^= wcXorByte;
		wcXorByte = wcTemp;
	}
}

MBL_CLASS_RXOR10D::~MBL_CLASS_RXOR10D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}