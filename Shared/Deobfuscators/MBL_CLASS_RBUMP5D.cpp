#pragma once
#include <Windows.h>
#include "MBL_CLASS_RBUMP5D.h"

MBL_CLASS_RBUMP5D::MBL_CLASS_RBUMP5D(void)
{
	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	cStringPointer = NULL;
	iCStringPointerLen = NULL;
}

MBL_CLASS_RBUMP5D::~MBL_CLASS_RBUMP5D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}

void MBL_CLASS_RBUMP5D::Scramble(char cXorKey, char *cString, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

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

void MBL_CLASS_RBUMP5D::Scramble(wchar_t wcXorKey, wchar_t *wcString, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

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