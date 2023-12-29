#pragma once

class MBL_CLASS_RBUMP11D
{
public:

	MBL_CLASS_RBUMP11D(char *cString, int iNumOfChars, char cXorKey);
	MBL_CLASS_RBUMP11D(wchar_t *wcString, int iNumOfChars, wchar_t wcXorKey);
	~MBL_CLASS_RBUMP11D(void);

protected:
	char *cStringPointer;
	int iCStringPointerLen;
	wchar_t *wcStringPointer;
	int iWStringPointerLen;

};
