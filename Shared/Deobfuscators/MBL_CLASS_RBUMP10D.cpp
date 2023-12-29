#pragma once
#include <Windows.h>
#include "MBL_CLASS_RBUMP10D.h"

MBL_CLASS_RBUMP10D::MBL_CLASS_RBUMP10D(char *cString, int iNumOfChars, char cXorKey)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Bump
	for (int i = iNumOfChars - 1; i > 0; i--)
		cString[i] -= cString[i - 1];
	cString[0] += cXorKey;
}

MBL_CLASS_RBUMP10D::MBL_CLASS_RBUMP10D(wchar_t *wcString, int iNumOfChars, wchar_t wcXorKey)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Bump
	for (int i = iNumOfChars - 1; i > 0; i--)
		wcString[i] -= wcString[i - 1];
	wcString[0] += wcXorKey;

}

MBL_CLASS_RBUMP10D::~MBL_CLASS_RBUMP10D(void)
{
	if (cStringPointer)
		SecureZeroMemory(cStringPointer, iCStringPointerLen);

	if (wcStringPointer)
		SecureZeroMemory(wcStringPointer, iWStringPointerLen);
}