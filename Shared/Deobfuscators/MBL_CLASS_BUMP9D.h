#pragma once

class MBL_CLASS_BUMP9D
{
public:

	MBL_CLASS_BUMP9D(char *cXorKey, char *cString, int iNumOfChars);
	MBL_CLASS_BUMP9D(wchar_t *wcXorKey, wchar_t *wcString, int iNumOfChars);
	~MBL_CLASS_BUMP9D(void);

protected:
	char *cStringPointer;
	int iCStringPointerLen;
	wchar_t *wcStringPointer;
	int iWStringPointerLen;

};
