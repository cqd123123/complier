#ifndef _COMPILER_SYNTAX_TREE
#define _COMPILER_SYNTAX_TREE
#pragma warning(disable:4786)
#include "TNode.h"
#include "errorlist.h"
#include "TLeaf.h"
#include "SSTable.h"

#define _NODE_CLASS_BEGIN(class_name, _tree_type) class class_name:public TNode\
{\
public:\
	class_name(unsigned int _line){line = _line; tree_type = _tree_type;}

#define  _LEAF_NODE_CLASS(class_name,_tree_type) class class_name:public TLeaf\
	{\
	public:\
	class_name(token _token){leaf_token = _token; line = _token.get_line(); tree_type = _tree_type;}

#define  _CLASS_END };

namespace SyntaxTree
{
	_NODE_CLASS_BEGIN(program,PROGRAM)
	public:
		void visit(SSTable&, InsCtrl&, errorlist&);
	_CLASS_END

	_NODE_CLASS_BEGIN(sub_program,SUB_PROGRAM)
	public:
		void visit(SSTable&, InsCtrl&, errorlist&);
_CLASS_END
	_NODE_CLASS_BEGIN(const_declaration,CONST_DECLARATION)_CLASS_END
	_NODE_CLASS_BEGIN(var_declaration,VAR_DECLARATION)_CLASS_END
	_NODE_CLASS_BEGIN(procedure_declaration,PROCEDURE_DECLARATION)_CLASS_END
 	_NODE_CLASS_BEGIN(function_declaration,FUNCTION_DECLARATION)_CLASS_END
	_NODE_CLASS_BEGIN(function_definition,FUNCTION_DEFINITION)
		public:
			void visit(SSTable&,InsCtrl&,errorlist&);
	_CLASS_END
	_NODE_CLASS_BEGIN(procedure_definition,PROCEDURE_DEFINITION)
		public:
			void visit(SSTable&,InsCtrl&,errorlist&);
	_CLASS_END

	_NODE_CLASS_BEGIN(compound_statement,COMPOUND_STATEMENT)_CLASS_END
	_NODE_CLASS_BEGIN(const_definition,CONST_DEFINITION)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&);
	_CLASS_END
	_NODE_CLASS_BEGIN(const_value,CONST_VALUE)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&,symbol*&);
	_CLASS_END
	_NODE_CLASS_BEGIN(var_definition,VAR_DEFINITION)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&);
	_CLASS_END
	_NODE_CLASS_BEGIN(type,TYPE)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&,symbol*&);
	_CLASS_END
	_NODE_CLASS_BEGIN(procedure_first,PROCEDURE_FIRST)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&,symbol*&);
	_CLASS_END
	_NODE_CLASS_BEGIN(form_param_list,FORM_PARAM_LIST)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&,symbol::arguments*);
	_CLASS_END
		_NODE_CLASS_BEGIN(function_first,FUNCTION_FIRST)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&,symbol*&);
	_CLASS_END
	_NODE_CLASS_BEGIN(statement,STATEMENT)_CLASS_END
 	_NODE_CLASS_BEGIN(assign_statement,ASSIGNMENT_STATEMENT)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&);
	_CLASS_END

	_NODE_CLASS_BEGIN(condition_statement,CONDITION_STATEMENT)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&);
	_CLASS_END
	_NODE_CLASS_BEGIN(repeat_loop_statement,REPEAT_LOOP_STATEMENT)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&);
	_CLASS_END

	_NODE_CLASS_BEGIN(call_procedure_statement,CALL_PROCEDURE_STATEMENT)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&);
	_CLASS_END
	_NODE_CLASS_BEGIN(call_function_statement,CALL_FUNCTION_STATEMENT)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&,symbol*&);
	_CLASS_END
		_NODE_CLASS_BEGIN(read_statement,READ_STATEMENT)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&);
	_CLASS_END
	_NODE_CLASS_BEGIN(write_statement,WRITE_STATEMENT)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&);
	_CLASS_END
	_NODE_CLASS_BEGIN(for_loop_statement,FOR_LOOP_STATEMENT)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&);
	_CLASS_END
	_NODE_CLASS_BEGIN(expression,EXPRESSION)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&,symbol*&,symbol*&);
	_CLASS_END

		//FUNCTION_IDENTIFIER,
	_NODE_CLASS_BEGIN(term,TERM)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&,symbol*&,symbol*&);
	_CLASS_END
	_NODE_CLASS_BEGIN(factor,FACTOR)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&,symbol*&,symbol*&);
	_CLASS_END
	_NODE_CLASS_BEGIN(real_param_list,REAL_PARAM_LIST)
		public:
			void visit(SSTable&, InsCtrl&, errorlist&,symbol::arguments*);
	_CLASS_END
	_NODE_CLASS_BEGIN(condition,CONDITION)
		public:
			void visit(SSTable&,InsCtrl&,errorlist&,symbol*&);
	_CLASS_END
	_NODE_CLASS_BEGIN(real_param,REAL_PARAM)
		public:
			void visit(SSTable&,InsCtrl&,errorlist&,symbol*&);
	_CLASS_END
	_LEAF_NODE_CLASS(identifier,IDENTIFIER)
		public:
			void visit(SSTable&,InsCtrl&,errorlist&,symbol*&);
			void visit(SSTable&,InsCtrl&,errorlist&,std::string&);
	_CLASS_END
	_LEAF_NODE_CLASS(relation_operator,RELATION_OPERATOR)_CLASS_END
	_LEAF_NODE_CLASS(add_operator,ADD_OPERATOR)_CLASS_END
	_LEAF_NODE_CLASS(lit_unsigned_int,LIT_UNSIGNED_INT)
		public:
			void visit(SSTable&,InsCtrl&,errorlist&,symbol*&);
	_CLASS_END
	_LEAF_NODE_CLASS(lit_char,LIT_CHAR)
		public:
			void visit(SSTable&,InsCtrl&,errorlist&,symbol*&);
	_CLASS_END
	_LEAF_NODE_CLASS(lit_string,LIT_STRING)
		public:
			void visit(SSTable&,InsCtrl&,errorlist&,symbol*&);
	_CLASS_END
	_LEAF_NODE_CLASS(basic_type,BASIC_TYPE)_CLASS_END
	_LEAF_NODE_CLASS(assign_operator,ASSIGN_OPERATOR)_CLASS_END
	_LEAF_NODE_CLASS(multiply_operator,MULTIPLY_OPERATOR)_CLASS_END
	_LEAF_NODE_CLASS(increment,INCREMENT)_CLASS_END
	_LEAF_NODE_CLASS(decrement,DECREMENT)_CLASS_END
	_LEAF_NODE_CLASS(pass_ref,PAS_REF)
		public:
			void visit(SSTable&,InsCtrl&,errorlist&,std::string&);
	_CLASS_END
}
#endif

