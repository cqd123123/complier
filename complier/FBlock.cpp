#pragma warning(disable:4786)
#include "FBlock.h"
#include <CASSERT>

unsigned int FBlock::instance_number = 0;
FBlock::FBlock(SymbolTable* cur_stbl)
{
	cur_func_tbl=cur_stbl;
	new_basic_block();
	new_block = false;
	following_label = false;
	is_end = false;
	former_block = NULL;
	number = instance_number++;
}
void FBlock::new_basic_block()
{
	BBlock* block=new BBlock(this->cur_func_tbl);
	all_blocks.push_back(block);
}
void FBlock::add_ins(Quadcode* ins)
{	assert(!is_end);
	index_map[ins->get_index()]=this->all_ins.size();	//code-index to the index of code in the function block
	all_ins.push_back(ins);
	Quadcode::Quad_type typ=ins->get_quad_type();
	if(new_block&&typ!=Quadcode::PARAM&&typ!=Quadcode::LABEL&&typ!=Quadcode::ENDF
		&&typ!=Quadcode::ENDP&&typ!=Quadcode::ENDM)	//this flag will be set by branch ins. since the ins followed by branch is an entry for new block
	{
		new_basic_block();
		new_block=false;
		connect_BBlock(former_block,all_blocks.back());
	}
	switch(ins->get_quad_type())
	{
		case Quadcode::PARAM:
			new_block=true;
			following_label=false;
			former_block=this->all_blocks.back();
			break;
		case Quadcode::ENDF:
		case Quadcode::ENDM:
		case Quadcode::ENDP:
			if(!following_label)
			{
				new_basic_block();
				connect_BBlock(former_block,all_blocks.back());
			}
			is_end=true;
			break;
		case Quadcode::LABEL:
			if(!following_label)
			{
				new_basic_block();
				following_label=true;
				connect_BBlock(former_block,all_blocks.back());
			}
			block_entry[ins->get_src_arg()[0]]=this->all_blocks.back();
			new_block=false;
			break;
		case Quadcode::JMP:
			block_exit[this->all_blocks.back()]=ins->get_src_arg()[0];
			new_block=true;
			following_label=false;
			former_block=NULL;
			break;
		case Quadcode::JEQ:
		case Quadcode::JGE:
		case Quadcode::JGT:
		case Quadcode::JLE:
		case Quadcode::JLT:
		case Quadcode::JNE:
			new_block=true;
			following_label=false;
			former_block=this->all_blocks.back();
			block_exit[this->all_blocks.back()]=ins->get_src_arg()[2];
			break;
		default:
			following_label=false;
			former_block=this->all_blocks.back();
	}
	all_blocks.back()->add_code(ins);
	if(is_end) flow_BBlock();
}
void FBlock::flow_BBlock()
{
	for(std::map<BBlock*,symbol*>::iterator it=block_exit.begin();it!=block_exit.end();it++)
	{
		symbol* sym=it->second;
		std::map<symbol*,BBlock*>::iterator it1=block_entry.find(sym);
		assert(it1!=block_entry.end());
		connect_BBlock(it->first,it1->second);
	}
}
void FBlock::connect_BBlock(BBlock* entry,BBlock* exit)
{
	if(entry!=NULL&&exit!=NULL)
	{
		entry->set_exit(exit);
		exit->set_entry(entry);
	}
}
std::vector<Quadcode*> FBlock::get_all_ins() const
{
	return this->all_ins;
}
std::vector<BBlock*> FBlock::get_basic_blocks() const
{
	return this->all_blocks;
}
void FBlock::reset()
{
	instance_number=0;
}
SymbolTable* FBlock::get_function_stable()
{
	return all_ins[0]->get_obj_arg()->get_cur_scope();
}
Quadcode* FBlock::get_code(unsigned int index)
{
	return all_ins[index_map[index]];		//this index is the index of the quad code
}
void operator<<(std::ostream& out,const FBlock& block)
{
	out<<"Function block"<<block.number<<std::endl;
	for(int i=0;i<block.all_blocks.size();i++)
	{
		out<<*block.all_blocks[i];
	}
	out<<std::endl;
}