#pragma once

class MBL_CLASS_BUMP11D
{
public:

	MBL_CLASS_BUMP11D(char *cXorKey, char *cString, int iNumOfChars);
	MBL_CLASS_BUMP11D(wchar_t *wcXorKey, wchar_t *wcString, int iNumOfChars);
	~MBL_CLASS_BUMP11D(void);

protected:
	char *cStringPointer;
	int iCStringPointerLen;
	wchar_t *wcStringPointer;
	int iWStringPointerLen;

};
