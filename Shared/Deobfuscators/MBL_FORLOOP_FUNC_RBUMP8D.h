#pragma once

static void MBL_FORLOOP_FUNC_RBUMP8D(char *cString, int iNumOfChars, char cXorKey)
{
	if (cString == 0 || iNumOfChars <= 0 || cXorKey == 0)
		return;

	for (int i = 0; i < iNumOfChars - 1; i++)
		cString[i] += cString[i + 1];

	cString[iNumOfChars - 1] -= cXorKey;

}

static void MBL_FORLOOP_FUNC_RBUMP8D(wchar_t *wcString, int iNumOfChars, wchar_t wcXorKey)
{
	if (wcString == 0 || iNumOfChars <= 0 || wcXorKey == 0)
		return;

	for (int i = 0; i < iNumOfChars - 1; i++)
		wcString[i] += wcString[i + 1];

	wcString[iNumOfChars - 1] -= wcXorKey;

}