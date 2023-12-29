#pragma once

class MBL_CLASS_BUMP8D
{
public:

	MBL_CLASS_BUMP8D(void);
	~MBL_CLASS_BUMP8D(void);
	void Scramble(char *cString, int iNumOfChars, char cXorKey);
	void Scramble(wchar_t *wcString, int iNumOfChars, wchar_t wcXorKey);

protected:
	char *cStringPointer;
	int iCStringPointerLen;
	wchar_t *wcStringPointer;
	int iWStringPointerLen;

};
