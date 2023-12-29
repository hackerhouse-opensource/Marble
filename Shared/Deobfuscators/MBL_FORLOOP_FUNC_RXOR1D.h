#pragma once

static void MBL_FORLOOP_FUNC_RXOR1D(char *cString, int iNumOfChars, char cXorKey)
{
	if (cString == 0 || iNumOfChars <= 0 || cXorKey == 0)
		return;

	char cXor = cString[0];
	cString[0] ^= cXorKey;
	for (int i = 1; i < iNumOfChars; i++)
	{
		char cTemp = cString[i];
		cString[i] ^= cXor;
		cXor = cTemp;
	}
}

static void MBL_FORLOOP_FUNC_RXOR1D(wchar_t *wcString, int iNumOfChars, wchar_t wcXorKey)
{
	if (wcString == 0 || iNumOfChars <= 0 || wcXorKey == 0)
		return;

	wchar_t wcXor = wcString[0];
	wcString[0] ^= wcXorKey;
	for (int i = 1; i < iNumOfChars; i++)
	{
		wchar_t wcTemp = wcString[i];
		wcString[i] ^= wcXor;
		wcXor = wcTemp;
	}

}