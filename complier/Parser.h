#ifndef _COMPILER_PARSER
#define _COMPILER_PARSER
#pragma warning(disable:4786)
#include "TNode.h"
#include <iostream>
#include "tokenizer.h"
#include "errorlist.h"
#include "token.h"

class Parser  
{
public:
	Parser(tokenizer,errorlist&);
	virtual ~Parser();
	TNode* parse();

private:
	tokenizer tk;
	errorlist &erl;
	token current;
	token before_token;
	TNode *astree;

	void program();
	void sub_program(TNode*);
	void const_declaration(TNode*);
	void var_declaration(TNode*);
	void procedure_declaration(TNode*);
	void function_declaration(TNode*);
	void function_definition(TNode*);
	void procedure_definition(TNode*);
	void compound_statement(TNode*);
	void const_definition(TNode*);
	void const_value(TNode*);
	void var_definition(TNode*);
	void type(TNode*);
	void procedure_first(TNode*);
	void form_param_list(TNode*);
	void function_first(TNode*);
	void statement(TNode*);
	void assign_statement(TNode*);
	void condition_statement(TNode*);
	void condition(TNode*);
	void repeat_loop_statement(TNode*);
	void call_procedure_statement(TNode*);
	void call_function_statement(TNode*);
	void read_statement(TNode*);
	void write_statement(TNode*);
	void for_loop_statement(TNode*);
	void increment(TNode*);
	void decrement(TNode*);
	void pass_ref(TNode*);

	void expression(TNode*);
	void function_identifier(TNode*);
	void term(TNode*);
	void factor(TNode*);
	void real_param_list(TNode*);
    void real_param(TNode*);////non-leaf-node


	void identifier(TNode*);//leaf
	//void litral(TNode*);//LIT_UNSIGNED_INT,LIT_STRING,LIT_CHAR
	void lit_unsigned_int(TNode*);
	void lit_string(TNode*);
	void lit_char(TNode*);
	void basic_type(TNode*);//leave node, int or char
	void add_operator(TNode*);
	void multiply_operator(TNode*);
	void relation_operator(TNode*);
	void assign_operator(TNode*);
	void left_parenthese(TNode*); //(
	void right_parenthese(TNode*);//)
	void left_bracket(TNode*); //[
	void right_bracket(TNode*); //]
	void semicolon(TNode*);
	void colon(TNode*);
	void comma(TNode*);
	void period(TNode*);

	void _array(TNode*);//reserved word
	void _begin(TNode*);//prevent confusion with the reserve word in c++
	void _char(TNode*);
	void _const(TNode*);
	void _do(TNode*);
	void _downto(TNode*);
	void _else(TNode*);
	void _end(TNode*);
	void _for(TNode*);
	void _function(TNode*);
	void _if(TNode*);
	void _integer(TNode*);
	void _of(TNode*);
	void _procedure(TNode*);
	void _read(TNode*);
	void _repeat(TNode*);
	void _then(TNode*);
	void _to(TNode*);
	void _until(TNode*);
	void _var(TNode*);
	void _write(TNode*);

};

#endif
