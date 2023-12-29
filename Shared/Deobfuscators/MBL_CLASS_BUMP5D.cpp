#pragma once
#include <Windows.h>
#include "MBL_CLASS_BUMP5D.h"

MBL_CLASS_BUMP5D::MBL_CLASS_BUMP5D(void)
{
	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;
}

MBL_CLASS_BUMP5D::~MBL_CLASS_BUMP5D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}

void MBL_CLASS_BUMP5D::Scramble(char *cString, int iNumOfChars, char cXorKey)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		cString[i] -= cXorKey;
}

void MBL_CLASS_BUMP5D::Scramble(wchar_t *wcString, int iNumOfChars, wchar_t wcXorKey)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		wcString[i] -= wcXorKey;
}

