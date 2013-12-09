#ifndef _COMPILER_DAG
#define _COMPILER_DAG
#pragma warning(disable:4786)
#include "InsCtrl.h"
#include "SSTable.h"
#include "Quadcode.h"
#include <MAP>
#include <VECTOR>

typedef std::vector<symbol*> symbol_vec;
typedef std::set<unsigned int> num_set;
typedef std::vector<unsigned int> num_vec;
typedef std::pair<symbol*, unsigned int> sym_node_pair;
typedef std::vector<sym_node_pair> node_pair_vec;

class DNode
{
public: DNode(symbol*);
		DNode(Quadcode::Quad_type, unsigned int);
		DNode(Quadcode::Quad_type, unsigned int, unsigned int);	//two children
		bool is_leaf;
		bool two_children;
		bool enable;
		Quadcode::Quad_type typ;
		symbol* sym;
		unsigned int left;	//left node index
		unsigned int right;	//right node index
};

class Dag  
{
public:
	Dag(InsCtrl*&);
private:

	std::map<unsigned int, symbol_vec> nodeNum_symbol;
	std::map<symbol*, unsigned int> symbol_nodeNum;
	std::map<Quadcode::Quad_type, num_set> operators; //stat of operator eg <+ <1,2,3...>> etc
	std::vector<DNode*> dag_nodes;
	std::map<unsigned int, num_vec> node_parents;
	std::map<unsigned int, node_pair_vec> assignment;	//<1, <<symbol, 1>,<symbol, 2>,...>>
	std::multimap<symbol*, symbol*> init_val;
	unsigned int last_oper;
	void construct_dag(InsCtrl*& ins);
	void output_quadcode(InsCtrl& ins);
	void flush();
	void update_node(symbol*, unsigned int);
	symbol* get_rep_operand(std::map<unsigned int, symbol*>&, unsigned int);
	void update_origin(symbol*, unsigned int);
	void node_assign(InsCtrl&, std::map<unsigned int, symbol*>&, unsigned int);
};

#endif