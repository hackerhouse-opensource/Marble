#pragma once
#include "IScramble.h"

class MBL_FORLOOP_XOR4 : public IScramble
{
public:

	MBL_FORLOOP_XOR4(void);
	~MBL_FORLOOP_XOR4(void);

	/*
		Args:
			wcToScramble[in]: is the buffer containing a Wide Char string to scramble
			iNumOfChars[in]: the number of WCHARs in the buffer
	*/
	int ScrambleW(wchar_t *wcToScramble, unsigned int iNumOfChars);

	/*
		Args:
			cToScramble[in]: is the buffer containing a char string to scramble
			iNumOfChars[in]: the number of CHARs in the buffer
	*/
	int ScrambleA(char *cToScramble, unsigned int NumOfChars);


	/*
		Args:
			cVarName[in]: the name of the variable being replaced
			cStringLiteral[in]: the string literal to be added to the insert (after scrambling)
			iNumOfChars[in]: the number of characters in the buffer
			cInsert[out]: the insert to replace CARBLE\BARBLE declaration in the c/cpp file
	*/
	int GenerateInsertA(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert);

	/*
		Args:
			cVarName[in]: the name of the variable being replaced
			wcStringLiteral[in]: the string literal to be added to the insert (after scrambling)
			iNumOfChars[in]: the number of characters in the buffer
			cInsert[out]: the insert to replace WARBLE declaration in the c/cpp file
	*/
	int GenerateInsertW(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert);

protected:
	wchar_t wcKey[8];
	char *cWKeyLiteral;
	unsigned char cKey[8];
	char *cKeyLiteral;

};
