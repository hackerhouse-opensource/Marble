#pragma once

static void MBL_FORLOOP_FUNC_RBUMP2D(char *cString, int iNumOfChars, char cXorKey)
{
	if (cString == 0 || iNumOfChars <= 0 || cXorKey == 0)
		return;

	for (int i = iNumOfChars - 1; i > 0; i--)
		cString[i] += cString[i - 1];

	cString[0] += cXorKey;

}

static void MBL_FORLOOP_FUNC_RBUMP2D(wchar_t *wcString, int iNumOfChars, wchar_t wcXorKey)
{
	if (wcString == 0 || iNumOfChars <= 0 || wcXorKey == 0)
		return;

	for (int i = iNumOfChars - 1; i > 0; i--)
		wcString[i] += wcString[i - 1];

	wcString[0] += wcXorKey;

}