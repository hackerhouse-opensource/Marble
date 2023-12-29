#pragma once

class MBL_CLASS_XOR7D
{
public:

	MBL_CLASS_XOR7D(void);
	~MBL_CLASS_XOR7D(void);

	void Scramble(char cXorKey, int iNumOfChars, char *cString);
	void Scramble(wchar_t wcXorKey, int iNumOfChars, wchar_t *wcString);

protected:
	char *cStringPointer;
	int iCStringPointerLen;
	wchar_t *wcStringPointer;
	int iWStringPointerLen;

};
