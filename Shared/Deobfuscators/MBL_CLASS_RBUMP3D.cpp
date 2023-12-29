#pragma once
#include <Windows.h>
#include "MBL_CLASS_RBUMP3D.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

MBL_CLASS_RBUMP3D::MBL_CLASS_RBUMP3D(char cXorKey, int iNumOfChars, char *cString)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Bump
	for (int i = 0; i < iNumOfChars - 1; i++)
		cString[i] += cString[i + 1];
	cString[iNumOfChars - 1] += cXorKey;
}

MBL_CLASS_RBUMP3D::MBL_CLASS_RBUMP3D(wchar_t wcXorKey, int iNumOfChars, wchar_t *wcString)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Bump
	for (int i = 0; i < iNumOfChars - 1; i++)
		wcString[i] += wcString[i + 1];
	wcString[iNumOfChars - 1] += wcXorKey;

}

MBL_CLASS_RBUMP3D::~MBL_CLASS_RBUMP3D(void)
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