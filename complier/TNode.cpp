#pragma warning(disable:4786)
#include "TNode.h"
#include <VECTOR>
#include <CASSERT>

const std::string NODE_TYPE_MAP[]=
{
	"PROGRAM",
	"SUB_PROGRAM",
	"CONST_DECLARATION",
	"VAR_DECLARATION",
	"PROCEDURE_DECLARATION",
	"FUNCTION_DECLARATION",
	"PROCEDURE_DEFINITION",
	"FUNCTION_DEFINITION",
	"COMPOUND_STATEMENT",
	"CONST_DEFINITION",
	"CONST_VALUE",
	"VAR_DEFINITION",
	"TYPE",
	"PROCEDURE_FIRST",
	"FORM_PARAM_LIST",
	"FUNCTION_FIRST",
	"STATEMENT",
	"ASSIGNMENT_STATEMENT",
	"CONDITION_STATEMENT",
	"CONDITION",
	"REPEAT_LOOP_STATEMENT",
	"CALL_PROCEDURE_STATEMENT",
	"CALL_FUNCTION_STATEMENT",
	"READ_STATEMENT",
	"WRITE_STATEMENT",
	"FOR_LOOP_STATEMENT",
	"INCREMENT",
	"DECREMENT",
	"PAS_REF",
	"EXPRESSION",
	"FUNCTION_IDENTIFIER",
	"TERM",
	"FACTOR",
	"REAL_PARAM_LIST",
	"REAL_PARAM",////non-leaf-node
	"IDENTIFIER",//leaf
	"LITRAL",//LIT_UNSIGNED_INT,LIT_STRING,LIT_CHAR
	"BASIC_TYPE",//leave node, int or char
	"ADD_OPERATOR",
	"MULTIPLY_OPERATOR",
	"RELATION_OPERATOR",
	"ASSIGN_OPERATOR",
	"LEFT_PARENTHESE", //(
	"RIGHT_PARENTHESE",//)
	"LEFT_BRACKET", //[
	"RIGHT_BRACKET", //]
	"SEMICOLON",
	"COLON",
	"COMMA",
	"PERIOD",
	"_ARRAY",//reserved word
	"_BEGIN",
	"_CHAR",
	"_CONST",
	"_DO",
	"_DOWNTO",
	"_ELSE",
	"_END",
	"_FOR",
	"_FUNCTION","_IF","_INTEGER","_OF","_PROCEDURE","_READ","_REPEAT","_THEN","_TO","_UNTIL","_VAR","_WRITE"
};
TNode::~TNode()
{;}
TNode::TNode(node_type typ,unsigned int lin)
{
	tree_type=typ;
	line=lin;
}
void TNode::visit(SSTable& s_table,InsCtrl& ins,errorlist& erl)
{
	for(std::vector<TNode*>::iterator it=children.begin();it!=children.end();it++)
		(*it)->visit(s_table,ins,erl);
}
void TNode::visit(SSTable& s_table,InsCtrl& ins,errorlist& erl,std::string& str)
{
	assert(0);
}
void TNode::visit(SSTable& s_table,InsCtrl& ins,errorlist& erl,symbol*& sym)
{
	assert(0);
}
void TNode::visit(SSTable& s_table,InsCtrl& ins,errorlist& erl,token& tok)
{
	assert(0);std::cout<<"illegal visit"<<std::endl;
}
// void TNode::visit(SSTable&,InsCtrl&,errorlist&,symbol::arguments*&)
// {
// 	assert(0);
// }
void TNode::visit(SSTable&,InsCtrl&,errorlist&,symbol::arguments*)
{
	assert(0);
}
void TNode::visit(SSTable&,InsCtrl&,errorlist&,symbol*&,symbol*&)
{
	assert(0);
}
TNode::node_type TNode::get_node_type()
{
	return tree_type;
}
void operator<<(std::ostream& out, const TNode& _node)
{
	_node.to_string(out, 0);
}
void TNode::to_string(std::ostream& out, int indent) const
{
	for(int i = 0; i < indent; i++) out << "  ";
	out << "[" << NODE_TYPE_MAP[tree_type] << "]" <<"  layer "<<indent<<std::endl;
	for(unsigned int k = 0; k < children.size(); k++)
		children[k]->to_string(out, indent + 1);
	
}
