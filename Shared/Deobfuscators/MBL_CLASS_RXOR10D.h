#pragma once

class MBL_CLASS_RXOR10D
{
public:

	MBL_CLASS_RXOR10D(char cXorKey, char *cString, int iNumOfChars);
	MBL_CLASS_RXOR10D(wchar_t wcXorKey, wchar_t *wcString, int iNumOfChars);
	~MBL_CLASS_RXOR10D(void);

protected:
	char *cStringPointer;
	int iCStringPointerLen;
	wchar_t *wcStringPointer;
	int iWStringPointerLen;

};
