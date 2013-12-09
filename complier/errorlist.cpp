#pragma warning(disable:4786)
#include "errorlist.h"

errorlist::errorlist()
{
	errnum=0;
}

void errorlist::addError(std::string desc,unsigned int lineNum)
{
	errormsg er;
	er.des=desc;
	er.line=lineNum;
	errnum++;
	errors.push_back(er);
}
void errorlist::addWarning(std::string desc,unsigned int lineNum)
{
	errormsg er;
	er.des=desc;
	er.line=lineNum;
//	errnum++;
	warngings.push_back(er);
}
void errorlist::PrintError()
{
	std::cout<<"---------------Errors-----------------"<<std::endl;
	int num=0;
	if(errors.size()>=5) num=5; else num=errors.size();
	for(int i=0;i<num;i++)
	{
		errormsg er=errors[i];
		std::cout<<er.des.c_str()<<"  "<<"line "<<er.line<<std::endl;
	}
	std::cout<<"---------------Errors-----------------"<<std::endl;
}
void errorlist::PrintWarning()
{	if(warngings.size()==0) return;
	std::cout<<"---------------warnings-----------------"<<std::endl;
	for(int i=0;i<warngings.size();i++)
	{
		errormsg er=warngings[i];
		std::cout<<er.des.c_str()<<"   "<<"line "<<er.line<<std::endl;
	}
	std::cout<<"---------------warnings-----------------"<<std::endl;
}
