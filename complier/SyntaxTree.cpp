#pragma warning(disable:4786)
#include "SyntaxTree.h"
#include <SSTREAM>
#include <CASSERT>
/*In C++, the reference parameters are initialized with the actual arguments when the function is called*/
#define  _SEMATIC_PROCEDURE_HEAD(name) void SyntaxTree::name::visit(SSTable& tbl, InsCtrl& ins, errorlist& erl)

// void SyntaxTree::program::visit(SymbolTable& tbl, InsCtrl& ins, errorlist& erl)
// {
// 	;
// }
_SEMATIC_PROCEDURE_HEAD(program)
{
	symbol* sym=symbol::proc_symbol("@global",NULL,0);
	tbl.new_symbol_table(sym);
	ins.main_begin(sym);
	TNode::visit(tbl,ins,erl);//sub program
	ins.main_end(sym);
	tbl.pop_symbol_table();//pop the global symbol table
}

void SyntaxTree::sub_program::visit(SSTable& tbl, InsCtrl& ins, errorlist& erl)
{
//	symbol* lab;
//	tbl.new_label(lab);
	//ins.jmp(lab);
	std::vector<TNode*> ::iterator it=this->children.begin();
	while(it!=this->children.end()-1)
	{
		(*it)->visit(tbl,ins,erl);//DECLARATIONs, statement
		it++;
	}
	//ins.add_label(lab);
	(*it)->visit(tbl,ins,erl);//compound statement
}

_SEMATIC_PROCEDURE_HEAD(const_definition)
{
	std::vector<TNode*>::iterator it=this->children.begin();
	if(this->children.size()!=3) return;
	token opt;
	std::string _id;
	(*it)->visit(tbl,ins,erl,_id);//identifier
	it++;
	(*it)->visit(tbl,ins,erl,opt);//operator, this case '='
	it++;
	if(opt.get_type()!=token::RELATION_OPERATOR||opt.get_value()!="=")
	{
		erl.addError("= expected",opt.get_line());
	}
	symbol* val=NULL;
	(*it)->visit(tbl,ins,erl,val);//const value
	if(val==NULL) return;
	if(!tbl.add_symbol(val->get_val_type(),_id,val->get_value()))//symbol not added into the table
	{
		erl.addError("Duplicated identifier: "+_id,this->getline());
	}
}
void SyntaxTree::const_value::visit(SSTable& tbl, InsCtrl& ins, errorlist& erl,symbol*& sym)
{	
	std::vector<TNode*>::iterator it=this->children.begin();
	if(it==this->children.end()) return;
	token tok1;
	(*it)->visit(tbl,ins,erl,tok1);//opt node or lit node
	if(tok1.get_type()==token::ADD_OPERATOR)
	{
		it++;
		(*it)->visit(tbl,ins,erl,sym);//literal node
		//if(tok2.get_type()==token::UNSIGNED_INTEGER_CONST)
		//{
		//	symbol* s=symbol::lit_symbol(tok1.get_value()+tok2.get_value(),symbol::INTEGER);
		//sym=lit_s;
		//std::cout<<*lit_s;
		//}
		//else
		//{
		//	erl.addError("should follow integer",tok1.get_line());
		//}
	}
	else if(tok1.get_type()==token::UNSIGNED_INTEGER_CONST)
	{
		symbol* s=symbol::lit_symbol(tok1.get_value(),symbol::INTEGER);
		sym=s;
		tbl.add_literal(sym);
	}
	else if(tok1.get_type()==token::CHAR_CONST)
	{
		std::string val;
		std::stringstream ss;
		ss<<tok1.get_value().c_str();
		ss>>val;
		symbol* s=symbol::lit_symbol(val,symbol::CHAR);
		sym=s;
		tbl.add_literal(sym);
	}
	else
		erl.addError("Unknow value encountered: "+tok1.get_value(),tok1.get_line());
}
_SEMATIC_PROCEDURE_HEAD(var_definition)
{
	std::vector<TNode*>::iterator it=this->children.begin();
	if(it==this->children.end()) return;
	std::vector<std::string> id_list;
	while(it!=(this->children.end()-1))
	{
		std::string _id;
		(*it)->visit(tbl,ins,erl,_id);//identifier
		id_list.push_back(_id);
		it++;
	}
	symbol* typ=NULL;
	(*it)->visit(tbl,ins,erl,typ);//type node
	if(typ==NULL) return;
	for(int i=0;i<id_list.size();i++)
	{
		if(typ->get_index()==-1)//not array
		{
			if(!tbl.add_symbol(typ->get_val_type(),id_list[i]))
				erl.addError("dulicated variable: "+id_list[i],this->getline());
		}
		else//array
		{
			if(!tbl.add_array(typ->get_val_type(),id_list[i],typ->get_index()))
				erl.addError("duplicated array: "+id_list[i],this->getline());
		}
	}
}
void SyntaxTree::type::visit(SSTable& tbl, InsCtrl& ins, errorlist& erl,symbol*& sym)
{
	std::vector<TNode*> ::iterator it=this->children.begin();
	//if(it==this->children.end()) return;
	symbol* s=NULL;
	if(this->get_child_size()==1)
	{
		token tok;
		(*it)->visit(tbl,ins,erl,tok);//basic type
		if(tok.get_type()==token::INTEGER)
			s=symbol::type_symbol(symbol::INTEGER,-1);
		else if(tok.get_type()==token::CHAR)
			s=symbol::type_symbol(symbol::CHAR,-1);
		else return;
		sym=s;
	}
	else
	{
		unsigned int size;
		std::stringstream ss;
		token tk1,tk2;
		(*it)->visit(tbl,ins,erl,tk1);//unsigned int lit
		if(tk1.get_type()!=token::UNSIGNED_INTEGER_CONST) return;//////////
		symbol* lit=symbol::lit_symbol(tk1.get_value(),symbol::INTEGER);
		tbl.add_literal(lit);
		it++;
		(*it)->visit(tbl,ins,erl,tk2);//basic type
		ss<<tk1.get_value();
		ss>>size;
		if(tk2.get_type()==token::INTEGER)
			s=symbol::type_symbol(symbol::INTEGER,size);
		else if(tk2.get_type()==token::CHAR)
			s=symbol::type_symbol(symbol::CHAR,size);
		else return;
		sym=s;
	}
}
_SEMATIC_PROCEDURE_HEAD(procedure_definition)
{
	std::vector<TNode*>::iterator it=this->children.begin();
	//if(it==this->children.end()) return;
	symbol* proc=NULL;
	(*it)->visit(tbl,ins,erl,proc);//procedure first
	it++;
//	std::cout<<*proc;
//	tbl.DumpSymBols();
	(*it)->visit(tbl,ins,erl);//sub program
	if(proc!=NULL)
	{
		ins.end_fun(proc);//no difference
		tbl.pop_symbol_table();
	}
}
void SyntaxTree::procedure_first::visit(SSTable& tbl, InsCtrl& ins,errorlist& erl,symbol*& fun)
{
	std::vector<TNode*>::iterator it=this->children.begin();
	//if(it==this->children.end()) return;
	std::string _id;
	symbol::arguments* args=new symbol::arguments();
	symbol* proc=NULL;
	(*it)->visit(tbl,ins,erl,_id);//identifier
	it++;
	if(!tbl.add_symbol(_id,args,proc))
	{
		erl.addError("Duplicated procedure: "+_id,this->getline());
	}
	else
	{
		if(proc==NULL) return;
		ins.fun(proc);
		tbl.new_symbol_table(proc);
		while(it!=this->children.end())
		{
			(*it)->visit(tbl,ins,erl,args);//param list;
			it++;
		}
		fun=proc;
	}
}
void SyntaxTree::form_param_list::visit(SSTable& tbl, InsCtrl& ins, errorlist& erl,symbol::arguments* args)
{
	std::vector<TNode*>::iterator it=this->children.begin();
//	if(it==this->children.end()) return;
	std::vector<std::string> id_list;
	bool is_ref=false;
	std::string vall;
	(*it)->visit(tbl,ins,erl,vall);		//first one whether var
	it++;
	if(vall=="var") is_ref=true;
	else
	{
		is_ref=false;
		id_list.push_back(vall);
	}
	while(it!=(this->children.end()-1))
	{
		std::string _id;
		(*it)->visit(tbl,ins,erl,_id);//identifier
		id_list.push_back(_id);
		it++;
	}
	token type;
	(*it)->visit(tbl,ins,erl,type);//basic type
	if(type.get_type()==token::INTEGER)
	{
		for(int i=0;i<id_list.size();i++)
		{
			if(!tbl.add_argument(symbol::INTEGER,id_list[i],args,is_ref))
			{
				erl.addError("Duplicated argument: "+id_list[i],this->getline());
			}
			else
			{
				ins.param(args->back());
			}
		}
	}
	else if(type.get_type()==token::CHAR)
	{
		for(int i=0;i<id_list.size();i++)
		{
			if(!tbl.add_argument(symbol::CHAR,id_list[i],args,is_ref))
			{
				erl.addError("Duplicated argument: "+id_list[i],this->getline());
			}
			else
			{
				ins.param(args->back());
			}
		}
	}
	else
		assert(0);
}
_SEMATIC_PROCEDURE_HEAD(function_definition)
{
	std::vector<TNode*>::iterator it=this->children.begin();
//	if(it==this->children.end()) return;
	symbol* func=NULL;
	(*it)->visit(tbl,ins,erl,func);//function first
	it++;
	(*it)->visit(tbl,ins,erl);//sub program
	if(func!=NULL)
	{
		ins.end_fun(func);
		//tbl.DumpSymBols();
		tbl.pop_symbol_table();
	}
}
void SyntaxTree::function_first::visit(SSTable& tbl, InsCtrl& ins, errorlist& erl,symbol*& fun)
{
	std::vector<TNode*>::iterator it=this->children.begin();
//	if(it==this->children.end()) return;
	std::string _id;
	symbol::arguments* args=new symbol::arguments();
	symbol* proc=NULL;
	(*it)->visit(tbl,ins,erl,_id);//identifier
	it++;
	token type;
	(*(this->children.end()-1))->visit(tbl,ins,erl,type);//type
	if(type.get_type()==token::INTEGER)//add the function sym into symbol table
	{
		if(!tbl.add_symbol(symbol::INTEGER,_id,args,proc))
		{
			erl.addError("Duplicated function: "+_id,this->getline());
		}
	}
	else if(type.get_type()==token::CHAR)
	{
		if(!tbl.add_symbol(symbol::CHAR,_id,args,proc))
		{
			erl.addError("Duplicated function: "+_id,this->getline());
		}
	}
	if(proc!=NULL&&(type.get_type()==token::CHAR||type.get_type()==token::INTEGER))
	{
		ins.fun(proc);
		tbl.new_symbol_table(proc);
		while(it!=(this->children.end()-1))
		{	
			(*it)->visit(tbl,ins,erl,args);//param list;
			it++;	
		}
		fun=proc;
	}
}
_SEMATIC_PROCEDURE_HEAD(assign_statement)
{
	std::vector<TNode*>::iterator it=this->children.begin();
//	if(it==this->children.end()) return;
	unsigned int num=this->children.size();
	symbol* first=NULL;
	symbol* index=NULL;
	symbol* last=NULL;
	symbol* _i_index=NULL;
	symbol* _last_index=NULL;
	(*it)->visit(tbl,ins,erl,first);//identifier
	if(first==NULL) return;//undeclared identifier
	it++;
	if(num==4)
	{
		(*it)->visit(tbl,ins,erl,index,_i_index);//expression
		if(index==NULL) return;
		it++;
	}

	token tok;
	(*it)->visit(tbl,ins,erl,tok);//assign token
	if(tok.get_type()!=token::ASSIGN_OPERATOR)
	{
		erl.addError("Assign operator expected",this->getline());
	}
	it++;
	(*it)->visit(tbl,ins,erl,last,_last_index);//expression
	if(last==NULL) return;

	symbol::symbol_type typ=first->get_sym_type();
	if(typ!=symbol::VARIABLE&&typ!=symbol::ARGUMENT&&typ!=symbol::ARRAY&&typ!=symbol::FUNCTION&&typ!=symbol::REFSYM)
	{
		erl.addError("Invalid left value type: "+first->get_id(),this->getline());
	}
	if(first->get_sym_type()==symbol::FUNCTION)//the internal function can assign value to this idenfifier, while other function and outer function can not
	{
		symbol* sym=tbl.get_current_scope();//current scope
		if(sym->get_id()!="@global")
		while(sym->get_id()!=first->get_id()&&sym->get_scope()->get_scope()->get_id()!="@global")//forward
		{
			sym=sym->get_scope()->get_scope();
		}
		if(sym->get_id()!=first->get_id())
			erl.addError("Invalid assignment to function identifier: "+first->get_id(),this->getline());
	}
	if(first->get_val_type()!=last->get_val_type())
	{
		erl.addWarning("Type mismatch",this->getline());
	}
	if(num==4&&first->get_sym_type()!=symbol::ARRAY)
	{
		erl.addError("Add index to non-array: "+first->get_id(),this->getline());
	}
// 	if(last->get_sym_type()!=symbol::VARIABLE&&last->get_sym_type()!=symbol::ARGUMENT&&last->get_sym_type()!=symbol::ARRAY
// 		&&last->get_sym_type()!=symbol::LITERAL&&last->get_sym_type()!=symbol::CONST)
// 	{
// 		erl.addError("Invalid right value type",this->getline());
// 	}
	if(first->get_sym_type()==symbol::ARRAY&&num==3)
	{
		erl.addError("Use array identifier without index: "+first->get_id(),this->getline());
	}
	if(last->get_sym_type()==symbol::ARRAY||last->get_sym_type()==symbol::REFSYM)
	{
		symbol* temp1;
		tbl.new_temp(temp1,last->get_val_type());
		if(last->get_sym_type()==symbol::ARRAY) ins.set_index(last,_last_index,true);
		ins.assign(temp1,last);
		last=temp1;
		last->set_marked(false);
	}
	if(first->get_sym_type()==symbol::ARRAY&&num==4)
	{
		//first=symbol::array_symbol(first);
		symbol* temp;
		tbl.new_temp(temp,index->get_val_type());
		if(index->get_sym_type()==symbol::ARRAY) ins.set_index(index,_i_index,true);
		ins.assign(temp,index);
		ins.set_index(first,temp,true);
		//first->set_array_index(temp);
		first->set_marked(true);
	}
	ins.assign(first,last);
}
_SEMATIC_PROCEDURE_HEAD(condition_statement)
{	
	std::vector<TNode*>::iterator it=this->children.begin();
//	if(it==this->children.end()) return;
	symbol* _else=NULL;
	symbol* _end=NULL;
	(*it)->visit(tbl,ins,erl,_else);//condition
	it++;
	(*it)->visit(tbl,ins,erl);//then section
	it++;
	if(it!=this->children.end())
	{
		tbl.new_label(_end);
		ins.jmp(_end);
		if(_else!=NULL)
		ins.add_label(_else);
		(*it)->visit(tbl,ins,erl);//else section
		it++;
		ins.add_label(_end);
	}
	else
		ins.add_label(_else);
}
void SyntaxTree::condition::visit(SSTable& tbl,InsCtrl& ins,errorlist& erl,symbol*& sym)
{
	std::vector<TNode*>::iterator it=this->children.begin();
	if(this->children.size()!=3) return;
	symbol* arg1=NULL;
	token opt;
	symbol* arg2=NULL;
	symbol* arg1_index=NULL;
	symbol* arg2_index=NULL;
	(*it)->visit(tbl,ins,erl,arg1,arg1_index);		//expression
	if(arg1->get_sym_type()==symbol::ARRAY||arg1->get_sym_type()==symbol::REFSYM||tbl.is_outer(arg1))
	{
		symbol* temp;tbl.new_temp(temp,arg1->get_val_type());
		if(arg1->get_sym_type()==symbol::ARRAY) ins.set_index(arg1,arg1_index,true);
		ins.assign(temp,arg1);arg1->set_marked(false);
		arg1=temp;
	}
	it++;
	(*it)->visit(tbl,ins,erl,opt);
	it++;
	(*it)->visit(tbl,ins,erl,arg2,arg2_index);		//expression
	if(arg2->get_sym_type()==symbol::ARRAY||arg2->get_sym_type()==symbol::REFSYM||tbl.is_outer(arg2))
	{
		symbol* temp;tbl.new_temp(temp,arg2->get_val_type());
		if(arg2->get_sym_type()==symbol::ARRAY) ins.set_index(arg2,arg2_index,true);
		ins.assign(temp,arg2);arg2->set_marked(false);
		arg2=temp;
	}
	if(sym==NULL)
		tbl.new_label(sym);
	if(arg1!=NULL&&arg2!=NULL)
		ins.branch(arg1,opt,arg2,sym);
}
_SEMATIC_PROCEDURE_HEAD(repeat_loop_statement)
{
	symbol* _label;
	std::vector<TNode*>::iterator it=this->children.begin();
//	if(it==this->children.end()) return;
	tbl.new_label(_label);
	ins.add_label(_label);
	(*it)->visit(tbl,ins,erl);//statement
	it++;
	(*it)->visit(tbl,ins,erl,_label);//condition
}

_SEMATIC_PROCEDURE_HEAD(read_statement)
{
	std::vector<TNode*>::iterator it=this->children.begin();
//	if(it==this->children.end()) return;
	while(it!=this->children.end())
	{
		symbol* sym=NULL;
		(*it)->visit(tbl,ins,erl,sym);		//identifier
		it++;
		if(sym!=NULL)
		   ins.read(sym);
	}
}
_SEMATIC_PROCEDURE_HEAD(write_statement)
{
	std::vector<TNode*>::iterator it=this->children.begin();
//	if(it==this->children.end()) return;
	symbol* _index=NULL;
	while(it!=this->children.end())
	{
		symbol* sym=NULL;
		if((*it)->get_node_type()==TNode::EXPRESSION)
		(*it)->visit(tbl,ins,erl,sym,_index);	//expression
		else (*it)->visit(tbl,ins,erl,sym);		//lit_string, etc..
		it++;
		if(sym->get_sym_type()==symbol::ARRAY&&_index!=NULL) ins.set_index(sym,_index,true);
		if(sym!=NULL)
		 ins.write(sym);
	}
	ins.shift_line();
}
_SEMATIC_PROCEDURE_HEAD(for_loop_statement)
{
	std::vector<TNode*> ::iterator it=this->children.begin();
//	if(it==this->children.end()) return;
	symbol* identif=NULL;
	symbol* _start=NULL;
	symbol* _end=NULL;
	symbol* _lab0=NULL;
	symbol* _lab1=NULL;
	symbol* _start_index=NULL;
	symbol* _end_index=NULL;
	token tk,_de_in;

	(*it)->visit(tbl,ins,erl,identif);//identifier
	if(identif==NULL) return;
	
	it++;
	(*it)->visit(tbl,ins,erl,tk);//assign opt
	if(tk.get_type()!=token::ASSIGN_OPERATOR)
	{
		erl.addError("assign opt expected",this->getline());
	}
	it++;
	(*it)->visit(tbl,ins,erl,_start,_start_index);//start expression
	it++;
	if(_start==NULL) return;
	if(_start->get_sym_type()==symbol::ARRAY&&_start_index!=NULL)
		ins.set_index(_start,_start_index,true);
	if(_start!=NULL&&identif!=NULL)///////////////////should this assign follow the rules
	{
		ins.assign(identif,_start);
	}
	tbl.new_label(_lab0);
	ins.add_label(_lab0);
	(*it)->visit(tbl,ins,erl,_de_in);//decrease or increase
	it++;
	(*it)->visit(tbl,ins,erl,_end,_end_index);//end expresstion
	it++;
	tbl.new_label(_lab1);
	if(_end==NULL) return;
	if(_de_in.get_type()==token::TO)
	{
		token auxtok0=token("<=",token::RELATION_OPERATOR);
		if(_end->get_sym_type()==symbol::ARRAY&&_end_index!=NULL) ins.set_index(_end,_end_index,true);
		if(_end->get_sym_type()==symbol::ARRAY||_end->get_sym_type()==symbol::REFSYM||tbl.is_outer(_end))
		{
			symbol* temp=NULL;
			tbl.new_temp(temp,_end->get_val_type());
			ins.assign(temp,_end);
			_end=temp;
		}
		if(_end!=NULL&&identif!=NULL)
			ins.branch(identif,auxtok0,_end,_lab1);
	}
	else if(_de_in.get_type()==token::DOWNTO)
	{
		token auxtok0=token(">=",token::RELATION_OPERATOR);
		if(_end->get_sym_type()==symbol::ARRAY&&_end_index!=NULL) ins.set_index(_end,_end_index,true);
		if(_end->get_sym_type()==symbol::ARRAY||_end->get_sym_type()==symbol::REFSYM||tbl.is_outer(_end))
		{
			symbol* temp=NULL;
			tbl.new_temp(temp,_end->get_val_type());
			ins.assign(temp,_end);
			_end=temp;
		}
		if(_end!=NULL&&identif!=NULL)
			ins.branch(identif,auxtok0,_end,_lab1);
	}
	else 
		return;
	(*it)->visit(tbl,ins,erl);//statement

	if(_de_in.get_type()==token::TO)
	{
		token auxtok1=token("+",token::ADD_OPERATOR);
		symbol* auxsym;
		symbol* ress=NULL;
		auxsym=symbol::lit_symbol("1",symbol::INTEGER);
		tbl.add_literal(auxsym);
		if(identif!=NULL)
		{
			ins.arithmetic_operation(identif,auxtok1,auxsym,ress);
		}
		if(ress==NULL) return;
		ins.assign(identif,ress);
	}
	else if(_de_in.get_type()==token::DOWNTO)
	{
		token auxtok1=token("-",token::ADD_OPERATOR);
		symbol* auxsym;
		auxsym=symbol::lit_symbol("1",symbol::INTEGER);
		tbl.add_literal(auxsym);
		symbol* ress=NULL;
		if(identif!=NULL)
		{
			ins.arithmetic_operation(identif,auxtok1,auxsym,ress);
		}
		if(ress==NULL) return;
		ins.assign(identif,ress);
	}
	else 
		return;
	ins.jmp(_lab0);
	ins.add_label(_lab1);
	ins.assign(identif,_end);
}
//might return a array symbol with an index
void SyntaxTree::expression::visit(SSTable& tbl, InsCtrl& ins, errorlist& erl,symbol*& result,symbol*& _index)
{
	std::vector<TNode*>::iterator it=this->children.begin();
//	if(it==this->children.end()) return;//expression have no child, error in parse stage
	token first_opt;
	symbol* arg1=NULL;
	symbol* arg2=NULL;
	symbol* index1=NULL;
	symbol* index2=NULL;
	if(this->children.size()%2==0)//has opt before
	{
		(*it)->visit(tbl,ins,erl,first_opt);
		it++;
	}
	(*it)->visit(tbl,ins,erl,arg1,index1);//term
	if(arg1==NULL) return;
	it++;
	bool set=false;
	if(first_opt.get_value()=="-")
	{
		if(arg1->get_sym_type()==symbol::ARRAY)
		{
			symbol* temp=NULL;
			tbl.new_temp(temp,arg1->get_val_type());
			assert(index1!=NULL);
			ins.set_index(arg1,index1,true);
			set=true;
			ins.assign(temp,arg1);
			arg1=temp;
		}
		ins.inverse(arg1,arg1);
	}
	result=arg1;

	while(it!=this->children.end())
	{
		(*it)->visit(tbl,ins,erl,first_opt);
		it++;
		(*it)->visit(tbl,ins,erl,arg2,index2); //term
		it++;
		symbol::symbol_type typ1,typ2;
		typ1=arg1->get_sym_type();typ2=arg2->get_sym_type();
		if(index1!=NULL&&!set) {if(arg1->get_sym_type()!=symbol::ARRAY&&!set) {ins.print_all_ins();std::cout<<*arg1; std::cout<<*index1;}
			assert(arg1->get_sym_type()==symbol::ARRAY||set); ins.set_index(arg1,index1,true);set=true;}
		if((typ1==symbol::REFSYM||typ1==symbol::ARRAY||tbl.is_outer(arg1))&&(typ2==symbol::ARRAY||typ2==symbol::REFSYM||tbl.is_outer(arg2)))
		{
			symbol* temp;
			tbl.new_temp(temp,arg1->get_val_type());
			ins.assign(temp,arg1);arg1->set_marked(false);
			arg1=temp;
		}
		if(index2!=NULL) {
		assert(arg2->get_sym_type()==symbol::ARRAY);ins.set_index(arg2,index2,true);}
		if(arg2!=NULL)
			ins.arithmetic_operation(arg1,first_opt,arg2,result); //this result will always be a temp variable
		arg1=result;
	}
	if(result->get_sym_type()==symbol::ARRAY)
	{
		assert(index1!=NULL); _index=index1;//ins.set_index(arg1,index1,true);
	}
}
//might return an array symbol with an index
void SyntaxTree::term::visit(SSTable& tbl, InsCtrl& ins, errorlist& erl,symbol*& result,symbol*& _index)
{	
	std::vector<TNode*>::iterator it=this->children.begin();
//	if(it==this->children.end()) return;
	symbol* arg1=NULL;
	symbol* arg2=NULL;
	symbol* index=NULL;
	symbol* index2=NULL;
	token opt;
	(*it)->visit(tbl,ins,erl,arg1,index2);//factor
	if(arg1==NULL) return;
	it++;
	result=arg1;
	bool set=false;
	while(it!=this->children.end())
	{
		(*it)->visit(tbl,ins,erl,opt);//operator node
		it++;
		(*it)->visit(tbl,ins,erl,arg2,index);//factor
		it++;
		symbol::symbol_type typ1,typ2;
		typ1=arg1->get_sym_type();typ2=arg2->get_sym_type();	//the order of the following clauses should not be chaged, inorder to preserve the right
		if(index2!=NULL&&!set) {assert(arg1->get_sym_type()==symbol::ARRAY||set);	//order of the quad codes sequences
		ins.set_index(arg1,index2,true);set=true;}	//the rules for set_index: it should be placed the nearest use of an array
		if((typ1==symbol::ARRAY||typ1==symbol::REFSYM)&&(typ2==symbol::ARRAY||typ2==symbol::REFSYM))
		{
			symbol* temp;
			tbl.new_temp(temp,arg1->get_val_type());
			ins.assign(temp,arg1);arg1->set_marked(false);
			arg1=temp;
		}
		if(index!=NULL){assert(arg2->get_sym_type()==symbol::ARRAY);ins.set_index(arg2,index,true);}
		if(arg2!=NULL)
			ins.arithmetic_operation(arg1,opt,arg2,result);
		arg1=result;
	}
	if(result->get_sym_type()==symbol::ARRAY) _index=index2; else _index=NULL;
}
void SyntaxTree::factor::visit(SSTable& tbl, InsCtrl& ins, errorlist& erl,symbol*& result,symbol*& _index)
{	
	std::vector<TNode*>::iterator it=this->children.begin();
//	if(it==this->children.end()) return;
	symbol* sym=NULL;
	symbol* index=NULL;
	symbol* _i_index=NULL;	//the index of the index of the array
	symbol* _r_index=NULL;	//returned array
	if(this->children.size()==2) //array followed by expression
	{
		(*it)->visit(tbl,ins,erl,sym);//identif, look table
		if(sym==NULL) return;
		it++;
		(*it)->visit(tbl,ins,erl,index,_i_index);//expression
		if(index==NULL) return;
		symbol* temp;
		tbl.new_temp(temp,index->get_val_type());
		if(index->get_sym_type()==symbol::ARRAY&&_i_index!=NULL) ins.set_index(index,_i_index,true);
		ins.assign(temp,index);
		if(index==NULL) return;
		if(sym->get_sym_type()!=symbol::ARRAY)
		{
			erl.addError("Add index to non-array: "+sym->get_id(),this->getline());
		}
		_index=temp;
	//	ins.set_index(sym,temp,true);
		sym->set_marked(true);
		result=sym;
	}
	else
	{
		if((*it)->get_node_type()==TNode::EXPRESSION)
			(*it)->visit(tbl,ins,erl,sym,_r_index);//expression,
		else
			(*it)->visit(tbl,ins,erl,sym);	//identifier,lit_unsigned_int,call function stmt
		if(sym==NULL) return;
		if(sym->get_sym_type()==symbol::ARRAY&&!sym->get_maked())
		{
			erl.addError("use array without index: "+sym->get_id(),this->getline());
		}
		if(sym->get_maked()) sym->set_marked(false);
		if(sym->get_sym_type()!=symbol::VARIABLE&&sym->get_sym_type()!=symbol::CONST&&sym->get_sym_type()!=symbol::LITERAL
			&&sym->get_sym_type()!=symbol::ARGUMENT&&sym->get_sym_type()!=symbol::REFSYM&&sym->get_sym_type()!=symbol::ARRAY)
		{
			erl.addError("invalid factor type found: "+sym->get_id(),this->getline());
		}
		_index=NULL;
		if(sym->get_sym_type()==symbol::ARRAY&&_r_index!=NULL) _index=_r_index;//ins.set_index(sym,_r_index);
		result=sym;
	}
//	result->set_marked(false);
}
_SEMATIC_PROCEDURE_HEAD(call_procedure_statement)//check whether is procedure
{
	
	std::vector<TNode*>::iterator it=this->children.begin();
//	if(it==this->children.end()) return;
	symbol* proc=NULL;
	symbol::arguments* args=new symbol::arguments();
	(*it)->visit(tbl,ins,erl,proc);//identifier
	if(proc==NULL) return;
	if(proc->get_sym_type()!=symbol::PROCEDURE)
	{
		erl.addError("Procedure expected: "+proc->get_id(),this->getline());
		return;
	}
	it++;

	args->push_back(proc);
	(*it)->visit(tbl,ins,erl,args);//real param list
	it++;
	ins.call(proc);
}
void SyntaxTree::call_function_statement::visit(SSTable& tbl, InsCtrl& ins, errorlist& erl,symbol*& sym)
{	
	std::vector<TNode*>::iterator it=this->children.begin();
//	if(it==this->children.end()) return;
//	std::cout<<*sym;
	symbol* func=NULL;
	symbol::arguments* args=new symbol::arguments();
	(*it)->visit(tbl,ins,erl,func);//identifier
	if(func==NULL) return;//undeclared identifier
	it++;
	if(func->get_sym_type()!=symbol::FUNCTION)
	{
		erl.addError("Function expected: "+func->get_id(),this->getline());
	}
	else
	{	args->push_back(func);
		(*it)->visit(tbl,ins,erl,args);//params
		it++;
	}
	//std::cout<<*sym;
	//ins.assign(,func);
	tbl.new_temp(sym,func->get_val_type());
	ins.call(func);
	symbol* zeroo=symbol::lit_symbol("0",func->get_val_type());
	tbl.add_literal(zeroo);
	ins.assign(sym,func);
	ins.assign(func,zeroo);
}
void SyntaxTree::real_param_list::visit(SSTable& tbl, InsCtrl& ins, errorlist& erl,symbol::arguments* params)
{	
	symbol* func=NULL;
	func=params->back();
	if(func==NULL) return;
	params->pop_back();
	symbol::arguments* args;
	bool is_ref=false;
	//std::vector<TNode*>::iterator it=this->children.begin();
	if(func->get_sym_type()!=symbol::FUNCTION&&func->get_sym_type()!=symbol::PROCEDURE)
	{
		erl.addError("Use non-function or procedure identifier as function|procedure: "+func->get_id(),this->getline());
	}
	args=func->get_params();
	symbol::arguments::reverse_iterator it=args->rbegin();
	for(std::vector<TNode*>::reverse_iterator it1=this->children.rbegin();it1!=this->children.rend();it1++)
	{
		symbol* arg=NULL;
		(*it1)->visit(tbl,ins,erl,arg);//real param
		if(arg==NULL) continue;
		params->insert(params->begin(),arg);
		if(it!=args->rend())
		{	
			symbol::symbol_value_type tp1=(*it)->get_val_type();
			symbol::symbol_value_type tp2=arg->get_val_type();
			symbol::symbol_type stp1=(*it)->get_sym_type();//sym type of form param
			symbol::symbol_type stp2=arg->get_sym_type(); //sym type of actual param
			if(stp1==symbol::REFSYM) is_ref=true;
			else is_ref=false;
			if(tp1!=tp2)
			{
				erl.addWarning("Type mismatch: "+arg->get_id(),this->getline());
			}//how to handle nested reference
			if(stp1==symbol::REFSYM&&(stp2!=symbol::VARIABLE&&stp2!=symbol::ARRAY&&stp2!=symbol::FUNCTION
				&&stp2!=symbol::REFSYM&&stp2!=symbol::ARGUMENT))//reference symbol can refer to a reference symbol
				erl.addError("Variable expected in reference parameter list: "+arg->get_id(),this->getline());
			if(stp1==symbol::REFSYM&&arg->get_id()[0]=='$')//get temp variable, should not have expression in reference parameter
				erl.addError("Variable expected in reference parameter list",this->getline());
			if(!is_ref)
				ins.push(arg);
			else
			{ins.set_ref(arg);(*it)->set_reference(arg);}
			it++;
		}	
	}
	if(args->size()!=params->size())
	{
		erl.addError("Argument amount mismatch: "+func->get_id(),this->getline());
	}
}
void SyntaxTree::real_param::visit(SSTable& tbl,InsCtrl& ins,errorlist& erl,symbol*& result)
{
	symbol* _index=NULL;
	std::vector<TNode*>::iterator it=this->children.begin();
//	if(it==this->children.end()) return;
	(*it)->visit(tbl,ins,erl,result,_index);//expression
	if(result->get_sym_type()==symbol::ARRAY&&_index!=NULL) ins.set_index(result,_index,true);
	it++;
}
void SyntaxTree::identifier::visit(SSTable& tbl,InsCtrl& ins,errorlist& erl,std::string& _id)
{
	_id=this->get_token().get_value();
}
void SyntaxTree::identifier::visit(SSTable& tbl,InsCtrl& ins,errorlist& erl,symbol*& sym)
{
	symbol* ss=NULL;
	ss=tbl.look_up(this->get_token().get_value());
	if(ss==NULL)
	{
		erl.addError("Undeclared identifier: "+this->get_token().get_value(),this->getline());
	}
	sym=ss;
//	std::cout<<*ss;
}
void SyntaxTree::lit_unsigned_int::visit(SSTable& tbl,InsCtrl& ins,errorlist& erl,symbol*& sym)
{	
	symbol* ss=NULL;
	assert(this->get_token().get_type()==token::UNSIGNED_INTEGER_CONST);
	ss=symbol::lit_symbol(this->get_token().get_value(),symbol::INTEGER);
	sym=ss;	
	tbl.add_literal(sym);
}
void SyntaxTree::lit_char::visit(SSTable& tbl,InsCtrl& ins,errorlist& erl,symbol*& sym)
{
	symbol* ss=NULL;
	assert(this->get_token().get_type()==token::CHAR_CONST);
	ss=symbol::lit_symbol(this->get_token().get_value(),symbol::CHAR);
	sym=ss;
	tbl.add_literal(sym);
}
void SyntaxTree::lit_string::visit(SSTable& tbl,InsCtrl& ins,errorlist& erl,symbol*& sym)
{
	symbol* ss=NULL;
	assert(this->get_token().get_type()==token::STRING_CONST);
	ss=symbol::lit_symbol(this->get_token().get_value(),symbol::STRING);
	sym=ss;
	tbl.add_literal(sym);
}
void SyntaxTree::pass_ref::visit(SSTable& tbl,InsCtrl& ins,errorlist& erl,std::string& res)
{
	res=this->get_token().get_value();
}