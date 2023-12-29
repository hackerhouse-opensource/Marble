#pragma once

class MBL_CLASS_RBUMP6D
{
public:

	MBL_CLASS_RBUMP6D(void);
	void Scramble(char cXorKey, char *cString, int iNumOfChars);
	void Scramble(wchar_t wcXorKey, wchar_t *wcString, int iNumOfChars);
	~MBL_CLASS_RBUMP6D(void);

protected:
	char *cStringPointer;
	int iCStringPointerLen;
	wchar_t *wcStringPointer;
	int iWStringPointerLen;

};
