#pragma once
#include <Windows.h>
#include "MBL_CLASS_XOR3D.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

MBL_CLASS_XOR3D::MBL_CLASS_XOR3D(char cXorKey, char *cString, int iNumOfChars)
{
	cStringPointer = cString;
	iCStringPointerLen = iNumOfChars;

	wcStringPointer = NULL;
	iWStringPointerLen = NULL;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		cString[i] ^= cXorKey;
}

MBL_CLASS_XOR3D::MBL_CLASS_XOR3D(wchar_t wcXorKey, wchar_t *wcString, int iNumOfChars)
{
	wcStringPointer = wcString;
	iWStringPointerLen = iNumOfChars;

	cStringPointer = 0;
	iCStringPointerLen = 0;

	//Xor
	for (int i = 0; i < iNumOfChars; i++)
		wcString[i] ^= wcXorKey;
}

MBL_CLASS_XOR3D::~MBL_CLASS_XOR3D(void)
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