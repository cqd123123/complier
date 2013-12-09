#ifndef _COMPILER_TLNODE
#define _COMPILER_TLNODE
#pragma warning(disable:4786)

#include "token.h"
#include "TNode.h"
class TLeaf:public TNode
{
public:
	TLeaf(){};
	void set_token(token);
    token get_token()	const;
	TLeaf(token,node_type);	
	virtual void visit(SSTable&,InsCtrl&,errorlist&,token&);
protected:
	token leaf_token;
	virtual void to_string(std::ostream&, int) const;

};
#endif