#pragma once
#include <Windows.h>
#include "MBL_CLASS_RXOR6D.h"

MBL_CLASS_RXOR6D::MBL_CLASS_RXOR6D(void)
{

}

MBL_CLASS_RXOR6D::~MBL_CLASS_RXOR6D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}

void MBL_CLASS_RXOR6D::Scramble(char *cString, char cXorKey, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	char cXor =  cString[0];
	for (int i = 1; i < iNumOfChars; i++)
	{
		char cTemp = cString[i];
		cString[i] ^= cXor;
		cXor = cTemp;
	}
	cString[0] ^= cXorKey;
}

void MBL_CLASS_RXOR6D::Scramble(wchar_t *wcString, wchar_t wcXorKey, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Xor
	wchar_t wcXor = wcString[0];
	for (int i = 1; i < iNumOfChars; i++)
	{
		wchar_t wcTemp = wcString[i];
		wcString[i] ^= wcXor;
		wcXor = wcTemp;
	}
	wcString[0] ^= wcXorKey;
}

