#pragma once
#include <Windows.h>
#include "MBL_CLASS_RBUMP1D.h"

MBL_CLASS_RBUMP1D::MBL_CLASS_RBUMP1D(char cXorKey, int iNumOfChars, char *cString)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Bump
	char cBump = cString[0];
	for (int i = 1; i < iNumOfChars; i++)
	{
		char cTempChar = cString[i];
		cString[i] -= cBump;
		cBump = cTempChar;

	}
	cString[0] -= cXorKey;
}

MBL_CLASS_RBUMP1D::MBL_CLASS_RBUMP1D(wchar_t wcXorKey, int iNumOfChars, wchar_t *wcString)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Bump
	wchar_t wcBump = wcString[0];
	for (int i = 1; i < iNumOfChars; i++)
	{
		wchar_t wcTempChar = wcString[i];
		wcString[i] -= wcBump;
		wcBump = wcTempChar;

	}
	wcString[0] -= wcXorKey;
	
}

MBL_CLASS_RBUMP1D::~MBL_CLASS_RBUMP1D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}