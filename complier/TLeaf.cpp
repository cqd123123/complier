#pragma warning(disable:4786)
#include <IOSTREAM>
#include "TLeaf.h"
#include "TNode.h"

TLeaf::TLeaf(token tok, node_type type)
{
	leaf_token=tok;
	line=tok.get_line();
	tree_type=type;
}
token TLeaf::get_token() const
{
	return leaf_token;
}

void TLeaf::to_string(std::ostream& out, int indent) const
{
	for(int i = 0; i < indent; i++) out << "  ";
	out << this->get_token();
}
void TLeaf::visit(SSTable& tbl,InsCtrl& ins,errorlist& erl,token& tok)
{
	tok=leaf_token;
}