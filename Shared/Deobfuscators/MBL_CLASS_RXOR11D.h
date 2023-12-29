#pragma once

class MBL_CLASS_RXOR11D
{
public:

	MBL_CLASS_RXOR11D(char cXorKey, char *cString, int iNumOfChars);
	MBL_CLASS_RXOR11D(wchar_t wcXorKey, wchar_t *wcString, int iNumOfChars);
	~MBL_CLASS_RXOR11D(void);

protected:
	char *cStringPointer;
	int iCStringPointerLen;
	wchar_t *wcStringPointer;
	int iWStringPointerLen;

};
