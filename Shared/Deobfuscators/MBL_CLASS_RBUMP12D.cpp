#pragma once
#include <Windows.h>
#include "MBL_CLASS_RBUMP12D.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

MBL_CLASS_RBUMP12D::MBL_CLASS_RBUMP12D(char *cString, int iNumOfChars, char cXorKey)
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

MBL_CLASS_RBUMP12D::MBL_CLASS_RBUMP12D(wchar_t *wcString, int iNumOfChars, wchar_t wcXorKey)
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

MBL_CLASS_RBUMP12D::~MBL_CLASS_RBUMP12D(void)
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