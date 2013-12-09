#pragma warning(disable:4786)
#include "utility.h"
#include <STRING>
#include <SSTREAM>
#include "symbol.h"
#include <CASSERT>

namespace utility{

	std::string to_string(std::string value)
	{
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		for(unsigned int i = 0; i < value.size(); i++)
		{
			if(value[i] == '%')
				ss << "%%";
			else
				ss << value[i];
		}
		ss << ", 0";
		return ss.str();
	}
	std::string to_string(int val)
	{
		std::stringstream ss(std::stringstream::in|std::stringstream::out);
		ss<<val;
		return ss.str();
	}
	std::string to_value_string(symbol* sym)
	{
		switch(sym->get_val_type())
		{
		case symbol::CHAR:
		case symbol::INTEGER:
			return sym->get_value();
		case symbol::STRING:
			return to_string(sym->get_value());
		default:
			return "";
		}
	}
	std::string get_func_name(symbol* sym)
	{
		assert(sym->get_sym_type()==symbol::PROCEDURE||sym->get_sym_type()==symbol::FUNCTION);
		std::stringstream ss(std::stringstream::in|std::stringstream::out);
		ss<<sym->get_id();
		ss<<"@";
		ss<<sym->get_index();
		return ss.str();
	}
	bool is_arthimethic(Quadcode* code)
	{
		Quadcode::Quad_type typ=code->get_quad_type();
		return typ==Quadcode::ADD||typ==Quadcode::SUB||typ==Quadcode::MUL||typ==Quadcode::DIV;
	}
	namespace tokenizer{
		bool is_letter(char ch)
		{ return isalpha(ch);}

		bool is_non_zero(char ch)
		{return (ch >= '1'&&ch <= '9');}

		bool is_digit(char ch)
		{return is_non_zero(ch)||ch=='0';}

		bool is_white_space(char ch)
		{
			if(isspace(ch)) //will return NULL if not
				return true;
			else
				return false;
		}

		bool is_add_operator(char ch)
		{return (ch=='+'||ch=='-');}

		bool is_multiply_operator(char ch)
		{return (ch=='*'||ch=='/');}

		bool is_char_const(char ch)
		{return (isalpha(ch)||is_digit(ch));}

		bool is_in_string(char ch)
		{ return (ch==32||ch==33||(ch >=35 && ch<=126));}

		bool is_separator(char ch)
		{return (is_white_space(ch) ||
				 is_add_operator(ch)||
   				 is_multiply_operator(ch)||
				 ch == '(' ||
				 ch == ')' ||
				 ch == '{' ||
				 ch == '}' ||
				 ch == ',' ||
				 ch == ';' ||
				 ch == '>' ||
		 		 ch == '<' ||
				 ch == '!' ||
				 ch == '=' ||
				 ch == '"' ||
				 ch == '\'');
		}
	}
}
