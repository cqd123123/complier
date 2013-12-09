#pragma warning(disable:4786)
#include "Parser.h"
#include "TNode.h"
#include "TLeaf.h"
#include "SyntaxTree.h"
namespace
{
#define _IS_FUNCTION(name, value) bool is_##name(token current)\
	{\
	return current.get_type() == token::value;\
	}


	_IS_FUNCTION(left_parenthese, LEFT_PARENTHESE);
	_IS_FUNCTION(right_parenthese, RIGHT_PARENTHESE);
	_IS_FUNCTION(left_bracket, LEFT_BRACKET);
	_IS_FUNCTION(right_bracket, RIGHT_BRACKET);
	_IS_FUNCTION(semicolon,SEMICOLON);
	_IS_FUNCTION(colon, COLON);
	_IS_FUNCTION(period, PERIOD);
	_IS_FUNCTION(comma, COMMA);
	_IS_FUNCTION(add_operator, ADD_OPERATOR);
	_IS_FUNCTION(multiply_operator, MULTIPLY_OPERATOR);
	_IS_FUNCTION(relation_operator, RELATION_OPERATOR);
	_IS_FUNCTION(assign_operator, ASSIGN_OPERATOR);
	_IS_FUNCTION(_array, ARRAY);//<---------------reserved
	_IS_FUNCTION(_begin,BEGIN);
	_IS_FUNCTION(_char,CHAR);
	_IS_FUNCTION(_const,CONST);
	_IS_FUNCTION(_do,DO);
	_IS_FUNCTION(_downto,DOWNTO);
	_IS_FUNCTION(_else,ELSE);
	_IS_FUNCTION(_end,END);
	_IS_FUNCTION(_for,FOR);
	_IS_FUNCTION(_function,FUNCTION);
	_IS_FUNCTION(_if,IF);
	_IS_FUNCTION(_integer,INTEGER);
	_IS_FUNCTION(_of,OF);
	_IS_FUNCTION(_procedure,PROCEDURE);
	_IS_FUNCTION(_read,READ);
	_IS_FUNCTION(_repeat,REPEAT);
	_IS_FUNCTION(_then,THEN);
	_IS_FUNCTION(_to,TO);
	_IS_FUNCTION(_until,UNTIL);
	_IS_FUNCTION(_var,VAR);
	_IS_FUNCTION(_write,WRITE);//<---------------reserved
	_IS_FUNCTION(identifier,IDENTIFIER);
	_IS_FUNCTION(signed_integer_const, SIGNED_INTEGER_CONST);
	_IS_FUNCTION(lit_unsigned_int, UNSIGNED_INTEGER_CONST);
	_IS_FUNCTION(lit_char,CHAR_CONST);
	_IS_FUNCTION(lit_string,STRING_CONST);
	_IS_FUNCTION(char_const,CHAR_CONST);
	_IS_FUNCTION(string_const,STRING_CONST);

	bool is_literal(token& current)
	{
		return is_signed_integer_const(current)||is_lit_unsigned_int(current)||is_char_const(current);
	}
	bool is_type(token& current)
	{
		return is__char(current)||is__integer(current);
	}
	bool is_var_declaration(token& current,token& before_token)
	{
		return is__var(current);
	}
	bool is_procedure_declaration(token& current)
	{
		return is__procedure(current);
	}
	bool is_function_declaration(token& current)
	{
		return is__function(current);
	}
	bool is_condition_statement(token& current)
	{
		return is__if(current);
	}
	bool is_loop_statement(token& current)
	{
		return is__repeat(current)||is__for(current);
	}
	bool is_basic_type(token& current)
	{
		return is__char(current)||is__integer(current);
	}
	bool is_increment(token& current)
	{
		return is__to(current);
	}
	bool is_decrement(token& current)
	{
		return is__downto(current);
	}
	bool is_expression(token& current)
	{
		return is_add_operator(current)||is_identifier(current)||is_lit_unsigned_int(current)||is_left_parenthese(current);
	}
	bool is_pass_ref(token& current)
	{
		return is__var(current);
	}
#undef _IS_FUNCTION
}
Parser::Parser(tokenizer _tk, errorlist& _el):tk(_tk), erl(_el)
{
	astree=NULL;
	//before_token=token("",token::NULL_TOKEN);//init, before_token
}

Parser::~Parser()
{
;
}

#define _prodedure_head(name) void Parser::name(TNode* parent)\
{\


#define _useless_TNode(name) _prodedure_head(name)\
_TNode_body(name)

#define _TNode_body(name)  if(is_##name(current))\
{\
	current=tk.next_token();\
}\
	else\
{\
    erl.addError(#name " expected!",current.get_line());\
	current=tk.next_token();\
}\
}

#define _CHILD_INSERTION(name) TNode* child = new SyntaxTree::name(current.get_line());\
parent->add(child);

#define _LEAF_CHILD_INSERTION(name) TNode* child = new SyntaxTree::name(current);\
parent->add(child);

#define _use_LEAF_TNode(name) _prodedure_head(name)\
	_LEAF_CHILD_INSERTION(name) _TNode_body(name)

#define _Procedure(name) _prodedure_head(name)\
_CHILD_INSERTION(name)

TNode* Parser::parse()
{
	if(astree==NULL)
	{
		current=tk.next_token();
		program();
		return astree;
	}
	else
		return astree;
	
}
void Parser::program()
{
	TNode* parent=new SyntaxTree::program(current.get_line());
	sub_program(parent);
	period(parent);
	astree=parent;
}
_Procedure(sub_program)
	if(is__const(current))
		const_declaration(child);
	if(is__var(current))
		var_declaration(child);
	while(is_function_declaration(current)||is_procedure_declaration(current))
	{
		if(is_function_declaration(current))
			function_declaration(child);
		else
			procedure_declaration(child);
	}
	compound_statement(child);
}
_Procedure(var_declaration)
	_var(child);
	var_definition(child);
	semicolon(child);
	while(current.get_type()==token::IDENTIFIER)
	{
		var_definition(child);
		semicolon(child);
	}
}
_Procedure(var_definition)
	identifier(child);
	while(current.get_type()==token::COMMA)
	{
		comma(child);
		identifier(child);
	}
	colon(child);
	type(child);
}
_Procedure(type)//if type get a unsigned integer and a basic type, then the identifier before is array
	if(is__array(current))
	{
		_array(child);
		left_bracket(child);
		lit_unsigned_int(child);
		right_bracket(child);
		_of(child);
		basic_type(child);
	}
	else
	{
		basic_type(child);
	}
}
_Procedure(const_declaration)
	_const(child);
	if(is_identifier(current))
		const_definition(child);
	else
	{
		erl.addError("const definition expected",current.get_line());
		if(current.get_type()==token::SEMICOLON) semicolon(child);
		return;
	}
	while(current.get_type()==token::COMMA)
	{
		comma(child);
		const_definition(child);
	}
	semicolon(child);
}
_Procedure(const_definition)
	identifier(child);
	relation_operator(child);
	if(is_add_operator(current)||is_lit_unsigned_int(current)||is_lit_char(current))
		const_value(child);
	else
		erl.addError("const value expected",current.get_line());
}
_use_LEAF_TNode(identifier)
_use_LEAF_TNode(relation_operator)

_Procedure(const_value)
	if(is_add_operator(current))
	{
		add_operator(child);
		lit_unsigned_int(child);
	}
	else if (is_lit_unsigned_int(current))
	{
		lit_unsigned_int(child);
	}
	else if(is_lit_char(current))
			lit_char(child);
	else
	{
		erl.addError("Const unsigned int or char const expected",current.get_line());
		current=tk.next_token();
	}

}
_Procedure(function_declaration)
	function_definition(child);
	while(tk.look_ahead(1).get_type()==token::FUNCTION)
	{
		semicolon(child);
		function_definition(child);
	}
	semicolon(child);
}
_Procedure(procedure_declaration)
	procedure_definition(child);
	while(tk.look_ahead(1).get_type()==token::PROCEDURE)
	{
		semicolon(child);
		procedure_definition(child);
	}
	semicolon(child);
}
_Procedure(function_definition)
	function_first(child);
	sub_program(child);
}
_Procedure(procedure_definition)
	procedure_first(child);
	sub_program(child);
}

_Procedure(function_first)
	_function(child);
	identifier(child);
	left_parenthese(child);
	if(current.get_type()==token::VAR||current.get_type()==token::IDENTIFIER)
	{
		form_param_list(child);
		while(current.get_type()==token::SEMICOLON)
		{
			semicolon(child);
			form_param_list(child);
		}
	}
	right_parenthese(child);
	colon(child);
	basic_type(child);
	semicolon(child);
}
_Procedure(procedure_first)
	_procedure(child);
	identifier(child);
	left_parenthese(child);
	if(current.get_type()==token::VAR||current.get_type()==token::IDENTIFIER)
	{
		form_param_list(child);
		while(current.get_type()==token::SEMICOLON)
		{
			semicolon(child);
			form_param_list(child);
		}
	}
	right_parenthese(child);
	semicolon(child);
}
_Procedure(form_param_list)
	if(current.get_type()==token::VAR)
		pass_ref(child);
	identifier(child);
	while(current.get_type()==token::COMMA)
	{	
		comma(child);
		identifier(child);
	}
	colon(child);
	basic_type(child);
// 	while(current.get_type()==token::SEMICOLON)
// 	{
// 		semicolon(child);
// 		form_param_list(child);
// 	}
}
_Procedure(compound_statement)
	_begin(child);
	statement(child);

	//if(current.get_type()==token::SEMICOLON)
	while(current.get_type()!=token::END&&current.get_type()!=token::NULL_TOKEN)
	{	//std::cout<<current;
		semicolon(child);
		statement(child);
	}
	_end(child);
}
_Procedure(statement)
	if(current.get_type()==token::IDENTIFIER&&(tk.look_ahead(1).get_type()==token::ASSIGN_OPERATOR||tk.look_ahead(1).get_type()==token::LEFT_BRACKET))
		assign_statement(child);
	else if(is__if(current))
		condition_statement(child);
	else if(is__repeat(current))
		repeat_loop_statement(child);
	else if(is__for(current))
		for_loop_statement(child);
	else if(current.get_type()==token::IDENTIFIER&&tk.look_ahead(1).get_type()==token::LEFT_PARENTHESE)
		call_procedure_statement(child);
	else if(current.get_type()==token::BEGIN)
		compound_statement(child);
	else if(current.get_type()==token::READ)
		read_statement(child);
	else if(current.get_type()==token::WRITE)
		write_statement(child);
	if(current.get_type()==token::IDENTIFIER&&tk.look_ahead(1).get_value()=="=")
	{
		erl.addError("Use '=' as assign operator",current.get_line());
	}
	if(current.get_type()==token::IDENTIFIER)
	{
		erl.addError("call procedure without parameter or assign without :=",current.get_line());
	}
}
_Procedure(assign_statement)
	identifier(child);//function or variable
	if(current.get_type()==token::LEFT_BRACKET)
	{
		left_bracket(child);
		expression(child);
		right_bracket(child);
	}
	assign_operator(child);
	expression(child);

}
_Procedure(expression)
	if(!is_expression(current))
	{
		erl.addError("expression expected: "+current.get_value(),current.get_line());
		return;
	}
	if(current.get_type()==token::ADD_OPERATOR)
	{
		add_operator(child);
	}
	term(child);
	while(current.get_type()==token::ADD_OPERATOR)
	{
		add_operator(child);
		term(child);
	}
}
_Procedure(term)
	factor(child);
	while(current.get_type()==token::MULTIPLY_OPERATOR)
	{
		multiply_operator(child);
		factor(child);
	}
}
_Procedure(factor)
	if(current.get_type()==token::UNSIGNED_INTEGER_CONST)
		lit_unsigned_int(child);
	else if(current.get_type()==token::LEFT_PARENTHESE)
	{
		left_parenthese(child);
		expression(child);
		right_parenthese(child);
	}
	else if(current.get_type()==token::IDENTIFIER&&tk.look_ahead(1).get_type()==token::LEFT_BRACKET)
	{
		identifier(child);
		left_bracket(child);
		expression(child);
		right_bracket(child);
	}
	else if(current.get_type()==token::IDENTIFIER&&tk.look_ahead(1).get_type()==token::LEFT_PARENTHESE)
	{
		call_function_statement(child);
	}
	else
		identifier(child);
}
_Procedure(call_function_statement)
	identifier(child);
	left_parenthese(child);
	real_param_list(child);
	right_parenthese(child);
}
_Procedure(real_param_list)
	if(current.get_type()==token::RIGHT_PARENTHESE)
		return;
	real_param(child);
	while(current.get_type()==token::COMMA)
	{
		comma(child);
		real_param(child);
	}
}
_Procedure(real_param)
	expression(child);
}
_Procedure(condition_statement)
	_if(child);
	condition(child);
	_then(child);
	statement(child);
	if(current.get_type()==token::ELSE)
	{
		_else(child);
		statement(child);
	}

}
_Procedure(condition)
	expression(child);
	relation_operator(child);
	expression(child);
}
_Procedure(repeat_loop_statement)
	_repeat(child);
	statement(child);
	_until(child);
	condition(child);
}
_Procedure(for_loop_statement)
	_for(child);
	identifier(child);
	assign_operator(child);
	expression(child);
	if(current.get_type()==token::TO)
	{
		increment(child);
	}
	else
		decrement(child);

	expression(child);
	_do(child);
	statement(child);
}
_Procedure(call_procedure_statement)

	identifier(child);
	left_parenthese(child);
	real_param_list(child);
	right_parenthese(child);
}
_Procedure(read_statement)
	_read(child);
	left_parenthese(child);
	identifier(child);
	while(current.get_type()==token::COMMA)
	{
		comma(child);
		identifier(child);
	}
	right_parenthese(child);
}
_Procedure(write_statement)
	_write(child);
	left_parenthese(child);
	if(current.get_type()==token::STRING_CONST&&tk.look_ahead(1).get_type()==token::COMMA)
	{
		lit_string(child);
		comma(child);
		expression(child);
	}
	else if(current.get_type()==token::STRING_CONST)
	{
		lit_string(child);
	}
	else
		expression(child);

	right_parenthese(child);
}
_use_LEAF_TNode(lit_unsigned_int)
_use_LEAF_TNode(lit_char)
_use_LEAF_TNode(lit_string)
_use_LEAF_TNode(add_operator)
_use_LEAF_TNode(basic_type)
_use_LEAF_TNode(assign_operator)
_use_LEAF_TNode(multiply_operator)
_use_LEAF_TNode(increment)
_use_LEAF_TNode(decrement)
_use_LEAF_TNode(pass_ref)
_useless_TNode(comma)
_useless_TNode(semicolon)
_useless_TNode(_const)
_useless_TNode(_var)
_useless_TNode(colon)
_useless_TNode(_array)
_useless_TNode(left_bracket)
_useless_TNode(right_bracket)
_useless_TNode(_of)
_useless_TNode(_function)
_useless_TNode(_procedure)
_useless_TNode(left_parenthese)
_useless_TNode(right_parenthese)
_useless_TNode(_begin)
_useless_TNode(_end)
_useless_TNode(_if)
_useless_TNode(_then)
_useless_TNode(_else)
_useless_TNode(_repeat)
_useless_TNode(_until)
_useless_TNode(_for)
_useless_TNode(_do)
_useless_TNode(_read)
_useless_TNode(_write)
_useless_TNode(period)