#pragma once
#include <Windows.h>
#include "MBL_CLASS_RBUMP4D.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

MBL_CLASS_RBUMP4D::MBL_CLASS_RBUMP4D(char cXorKey, int iNumOfChars, char *cString)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Bump
	char cBump = cString[iNumOfChars - 1];
	for (int i = iNumOfChars - 2; i >= 0; i--)
	{
		char cTempChar = cString[i];
		cString[i] += cBump;
		cBump = cTempChar;

	}
	cString[iNumOfChars - 1] += cXorKey;
}

MBL_CLASS_RBUMP4D::MBL_CLASS_RBUMP4D(wchar_t wcXorKey, int iNumOfChars, wchar_t *wcString)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Bump
	wchar_t wcBump = wcString[iNumOfChars - 1];
	for (int i = iNumOfChars - 2; i >= 0; i--)
	{
		wchar_t wcTempChar = wcString[i];
		wcString[i] += wcBump;
		wcBump = wcTempChar;

	}
	wcString[iNumOfChars - 1] += wcXorKey;

}

MBL_CLASS_RBUMP4D::~MBL_CLASS_RBUMP4D(void)
{
	srand((DWORD) time(NULL));

	if (cStringPointer)
	{
		for (int i = iCStringPointerLen - 1; i >= 0; i--)
			cStringPointer[i] = (unsigned char)rand() % 255;
	}


	if (wcStringPointer)
	{
		for (int i = iWStringPointerLen - 1; i >= 0; i--)
			wcStringPointer[i] = rand() % 65534;
	}
}