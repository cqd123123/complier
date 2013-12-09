#pragma warning(disable:4786)
#include "symbol.h"
#include <VECTOR>
#include "SymbolTable.h"
#include <CASSERT>
#include <IOSTREAM>
#include <SET>
const std::string VALUE_TYPE_MAP[] = 
{
		"CHAR",
		"STRING",
		"INTEGER"
};
const std::string SYMBOL_TYPE_MAP[]=
{
		"LITERAL",
		"CONST",
		"VARIABLE",
		"ARGUMENT",
		"FUNCTION",
		"PROCEDURE",
		"LABEL",
		"ARRAY",
		"AUXSYM",
		"REFSYM"
};

symbol::symbol_type symbol::get_sym_type()
{
	return type;
}
symbol::symbol_value_type symbol::get_val_type()
{
	return val_type;
}
std::string symbol::get_id()
{
	return id;
}
std::string symbol::get_value()
{
	return val;
}
std::vector<symbol*>* symbol::get_params()
{
	return params;
}
symbol* symbol::lit_symbol(std::string vale, symbol_value_type typ)
{
	symbol* s=new symbol();
	s->val=vale;
	s->val_type=typ;
	s->type=LITERAL;
	return s;
}
symbol* symbol::func_symbol(std::string _id,symbol_value_type typ,std::vector<symbol*>* param, unsigned int Index)
{
	symbol* s=new symbol();
	s->val_type=typ;
	s->id=_id;
	s->params=param;
	s->type=FUNCTION;
	s->size=4;
	s->index=Index;
	return s;
}
symbol* symbol::proc_symbol(std::string _id,std::vector<symbol*>* param, unsigned int Index)
{
	symbol* s=new symbol();
	s->id=_id;
	s->params=param;
	s->type=PROCEDURE;
	s->size=0;
	s->index=Index;
	return s;
}
symbol* symbol::var_symbol(std::string _id,symbol_value_type typ)
{
	symbol* s=new symbol();
	s->id=_id;
	s->val_type=typ;
	s->size=4;
	s->type=VARIABLE;
	return s;
}
symbol* symbol::label_symbol(std::string _id)
{
	symbol* s=new symbol();
	s->type=LABEL;
	s->size=0;
	s->id=_id;
	return s;
}
symbol* symbol::const_symbol(std::string _id,std::string val, symbol_value_type typ)
{
	symbol* s=new symbol();
	s->type=CONST;
	s->val=val;
	s->id=_id;
	s->size=4;
	s->val_type=typ;
	return s;
}
symbol* symbol::array_symbol(std::string _id,unsigned int size,symbol_value_type typ)
{
	symbol* s=new symbol();
	s->type=ARRAY;
	s->val_type=typ;
	s->id=_id;
	s->size=size*4;
	s->array_index.clear();
	s->marked=false;
	return s;
}
symbol* symbol::array_symbol(symbol* sym)//take care init all the properity of new symbol with sym
{
	assert(sym->get_sym_type()==symbol::ARRAY);
	symbol* s=new symbol();
	s->type=ARRAY;
	s->val_type=sym->get_val_type();
	s->id=sym->get_id();
	s->size=sym->size;
	s->array_index.clear();
	s->scope=sym->scope;
	s->marked=false;
	return s;
}
void symbol::set_array_index(symbol* _index,bool back, unsigned int _code_line)
{
//	assert(this->get_sym_type()==symbol::ARRAY);
	if(back) this->array_index.push_back(_index);
	else this->array_index.push_front(_index);
	_index->code_line=_code_line;
}
unsigned int symbol::get_code_index()
{
	return code_line;
}
symbol* symbol::argument_symbol(std::string _id ,symbol_value_type typ)
{
	symbol* s=new symbol();
	s->type=ARGUMENT;
	s->val_type=typ;
	s->id=_id;
	s->size=4;
	return s;
}
symbol* symbol::type_symbol(symbol::symbol_value_type typ, int size)
{
	symbol* s=new symbol();
	s->type=AUXSYM;
	s->val_type=typ;
	s->index=size;
	return s;
}
/*take care, this symbol directly refer to the address of a given sym
during the process, it will be initialized when the function is called,
and will be assigned to the same memory address during the memory allocation process*/
symbol* symbol::ref_symbol(std::string _id, symbol::symbol_value_type typ)
{
	symbol* s=new symbol();
	s->type=REFSYM;
	s->val_type=typ;
	s->ref_sym=NULL;
	s->size=4;
	s->id=_id;
	return s;
}
void symbol::set_reference(symbol* ref)
{
	ref_sym=ref;
}
symbol* symbol::get_ref_sym()//get_ref_sym, possible nested reference
{
	symbol* sym=this->ref_sym;
	while(sym->get_sym_type()==symbol::REFSYM)
	{
		sym=sym->ref_sym;
	}
	assert(sym!=NULL);
	return sym;
}
bool symbol::is_ref_null()
{
	if(this->ref_sym==NULL) return true;
	symbol* sym=this->ref_sym;
	if(sym->ref_sym==NULL) return true;
	std::set<symbol*> path;
	path.insert(sym);
	while(sym->get_sym_type()==symbol::REFSYM&&path.find(sym->ref_sym)==path.end())
	{
		sym=sym->ref_sym;
		path.insert(sym);
	}
	if(sym->get_sym_type()==symbol::REFSYM) return true;
	return sym==NULL;
}
void symbol::set_scope(SymbolTable* tab)
{
	scope=tab;
}
SymbolTable* symbol::get_scope()
{
	return scope;
}
SymbolTable* symbol::get_cur_scope()
{
	return cur_scope;
}
int symbol::get_index()
{
	return index;
}
symbol* symbol::get_array_symbol(bool pop)
{
	if(array_index.size()==0) return NULL;
	symbol* _index=array_index.back();
	if(array_index.size()>=1&&pop) array_index.pop_back();
	return _index;
}
unsigned int symbol::get_size()
{
	return this->size;
}
bool symbol::is_global()
{
	return this->get_scope()->get_name()=="@global";
}
void symbol::set_cur_scope(SymbolTable* tbl)
{
	this->cur_scope=tbl;
}
bool symbol::get_maked()
{
	return this->marked;
}
void symbol::set_marked(bool mark)
{
	this->marked=mark;
}
void operator<<(std::ostream& out, const symbol& _symbol)
{
	if(_symbol.type == symbol::CONST)
	{
		out << "[COSNT " << VALUE_TYPE_MAP[_symbol.val_type] << " " << _symbol.id << " = " << _symbol.val;
	}
	else if((_symbol.type == symbol::VARIABLE))
	{
		out << "["<< VALUE_TYPE_MAP[_symbol.val_type] << " " << _symbol.id;
	}
	else if(_symbol.type==symbol::ARGUMENT)
	{
		out<<"["<<"arg "<<VALUE_TYPE_MAP[_symbol.val_type]<<" "<<_symbol.id;
	}
	else if(_symbol.type == symbol::FUNCTION)
	{
		out << "[" << VALUE_TYPE_MAP[_symbol.val_type] <<_symbol.index<< " " << _symbol.id << "(";
		std::vector<symbol*>::iterator iter = _symbol.params->begin();
		if(_symbol.params->size()!=0)
		{
			out << *(*iter);
			iter++;
			while(iter != _symbol.params->end())
			{
				out << ", " << *(*iter);
				iter++;
			}
		
		}	out << ")";
	}
	else if(_symbol.type == symbol::LABEL)
	{
		out << "[LABEL " << _symbol.id;
	}
	else if(_symbol.type == symbol::LITERAL)
	{
		out << "[" << VALUE_TYPE_MAP[_symbol.val_type] << " " << _symbol.val;
	}
	else if(_symbol.type==symbol::ARRAY)
	{
		out<<"["<<_symbol.id<<"["<<_symbol.size/4<<"]"<<":"<<VALUE_TYPE_MAP[_symbol.val_type];
	}
	else if(_symbol.type==symbol::PROCEDURE)
	{
		out<<"[PROC "<<_symbol.id;
	}
	else if(_symbol.type==symbol::REFSYM)
	{
//		if(_symbol.ref_sym!=NULL)
//			out<<"[REFSYM "<<_symbol.id<<"-> "<<_symbol.ref_sym->get_id();
//		else
			out<<"[REFSYMh "<<_symbol.id<<"-> "<<"NULL";
	}
	out << "]" << std::endl;
}