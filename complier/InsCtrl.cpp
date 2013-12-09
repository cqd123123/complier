#pragma warning(disable:4786)
#include "InsCtrl.h"
#include <FSTREAM>

InsCtrl::InsCtrl(SSTable& tab):sstab(tab)
{
	BBlock::reset();
	FBlock::reset();
	Quadcode::reset();
}
void InsCtrl::new_instruction(Quadcode* q)
{
	Quad_inss.push_back(q);
	update_reference(q);
}
std::vector<Quadcode*> InsCtrl::get_all_ins() const
{
	return Quad_inss;
}
void InsCtrl::arithmetic_operation(symbol* src1 , token opt, symbol* src2, symbol*& result)
{
	sstab.new_temp(result,symbol::INTEGER);
	if(opt.get_value()=="+")
	{
		Quadcode* ins=Quadcode::new_Quad_code(Quadcode::ADD,result,src1,src2);
		new_instruction(ins);
	}
	if(opt.get_value()=="-")
	{
		Quadcode* ins=Quadcode::new_Quad_code(Quadcode::SUB,result,src1,src2);
		new_instruction(ins);
	}
	if(opt.get_value()=="*")
	{
		Quadcode* ins=Quadcode::new_Quad_code(Quadcode::MUL,result,src1,src2);
		new_instruction(ins);
	}
	if(opt.get_value()=="/")
	{
		Quadcode* ins=Quadcode::new_Quad_code(Quadcode::DIV,result,src1,src2);
		new_instruction(ins);
	}
}
void InsCtrl::branch(symbol* src1, token opt, symbol* src2, symbol*& label)
{
	if(opt.get_value()=="=")//equal
	{
		Quadcode* ins=Quadcode::new_Quad_code(Quadcode::JNE,label,src1,src2);//jump when not equal
		new_instruction(ins);
	}
	if(opt.get_value()=="<>")
	{
		Quadcode* ins=Quadcode::new_Quad_code(Quadcode::JEQ,label,src1,src2);
		new_instruction(ins);
	}
	if(opt.get_value()==">")
	{
		Quadcode* ins=Quadcode::new_Quad_code(Quadcode::JLE,label,src1,src2);
		new_instruction(ins);
	}
	if(opt.get_value()=="<")
	{
		Quadcode* ins=Quadcode::new_Quad_code(Quadcode::JGE,label,src1,src2);
		new_instruction(ins);
	}
	if(opt.get_value()==">=")
	{
		Quadcode* ins=Quadcode::new_Quad_code(Quadcode::JLT,label,src1,src2);
		new_instruction(ins);
	}
	if(opt.get_value()=="<=")
	{
		Quadcode* ins=Quadcode::new_Quad_code(Quadcode::JGT,label,src1,src2);
		new_instruction(ins);
	}
}

void InsCtrl::assign(symbol* obj, symbol* src)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::ASSIGN,obj,src);
	if(obj->get_sym_type()==symbol::ARRAY) obj->set_marked(false);
	if(src->get_sym_type()==symbol::ARRAY) src->set_marked(false);
	new_instruction(ins);
}
void InsCtrl::write(symbol* sym)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::WRITE,sym);
	new_instruction(ins);
}
void InsCtrl::read(symbol* sym)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::READ, sym);
	new_instruction(ins);
}
void InsCtrl::fun(symbol* sym)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::FUNC,sym);
	new_instruction(ins);
}
void InsCtrl::proc(symbol* sym)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::PROC,sym);
	new_instruction(ins);
}
void InsCtrl::param(symbol* sym)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::PARAM,sym);
	new_instruction(ins);
}
void InsCtrl::set_ref(symbol* sym)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::SREF,sym);
	new_instruction(ins);
}
void InsCtrl::set_index(symbol* array,symbol* index,bool back)
{
	if(back)
	{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::SIND,array,index);
	new_instruction(ins);
	}
	else {
		Quadcode* ins=Quadcode::new_Quad_code(Quadcode::SIND,array,index,index);
		new_instruction(ins);
	}
}
void InsCtrl::call(symbol* sym)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::CALL,sym);
	new_instruction(ins);
}
void InsCtrl::call(symbol* src, symbol*& obj)
{
//	sstab.new_temp(obj,src->get_val_type());
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::CALL,obj,src);
	new_instruction(ins);
}
void InsCtrl::push(symbol* src)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::PUSH,src);
	new_instruction(ins);
}
void InsCtrl::ret(symbol* sym)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::RET,sym);
	new_instruction(ins);
}
void InsCtrl::ret(symbol* sym1, symbol* sym2)//both src
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::RET,sym1,sym2);
	new_instruction(ins);
}
void InsCtrl::end_fun(symbol* sym)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::ENDF,sym);
	new_instruction(ins);
}
void InsCtrl::end_proc(symbol* sym)//should be same as endf,so quadcode will use endf
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::ENDP,sym);
	new_instruction(ins);
}
void InsCtrl::add_label(symbol* sym)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::LABEL,sym);
	new_instruction(ins);
}
void InsCtrl::jmp(symbol* target)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::JMP,target);
	new_instruction(ins);
}
void InsCtrl::main_begin(symbol* scope)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::MAIN,scope);
	new_instruction(ins);
}
void InsCtrl::main_end(symbol* scope)
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::ENDM,scope);
	new_instruction(ins);
}
void InsCtrl::shift_line()
{
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::LINE);
	new_instruction(ins);
}
void InsCtrl::update_reference(Quadcode* q)
{
	std::vector<symbol*> src=q->get_src_arg();
	symbol* obj=q->get_obj_arg();
	for(int i=0;i<src.size();i++)
	{
		if(src[i]!=NULL)
		{
			if(src[i]->get_sym_type()!=symbol::LITERAL&&src[i]->get_scope()!=NULL)
			src[i]->get_scope()->update_reference(src[i],1);
		}
	}
	if(obj!=NULL)
	{
		if(obj->get_sym_type()!=symbol::LITERAL&&obj->get_scope()!=NULL)
		obj->get_scope()->update_reference(obj,1);
	}
}
void InsCtrl::inverse(symbol* src, symbol*& obj)
{
	sstab.new_temp(obj, obj->get_val_type());
	Quadcode* ins=Quadcode::new_Quad_code(Quadcode::INV,obj,src);
	new_instruction(ins);
}
void InsCtrl::reorder_all_ins()
{
	code_block* blk=new code_block();
	stack_code_block.push_back(blk);
	all_code_block.push_back(blk);
	for(int i=0;i<Quad_inss.size();i++)
	{
		if(Quad_inss[i]->get_quad_type()==Quadcode::FUNC)
		{
			code_block* blk=new code_block();
			stack_code_block.push_back(blk);
			all_code_block.push_back(blk);
		}
		if(Quad_inss[i]->get_quad_type()==Quadcode::ENDF)
		{
			stack_code_block.back()->add_ins(Quad_inss[i]);
			stack_code_block.pop_back();
			continue;
		}
		stack_code_block.back()->add_ins(Quad_inss[i]);
	}
	unsigned int index=0;
	Quad_inss.clear();
	for(int k=0;k<all_code_block.size();k++)
	{   
		code_block* blk=all_code_block[k];
		for(int j=0;j<blk->codes.size();j++)
		{
			blk->codes[j]->set_index(index++);
			Quad_inss.push_back(blk->codes[j]);
		}
	}
	for(int kk=0;kk<Quad_inss.size();kk++)
	{
		Quadcode::Quad_type typ;
		typ=Quad_inss[kk]->get_quad_type();
		if(typ== Quadcode::FUNC||typ==Quadcode::MAIN||typ==Quadcode::PROC)
		{
			this->func_block.push_back(new FBlock(sstab.get_table(Quad_inss[kk]->get_obj_arg())));
		}
		func_block.back()->add_ins(Quad_inss[kk]);
	}
	FBlock* blkk=func_block.front();
	func_block.erase(func_block.begin());
	func_block.push_back(blkk);
}
std::vector<FBlock*> InsCtrl::get_all_func_block()
{
	return func_block;
}
void InsCtrl::print_all_ins()
{
	for(int i=0;i<Quad_inss.size();i++)
		std::cout<<*Quad_inss[i];
}
