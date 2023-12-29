#pragma once

class MBL_CLASS_XOR3D
{
public:

	MBL_CLASS_XOR3D(char cXorKey, char *cString, int iNumOfChars);
	MBL_CLASS_XOR3D(wchar_t wcXorKey, wchar_t *wcString, int iNumOfChars);
	~MBL_CLASS_XOR3D(void);

protected:
	char *cStringPointer;
	int iCStringPointerLen;
	wchar_t *wcStringPointer;
	int iWStringPointerLen;

};
