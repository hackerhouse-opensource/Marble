

/*
* Marble is a set of files inteded to provide randomized obfuscation for strings and byte
* arrays. Both the implemenation and key will be randomized on Release build (unless it
* is explicitly required that it be done on Debug).
*
*
*
*
*
*
* Framework Setup: Marbler
*
*
*
*
*
*
*
*
*
*
*/

typedef wchar_t WARBLE;			//For Obfuscating Wide-Char Arrays
typedef unsigned char CARBLE;	//For Obfuscating Char Arrays
typedef unsigned char BARBLE;	//For Obfuscating Byte Arrays


#define NOCHANGE
#define FARBLE1


//----------------------------Define filters for the pool of obfuscation techniques--------------------------

/*
	Define CLEANUPSONLY if you wish to only choose from the pool of obfuscation techniques that cleanup after themselves by clearing or reobfuscating the variable
*/
//#define CLEANUPSONLY //Always use forward slashes to comment out this define

/*
	Define NOCPP if you wish to only choose from the pool of obfuscation techniques that do not/not pull in the C++ runtime. 
*/
//#define NOCPP //Always use forward slashes to comment out this define




/*
	Include a specific algorithm to use - if none are specified a random one will be chosen. If specific
	algorithms are to be avoided either remove them from the header file or put a -- in front of the
	include statement. Example: //--#include "MBLObfuscationAlgorithm1.h". To specify a specific algorithm
	you may also just remove the comments preceding the include.

	No double forward slash preceding = Use this algorithm
	Two forward slashes preceding = Include algorithm in pool to choose from
	Two forward slashes and two hyphens = Exclude algorithm from pool
	Delete include statement = Algorithm not included in pool


	All algorithms within defines (NOCPP, CLEANUPSONLY) will be parsed for exclusion and inclusion.
*/

#ifndef NOCPP

//----------------------------------- C++ Obfuscation Algorithms--------------------------

//Class random key forward through array, constructor only, private variable, zero clear
//#include "MBL_CLASS_XOR1D.h"

//Class random key backwards through array, constructor only, private variable, zero clear
//#include "MBL_CLASS_XOR2D.h"

//Class random key forward through array, constructor only, private variable, random clear
//#include "MBL_CLASS_XOR3D.h"

//Class random key backwards through array, constructor only, private variable, random clear
//#include "MBL_CLASS_XOR4D.h"

//Class random key forward through array, constructor only, separate function, private variable, zero clear
//#include "MBL_CLASS_XOR5D.h"

//Class random key backwards through array, constructor only, separate function, private variable, zero clear
//#include "MBL_CLASS_XOR6D.h"

//Class random key forward through array, constructor only, separate function, private variable, random clear
//#include "MBL_CLASS_XOR7D.h"

//Class random key backwards through array, constructor only, separate function, private variable, random clear
//#include "MBL_CLASS_XOR8D.h"

//Class random 8-byte key forward through array, constructor only, public variable, zero clear
//#include "MBL_CLASS_XOR9D.h"

//Class random 8-byte key backwards through array, constructor only, public variable, zero clear
//#include "MBL_CLASS_XOR10D.h"

//Class random 8-byte key forward through array, constructor only, public variable, random clear
//#include "MBL_CLASS_XOR11D.h"

//Class random 8-byte key backwards through array, constructor only, public variable, random clear
//#include "MBL_CLASS_XOR12D.h"



//Class random key forward through array zero clear
//#include "MBL_CLASS_RXOR1D.h"

//Class random key backwards through array zero clear
//#include "MBL_CLASS_RXOR2D.h"

//Class random key forward through array random clear
//#include "MBL_CLASS_RXOR3D.h"

//Class random key backwards through array random clear
//#include "MBL_CLASS_RXOR4D.h"

//Class random key function forward through array zero clear
//#include "MBL_CLASS_RXOR5D.h"

//Class random key function backwards through array zero clear
//#include "MBL_CLASS_RXOR6D.h"

//Class random key function forward through array random clear
//#include "MBL_CLASS_RXOR7D.h"

//Class random key function backwards through array random clear
//#include "MBL_CLASS_RXOR8D.h"

//Class random key reverse roll forward through array zero clear
//#include "MBL_CLASS_RXOR9D.h"

//Class random key reverse roll backwards through array zero clear
//#include "MBL_CLASS_RXOR10D.h"

//Class random key reverse roll forward through array random clear
//#include "MBL_CLASS_RXOR11D.h"

//Class random key reverse roll backwards through array random clear
//#include "MBL_CLASS_RXOR12D.h"



//Class random bump between 5-250 forward zero clear
//#include "MBL_CLASS_BUMP1D.h"

//Class random bump between 10-240 backwards zero clear
//#include "MBL_CLASS_BUMP2D.h"

//Class random bump between 5-250 forward random clear
//#include "MBL_CLASS_BUMP3D.h"

//Class random bump between 10-240 backwards random clear
//#include "MBL_CLASS_BUMP4D.h"

//Class random bump function between 5-250 forward zero clear
//#include "MBL_CLASS_BUMP5D.h"

//Class random bump function between 10-240 backwards zero clear
//#include "MBL_CLASS_BUMP6D.h"

//Class random bump function between 5-250 forward random clear
//#include "MBL_CLASS_BUMP7D.h"

//Class random bump function between 10-240 backwards random clear
//#include "MBL_CLASS_BUMP8D.h"

//Class random bump 8-byte key between 5-250 forward zero clear
//#include "MBL_CLASS_BUMP9D.h"

//Class random bump 8-byte key between 10-240 backwards zero clear
//#include "MBL_CLASS_BUMP10D.h"

//Class random bump 8-byte key between 5-250 forward random clear
//#include "MBL_CLASS_BUMP11D.h"

//Class random bump 8-byte key between 10-240 backwards random clear
//#include "MBL_CLASS_BUMP12D.h"



//Class rolling random bump between 5-250 forward zero clear
//#include "MBL_CLASS_RBUMP1D.h"

//Class rolling random bump between 10-240 backwards zero clear
//#include "MBL_CLASS_RBUMP2D.h"

//Class rolling random bump between 5-250 forward random clear
//#include "MBL_CLASS_RBUMP3D.h"

//Class rolling random bump between 10-240 backwards random clear
//#include "MBL_CLASS_RBUMP4D.h"

//Class rolling random bump function between 5-250 forward zero clear
//#include "MBL_CLASS_RBUMP5D.h"

//Class rolling random bump function between 10-240 backwards zero clear
//#include "MBL_CLASS_RBUMP6D.h"

//Class rolling random bump function between 5-250 forward random clear
//#include "MBL_CLASS_RBUMP7D.h"

//Class rolling random bump function between 10-240 backwards random clear
//#include "MBL_CLASS_RBUMP8D.h"

//Class rolling random bump between 5-250 forward zero clear flip +/-
//#include "MBL_CLASS_RBUMP9D.h"

//Class rolling random bump between 10-240 backwards zero clear flip +/-
//#include "MBL_CLASS_RBUMP10D.h"

//Class rolling random bump between 5-250 forward random clear flip +/-
//#include "MBL_CLASS_RBUMP11D.h"

//Class rolling random bump between 10-240 backwards random clear flip +/-
//#include "MBL_CLASS_RBUMP12D.h"


//----------------------------------------------------------------------------------------

#endif



//----------------------------------- C Obfuscation Algorithms----------------------------

//Random character forward through array
//#include "MBL_FORLOOP_XOR1D.h"

//Random character backwards through array
//#include "MBL_FORLOOP_XOR2D.h"

//Random 8 byte key forwards through array
//#include "MBL_FORLOOP_XOR3D.h"

//Random 8 byte key backwards through array
//#include "MBL_FORLOOP_XOR4D.h"


//Random char key function forwards through array
//#include "MBL_FORLOOP_FUNC_XOR1D.h"

//Random char key function backwards through array
//#include "MBL_FORLOOP_FUNC_XOR2D.h"

//Random 8-byte key function forwards through array
//#include "MBL_FORLOOP_FUNC_XOR3D.h"

//Random 8-byte key function backwards through array
//#include "MBL_FORLOOP_FUNC_XOR4D.h"

//Random 16-byte key function forwards through array
//#include "MBL_FORLOOP_FUNC_XOR5D.h"

//Random 16-byte key function backwards through array
//#include "MBL_FORLOOP_FUNC_XOR6D.h"

//Supplied key (Unicode and UTF-8 versions) function backwards through array
//--#include "MBL_FORLOOP_FUNC_XOR7D.h"

//#ifdef MBL_FORLOOP_FUNC_XOR7D
//
//#define MBL_FORLOOP_FUNC_XOR7DA = "";
//#define MBL_FORLOOP_FUNC_XOR7DW = L"";
//
//#endif

//Supplied key (Unicode and UTF-8 versions) function backwards through array
//--#include "MBL_FORLOOP_FUNC_XOR8D.h"

//#ifdef MBL_FORLOOP_FUNC_XORD8
//
//#define MBL_FORLOOP_FUNC_XOR8DA = "";
//#define MBL_FORLOOP_FUNC_XOR8DW = L"";
//
//#endif


//------------------------------------------


//Random key forward roll forward unscram
//#include "MBL_FORLOOP_RXOR1D.h"

//Random key forward roll backwards unscram
//#include "MBL_FORLOOP_RXOR2D.h"

//Random key backward roll forward unscram
//#include "MBL_FORLOOP_RXOR3D.h"

//Random key backward roll backward unscram
//#include "MBL_FORLOOP_RXOR4D.h"


//Random key function forward roll forward unscram
//#include "MBL_FORLOOP_FUNC_RXOR1D.h"

//Random key function forward roll backwards unscram
//#include "MBL_FORLOOP_FUNC_RXOR2D.h"

//Random key function backward roll forward unscram
//#include "MBL_FORLOOP_FUNC_RXOR3D.h"

//Random key function backward roll forward unscram
//#include "MBL_FORLOOP_FUNC_RXOR4D.h"


//----------------------------------------------


//Random bump between 15-240/65520 forward
//#include "MBL_FORLOOP_BUMP1D.h"

//Random bump between 15-240/65520 backwards
//#include "MBL_FORLOOP_BUMP2D.h"

//Random bump negative between 15-240/65520 forward 
//#include "MBL_FORLOOP_BUMP3D.h"

//Random bump negative between 15-240/65520 backward
//#include "MBL_FORLOOP_BUMP4D.h"

//Random bump 4-byte key between 15-240/65520 forward
//#include "MBL_FORLOOP_BUMP5D.h"

//Random bump 6-byte key between 15-240/65520 backward
//#include "MBL_FORLOOP_BUMP6D.h"

//Random bump 8-byte key negative between 15-240/65520 forward
//#include "MBL_FORLOOP_BUMP7D.h"

//Random bump 12-byte key negative between 15-240/65520 backward
//#include "MBL_FORLOOP_BUMP8D.h"

//Random bump 16-byte key between 15-240/65520 forward
//#include "MBL_FORLOOP_BUMP9D.h"

//Random bump 8-byte key between 15-240/65520 backwards
//#include "MBL_FORLOOP_BUMP10D.h"

//Random bump 16-byte key negative between 15-240/65520 forward
//#include "MBL_FORLOOP_BUMP11D.h"

//Random bump 8-byte key negative between 15-240/65520 backwards
//#include "MBL_FORLOOP_BUMP12D.h"


//Random bump func between 15-240/65520 forward
//#include "MBL_FORLOOP_FUNC_BUMP1D.h"

//Random bump func between 15-240/65520 backwards
//#include "MBL_FORLOOP_FUNC_BUMP2D.h"

//Random bump func negative between 15-240/65520 forward 
//#include "MBL_FORLOOP_FUNC_BUMP3D.h"

//Random bump func negative between 15-240/65520 backward
//#include "MBL_FORLOOP_FUNC_BUMP4D.h"

//Random bump func 8-byte key between 15-240/65520 forward
//#include "MBL_FORLOOP_FUNC_BUMP5D.h"

//Random bump func 8-byte key between 15-240/65520 backward
//#include "MBL_FORLOOP_FUNC_BUMP6D.h"

//Random bump func 8-byte key negative between 15-240/65520 forward
//#include "MBL_FORLOOP_FUNC_BUMP7D.h"

//Random bump func 8-byte key negative between 15-240/65520 backward
//#include "MBL_FORLOOP_FUNC_BUMP8D.h"

//Random bump func 16-byte key between 15-240/65520 forward
//#include "MBL_FORLOOP_FUNC_BUMP9D.h"

//Random bump func 16-byte key between 15-240/65520 backwards
//#include "MBL_FORLOOP_FUNC_BUMP10D.h"

//Random bump func 16-byte key negative between 15-240/65520 forward
//#include "MBL_FORLOOP_FUNC_BUMP11D.h"

//Random bump func 16-byte key negative between 15-240/65520 backwards
//#include "MBL_FORLOOP_FUNC_BUMP12D.h"

//Supplied key (Unicode and UTF-8 versions) bump func forward
//--#include "MBL_FORLOOP_FUNC_BUMP13D.h"

//#ifdef MBL_FORLOOP_FUNC_BUMP13D
//
//#define MBL_FORLOOP_FUNC_BUMP13DA = "";
//#define MBL_FORLOOP_FUNC_BUMP13DW = L"";
//
//#endif

//Supplied key (Unicode and UTF-8 versions) bump func backward
//--#include "MBL_FORLOOP_FUNC_BUMP14D.h"

//#ifdef MBL_FORLOOP_FUNC_BUMP14D
//
//#define MBL_FORLOOP_FUNC_BUMP14DA = "";
//#define MBL_FORLOOP_FUNC_BUMP14DW = L"";
//
//#endif


//-------------------------------------

//Random rolling bump between 15-240/65520 forward
//#include "MBL_FORLOOP_RBUMP1D.h"

//Random rolling bump between 15-240/65520 backwards
//#include "MBL_FORLOOP_RBUMP2D.h"

//Random rolling bump negative between 15-240/65520 forward 
//#include "MBL_FORLOOP_RBUMP3D.h"

//Random rolling bump negative between 15-240/65520 backward
//#include "MBL_FORLOOP_RBUMP4D.h"

//Random rolling bump between 15-240/65520 forward +/- flip
//#include "MBL_FORLOOP_RBUMP5D.h"

//Random rolling bump between 15-240/65520 backwards +/- flip
//#include "MBL_FORLOOP_RBUMP6D.h"

//Random rolling bump negative between 15-240/65520 forward +/- flip 
//#include "MBL_FORLOOP_RBUMP7D.h"

//Random rolling bump negative between 15-240/65520 backward +/- flip
//#include "MBL_FORLOOP_RBUMP8D.h"



//Random rolling key bump func between 15-240/65520 forward
//#include "MBL_FORLOOP_FUNC_RBUMP1D.h"

//Random rolling key bump func between 15-240/65520 backwards
//#include "MBL_FORLOOP_FUNC_RBUMP2D.h"

//Random rolling key bump func negative between 15-240/65520 forward 
//#include "MBL_FORLOOP_FUNC_RBUMP3D.h"

//Random rolling key bump func negative between 15-240/65520 backward
//#include "MBL_FORLOOP_FUNC_RBUMP4D.h"

//Random rolling key bump func between 15-240/65520 forward +/- flip
//#include "MBL_FORLOOP_FUNC_RBUMP5D.h"

//Random rolling key bump func between 15-240/65520 backwards +/- flip
//#include "MBL_FORLOOP_FUNC_RBUMP6D.h"

//Random rolling key bump func negative between 15-240/65520 forward +/- flip
//#include "MBL_FORLOOP_FUNC_RBUMP7D.h"

//Random rolling key bump func negative between 15-240/65520 backward +/- flip
//#include "MBL_FORLOOP_FUNC_RBUMP8D.h"




//----------------------------------------------------------------------------------------