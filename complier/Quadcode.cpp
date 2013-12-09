#pragma warning(disable:4786)
#include "symbol.h"
#include "Quadcode.h"
#include <VECTOR>
#include <STRING>
#include <CASSERT>
#include <iomanip>

unsigned int Quadcode::counter=0;

const std::string Quad_Code_Map[]=
{
		"ADD",
		"ASSIGN",
		"MUL",
		"SUB",
		"DIV",//divide
		"INV",//inverse
		"CALL",//call
		"FUNC",
		"PROC",
		"JMP",
		"JGT",//jump greater than
		"JGE",//jump great equal
		"JEQ",//jump eq
		"JNE",
		"JLE",//jump less equal
		"JLT",//jump less than
		"LABEL",
		"READ",
		"WRITE",
		"PARAM",//parameter
		"SREF",
		"ENDF",
		"ENDP",
		"RET",
		"PUSH",
		"POP",
		"MAIN",
		"ENDM",
		"SIND",
		"LINE"
};
Quadcode::Quadcode()
{
	index=counter++;
	obj=NULL;
	enable=true;
}
Quadcode* Quadcode::new_Quad_code(Quad_type typ,symbol* obj, symbol* src1, symbol* src2)
{
	Quadcode* qcode=new Quadcode();
	switch(typ)
	{
		case ADD:
		case DIV:
		case MUL:
		case SUB:
			qcode->type=typ;
			qcode->src.push_back(src1);
			qcode->src.push_back(src2);
			qcode->obj=obj;
			break;
		case JEQ:
		case JNE:
		case JGE:
		case JGT:
		case JLE:
		case JLT:
			qcode->type=typ;
			qcode->src.push_back(src1);
			qcode->src.push_back(src2);
			qcode->src.push_back(obj);
			break;
		case SIND:
			qcode->type=typ;
			qcode->src.push_back(obj);
			qcode->src.push_back(src1);
			qcode->src.push_back(src2);
			break;
		default:
			assert(0);
	}
	qcode->symbols.push_back(obj);
	qcode->symbols.push_back(src1);
	qcode->symbols.push_back(src2);
	return qcode;
}
Quadcode* Quadcode::new_Quad_code(Quad_type typ,symbol* obj,symbol* src)
{
	Quadcode* qcode=new Quadcode();
	switch(typ)
	{
		case ASSIGN:
		case INV:
		case CALL:
			qcode->obj=obj;
			qcode->src.push_back(src);
			qcode->type=typ;
			break;
		case RET: //both src
		case SIND:
			qcode->src.push_back(obj);
			qcode->src.push_back(src);
			qcode->type=typ;
			break;
		default:
			assert(0);
	}
	qcode->symbols.push_back(obj);
	qcode->symbols.push_back(src);
	return qcode;
}
Quadcode* Quadcode::new_Quad_code(Quad_type typ,symbol* src)
{
	Quadcode* qcode=new Quadcode();
	switch(typ)
	{
		case CALL:
		case PUSH:
		case WRITE:	
	//	case PROC:
		case RET:
		case JMP:
		case SREF:
		case LABEL:
//		case SIND:
			qcode->src.push_back(src);
			qcode->type=typ;
			break;
		case READ://read from console
		case FUNC:
		case ENDF:
		case ENDP:
		case MAIN:
		case ENDM:
		case PARAM:
			qcode->obj=src;
			qcode->type=typ;
			break;
		default:
			assert(0);
	}
	qcode->symbols.push_back(src);
	return qcode;
}
Quadcode* Quadcode::new_Quad_code(Quad_type typ)
{
	Quadcode* qcode=new Quadcode();
	switch (typ)
	{
//	case MAIN:
	case ENDM:
	case LINE:
		qcode->type=typ;
		break;
	default:
		assert(0);
	}
	return qcode;
}
bool Quadcode::is_enable() const
{
	return enable;
}
void Quadcode::disable()
{
	enable=false;
	std::vector<symbol*> src_args=this->get_src_arg();
	symbol* obj=this->get_obj_arg();
	for(int i=0;i<src_args.size();i++)
	{
		if(src_args[i]->get_sym_type()!=symbol::LITERAL)
			src_args[i]->get_scope()->update_reference(src_args[i],-1);
	}
	if(obj!=NULL)
		if(obj->get_sym_type()!=symbol::LITERAL)
			obj->get_scope()->update_reference(obj,-1);
}
std::vector<symbol*> Quadcode::get_all_sym()
{
	return symbols;
}
unsigned int Quadcode::get_index() const
{
	return index;
}
symbol* Quadcode::get_obj_arg()
{
	return obj;
}
std::vector<symbol*> Quadcode::get_src_arg()
{
	return src;
}
Quadcode::Quad_type Quadcode::get_quad_type() const
{
	return type;
}
unsigned int Quadcode::get_args_size() const
{
	return symbols.size();
}
void Quadcode::set_index(unsigned int index)
{
	this->index=index;
}
void Quadcode::reset()
{
	counter=0;
}
void operator<<(std::ostream& out, const Quadcode& q)
{
	if(!q.enable) out << "/* ";
	out << std::setiosflags(std::ios_base::left) << std::setw(4) << q.get_index() << ": ";
	out << std::setiosflags(std::ios_base::left) << std::setw(16) << Quad_Code_Map[q.get_quad_type()];
	for(unsigned int i = 0; i < q.symbols.size(); i++)
	{
		if(q.symbols[i]==NULL) {out<<std::setw(8)<<"[NULL]"; continue;}
		if(q.symbols[i]->get_sym_type()==symbol::LITERAL)
			out << std::setw(8) << q.symbols[i]->get_value();
		else if(q.symbols[i]->get_sym_type()==symbol::ARRAY)
		{
	//		if(q.symbols[i]->get_array_symbol(false)!=NULL)
	//		{
	//			symbol* index=q.symbols[i]->get_array_symbol(false);
	//			if(index->get_sym_type()==symbol::LITERAL)
	//				out<<q.symbols[i]->get_id()<<"["<<q.symbols[i]->get_array_symbol(false)->get_value()<<std::setw(8)<<"]";
	//			else
	//				out<<q.symbols[i]->get_id()<<"["<<q.symbols[i]->get_array_symbol(false)->get_id()<<std::setw(8)<<"]";
	//		}
	//		else
	//			out<<q.symbols[i]->get_id()<<"["<<"NULL"<<std::setw(8)<<"]";
			out<<q.symbols[i]->get_id()<<std::setw(8)<<"[]";
		}
		else if(q.symbols[i]->get_sym_type()==symbol::REFSYM)
		{
//			if(!q.symbols[i]->is_ref_null())
//				out<<"[REFSYM "<<q.symbols[i]->get_id()<<"-> "<<q.symbols[i]->get_ref_sym()->get_id()<<std::setw(4)<<"]";
//			else
			out<<"[REFSYM "<<q.symbols[i]->get_id()<<std::setw(8)<<"]";
		}
		else if(q.symbols[i]->get_sym_type()==symbol::FUNCTION||q.symbols[i]->get_sym_type()==symbol::PROCEDURE)
		{
			out<<q.symbols[i]->get_id()<<"@"<<std::setw(8)<<q.symbols[i]->get_index();
		}
		else
			out << std::setw(8) << q.symbols[i]->get_id();
	}
	if(!q.enable) out << " */";
	out << std::endl;
}

