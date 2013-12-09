#pragma warning(disable:4786)
#include "Dag.h"
#include <CASSERT>
#include <iostream>
#include <STDIO.H>
#include <FSTREAM>

DNode::DNode(symbol* _leaf)		//the leaf node. init value
{
	sym=_leaf;
	is_leaf=true;
	two_children=false;
	enable=true;
}
DNode::DNode(Quadcode::Quad_type _typ, unsigned int _left)	//dag node, one child
{
	is_leaf=false;
	two_children=false;
	enable=true;
	typ=_typ;
	left=_left;
	sym=NULL;
}
DNode::DNode(Quadcode::Quad_type _typ, unsigned int _left, unsigned int _right)		//dag node, two children
{
	is_leaf=false;
	two_children=true;
	enable=true;
	typ=_typ;
	left=_left;
	right=_right;
	sym=NULL;
}
namespace
{
	bool find_sym(symbol* arg, std::map<symbol*, unsigned int>& sym_node, unsigned int& index)//find symbol in the dag tree, return index
	{
		assert(arg!=NULL);
		std::map<symbol*, unsigned int>::iterator it=sym_node.find(arg);
		if(it!=sym_node.end())
		{
			index=it->second;
			return true;
		}
		else
			return false;
	}
	//find an operator in the given dag graph
	bool find_operator(Quadcode::Quad_type oper,std::map<Quadcode::Quad_type, num_set>& operators, 
		std::vector<DNode*>& nodes, unsigned int left, unsigned int right, unsigned int& index)
	{
		std::map<Quadcode::Quad_type, num_set>::iterator it=operators.find(oper);
		if(it!=operators.end())
		{
			std::set<unsigned int>& list=it->second;//the list of node index with the operation
			for(std::set<unsigned int>::iterator i=list.begin(); i!=list.end();i++)
			{
				if(nodes[*i]->left==left&&nodes[*i]->right==right)
				{
					index=*i;
					return true;
				}
			}
			return false;
		}
		else
			return false;
	}
	//find operator only have left child according to the required child node.
	bool find_operator(Quadcode::Quad_type oper, std::map<Quadcode::Quad_type, num_set>& operators,
		std::vector<DNode*>& nodes, unsigned int left, unsigned int& index)
	{
		std::map<Quadcode::Quad_type, num_set>::iterator it=operators.find(oper);
		if(it!=operators.end())
		{
			std::set<unsigned int>& list=it->second;
			for(std::set<unsigned int>::iterator i=list.begin();i!=list.end();i++)
			{
				if(nodes[*i]->left==left)
				{
					index=*i;
					return true;
				}
			}
			return false;
		}
		else
			return false;
	}
	bool is_no_parent(std::vector<DNode*>& nodes, unsigned int index)	//whether a operator node have parents
	{
		for(unsigned int i=0;i<nodes.size();i++)
		{
			if(!nodes[i]->enable) continue;		//if disabled .. not considered as a parent
			if(!nodes[i]->is_leaf)
			{
				if(nodes[i]->left==index)	//a node have a child of a given index
					return false;
				if(nodes[i]->two_children&&nodes[i]->right==index)
					return false;
			}
		}
		return true;
	}
	Quadcode* new_instruction(Quadcode::Quad_type oper,symbol* arg1, symbol* arg2, symbol* arg3)
	{
		assert(arg1!=NULL&&arg2!=NULL&&arg3!=NULL);
		switch(oper)
		{
		case Quadcode::ADD:
		case Quadcode::SUB:
		case Quadcode::MUL:
		case Quadcode::DIV:
			return Quadcode::new_Quad_code(oper, arg1, arg2, arg3);
		case Quadcode::ASSIGN:
		case Quadcode::INV:
			return Quadcode::new_Quad_code(oper, arg1, arg3);
		default:
			assert(0);
			return NULL;
		}
	}
}
Dag::Dag(InsCtrl*& ins)
{
	construct_dag(ins);
}
void Dag::construct_dag(InsCtrl*& ins)
{
	InsCtrl* new_ins=new InsCtrl(ins->sstab);	//get the stack symbol table
	last_oper=0;
	std::vector<FBlock*> ins_fblk=ins->get_all_func_block();
	for(int i=0;i<ins_fblk.size();i++)
	{
		FBlock* fblk=ins_fblk[i];
		for(int j=0;j<fblk->all_blocks.size();j++)
		{
			BBlock* bblk=fblk->all_blocks[j];
			if(!bblk->has_array())	//we currently do not consider arraies, migh
			{
				for(int k=0;k<bblk->all_ins.size();k++)
				{
					Quadcode* q=bblk->all_ins[k];
				
					Quadcode* code=NULL;
					Quadcode::Quad_type oper=q->get_quad_type();
 					symbol* src_arg1=NULL;
 					symbol* src_arg2=NULL;
 					symbol* obj_arg=NULL;
					unsigned int index1;
					unsigned int index2;
					unsigned int index3;
					switch(oper)
					{
					case  Quadcode::ASSIGN:
						src_arg1=q->get_src_arg()[0];
						obj_arg=q->get_obj_arg();
						if(!find_sym(src_arg1,symbol_nodeNum,index1))	//cannot find the src node in the dag tree
						{
							index1=dag_nodes.size();
							dag_nodes.push_back(new DNode(src_arg1));	//src first leave node
							update_node(src_arg1,index1);	//update the src symbol index
						}
						update_node(obj_arg,index1);	//update the obj arg, and the index num is the tree node index obj was assigned to
						assignment[last_oper].push_back(std::pair<symbol*, unsigned int>(obj_arg,index1));	//assign information, the index begin with 0
						break;
					case Quadcode::ADD:
					case Quadcode::SUB:
					case Quadcode::MUL:
					case Quadcode::DIV:
						src_arg1=q->get_src_arg()[0];
						src_arg2=q->get_src_arg()[1];
						obj_arg=q->get_obj_arg();
						if(!find_sym(src_arg1,symbol_nodeNum,index1))	//cannot find the src node in dag tree
						{
							index1=dag_nodes.size();
							dag_nodes.push_back(new DNode(src_arg1));
							update_node(src_arg1,index1);
						}
						if(!find_sym(src_arg2,symbol_nodeNum,index2))
						{
							index2=dag_nodes.size();
							dag_nodes.push_back(new DNode(src_arg2));
							update_node(src_arg2,index2);
						}
						if(!find_operator(oper,operators,dag_nodes,index1,index2,index3))
						{
							index3=dag_nodes.size();
							dag_nodes.push_back(new DNode(oper,index1,index2));
							operators[oper].insert(index3);
							last_oper=dag_nodes.size();
						}
						update_node(obj_arg,index3);
						assignment[last_oper].push_back(std::pair<symbol*,unsigned int>(obj_arg,index3));
						break;
					case Quadcode::INV:
						src_arg1=q->get_src_arg()[0];
						obj_arg=q->get_obj_arg();
						if(!find_sym(src_arg1,symbol_nodeNum,index1))
						{
							index1=dag_nodes.size();
							dag_nodes.push_back(new DNode(src_arg1));
							update_node(src_arg1,index1);
						}
						if(!find_operator(oper,operators,dag_nodes,index1,index2))
						{
							index2=dag_nodes.size();
							dag_nodes.push_back(new DNode(oper,index1));
							operators[oper].insert(index2);
							last_oper=dag_nodes.size();
						}
						update_node(obj_arg,index2);
						assignment[last_oper].push_back(std::pair<symbol*,unsigned int>(obj_arg,index2));
						break;
					case Quadcode::CALL:
					case Quadcode::SIND:
						output_quadcode(*new_ins);
						src_arg1=q->get_src_arg()[0];
						obj_arg=q->get_obj_arg();
						if(obj_arg==NULL)
							code=Quadcode::new_Quad_code(oper,src_arg1);
						else code=Quadcode::new_Quad_code(oper,obj_arg,src_arg1);
						new_ins->new_instruction(code);
						break;

					case Quadcode::JMP:
					case Quadcode::LABEL:
//					case Quadcode::READ:
					case Quadcode::WRITE:
					case Quadcode::PUSH:
					case Quadcode::SREF:
						output_quadcode(*new_ins);
						src_arg1=q->get_src_arg()[0];
						code=Quadcode::new_Quad_code(oper,src_arg1);
						new_ins->new_instruction(code);
						break;
					case Quadcode::FUNC:
					case Quadcode::MAIN:
					case Quadcode::ENDF:
					case Quadcode::ENDM:
					case Quadcode::PARAM:
					case Quadcode::READ:
						output_quadcode(*new_ins);
						obj_arg=q->get_obj_arg();
						code=Quadcode::new_Quad_code(oper,obj_arg);
						new_ins->new_instruction(code);
						break;
					case Quadcode::JEQ:
					case Quadcode::JGE:
					case Quadcode::JGT:
					case Quadcode::JLE:
					case Quadcode::JLT:
					case Quadcode::JNE:
						output_quadcode(*new_ins);
						src_arg1=q->get_src_arg()[0];
						src_arg2=q->get_src_arg()[1];
						obj_arg=q->get_src_arg()[2];
						code=Quadcode::new_Quad_code(oper,obj_arg,src_arg1,src_arg2);
						new_ins->new_instruction(code);
						break;
					case Quadcode::LINE:
						output_quadcode(*new_ins);
						code=Quadcode::new_Quad_code(oper);
						new_ins->new_instruction(code);
						break;
					default:
						assert(0);
					}
				
//					std::cout<<"y  "<<*q;
					q->disable();
					}
					output_quadcode(*new_ins);
				}
				else
				{
					for(int m=0;m<bblk->all_ins.size();m++)
					{//	std::cout<<"s  "<<*bblk->all_ins[m];
						new_ins->new_instruction(bblk->all_ins[m]);
					}
					output_quadcode(*new_ins);
				}				
			}
			output_quadcode(*new_ins);
	}

	delete ins;
	new_ins->reorder_all_ins();
	std::ofstream file("block_dag.txt",std::ios::out);
	std::vector<FBlock*> blo=new_ins->get_all_func_block();
	for(int i1=0;i1<blo.size();i1++)
	{
		file<<*blo[i1];
	}
	ins=new_ins;
}


void Dag::update_node(symbol* sym, unsigned int index)	//renew a node
{
	nodeNum_symbol[index].push_back(sym);	//push the sym into a new dag node
	if(symbol_nodeNum.find(sym)!=symbol_nodeNum.end())	//erase the symbol in the previous tree node
	{
		unsigned int _i=symbol_nodeNum[sym];	//the node number of that symbol
		std::vector<symbol*>::iterator it=nodeNum_symbol[_i].begin();	//find the symbol in the structure
		while(*it!=sym&&it!=nodeNum_symbol[_i].end()) it++;
		assert(it!=nodeNum_symbol[_i].end());
		nodeNum_symbol[_i].erase(it);
	}
	symbol_nodeNum[sym]=index;
}
void Dag::node_assign(InsCtrl& ins, std::map<unsigned int, symbol*>& node_operand, unsigned int index)
{
	std::vector<sym_node_pair> _assign=assignment[index];//the symbol is associated with the node in the second <symbol, node index ....>
	for(unsigned int i=0;i<_assign.size();i++)
	{
		symbol* arg1=_assign[i].first;	//need to assign the value of node[_assign[i].second ] to the symbol
		symbol *arg2=get_rep_operand(node_operand,_assign[i].second);	//get the "representative" symbol of that node
		update_origin(arg1,_assign[i].second);
		if(arg1!=arg2)
			ins.assign(arg1,arg2);
	}
}
void Dag::update_origin(symbol* sym, unsigned int index)
{
	for(std::multimap<symbol*,symbol*>::iterator it=init_val.begin();it!=init_val.end();it++)
	{
		if(it->second==sym)
		{//strip the element from the origin list, if this function is called, no longer in the ori list
			init_val.erase(it);
			break;
		}
	
	}
	if(dag_nodes[index]->is_leaf)
	{//if same symbol with same index on the leave and called this function twice, will erase and then add
		symbol* source=dag_nodes[index]->sym;
		init_val.insert(std::pair<symbol*,symbol*>(source,sym));//first symbol(source) is always in the leaves
	}
}

symbol* Dag::get_rep_operand(std::map<unsigned int, symbol*>& node_operand, unsigned int index)
{
	std::map<unsigned int, symbol*>::iterator it=node_operand.find(index);//every node with index i have a representative sym, save  in the structure node_op
	if(it!=node_operand.end())	//the required node index have already get a rep symbol
	{
		return it->second;	//just return it
	}
	else if(dag_nodes[index]->is_leaf)
	{	 //init_val have assignment information, handle situation like, <a, b>, <a,c>, will return c,
		std::multimap<symbol*,symbol*>::iterator it1=init_val.lower_bound(dag_nodes[index]->sym);
		std::multimap<symbol*,symbol*>::iterator it2=init_val.upper_bound(dag_nodes[index]->sym);
	
		if(it1!=it2){ return it1->second;}
		else return dag_nodes[index]->sym; //a leaf has only one symbol, i.e, <5>, will return the leaf node
	}
	else
	{
		std::map<unsigned int, symbol_vec>::iterator it3=nodeNum_symbol.find(index);
		assert(it3!=nodeNum_symbol.end());
		symbol_vec& list=it3->second;
		for(int i=0;i<list.size();i++)	//return non-temp variable first
		{
			if(list[i]->get_id()[0]!='$')	//not temp variable
				return node_operand[index]=list[i];
		}
		return node_operand[index]=list[0];
	}
	assert(0);
	return NULL;
}

void Dag::output_quadcode(InsCtrl& ins)
{
	std::vector<unsigned int> oper_stack;
	std::map<unsigned int, symbol*> obj_operand;

	//get the reverse order of the all operators, will be put in the oper_stack
	while(!operators.empty())
	{
		bool find=false;
		unsigned int _index=0;
		//first find the node index which do not have a parent
		for(std::map<Quadcode::Quad_type,num_set>::iterator it=operators.begin();it!=operators.end();it++)
		{
			num_set& node_set=it->second;	//the list of node index which the operator is in
			for(num_set::iterator it1=node_set.begin();it1!=node_set.end();it1++)
			{
				if(is_no_parent(dag_nodes,*it1)&&(*it1>_index||!find))	//the dag tree can have multiple branches, so we find the one with maximum index
				{
					_index=*it1;
					find=true;
				}
			}
		}
		
		if(find)	//we have find the index
		{
			do
			{
				std::map<Quadcode::Quad_type,num_set>::iterator it=operators.find(dag_nodes[_index]->typ); //find the given operator set
				assert(it!=operators.end());
				num_set& node_set=it->second; //the node set of the operation
				num_set::iterator it1=node_set.find(_index);
				node_set.erase(it1);	//erase the find node
				if(node_set.empty()) operators.erase(it);

				oper_stack.push_back(_index);
				dag_nodes[_index]->enable=false;
				_index=dag_nodes[_index]->left;	//traverse the left branch
			}
			while(!dag_nodes[_index]->is_leaf&&is_no_parent(dag_nodes,_index));	//while the operation node have no parent
		}
		else
		{assert(operators.empty());}
	}

	//already get the operator stack , output the code in the reverse order

	node_assign(ins,obj_operand,0);
	for(int i=oper_stack.size();i>0;i--)
	{
		DNode* node=dag_nodes[oper_stack[i-1]];	//in reverse order
		Quadcode::Quad_type typ=node->typ;
		Quadcode* code=NULL;
		symbol* src_arg1=NULL;
		symbol* src_arg2=NULL;
		symbol* obj_arg=NULL;

		assert(!node->is_leaf);
		src_arg1=get_rep_operand(obj_operand,node->left);
		if(node->two_children) src_arg2=get_rep_operand(obj_operand,node->right);
		obj_arg=get_rep_operand(obj_operand,oper_stack[i-1]);
		update_origin(obj_arg,oper_stack[i-1]);		//if not leave, strip it out
		if(src_arg2!=NULL) code=new_instruction(typ,obj_arg,src_arg1,src_arg2);
		else code=new_instruction(typ,obj_arg,src_arg1,src_arg1);
		ins.new_instruction(code);
		node_assign(ins,obj_operand,oper_stack[i-1]+1);		//plus one because basic index of assign_info is 1
	}
	flush();
}
void Dag::flush()	//reset data members
{
	symbol_nodeNum.clear();
	operators.clear();
	dag_nodes.clear();
	node_parents.clear();
	nodeNum_symbol.clear();
	assignment.clear();
	init_val.clear();
	last_oper = 0;
}
