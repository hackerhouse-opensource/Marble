#pragma once
#include <Windows.h>
#include "MBL_CLASS_RBUMP2D.h"

MBL_CLASS_RBUMP2D::MBL_CLASS_RBUMP2D(char cXorKey, int iNumOfChars, char *cString)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Bump
	for (int i = iNumOfChars - 1; i > 0; i--)
		cString[i] -= cString[i - 1];
	cString[0] -= cXorKey;
}

MBL_CLASS_RBUMP2D::MBL_CLASS_RBUMP2D(wchar_t wcXorKey, int iNumOfChars, wchar_t *wcString)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Bump
	for (int i = iNumOfChars - 1; i > 0; i--)
		wcString[i] -= wcString[i - 1];
	wcString[0] -= wcXorKey;

}

MBL_CLASS_RBUMP2D::~MBL_CLASS_RBUMP2D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}