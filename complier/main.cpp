#pragma warning(disable:4786)
#include <iostream>
#include <STDIO.H>
#include <FSTREAM>
#include <CSTDLIB>
#include <SSTREAM>

#include "topUnit.h"
#include "tokenizer.h"
#include "token.h"
#include "Parser.h"
#include "errorlist.h"
#include "TNode.h"
#include "symbol.h"
#include "SSTable.h"
#include "InsCtrl.h"
#include "asm_translator.h"
#include "Dag.h"
#include "Data_stream.h"
///////////////////////////////////////////////////////////////////////////
//optimization enabled, data_stream, dag, peephole
int main(int argc, char* argv[])
{
	std::cout<<"in compile"<<std::endl;
	std::cout<<"please input file name"<<std::endl;
//	char name[1024];
	std::stringstream ss(std::stringstream::in|std::stringstream::out);
	ss<<"test.txt";
//	scanf("%s",name);
	FILE* in=fopen(ss.str().c_str(),"r");
	if(!in) {std::cout<<"file not find"<<std::endl; return 0;}
	tokenizer tk(in);
	errorlist* el = new errorlist();
	SSTable* table = new SSTable();
	InsCtrl* ins = new InsCtrl(*table);

	Parser ps(tk,*el);
	TNode* node=ps.parse();
//	std::cout<<*node;
 	fclose(in);
	if(el->getErrorNum()==0)
		node->visit(*table,*ins,*el);
	else
	{ el->PrintError();return 0;}
	ins->reorder_all_ins();
	ins->print_all_ins();

	if(el->getErrorNum()!=0)
	{el->PrintError(); return 0;}
	el->PrintWarning();	
	std::ofstream file("block.txt",std::ios::out);
	std::vector<FBlock*> blo=ins->get_all_func_block();
	for(int i=0;i<blo.size();i++)
	{
		file<<*blo[i];
	}
	Dag da(ins);	
	Data_stream* ds=NULL;
	ds=new Data_stream(ins);
	
	asm_translator tl(table,ins,ds);
	std::ofstream ff("test.asm",std::ios::out);
	tl.translate(ff);
	system("ml /c /coff test.asm");
	system("link /subsystem:console test.obj");
	std::cout<<"---------------------running-----------------------"<<std::endl;
	system("test.exe");

	return 0;	
}
