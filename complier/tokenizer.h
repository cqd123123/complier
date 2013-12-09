#ifndef _COMPILER_TOKENIZER
#define _COMPILER_TOKENIZER
#pragma warning(disable:4786)
#include "token.h"
#include <iostream>
#include <deque>
#include "codeBuffer.h"
#include <stdio.h>
class tokenizer  
{
public:
	tokenizer(FILE*& in);
	token look_ahead(unsigned int);
	token next_token();

private:
	const static std::string RESERVE[21];

    token _get_next();
	std::deque<token> ahead_tokens;
	token identifier(char);
	token number(char);
	token char_const(char);
	token string_const(char);
	
    token multiply_operator(char);
    token add_operator(char);
    token assign_operator_or_colon(char);
    token colon(char);
	
	token relation_operator(char);
	token left_parenthese(char);
	token right_parenthese(char);
	token left_bracket(char);//[
	token right_bracket(char);//]
	token semicolon(char);
	token comma(char);
	token period(char);
	int line_num;
	int is_reserve(std::string name);
	bool has_more_token();
	codeBuffer* input;
};

#endif
