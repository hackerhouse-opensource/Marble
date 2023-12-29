#pragma once

static void MBL_FORLOOP_FUNC_BUMP2D(char *cString, int iNumOfChars, char cXorKey)
{
	if (cString == 0 || iNumOfChars <= 0 || cXorKey == 0)
		return;

	for (int i = iNumOfChars - 1; i >= 0; i--)
		cString[i] -= cXorKey;

}

static void MBL_FORLOOP_FUNC_BUMP2D(wchar_t *wcString, int iNumOfChars, wchar_t wcXorKey)
{
	if (wcString == 0 || iNumOfChars <= 0 || wcXorKey == 0)
		return;
	for (int i = iNumOfChars - 1; i >= 0; i--)
		wcString[i] -= wcXorKey;

}