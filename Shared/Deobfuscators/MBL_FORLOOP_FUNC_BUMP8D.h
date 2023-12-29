#pragma once

static void MBL_FORLOOP_FUNC_BUMP8D(char *cString, int iNumOfChars, char *cXorKey)
{
	if (cString == 0 || iNumOfChars <= 0 || cXorKey == 0)
		return;

	for (int i = iNumOfChars - 1; i >= 0; i--)
		cString[i] += cXorKey[i % 8];

}

static void MBL_FORLOOP_FUNC_BUMP8D(wchar_t *wcString, int iNumOfChars, wchar_t *wcXorKey)
{
	if (wcString == 0 || iNumOfChars <= 0 || wcXorKey == 0)
		return;
	for (int i = iNumOfChars - 1; i >= 0; i--)
		wcString[i] += wcXorKey[i % 8];

}