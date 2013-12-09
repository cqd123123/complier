#ifndef _COMPILER_ERRORLIST
#define _COMPILER_ERRORLIST
#pragma warning(disable:4786)
#include <VECTOR>
#include <IOSTREAM>
class errorlist  
{
public:
	const enum Error_Type
	{
		UNDEFINED_IDENTIFIER
	};
	void addError(std::string ,unsigned int);
	void addWarning(std::string, unsigned int);
	void PrintError();
	void PrintWarning();
	unsigned int getErrorNum(){return errnum;}
	errorlist();
	typedef struct errormsg{
		std::string des;
		unsigned int line;
	};
private:
	std::vector<errormsg> errors;
	std::vector<errormsg> warngings;
	unsigned int errnum;
};
#endif