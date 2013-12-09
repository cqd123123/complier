#ifndef _COMPILER_TOKEN
#define _COMPILER_TOKEN
#pragma warning(disable:4786)
#include <STRING>
struct token  
{
public:
	token();
	const static enum token_type{
		ARRAY,
		BEGIN,
		CHAR,
	    CONST,
		DO,
		DOWNTO,
		ELSE,
		END,
		FOR,
		FUNCTION,IF,INTEGER,OF,PROCEDURE,READ,REPEAT,THEN,TO,UNTIL,VAR,WRITE,
		IDENTIFIER, SIGNED_INTEGER_CONST, UNSIGNED_INTEGER_CONST,
		CHAR_CONST,
		STRING_CONST,
		ADD_OPERATOR,
		MULTIPLY_OPERATOR,
		RELATION_OPERATOR,
		ASSIGN_OPERATOR,
		LEFT_PARENTHESE,
		RIGHT_PARENTHESE,
		LEFT_BRACKET, //[
		RIGHT_BRACKET, //]
		SEMICOLON,
		COLON,
		COMMA, PERIOD,NULL_TOKEN,
		//////
		INVALID_CHAR,
		INVALID_INT,
		INVALID_STRING,
		ZERO_PREFIX_NUMBER,
		UNKNOWN_TOKEN,
	};
	token(std::string value,token_type type,unsigned int line);
	token(std::string, token_type);

	std::string get_value(){return value;};
	token_type get_type(){return type;};
    unsigned int get_line(){return line;}; 
	friend void operator<<(std::ostream&, const token&);
private:
	std::string value;
	unsigned int line;
	token_type type;
};
#endif
