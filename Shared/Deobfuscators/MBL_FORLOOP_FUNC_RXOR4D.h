#pragma once

static void MBL_FORLOOP_FUNC_RXOR4D(char *cString, int iNumOfChars, char cXorKey)
{
	if (cString == 0 || iNumOfChars <= 0 || cXorKey == 0)
		return;

	char cXor = cString[iNumOfChars - 1];

	for (int i = iNumOfChars - 2; i >= 0; i--)
	{
		char cTemp = cString[i];
		cString[i] ^= cXor;
		cXor = cTemp;
	}

	cString[iNumOfChars - 1] ^= cXorKey;
}

static void MBL_FORLOOP_FUNC_RXOR4D(wchar_t *wcString, int iNumOfChars, wchar_t wcXorKey)
{
	if (wcString == 0 || iNumOfChars <= 0 || wcXorKey == 0)
		return;

	wchar_t wcXor = wcString[iNumOfChars - 1];

	for (int i = iNumOfChars - 2; i >= 0; i--)
	{
		wchar_t wcTemp = wcString[i];
		wcString[i] ^= wcXor;
		wcXor = wcTemp;
	}

	wcString[iNumOfChars - 1] ^= wcXorKey;

}