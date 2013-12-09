#ifndef _COMPILER_CODEBUFFER
#define _COMPILER_CODEBUFFER
#pragma warning(disable:4786)
#include <STRING>
#include <ISTREAM>
#include <STDIO.H>
class codeBuffer  
{
public:
	
	codeBuffer(FILE*& in);
	virtual ~codeBuffer();
	bool getnext(char& ch);
	void unget();
    bool eof();
private:
	std::string code;
	unsigned int index;
};
#endif