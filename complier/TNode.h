#ifndef _COMPILER_TNODE
#define _COMPILER_TNODE
#pragma warning(disable:4786)
#include "token.h"
#include <VECTOR>
#include <IOSTREAM>
#include "symbol.h"
//#include "Quadcode.h"
#include "errorlist.h"
#include "SSTable.h"
#include "InsCtrl.h"

class TNode  
{
public:
	const static enum node_type
	{
		PROGRAM,
		SUB_PROGRAM,
		CONST_DECLARATION,
		VAR_DECLARATION,
		PROCEDURE_DECLARATION,
		FUNCTION_DECLARATION,
		PROCEDURE_DEFINITION,
		FUNCTION_DEFINITION,
		COMPOUND_STATEMENT,
		CONST_DEFINITION,
		CONST_VALUE,
		VAR_DEFINITION,
		TYPE,
		PROCEDURE_FIRST,
		FORM_PARAM_LIST,
		FUNCTION_FIRST,
		STATEMENT,
		ASSIGNMENT_STATEMENT,
		CONDITION_STATEMENT,
		CONDITION,
		REPEAT_LOOP_STATEMENT,
		CALL_PROCEDURE_STATEMENT,
		CALL_FUNCTION_STATEMENT,
		READ_STATEMENT,
		WRITE_STATEMENT,
		FOR_LOOP_STATEMENT,
		INCREMENT,
		DECREMENT,
		PAS_REF,
		EXPRESSION,
		FUNCTION_IDENTIFIER,
		TERM,
		FACTOR,
		REAL_PARAM_LIST,
		REAL_PARAM,////non-leaf-node
		IDENTIFIER,//leaf
		LIT_UNSIGNED_INT,
		LIT_STRING,
		LIT_CHAR,
		BASIC_TYPE,//leave node, int or char
		ADD_OPERATOR,
		MULTIPLY_OPERATOR,
		RELATION_OPERATOR,
		ASSIGN_OPERATOR,
		LEFT_PARENTHESE, //(
		RIGHT_PARENTHESE,//)
		LEFT_BRACKET, //[
		RIGHT_BRACKET, //]
		SEMICOLON,
		COLON,
		COMMA,
		PERIOD,
		_ARRAY,//reserved word
		_BEGIN,//prevent confusion with the reserve word in c++
		_CHAR,
		_CONST,
		_DO,
		_DOWNTO,
		_ELSE,
		_END,
		_FOR,
		_FUNCTION,_IF,_INTEGER,_OF,_PROCEDURE,_READ,_REPEAT,_THEN,_TO,_UNTIL,_VAR,_WRITE
	};
	void add(TNode* node){children.push_back(node);}
	virtual void visit(SSTable&,InsCtrl&,errorlist&);
	virtual void visit(SSTable&,InsCtrl&,errorlist&,symbol*&);
	virtual void visit(SSTable&,InsCtrl&,errorlist&,std::string&);
	virtual void visit(SSTable&,InsCtrl&,errorlist&,token&);
	virtual void visit(SSTable&,InsCtrl&,errorlist&,symbol::arguments*);
	virtual void visit(SSTable&,InsCtrl&,errorlist&,symbol*&,symbol*&);
    int get_child_size(){return children.size();}
	node_type get_node_type();
	virtual ~TNode(); 
	TNode(){};
	TNode(node_type,unsigned int);
	unsigned int getline(){return line;}
	friend void operator<<(std::ostream&, const TNode&);
protected:

	node_type tree_type;
	std::vector<TNode*> children;
	unsigned int line;

	virtual void to_string(std::ostream&, int) const;
};

#endif // !defined(AFX_TNODE_H__FDD7F758_4445_430B_B379_B8CAC8A9A980__INCLUDED_)

