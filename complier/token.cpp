#pragma warning(disable:4786)
#include "token.h"
#include <STRING>

namespace
{
	static std::string TOKEN_TYPE_STRING_MAP[]=
	{
		"ARRAY",
		"BEGIN",
		"CHAR",
		"CONST",
		"DO",
		"DOWNTO",
		"ELSE",
		"END",
		"FOR",
		"FUNCTION","IF","INTEGER","OF","PROCEDURE","READ","REPEAT","THEN","TO","UNTIL","VAR","WRITE",	
		"IDENTIFIER","SIGNED_INTEGER_CONST", "UNSIGNED_INTEGER_CONST",
		"CHAR_CONST",
		"STRING_CONST",
		"ADD_OPERATOR",
		"MULTIPLY_OPERATOR",
		"RELATION_OPERATOR",
		"ASSIGN_OPERATOR",
		"LEFT_PARENTHESE",
		"RIGHT_PARENTHESE",
		"LEFT_BRACKET", //[
		"RIGHT_BRACKET", //]
		"SEMICOLON",
		"COLON",
		"COMMA","PERIOD",
		"NULL_TOKEN",
		"INVALID_CHAR",
		"INVALID_INT",
	    "INVALID_STRING",
		"ZERO_PREFIX_NUMBER",
		"UNKNOWN_TOKEN"
	};
}
token::token(std::string value,token_type type,unsigned int line)
{
	token::value=value;
	token::type=type;
	token::line=line;
	if(type!=CHAR_CONST&&type!=STRING_CONST)
	{
		for(int i=0;i<value.size();i++)
			value[i]=tolower(value[i]);
	}
}
token::token(std::string _value, token::token_type _type):value(_value), type(_type)
{
	value = _value;
	type = _type;
	line = 0;
}
token::token()
{
	type=token::NULL_TOKEN;
}

void operator<<(std::ostream& out, const token& _token)
{
	out << "[" << TOKEN_TYPE_STRING_MAP[_token.type] << ":" << _token.value << "]@" << _token.line << std::endl;
}