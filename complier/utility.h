#ifndef _COMPILER_UTILITY
#define _COMPILER_UTILITY
#pragma warning(disable:4786)
#include <STRING>
#include <SSTREAM>
#include "symbol.h"
#include "Quadcode.h"
namespace utility  
{
	std::string to_string(std::string value);
	std::string to_value_string(symbol* sym);
	std::string to_string(int);
	std::string get_func_name(symbol*);
	bool is_arthimethic(Quadcode*);
	namespace tokenizer{
		bool is_letter(char ch);
		bool is_non_zero(char ch);
		bool is_digit(char ch);
		bool is_white_space(char ch);
		bool is_add_operator(char ch);
		bool is_multiply_operator(char ch);
		bool is_char_const(char ch);
		bool is_separator(char ch);
		bool is_in_string(char ch);
	};
};
#endif
