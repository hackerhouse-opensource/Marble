#pragma once
#include <Windows.h>
#include "MBL_CLASS_RBUMP6D.h"

MBL_CLASS_RBUMP6D::MBL_CLASS_RBUMP6D(void)
{
	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;
}

MBL_CLASS_RBUMP6D::~MBL_CLASS_RBUMP6D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}

void MBL_CLASS_RBUMP6D::Scramble(char cXorKey, char *cString, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	//Bump
	for (int i = iNumOfChars - 1; i > 0; i--)
		cString[i] -= cString[i - 1];
	cString[0] -= cXorKey;
}

void MBL_CLASS_RBUMP6D::Scramble(wchar_t wcXorKey, wchar_t *wcString, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	//Bump
	for (int i = iNumOfChars - 1; i > 0; i--)
		wcString[i] -= wcString[i - 1];
	wcString[0] -= wcXorKey;
}