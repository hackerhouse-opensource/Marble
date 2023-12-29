#pragma once
#include <Windows.h>
#include "MBL_CLASS_BUMP6D.h"

MBL_CLASS_BUMP6D::MBL_CLASS_BUMP6D(void)
{
	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;
}

MBL_CLASS_BUMP6D::~MBL_CLASS_BUMP6D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}

void MBL_CLASS_BUMP6D::Scramble(char *cString, int iNumOfChars, char cXorKey)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		cString[i] -= cXorKey;
}

void MBL_CLASS_BUMP6D::Scramble(wchar_t *wcString, int iNumOfChars, wchar_t wcXorKey)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	//Xor
	for (int i = iNumOfChars - 1; i >= 0; i--)
		wcString[i] -= wcXorKey;
}

