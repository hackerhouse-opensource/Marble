#pragma once

class MBL_CLASS_XOR4D
{
public:

	MBL_CLASS_XOR4D(char cXorKey, char *cString, int iNumOfChars);
	MBL_CLASS_XOR4D(wchar_t wcXorKey, wchar_t *wcString, int iNumOfChars);
	~MBL_CLASS_XOR4D(void);

protected:
	char *cStringPointer;
	int iCStringPointerLen;
	wchar_t *wcStringPointer;
	int iWStringPointerLen;

};
