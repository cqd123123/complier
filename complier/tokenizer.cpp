#pragma warning(disable:4786)
#include "tokenizer.h"
#include "utility.h"
#include <cctype>


const std::string tokenizer::RESERVE[] = 
{
		"array",
		"begin",
		"char",
		"const",
		"do",
		"downto",
		"else",
		"end",
		"for",
		"function","if","integer","of","procedure","read","repeat","then","to","until","var","write"
};

bool tokenizer::has_more_token()
{
	char ptr;
	while(input->getnext(ptr) && utility::tokenizer::is_white_space(ptr))
		if(ptr == '\n')line_num++;
	if(input->eof())
	{
		return false;
	}
	else
	{
		input->unget();
		return true;
	}
}

tokenizer::tokenizer(FILE*& in)
{
	input=new codeBuffer(in);
	line_num = 1;
}
token tokenizer::next_token()
{
	if(!ahead_tokens.empty())
	{
		token next = ahead_tokens.front();
		ahead_tokens.pop_front();
		return next;
	}
	else if(!has_more_token())
	{
		return token("", token::NULL_TOKEN);
	}	
	else 
	{
		return _get_next();
	}
}
token tokenizer::_get_next()
{
	char val;
	while(input->getnext(val) && utility::tokenizer::is_white_space(val))//get over all white spaces
	{if(val == '\n') line_num++;}

	if(utility::tokenizer::is_letter(val))
	{
		return identifier(val);
	}
	if(utility::tokenizer::is_digit(val))
	{
		return number(val);
	}
	else
	{
		switch(val)
		{
		case '+':
		case '-':
			return add_operator(val);
		case '*':
		case '/':
			return multiply_operator(val);
		case ':':
			return assign_operator_or_colon(val);
		case '=':
		case '<':
		case '>':
			return relation_operator(val);
		case '(':
			return left_parenthese(val);
		case ')':
			return right_parenthese(val);
		case '[':
			return left_bracket(val);
		case ']':
			return right_bracket(val);
		case ';':
			return semicolon(val);
		case ',':
			return comma(val);
		case '\'':
			return char_const(val);
		case '"':
			return string_const(val);
		case '.':
			return period(val);
		default:
			return token(""+val,token::UNKNOWN_TOKEN,line_num);
		}
	}
}
token tokenizer::identifier(char ch)
{
	std::string val;
	do
	{
		val+=ch;
		input->getnext(ch);
	}while(utility::tokenizer::is_letter(ch)||utility::tokenizer::is_digit(ch));
	input->unget();
	if(is_reserve(val)!=-1)
	{return token(val,(token::token_type)is_reserve(val),line_num);}

	return token(val,token::IDENTIFIER,line_num);
}
token tokenizer::number(char ch)
{
	std::string val;
	do 
	{
		val+=ch;
		input->getnext(ch);
	} while (utility::tokenizer::is_digit(ch));
	input->unget();
	return token(val,token::UNSIGNED_INTEGER_CONST,line_num);
}
token tokenizer::char_const(char ch)
{
	std::string val;
	val+=ch;
	input->getnext(ch);
	val+=ch;
	if(!utility::tokenizer::is_char_const(ch))
	{
		input->getnext(ch);
		val+=ch;
		if(!(ch=='\'')) input->unget();
		return token(val,token::INVALID_CHAR,line_num);
	}
	input->getnext(ch);
	if(!(ch=='\''))
	{
		input->unget();
		return token(val,token::INVALID_CHAR,line_num);
	}
	val+=ch;
	return token(val,token::CHAR_CONST,line_num);
}
token tokenizer::string_const(char ch)
{
	std::string val;
	val += ch;
	while(input->getnext(ch) && ch != '"')
	{
		if(!utility::tokenizer::is_in_string(ch)){input->unget();return token(val, token::INVALID_STRING, line_num);}
		val += ch;
	}
	val += ch;
	return token(val, token::STRING_CONST, line_num);
}
token tokenizer::add_operator(char ch)//-,+
{
	std::string val;
	val+=ch;
	return token(val,token::ADD_OPERATOR,line_num);
}
token tokenizer::multiply_operator(char ch)//*,/
{
	std::string val;
	val+=ch;
	return token(val,token::MULTIPLY_OPERATOR,line_num);
}
token tokenizer::assign_operator_or_colon(char ch)//:=
{
	std::string val;
	val+=ch;
	input->getnext(ch);
	if(ch=='=')
	{
		val+=ch;
		return token(val,token::ASSIGN_OPERATOR,line_num);
	}
	else
	{
		input->unget();//:
		return token(val,token::COLON,line_num);
	}
}
token tokenizer::colon(char ch)
{
	std::string val;
	val+=ch;
	return token(val,token::COLON,line_num);
}

token tokenizer::relation_operator(char ch)//<,<=,>,>=,=,<>
{
	std::string val;
	val+=ch;
	input->getnext(ch);
	if(ch=='='||ch=='>')
		val+=ch;
	else
		input->unget();
	return token(val,token::RELATION_OPERATOR,line_num);
}
token tokenizer::right_bracket(char ch)  //[
{
	std::string val;
	val+=ch;
	return token(val,token::RIGHT_BRACKET,line_num);
}
token tokenizer::left_bracket(char ch)
{
	std::string val;
	val+=ch;
	return token(val,token::LEFT_BRACKET,line_num);
}
token tokenizer::right_parenthese(char ch)
{
	std::string val;
	val+=ch;
	return token(val,token::RIGHT_PARENTHESE,line_num);
}
token tokenizer::left_parenthese(char ch)
{
	std::string val;
	val+=ch;
	return token(val,token::LEFT_PARENTHESE,line_num);
}
token tokenizer::comma(char ch)
{
	std::string val;
	val+=ch;
	return token(val,token::COMMA,line_num);
}
token tokenizer::semicolon(char ch)
{
	std::string val;
	val+=ch;
	return token(val,token::SEMICOLON,line_num);
}
token tokenizer::period(char ch)
{
	std::string val;
	val+=ch;
	return token(val,token::PERIOD,line_num);
}
int tokenizer::is_reserve(std::string name)
{
	int l = 0;
	int r = 21 - 1;
	int mid;
	while(l <= r)
	{
		mid = (l + r) / 2;
		if(tokenizer::RESERVE[mid] == name) return mid;
		else if(tokenizer::RESERVE[mid] < name) l = mid + 1;
		else r = mid - 1;
	}
	return -1;
}
token tokenizer::look_ahead(unsigned int index)
{
	std::deque<token>::size_type i = ahead_tokens.size();
	while(i < index)
	{
		ahead_tokens.push_back(_get_next());
		i++;
	}
	return ahead_tokens[index - 1];
}