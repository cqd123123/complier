#ifndef _COMPILER_SYMBOL
#define _COMPILER_SYMBOL
#pragma warning(disable:4786)
#include "token.h"
#include <string>
#include <vector>
#include <DEQUE>
class SymbolTable;

class symbol  
{
public:
	const static enum symbol_type
	{
			LITERAL,
			CONST,
			VARIABLE,
			ARGUMENT,
			FUNCTION,
			PROCEDURE,
			LABEL,
			ARRAY,
			AUXSYM,
			REFSYM, //reference sym
	};
	const static enum symbol_value_type
	{
		CHAR,
		STRING,
		INTEGER
	};

	symbol(){ scope=NULL;marked=false;};

	std::string get_id();
	std::string get_value();
	symbol_type get_sym_type();
	symbol_value_type get_val_type();
	std::vector<symbol*>* get_params();
	void set_scope(SymbolTable*);
	void set_cur_scope(SymbolTable*);
	SymbolTable* get_scope();
	SymbolTable* get_cur_scope();
	int get_index();
	unsigned  int get_size();
	static symbol* lit_symbol(std::string, symbol_value_type);//get an lit symbol
	static symbol* label_symbol(std::string);//label , branch
	static symbol* var_symbol(std::string,symbol_value_type);
	static symbol* const_symbol(std::string,std::string, symbol_value_type);
	static symbol* func_symbol(std::string,symbol_value_type,std::vector<symbol*>*, unsigned int);//func, has bunches of params
	static symbol* proc_symbol(std::string,std::vector<symbol*>* , unsigned int);
	static symbol* array_symbol(std::string,unsigned int,symbol_value_type);
	static symbol* array_symbol(symbol* sym);
	static symbol* argument_symbol(std::string,symbol_value_type);

	symbol* get_array_symbol(bool pop);
	unsigned int get_code_index();
	static symbol* type_symbol(symbol::symbol_value_type, int size);//type symbol, should not in the quad code
	/*In C++, the reference parameters are initialized with the actual arguments when the function is called.
	A reference is an alias or an alternative name for an object.
	All operations applied to a reference act on the object to which the reference refers. 
	The address of a reference is the address of the aliased object.
	otherwise if the passed by value, the value will be pushed into the stack before the function is called,
	any reference or assignment to that parameter in the function will be done with the memory in the stack
	rather than the value that was passed by
	A reference symbol can refer to a reference symbol*/
	static symbol* ref_symbol(std::string,symbol_value_type);
	void set_reference(symbol*);
	void set_array_index(symbol*,bool back, unsigned int);
	symbol* get_ref_sym();
	bool is_ref_null();
	bool get_maked();
	void set_marked(bool);

	void set_enable(bool _enable){enabled=_enable;}
	bool is_enable(){ return enabled;}
	typedef std::vector<symbol*> arguments;

	bool is_global();
	friend void operator<<(std::ostream&, const symbol&);

private:
	symbol_type type;
	std::string id;
	std::string val;
	bool marked;
	symbol_value_type val_type;
	std::vector<symbol*> *params;
	unsigned int size;
	SymbolTable* scope; //the scope of a symbol
	SymbolTable* cur_scope;//current scope, only valid for function/proc symbol;
	std::deque<symbol*> array_index; //the index symbol of the array symbol. used by array symbol only
	int index;//the size of the array used by type symbol to pass the length of the array, not useful in other symbol
	symbol* ref_sym;//the symbol whitch was refered to
	bool enabled;
	unsigned int code_line;
};
#endif
