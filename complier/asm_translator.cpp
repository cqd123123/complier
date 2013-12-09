#pragma warning(disable:4786)
#include "asm_translator.h"
#include <FSTREAM>
#include "symbol.h"
#include "utility.h"
using namespace std;

asm_translator::asm_translator(SSTable* _tbl,InsCtrl* _ctrl, Data_stream* _das):memctr(_tbl,_ctrl,_das),das(_das)
{
	tbl=_tbl;
	ins=_ctrl;
//	MemCtrl memctr(_tbl,_ctrl);
}	

asm_translator::~asm_translator()
{;
}
void asm_translator::write_head(std::ostream out)	//out the head clause for the asm file
{
	std::ifstream fs("header.txt");
	char ch;
	while(fs.get(ch))
		out << ch;
	out<<std::endl;
}
void asm_translator::translate(std::ostream& out)
{
	write_head(out);
	write_body(out);
}
void asm_translator::write_const(std::ostream out)	//write the const declarations and literals, will be allocated statically
{
	out<< ".data" << std::endl << std::endl;
	out<< printf_char << " db \"%c\", 0" <<std::endl;
	out<< printf_int << " db \"%d\", 0" << std::endl;
	out<< new_line << " db 10, 13, 0" << std::endl;
	out<< scanf_char << " db \"%c\", 0" << std::endl;
	out<< scanf_int << " db \"%d\", 0" << std::endl;
	std::set<symbol*> consts = memctr.get_const_pool();
	for(std::set<symbol*>::iterator it=consts.begin();it!=consts.end();it++)
	{
		out<<memctr.get_op_name(*it);
		if((*it)->get_val_type()==symbol::STRING)
		{
			out<<" db "<<memctr.get_literal_value(*it)<<std::endl;
		}
		else
			out<<" dd "<<memctr.get_literal_value(*it)<<std::endl;
	}
	out<<std::endl;
	out<<".data?"<<std::endl<<std::endl;
	std::set<symbol*> globals=memctr.get_global_variable();
	for(std::set<symbol*>::iterator itt=globals.begin();itt!=globals.end();itt++)
	{
		if((*itt)->get_sym_type()!=symbol::ARRAY&&(*itt)->get_sym_type()!=symbol::FUNCTION)	//if array, multiple '?'
			out<<memctr.get_op_name(*itt)<<" dd ?"<<std::endl;
		else if((*itt)->get_sym_type()==symbol::FUNCTION)
			out<<memctr.get_address(*itt,cur_stbl)<<" dd ?"<<std::endl;
		else//array
		{
//			if(const_array_name.find(memctr.get_op_name(*itt))==const_array_name.end())
//			{
//				const_array_name.insert(memctr.get_op_name((*itt)));
				unsigned int size=(*itt)->get_size();size=size/4;
				out<<memctr.get_op_name(*itt)<<" dd ?";
				while(size>0)
				{
					out<<" , ? ";
					size--;
				}
				out<<std::endl;
//			}
		}
	}
}
//void asm_translator::translate_sind(FBlock* blk,Quadcode*)
void asm_translator::write_body(std::ostream out)	//translate the body
{
	std::vector<FBlock*> blk=ins->get_all_func_block();		//the quad codes was reordered, and saved into the f-blocks
	for(std::vector<FBlock*>::iterator it=blk.begin();it!=blk.end();it++)
	{
		cur_stbl=(*it)->get_function_stable();//set current symbol table
		translate(*it);
	}
	write_const(out);//first write the const
//	obj_code.Peephole();
	out<<obj_code;
}
void asm_translator::translate(FBlock* fblock)
{	
	vector<Quadcode*> quats = fblock->get_all_ins();
	for(unsigned int i = 0; i < quats.size(); i++)
	{
		translate(fblock, quats[i]);
	}
}
void asm_translator::translate(FBlock* scope, Quadcode* code)
{	//std::cout<<"_________________-"<<std::endl;
	if(!code->is_enable()) return;
	std::cout<<"de "<<*code;
	switch(code->get_quad_type())
	{
	case Quadcode::ASSIGN:
		translate_assign(scope,code); break;
	case Quadcode::ADD:
		translate_add(scope,code);break;
	case Quadcode::SUB:
		translate_sub(scope,code);break;
	case Quadcode::MUL:
		translate_mul(scope,code);break;
	case Quadcode::DIV:
		translate_div(scope,code);break;
	case Quadcode::ENDF:
	case Quadcode::ENDM:
	case Quadcode::ENDP:
		translate_endf(scope,code);break;
	case Quadcode::FUNC:
	case Quadcode::MAIN:
	case Quadcode::PROC:
		translate_fun(scope,code);break;
	case Quadcode::INV:
		translate_inv(scope,code);break;
	case Quadcode::JEQ:
		translate_jeq(scope,code);break;
	case Quadcode::JGE:
		translate_jge(scope,code);break;
	case Quadcode::JGT:
		translate_jgt(scope,code);break;
	case Quadcode::JLE:
		translate_jle(scope,code);break;
	case Quadcode::JLT:
		translate_jlt(scope,code);break;
	case Quadcode::JMP:
		translate_jmp(scope,code);break;
	case Quadcode::JNE:
		translate_jne(scope,code);break;
	case Quadcode::CALL:
		translate_call(scope,code);break;
	case Quadcode::LABEL:
		translate_label(scope,code);break;
	case Quadcode::PARAM:
		translate_param(scope,code);break;
	case Quadcode::PUSH:
		translate_push(scope,code);break;
	case Quadcode::SREF:
		translate_sref(scope,code);break;
	case Quadcode::SIND:
		translate_sind(scope,code);break;
	case Quadcode::READ:
		translate_read(scope,code);break;
	case Quadcode::RET:
		translate_ret(scope,code);break;
	case Quadcode::WRITE:
		translate_write(scope,code);break;
	case Quadcode::LINE:
		translate_new_line(scope,code);break;
	default:
		assert(0);
	}
}
void asm_translator::translate_assign(FBlock* scope, Quadcode* code)
{
	symbol* obj=code->get_obj_arg();
	symbol* src=code->get_src_arg()[0];
//	assert(obj->get_sym_type()!=symbol::ARRAY||src->get_sym_type()!=symbol::ARRAY);
	symbol::symbol_type obj_typ=obj->get_sym_type();
	symbol::symbol_type src_typ=src->get_sym_type();
	std::string src_pos_global;
	std::string obj_pos_global;
//	std::cout<<*obj;
	if(src_typ==symbol::ARRAY||src_typ==symbol::REFSYM||memctr.is_outer(src,cur_stbl)) //put src address into a reg
	{
		get_address_into_reg(EAX,src,code->get_index());
	}
	if(obj_typ!=symbol::REFSYM&&obj_typ!=symbol::ARRAY&&!memctr.is_outer(obj,cur_stbl))//object is not a reference symbol or a array
	{
		if(memctr.in_register(obj, code->get_index(), false) == NA) //cannot move from mem to mem
		{   
			occupy_local_register(obj, code->get_index(), false);
		}
		if(src_typ!=symbol::REFSYM&&src_typ!=symbol::ARRAY&&!memctr.is_outer(src,cur_stbl))
			obj_code.add(MOV, memctr.get_operand(obj, code->get_index(), false,cur_stbl), memctr.get_operand(src, code->get_index(), true,cur_stbl));
		else
		{
			obj_code.add(MOV,memctr.get_operand(obj,code->get_index(),false,cur_stbl),"["+REG_NAME_MAP[EAX]+"]");
			memctr.release_prempt_reg(EAX);
		}
	}
	else	//the object is not local variable
	{
		get_address_into_reg(ECX,obj,code->get_index());
		if(src_typ==symbol::ARRAY||src_typ==symbol::REFSYM||memctr.is_outer(src,cur_stbl))
		{
			obj_code.add(MOV,memctr.get_operand(EAX),"["+REG_NAME_MAP[EAX]+"]");//eax now contain the content of src
			obj_code.add(MOV,"["+REG_NAME_MAP[ECX]+"]",memctr.get_operand(EAX));
			memctr.release_prempt_reg(EAX);
			memctr.release_prempt_reg(ECX);
		}
		else
		{
			if(memctr.in_register(src,code->get_index(),true)==NA)//mov cannot move two 
				occupy_local_register(src,code->get_index(),true);
			obj_code.add(MOV,"["+REG_NAME_MAP[ECX]+"]",memctr.get_operand(src,code->get_index(),true,cur_stbl));
			memctr.release_prempt_reg(ECX);
		}
	}
}
void asm_translator::translate_inv(FBlock* scope, Quadcode* code)	//inverse
{
	symbol* src=code->get_src_arg()[0];
	symbol* obj=code->get_obj_arg();
	bool addr_s=(src->get_sym_type()!=symbol::ARRAY&&src->get_sym_type()!=symbol::REFSYM&&!memctr.is_outer(src,cur_stbl));
	bool addr_o=(obj->get_sym_type()!=symbol::ARRAY&&obj->get_sym_type()!=symbol::REFSYM&&!memctr.is_outer(obj,cur_stbl));
	if(!addr_o) get_address_into_reg(EAX,obj,code->get_index());
	else if(memctr.in_register(obj,code->get_index(),false)==NA)
		occupy_local_register(obj,code->get_index(),false);
	if(addr_o&&addr_s)	//both can be addressed by ebp statically
	{
		std::string obj_s=memctr.get_operand(obj,code->get_index(),false,cur_stbl);
		obj_code.add(MOV,obj_s,memctr.get_operand(src,code->get_index(),true,cur_stbl));
		obj_code.add(NEG,obj_s);
	}
	else if(addr_o&&!addr_s)	//the address of source operand should be calculated 
	{
		std::string obj_s=memctr.get_operand(obj,code->get_index(),false,cur_stbl);
		get_address_into_reg(ECX,src,code->get_index());
		obj_code.add(MOV,obj_s,"["+REG_NAME_MAP[ECX]+"]");
		obj_code.add(NEG,obj_s);
		memctr.release_prempt_reg(ECX);		//should release the preempted regs
	}
	else if(!addr_o&&addr_s)
	{
		if(memctr.in_register(src,code->get_index(),true)==NA) 
			occupy_local_register(src,code->get_index(),true);
		std::string src_s=memctr.get_operand(src,code->get_index(),true,cur_stbl);
		obj_code.add(NEG,src_s);
		obj_code.add(MOV,"["+REG_NAME_MAP[EAX]+"]",src_s);
		memctr.release_prempt_reg(EAX);memctr.release_prempt_reg(ECX);
	}else if(!addr_o&&!addr_s)
	{
		get_address_into_reg(ECX,src,code->get_index()); //src in ecx
		obj_code.add(MOV,REG_NAME_MAP[ECX],"["+REG_NAME_MAP[ECX]+"]");
		obj_code.add(NEG,REG_NAME_MAP[ECX]);
		obj_code.add(MOV,"["+REG_NAME_MAP[EAX]+"]",REG_NAME_MAP[ECX]);
		memctr.release_prempt_reg(EAX);memctr.release_prempt_reg(ECX);
	}
}
void asm_translator::translate_add(FBlock* scope, Quadcode* code)
{
	translate_arithmetic(scope,code,ADD);
}
void asm_translator::translate_sub(FBlock* scope, Quadcode* code)
{
	translate_arithmetic(scope,code,SUB);
}
void asm_translator::translate_mul(FBlock* scope, Quadcode* code)
{
	translate_arithmetic(scope,code,IMUL);
}
void asm_translator::translate_div(FBlock* scope, Quadcode* code)
{
	symbol* src1=code->get_src_arg()[0];
	symbol* src2=code->get_src_arg()[1];
	symbol* obj=code->get_obj_arg();
	bool src1_op=(src1->get_sym_type()!=symbol::ARRAY&&src1->get_sym_type()!=symbol::REFSYM&&!memctr.is_outer(src1,cur_stbl));
	bool src2_op=(src2->get_sym_type()!=symbol::ARRAY&&src2->get_sym_type()!=symbol::REFSYM&&!memctr.is_outer(src2,cur_stbl));
	assert(src2_op||src1_op);		//I put some extra requrement for quad code, at least one source operand can be addressed statically by ebp
	if(!src1_op) get_address_into_reg(ECX,src1,code->get_index());
	if(!src2_op) get_address_into_reg(ECX,src2,code->get_index());

	occupy_local_register(obj,EAX,code->get_index(),false);//the result will be left in eax, obj will always in local temp variable
	release_local_register(EDX,code->get_index()); //the reminder will be left into edx
	if(src1_op&&src2_op)
	{
		obj_code.add(MOV,"EAX",memctr.get_operand(src1,code->get_index(),true,cur_stbl));
		obj_code.add(CDQ);
		obj_code.add(IDIV,memctr.get_operand(src2,code->get_index(),true,cur_stbl));
	}else if(src1_op&&!src2_op)
	{		obj_code.add(MOV,"EAX",memctr.get_operand(src1,code->get_index(),true,cur_stbl));
		obj_code.add(CDQ);
		obj_code.add(IDIV,"dword ptr [ECX]");
	}
	else if(src2_op&&!src1_op)
	{
		obj_code.add(MOV,"EAX","dword ptr [ECX]");
		obj_code.add(CDQ);
		obj_code.add(IDIV,memctr.get_operand(src2,code->get_index(),true,cur_stbl));
	}
	if(!src1_op) memctr.release_prempt_reg(ECX);
	if(!src2_op) memctr.release_prempt_reg(ECX);
}

void asm_translator::translate_jgt(FBlock* scope, Quadcode* code)
{
	translate_branch(scope, code, JG);
}
void asm_translator::translate_jge(FBlock* scope, Quadcode* code)
{
	translate_branch(scope, code, JGE);
}
void asm_translator::translate_jlt(FBlock* scope, Quadcode* code)
{
	translate_branch(scope, code, JL);
}
void asm_translator::translate_jle(FBlock* scope, Quadcode* code)
{
	translate_branch(scope, code, JLE);
}
void asm_translator::translate_jeq(FBlock* scope, Quadcode* code)
{
	translate_branch(scope, code, JE);
}
void asm_translator::translate_jne(FBlock* scope, Quadcode* code)
{
	translate_branch(scope, code, JNE);
}

void asm_translator::translate_jmp(FBlock* scope, Quadcode* code)
{
	symbol* src = code->get_src_arg()[0];
	release_local_register(code->get_index());
	obj_code.add(JMP, memctr.get_op_name(src));
}
void asm_translator::translate_fun(FBlock* scope, Quadcode* code)
{
	symbol* f_name=code->get_obj_arg();
	obj_code.add(PROC,memctr.get_op_name(f_name));
	if(f_name->get_index()!=0) //save all the global regs begin function clause
	{
		obj_code.add(PUSH,memctr.get_operand(EBX));
		obj_code.add(PUSH,memctr.get_operand(EBP));
		obj_code.add(PUSH,memctr.get_operand(ESI));
		obj_code.add(PUSH,memctr.get_operand(EDI));
	}
	obj_code.add(SUB,memctr.get_operand(ESP),utility::to_string(memctr.get_local_space(f_name)));	//sub esp, allocate tje memory for local variables
	obj_code.add(MOV,memctr.get_operand(EBP),memctr.get_operand(ESP));	//assign the esp to ebp, ebp wii be used for address
}
void asm_translator::translate_call(FBlock* scope, Quadcode* code) //remember to write back all the regs here
{
	symbol* src = code->get_src_arg()[0];
//	symbol* obj = quat->get_object_args();
	release_local_register(code->get_index());	 //release all the local regs before enter other function
	int src_lev,cur_lev;
	src_lev=memctr.get_nested_lev(src);
	cur_lev=memctr.get_nested_lev(cur_stbl);
	if(src_lev>=cur_lev)	//set up the display area
	{
		for(int i=cur_lev-1;i>=0;i--)
			obj_code.add(PUSH, memctr.get_disp_addr(i,cur_stbl));
		if(src_lev==cur_lev+1)
			obj_code.add(PUSH,memctr.get_operand(EBP));
		assert(!(src_lev>(cur_lev+1)));
	}
	else
	{
		for(int i=0;i<src_lev;i++)
			obj_code.add(PUSH,memctr.get_disp_addr(i,cur_stbl));
	}
	obj_code.add(CALL, memctr.get_op_name(src));
}
void asm_translator::translate_sind(FBlock* scope,Quadcode* code) //the index can only be variable and with in the cur_stbl
{
	symbol* array=code->get_src_arg()[0];
	symbol* index=code->get_src_arg()[1];
	std::vector<symbol*> srcs=code->get_src_arg();
	assert(array->get_sym_type()==symbol::ARRAY);
	if(srcs.size()==2) array->set_array_index(index,true,code->get_index());
	else array->set_array_index(index,false,code->get_index());
}
void asm_translator::translate_push(FBlock* scope, Quadcode* code)
{
	symbol* obj=code->get_src_arg()[0];  //push, symbol in src arg, position 0
	//if(obj->get_sym_type()!=symbol::re)
	if(!memctr.is_outer(obj,cur_stbl)&&obj->get_sym_type()!=symbol::ARRAY&&obj->get_sym_type()!=symbol::REFSYM)
	{
		obj_code.add(PUSH,memctr.get_operand(obj,code->get_index(),true,cur_stbl));
	}
	else {	//put the address of src operand into a reg
		get_address_into_reg(EAX,obj,code->get_index());
		obj_code.add(PUSH,memctr.get_operand(EAX));
	}
//	memctr.increase_offset(4);
}
void asm_translator::translate_sref(FBlock* scope,Quadcode* code)	//reference symbol, should push the address
{
	symbol* sym=code->get_src_arg()[0];
	std::string real_addr;
	//occupy_local_register(sym,EAX,code->get_index(),true);
	if(sym->is_global()&&sym->get_sym_type()!=symbol::ARRAY&&sym->get_sym_type()!=symbol::REFSYM)
	{
		real_addr=memctr.get_real_address_global(sym,cur_stbl);
		obj_code.add(PUSH,real_addr);
//		memctr.increase_offset(4);
		return;
	}
	else //possible sref a reference symbol or an array symbol, a reference symmbol can refer to a reference
	{
		get_address_into_reg(EAX,sym,code->get_index());
		obj_code.add(PUSH,memctr.get_operand(EAX));
		memctr.release_prempt_reg(EAX);
	//	memctr.increase_offset(4);
	}
}
//in this case, the array will need two temp regs simultaneously,
//the reference and other type will need 1 temp regs
//the array will always indexed by a local temp variable
//in assignment stmt, the symbol on both side cannot be array simultaneously
//in the arithmetic statement, when encounter array, first assign it to a temporary local variable, then use the
//temp var to finish the statement
void asm_translator::get_address_into_reg(reg_name name,symbol* sym,unsigned int cur_line)	//get the address of a given symbol
{
	premmpt_local_register(name,cur_line);
	int var_delta;
	if(!sym->is_global())
	{
	
		if(!memctr.is_outer(sym,cur_stbl))	//local, mov ebp into the reg 'name' then add/sub the delta
		{	obj_code.add(MOV,memctr.get_operand(name),memctr.get_operand(EBP));
			var_delta=memctr.get_real_address_var(sym,cur_stbl);
			if(var_delta<0)
				obj_code.add(ADD,memctr.get_operand(name),utility::to_string(-var_delta));
			else if(var_delta>0)
				obj_code.add(SUB,memctr.get_operand(name),utility::to_string(var_delta));
		}
		else
		{
			get_outer_add_into_reg(name,sym,cur_stbl);
		}
		if(sym->get_sym_type()!=symbol::REFSYM&&sym->get_sym_type()!=symbol::ARRAY)
		{
			//now the reg name contain the address of the sym
		}
		else if(sym->get_sym_type()==symbol::REFSYM)
		{
			obj_code.add(MOV,memctr.get_operand(name),"["+REG_NAME_MAP[name]+"]");//now name contain the address it refers
		}
		else if(sym->get_sym_type()==symbol::ARRAY)	//I'v put some extra requirement on the index of the array, the index should only be local variable
		{
			symbol* index=sym->get_array_symbol(true);//if the index symbol is not in the reg, the index must in the current scope
	//		std::cout<<"array  "<<*index;
			assert(!memctr.is_outer(index,cur_stbl));
			if(memctr.in_register(index,cur_line,true)==NA)
				occupy_local_register(index, cur_line,true); //put it into reg
			obj_code.add(IMUL,memctr.get_operand(index,cur_line,true,cur_stbl),utility::to_string(4));	//the address is four bytes aligned
			obj_code.add(ADD,memctr.get_operand(name),memctr.get_operand(index,cur_line,true,cur_stbl));
			//now the name contains the address of the given array
		}
	}
	else	//only array is possible in global
	{
		symbol* index=sym->get_array_symbol(true);
		if(memctr.in_register(index,cur_line,true)==NA)
			occupy_local_register(index,cur_line,true);
		obj_code.add(MOV,memctr.get_operand(name),memctr.get_real_address_global(sym,cur_stbl));//load the current global address into reg
		if(sym->get_sym_type()==symbol::ARRAY)
		{
			obj_code.add(IMUL,memctr.get_operand(index,cur_line,true,cur_stbl),utility::to_string(4));
			obj_code.add(ADD,memctr.get_operand(name),memctr.get_operand(index,cur_line,true,cur_stbl));
		}
	}
}
//get the address of outer symbol into a reg
void asm_translator::get_outer_add_into_reg(reg_name name,symbol* sym,SymbolTable* cur_stbl)
{
	obj_code.add(MOV, memctr.get_operand(name),memctr.get_disp_addr(sym,cur_stbl));	//move the accordign display addr into the reg
	int delta=memctr.get_outer_address(sym,cur_stbl);
	if(delta<0)
		obj_code.add(ADD,memctr.get_operand(name),utility::to_string(-delta));	//add/sub the delta
	else if(delta>0)
		obj_code.add(SUB,memctr.get_operand(name),utility::to_string(delta)); //then the name contain the addr of sym
}
void asm_translator::translate_param(FBlock* scope, Quadcode* code)
{
	symbol* obj=code->get_obj_arg();
	if(memctr.in_register(obj,code->get_index(),false))
	{
		obj_code.add(MOV, memctr.get_operand(obj,code->get_index(),false,cur_stbl),memctr.get_address(obj,cur_stbl));
	}
}
void asm_translator::translate_ret(FBlock* scope, Quadcode* code)
{
	assert(0);
}
void asm_translator::translate_endf(FBlock* scope, Quadcode* code)
{
	symbol* sym=code->get_obj_arg();
	release_local_register(code->get_index());//release all the  local regs that used
	
	obj_code.add(LABEL,"@end"+memctr.get_op_name(sym));	//end clause
	obj_code.add(ADD,memctr.get_operand(ESP),utility::to_string(memctr.get_local_space(sym))); //add stack, delete the used local space
	if(sym->get_index()!=0)  //pop, recover all the global regs
	{
		obj_code.add(POP,memctr.get_operand(EDI));
		obj_code.add(POP,memctr.get_operand(ESI));
		obj_code.add(POP,memctr.get_operand(EBP));
		obj_code.add(POP,memctr.get_operand(EBX));
	}
	obj_code.add(RET,utility::to_string(memctr.get_args_space(sym)));//ret, add stack, release the space used by args,also display area
	obj_code.add(ENDP,memctr.get_op_name(sym));
	if(sym->get_index()==0)
		obj_code.add(END,memctr.get_op_name(sym));
}
void asm_translator::translate_label(FBlock* scope, Quadcode* code)
{
	symbol* src = code->get_src_arg()[0];
	release_local_register(code->get_index());//clear temporary reg pool
	obj_code.add(LABEL, memctr.get_op_name(src));
}
void asm_translator::translate_read(FBlock* scope, Quadcode* code)
{
	symbol* obj=code->get_obj_arg();
	release_local_register(code->get_index());	//before call other function, release all the local temp variable
	bool addr=(obj->get_sym_type()!=symbol::REFSYM&&obj->get_sym_type()!=symbol::ARRAY&&!memctr.is_outer(obj,cur_stbl));
	obj_code.add(SUB,"ESP","4");
	obj_code.add(PUSH,"ESP");
	if(obj->get_val_type()==symbol::INTEGER)
	{
		obj_code.add(PUSH,"offset "+scanf_int);
	}
	else if(obj->get_val_type()==symbol::CHAR)
	{
		obj_code.add(PUSH,"offset "+scanf_char);
	}
	obj_code.add(CALL,"scanf");
	obj_code.add(ADD,"ESP","8");	//ret the two pushed args, for scanf
	if(!addr) {
		get_address_into_reg(EAX,obj,code->get_index());
		obj_code.add(POP,"["+REG_NAME_MAP[EAX]+"]");
		memctr.release_prempt_reg(EAX);
	}
	else obj_code.add(POP,memctr.get_operand(obj,code->get_index(),false,cur_stbl));
}
void asm_translator::translate_write(FBlock* scope, Quadcode* code)
{
	symbol* src=code->get_src_arg()[0];
	release_local_register(code->get_index());//will call printf, so release all temp reg accordingly
	symbol::symbol_type type=src->get_sym_type();
	if(type!=symbol::REFSYM&&type!=symbol::ARRAY&&!memctr.is_outer(src,cur_stbl))
	{		std::string src_ope=memctr.get_operand(src,code->get_index(),true,cur_stbl);
		switch(src->get_val_type())
		{
		case symbol::INTEGER:
			obj_code.add(PUSH, src_ope);
			obj_code.add(PUSH, "offset " + printf_int);
			obj_code.add(CALL, "printf");
			obj_code.add(ADD, REG_NAME_MAP[ESP], "8");
			break;
		case symbol::CHAR:
			obj_code.add(PUSH, src_ope);
			obj_code.add(PUSH, "offset " + printf_char);
			obj_code.add(CALL, "printf");
			obj_code.add(ADD, REG_NAME_MAP[ESP], "8");
			break;
		case symbol::STRING:
			obj_code.add(PUSH, "offset " + src_ope);
			obj_code.add(CALL, "printf");
			obj_code.add(ADD,REG_NAME_MAP[ESP], "4");
			break;
		default:
			;
		}
	}
	else
	{
		get_address_into_reg(EAX,src,code->get_index());
		obj_code.add(PUSH,"["+REG_NAME_MAP[EAX]+"]");
		if(src->get_val_type()==symbol::INTEGER)
			obj_code.add(PUSH, "offset " + printf_int);
		else if(src->get_val_type()==symbol::CHAR)
			obj_code.add(PUSH, "offset " + printf_char);
		release_local_register(code->get_index());
		obj_code.add(CALL, "printf");
		obj_code.add(ADD, "ESP", "8");
		memctr.release_prempt_reg(EAX);
	}
}
void asm_translator::translate_new_line(FBlock* scope, Quadcode* code)	//equal to printf("\n")
{
	obj_code.add(PUSH,"offset "+new_line);
	obj_code.add(CALL,"printf");
	obj_code.add(ADD,"ESP","4");
}
//line is the line that symbol is in, cur_line is the current line, 
void asm_translator::write_reg_back(symbol* sym, reg_name name, unsigned int line, bool used, unsigned int cur_line)
{
	if(sym != NULL)
	{
		memctr.release_register(name);
		if(is_alive(sym, line, used, cur_line))//if this symbol is still alive, we need to write it back
		{
			if(!sym->get_sym_type()==symbol::LITERAL)
			{
				obj_code.add(MOV, memctr.get_operand(sym, line, used,cur_stbl), memctr.get_operand(name));
			}
		}
	}
}
//line is the line that symbol is in, in_line is the current line, judge whether that symbol is alive in current line
bool asm_translator::is_alive(symbol* sym, unsigned int line, bool use, unsigned int cur_line)
{
	if(das==NULL)
	{
		return true;
	}
	else	//if we do not have data stream, all variables will be assumed to be live,
	{
		if(sym->get_sym_type()==symbol::LITERAL)
		{
			return false;
		}
		else if(sym->is_global())
		{
			return true;
		}
		else	//if we have the data stream analysis, then if we still assumes that all symbols be alive, we may make fault by
		{		//write back the unnecessary values, ie, if one symbol have multiple webs according to it, if we assumes it is
				//alive, then we will write back the old value in the previous web to next web, so we will make a mistake
			return memctr.is_alive(sym, line, use, cur_line);
		}
	}
}
void asm_translator::release_local_register(reg_name name,unsigned int cur_line)
{   typedef pair<unsigned int, bool> typ;
	pair<symbol*, typ > prev = memctr.get_symbol_in_reg(name);
	if(prev.first!=NULL)
		write_reg_back(prev.first, name, prev.second.first, prev.second.second, cur_line);
	memctr.release_register(name);
}
void asm_translator::release_local_register(unsigned int in_line)
{
	release_local_register(EDI, in_line);
	release_local_register(ESI, in_line);
	release_local_register(EBP, in_line);
	release_local_register(EBX, in_line);
	release_local_register(EDX, in_line);
	release_local_register(ECX, in_line);
	release_local_register(EAX, in_line);
}
//occupy a local reg by a symbol, do not specify the reg used
void asm_translator::occupy_local_register(symbol* sym, unsigned int line, bool use)
{
	reg_name name=memctr.allocate(sym);
	occupy_local_register(sym,name,line,use);
}
//occupy a local reg by a symbol, specify the reg used
void asm_translator::occupy_local_register(symbol* sym, reg_name name, unsigned int line, bool use)
{
	release_local_register(name, line);
	obj_code.add(MOV, memctr.get_operand(name), memctr.get_operand(sym,line, use,cur_stbl));
	memctr.occupy_register(name, sym, line, use);
}
//first write back the symbol in the reg, then modify the data structure, <reg, symbol*>, and <symbol*, reg>
//this will occupy the reg, ie modify the reg statue, put it to the back of the reg heap, the  next required reg will not be this reg
void asm_translator::premmpt_local_register(reg_name name, unsigned int cur_line)
{
	release_local_register(name,cur_line);
	memctr.prempt_reg(name);
}
//translate a arthmetic code, ie, add ,sub, and mul
void asm_translator::translate_arithmetic(FBlock* scope, Quadcode* code, asm_type typ)	
{
	symbol* src1=code->get_src_arg()[0];
	symbol* src2=code->get_src_arg()[1];
	symbol* obj=code->get_obj_arg();	//the obj should only be local temp variale
	assert(!memctr.is_outer(obj,cur_stbl)&&obj->get_sym_type()!=symbol::ARRAY&&obj->get_sym_type()!=symbol::REFSYM);
	symbol::symbol_type src_typ1,src_typ2; src_typ1=src1->get_sym_type();src_typ2=src2->get_sym_type();
	bool addr_s1=(src_typ1!=symbol::ARRAY&&src_typ1!=symbol::REFSYM&&!memctr.is_outer(src1,cur_stbl));
	bool addr_s2=(src_typ2!=symbol::ARRAY&&src_typ2!=symbol::REFSYM&&!memctr.is_outer(src2,cur_stbl));
	//both addr_s1 and addr_s2 should not be false simultaneously, I've put additional requirement on the quad code
	if(src1==src2&&!addr_s1)
	{	
		get_address_into_reg(EAX,src1,code->get_index());
	}
	else
	{
		if(!addr_s1) get_address_into_reg(ECX,src1,code->get_index());
		if(!addr_s2) get_address_into_reg(EDX,src2,code->get_index());
	}
	if(memctr.in_register(obj, code->get_index(), false) == NA)//the obj must in a reg
	{   
		occupy_local_register(obj, code->get_index(), false);
	}
	std::string obj_op_f=memctr.get_operand(obj, code->get_index(),false,cur_stbl);
	std::string obj_op_t=memctr.get_operand(obj, code->get_index(),true,cur_stbl);
	std::string src1_op;if(addr_s1)src1_op=memctr.get_operand(src1,code->get_index(),true,cur_stbl);
	std::string src2_op;if(addr_s2)src2_op=memctr.get_operand(src2,code->get_index(),true,cur_stbl);
	if(src1 == obj||src2==obj)//two obj can be in different regs
	{
		if(strcmp(obj_op_f.c_str(),obj_op_t.c_str())!=0)
			obj_code.add(MOV, obj_op_f,obj_op_t);//this two obj can be in different regs!
		if(src1==obj&&addr_s2)
			obj_code.add(typ, obj_op_f, src2_op);
		else if(src1==obj&&!addr_s2)
			obj_code.add(typ,obj_op_f,"["+REG_NAME_MAP[EDX]+"]");

		if(src2==obj&&addr_s1)
			obj_code.add(typ,obj_op_f,src1_op);
		else if(src2==obj&&!addr_s1)
			obj_code.add(typ,obj_op_f,"["+REG_NAME_MAP[ECX]+"]");
		if(!addr_s1) memctr.release_prempt_reg(ECX); if(!addr_s2) memctr.release_prempt_reg(EDX);
	}
	else if(src1 == src2)
	{
		// obj != src1 == src2, so we have addr_s1=addr_s2
		if(addr_s1)
		{
			obj_code.add(MOV, obj_op_f, src1_op);
			obj_code.add(typ, obj_op_f,obj_op_f);
		}
		else
		{
			obj_code.add(MOV,obj_op_f,"["+REG_NAME_MAP[EAX]+"]");
			obj_code.add(typ,obj_op_f,obj_op_f);
			memctr.release_prempt_reg(EAX);
		}
	}
	else
	{
		// obj != src1 != src2
		if(addr_s1&&addr_s2)
		{
			obj_code.add(MOV, obj_op_f, src1_op);
			obj_code.add(typ, obj_op_f, src2_op);
		}
		else if(!addr_s1&&addr_s2)
		{
			obj_code.add(MOV,obj_op_f,"["+REG_NAME_MAP[ECX]+"]");
			obj_code.add(typ,obj_op_f,src2_op);
			memctr.release_prempt_reg(ECX);
		}
		else if(!addr_s2&&addr_s1)
		{
			obj_code.add(MOV,obj_op_f,src1_op);
			obj_code.add(typ,obj_op_f,"["+REG_NAME_MAP[EDX]+"]");
			memctr.release_prempt_reg(EDX);
		}
		else
		{
			obj_code.add(MOV,obj_op_f,"["+REG_NAME_MAP[ECX]+"]");
			obj_code.add(typ,obj_op_f,"["+REG_NAME_MAP[EDX]+"]");
			memctr.release_prempt_reg(EDX);
			memctr.release_prempt_reg(ECX);
		}
	}
}
void asm_translator::translate_branch(FBlock* scope, Quadcode* code, asm_type typ)
{
	symbol* src1 = code->get_src_arg()[0];
	symbol* src2 = code->get_src_arg()[1];
	symbol* target = code->get_src_arg()[2];
	symbol::symbol_type src_typ1,src_typ2; src_typ1=src1->get_sym_type();src_typ2=src2->get_sym_type();
	bool addr_s1=(src_typ1!=symbol::ARRAY&&src_typ1!=symbol::REFSYM&&!memctr.is_outer(src1,cur_stbl));
	bool addr_s2=(src_typ2!=symbol::ARRAY&&src_typ2!=symbol::REFSYM&&!memctr.is_outer(src2,cur_stbl));
	if(!addr_s1) get_address_into_reg(EAX,src1,code->get_index());
	if(!addr_s2) get_address_into_reg(ECX,src2,code->get_index());
	if(addr_s1&&addr_s2)
	{
		if(memctr.in_register(src1,code->get_index(),true)==NA&&memctr.in_register(src2,code->get_index(),true)==NA)
		{
			occupy_local_register(src1,code->get_index(),true);
		}
		obj_code.add(CMP,memctr.get_operand(src1,code->get_index(),true,cur_stbl),memctr.get_operand(src2,code->get_index(),true,cur_stbl));//can cmp compate two values in the memory?
	
	}
	if(!addr_s1&&addr_s2)
	{
		obj_code.add(MOV,REG_NAME_MAP[EAX],"["+REG_NAME_MAP[EAX]+"]"); //src1 in reg
		obj_code.add(CMP,REG_NAME_MAP[EAX],memctr.get_operand(src2,code->get_index(),true,cur_stbl));
		memctr.release_prempt_reg(EAX);
	} else if(!addr_s2&&addr_s1)
	{
		obj_code.add(MOV,REG_NAME_MAP[ECX],"["+REG_NAME_MAP[ECX]+"]");
		obj_code.add(CMP,memctr.get_operand(src1,code->get_index(),true,cur_stbl),REG_NAME_MAP[ECX]);
		memctr.release_prempt_reg(ECX);
	}else if(!addr_s1&&!addr_s2)
	{
		obj_code.add(MOV,REG_NAME_MAP[EAX],"["+REG_NAME_MAP[EAX]+"]");
		obj_code.add(CMP,REG_NAME_MAP[EAX],"["+REG_NAME_MAP[ECX]+"]");
		memctr.release_prempt_reg(EAX);memctr.release_prempt_reg(ECX);
	}
	release_local_register(code->get_index());  //another basic block begin, clear temporary reg pool
	obj_code.add(typ, memctr.get_op_name(target));
}