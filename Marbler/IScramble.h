#pragma once

/*
	Convention of IScramble
*/



class IScramble 
{

public:

	/*
		IScramble Children will provide functions for scrambling WCHAR and CHAR strings
	*/
	IScramble(void) {}
	virtual ~IScramble(void) = 0 {}

	/*
		Args:
			wcToScramble[in]: is the buffer containing a Wide Char string to scramble
			iNumOfChars[in]: the number of WCHARs in the buffer

			Ret: > 0 == SUCCESS,  <=0 == FAILURE
	*/
	virtual int ScrambleW(wchar_t *wcToScramble, unsigned int iNumOfChars) = 0;

	/*
		Args:
			cToScramble[in]: is the buffer containing a char string to scramble
			iNumOfChars[in]: the number of CHARs in the buffer

			Ret: > 0 == SUCCESS,  <=0 == FAILURE
	*/
	virtual int ScrambleA(char *cToScramble, unsigned int NumOfChars) = 0;


	/*
		Args:
			cVarName[in]: the name of the variable being replaced
			cStringLiteral[in]: the string literal to be added to the insert (after scrambling)
			iNumOfChars[in]: the number of characters in the buffer
			cInsert[out]: the insert to replace CARBLE\BARBLE declaration in the c/cpp file

			Ret: > 0 == SUCCESS,  <=0 == FAILURE
	*/
	virtual int GenerateInsertA(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert) = 0;
	
	/*
		Args:
			cVarName[in]: the name of the variable being replaced
			wcStringLiteral[in]: the string literal to be added to the insert (after scrambling)
			iNumOfChars[in]: the number of characters in the buffer
			cInsert[out]: the insert to replace WARBLE declaration in the c/cpp file

			Ret: > 0 == SUCCESS,  <=0 == FAILURE
	*/
	virtual int GenerateInsertW(char *cVarName, char *cStringLiteral, unsigned int iNumOfChars, char *&cInsert) = 0;

	/*
		Args:
			cBuffer[in]: byte or char buffer to be converted
			iNumOfChars[in]: number of characters in the provided buffer
			cOutput[out]: the returned output is the string literal of the buffer

			Ret: The number of characters returned in the output string

			Creates a string level from a char strings or byte buffer. (Example Result:  0x00, 0x01, 0x02 );
	*/
	int CreateStringLiteralA(unsigned char *cBuffer, int iNumOfChars, char *&cOutput);

	/*
		Args:
			cBuffer[in]: byte or char buffer to be converted
			iNumOfChars[in]: number of characters in the provided buffer
			cOutput[out]: the returned output is the string literal of the buffer

			Ret: The number of characters returned in the output string

			Creates a string level from a wchar string. (Example Result:  0x0000, 0x0001, 0x0002 );
	*/
	int CreateStringLiteralW(wchar_t *wcBuffer, int iNumOfChars, char *&cOutput);

};