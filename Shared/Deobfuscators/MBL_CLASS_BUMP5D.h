#pragma once

class MBL_CLASS_BUMP5D
{
public:

	MBL_CLASS_BUMP5D(void);
	~MBL_CLASS_BUMP5D(void);
	void Scramble(char *cString, int iNumOfChars, char cXorKey);
	void Scramble(wchar_t *wcString, int iNumOfChars, wchar_t wcXorKey);
	
protected:
	char *cStringPointer;
	int iCStringPointerLen;
	wchar_t *wcStringPointer;
	int iWStringPointerLen;

};
