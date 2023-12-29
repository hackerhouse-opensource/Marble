#pragma once
#include <Windows.h>
#include "MBL_CLASS_RBUMP11D.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

MBL_CLASS_RBUMP11D::MBL_CLASS_RBUMP11D(char *cString, int iNumOfChars, char cXorKey)
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
		cString[i] += cBump;
		cBump = cTempChar;

	}
	cString[0] -= cXorKey;
}

MBL_CLASS_RBUMP11D::MBL_CLASS_RBUMP11D(wchar_t *wcString, int iNumOfChars, wchar_t wcXorKey)
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
		wcString[i] += wcBump;
		wcBump = wcTempChar;

	}
	wcString[0] -= wcXorKey;

}

MBL_CLASS_RBUMP11D::~MBL_CLASS_RBUMP11D(void)
{
	srand((DWORD) time(NULL));

	if (cStringPointer)
	{
		for (int i = 0; i < iCStringPointerLen; i++)
			cStringPointer[i] = (unsigned char)rand() % 255;
	}


	if (wcStringPointer)
	{
		for (int i = 0; i < iWStringPointerLen; i++)
			wcStringPointer[i] = rand() % 65534;
	}
}