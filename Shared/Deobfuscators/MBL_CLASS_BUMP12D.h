#pragma once

class MBL_CLASS_BUMP12D
{
public:

	MBL_CLASS_BUMP12D(char *cXorKey, char *cString, int iNumOfChars);
	MBL_CLASS_BUMP12D(wchar_t *wcXorKey, wchar_t *wcString, int iNumOfChars);
	~MBL_CLASS_BUMP12D(void);

protected:
	char *cStringPointer;
	int iCStringPointerLen;
	wchar_t *wcStringPointer;
	int iWStringPointerLen;

};
