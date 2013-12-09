#pragma warning(disable:4786)
#include "codeBuffer.h"
#include <STRING>
#include <STDIO.H>
#include <IOSTREAM>
codeBuffer::codeBuffer(FILE*& in)
{
	char ch;
	ch=fgetc(in);
	while(ch!=EOF)
	{
		code+=ch;
		ch=fgetc(in);
	}
	code+=EOF;
	index=-1;
	fclose(in);
}

codeBuffer::~codeBuffer()
{
 ;
}
bool codeBuffer::getnext(char& ch)
{
	index++;
	ch=code.c_str()[index];
	if(ch==EOF)
		return false;
	else
	{return true;}
}
bool codeBuffer::eof()
{
	if(code.c_str()[index]==EOF||index>=code.size())
		return true;
	else
		return false;
}
void codeBuffer::unget()
{
	index--;
}