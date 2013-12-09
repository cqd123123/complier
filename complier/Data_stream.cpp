#pragma warning(disable:4786)
#include "Data_stream.h"
#include <OSTREAM>
#include <CASSERT>
#include <iostream>
#include <FSTREAM>

#define _NO_WASTE_SPACE(arg) ((arg).members.size() <= 1 || (arg).members.back() != 0)
const unsigned int bit_set::bits_per_byte = 8;
const unsigned int bit_set::bits_per_int = bit_set::bits_per_byte * sizeof(unsigned int);

bit_set bit_set::operator+(const unsigned int& element)  //if +, use the +=function
{
	bit_set result(*this);
	result+=element;
	return result;
}

bit_set bit_set::operator+(const bit_set& oper) //same..
{
	bit_set result(*this);
	result+=oper;
	return result;
}

bit_set& bit_set::operator+=(const unsigned int& element)//put an element into the bit_set
{
	unsigned int index=element/bits_per_int; //cal index, one int can accomodate 32 bits, ie. 32 elements
	unsigned int bit=element%bits_per_int; //..

	if(index>=members.size())
	{
		members.resize(index+1);
	}
	members[index]|=(1<<bit);
	assert(_NO_WASTE_SPACE((*this)));//no waste space...
	return *this;
}

bit_set& bit_set::operator+=(const bit_set& oper)//sum the set
{
	unsigned int lower_bound=members.size()<oper.members.size()?members.size():oper.members.size();
	unsigned int upper_bound=members.size()>oper.members.size()?members.size():oper.members.size();

	members.resize(upper_bound);
	for(unsigned int i=0;i<lower_bound;i++)
	{
		members[i]|=oper.members[i];
	}
	for(unsigned int i1=lower_bound;i1<oper.members.size();i1++)
	{
		members[i1]=oper.members[i1];
	}
	assert(_NO_WASTE_SPACE(*this));
	return *this;
}

bit_set bit_set::operator-(const unsigned int& element)
{
	bit_set result(*this);
	result-=element;
	return result;
}

bit_set& bit_set::operator-=(const unsigned int& element)
{
	unsigned int index=element/bits_per_int;
	unsigned int bit=element%bits_per_int;
	if(index<members.size())
	{
		members[index]&=(~(1<<bit));
	}
	for(unsigned int i=members.size();i>0;i--)//cut the redunant member, if we erase a element
	{
		if(members[i-1]||i==1)
		{
			members.resize(i);
			break;
		}
	}
	assert(_NO_WASTE_SPACE(*this));
	return *this;
}

bit_set bit_set::operator-(const bit_set& oper)
{
	bit_set result(*this);
	result -= oper;
	return result;
}

bit_set& bit_set::operator-=(const bit_set& oper)
{
	unsigned int lower_bound=members.size()<oper.members.size()?members.size():oper.members.size();

	for(unsigned int i=0;i<lower_bound;i++)
	{
		members[i]&=(~oper.members[i]);
	}
	for(unsigned int i1=members.size();i1>0;i1--)
	{
		if(members[i1-1]||i1==1)
		{
			members.resize(i1);
			break;
		}
	}
	assert(_NO_WASTE_SPACE(*this));
	return *this;
}

bit_set bit_set::operator&(const bit_set& oper)
{
	bit_set result(*this);
	result&=oper;
	return result;
}

bit_set& bit_set::operator&=(const bit_set& oper)
{
	unsigned int lower_bound=members.size()<oper.members.size()?members.size():oper.members.size();

	members.resize(lower_bound);
	for(unsigned int i=0;i<lower_bound;i++)
	{
		members[i]&=oper.members[i];
	}
	for(unsigned int i1=members.size();i1>0;i1--)//cut the redunant member
	{
		if(members[i1-1]||i1==1)
		{
			members.resize(i1);
			break;
		}
	}
	assert(_NO_WASTE_SPACE(*this));
	return *this;
}

bool bit_set::empty() const
{
	assert(_NO_WASTE_SPACE(*this));
	if(members.size()==0||(members.size()==1&&members[0]==0))
		return true;
	else
		return false;
}

bool bit_set::find(unsigned int element) const
{
	assert(_NO_WASTE_SPACE(*this));
	unsigned int index=element/bits_per_int;
	unsigned int bit=element%bits_per_int;
	if(index>=members.size())//the element is not in the set
	{
		return false;
	}
	else 
	{
		if(members[index]&(1<<bit))
			return true;
		else
			return false;
	}
}

std::vector<unsigned int> bit_set::get_numbers() const ///get the number array from the bit set
{
	unsigned int number = 0;
	std::vector<unsigned int> numbers;
	for(unsigned int i=0;i<members.size();i++)
	{
		for(unsigned int j=0;j<bits_per_int;j++)
		{
			if(members[i]&(1<<j))
			{
				numbers.push_back(number+j);
			}
		}
		number+=bits_per_int;
	}
	return numbers;
}

bool operator==(const bit_set& oper1, const bit_set& oper2)
{
	if(oper1.members.size() != oper2.members.size())return false;
	assert(_NO_WASTE_SPACE(oper1));
	assert(_NO_WASTE_SPACE(oper2));
	for(unsigned int i=0;i<oper1.members.size();i++)
	{
		if(oper1.members[i]!=oper2.members[i])
			return false;
	}
	return true;
}

bool operator!=(const bit_set& oper1, const bit_set& oper2)
{
	return !(oper1==oper2);
}

void operator<<(std::ostream& out, const bit_set& set)
{
	unsigned int number=0;
	bool first_element=true;
	out<<"{";
	for(unsigned int i=0;i<set.members.size();i++)
	{
		for(unsigned int j=0;j<bit_set::bits_per_int;j++)
		{
			if(set.members[i]&(1<<j))
			{
				if(!first_element)
					out << ",";
				else
					first_element = false;
				out<<(number+j);
			}
		}
		number+= bit_set::bits_per_int;
	}
	out<<"}"<<std::endl;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//reach definition

Reach_def::Reach_def(FBlock* _blk, std::map<symbol*, bit_set> _use, std::map<symbol*, bit_set> _def):fblk(_blk),use(_use),def(_def)
{
	_analyse();
}
//get the reach definition flow into the given code position
bit_set Reach_def::get_in_r_def(Quadcode* code) const
{
	if(code_in.find(code)!=code_in.end())
		return code_in.find(code)->second;
	else
		return bit_set();
}
//get the .. flow out the given code position
bit_set Reach_def::get_out_r_def(Quadcode* code) const
{
	if(code_out.find(code)!=code_out.end())
		return code_out.find(code)->second;
	else
		return bit_set();
}

void Reach_def::_analyse()
{
	std::map<BBlock*, bit_set> gen;
	std::map<BBlock*, bit_set> kill;
	std::map<BBlock*, bit_set> _in;
	std::map<BBlock*, bit_set> _out;
	std::map<Quadcode*, bit_set> code_gen;
	std::map<Quadcode*, bit_set> code_kill;

	//init gen and kill set;

	std::vector<BBlock*> bblk=fblk->get_basic_blocks();
	for(int i=0;i<bblk.size();i++)
	{
		BBlock *_blk=bblk[i];
		bit_set blk_gen;
		bit_set blk_kill;
		std::vector<Quadcode*> codes=_blk->get_all_ins();
		for(int j=0;j<codes.size();j++)
		{
			Quadcode* q=codes[j];
			Quadcode::Quad_type q_typ=q->get_quad_type();
			unsigned int index=q->get_index();
			symbol* obj=q->get_obj_arg();		//these code should count, they have obj args,
			bool cal=!(q_typ==Quadcode::ENDF||q_typ==Quadcode::ENDP||q_typ==Quadcode::FUNC||q_typ==Quadcode::MAIN||q_typ==Quadcode::ENDM);
			if(cal&&obj!=NULL)
			{
				blk_gen+=index;
				blk_kill+=(def[obj]-index);
				code_gen[q]+=index;
				code_kill[q]=(def[obj]-index);
			}
		}
		gen[_blk]=blk_gen;
		kill[_blk]=blk_kill;
//		_out[_blk]=blk_gen-blk_kill;//init out[blk]=gen-kill
//		std::cout<<*_blk;
//		std::cout<<gen[_blk];
//		std::cout<<kill[_blk];

	}

	//iterate until unchanged
	while(true)
	{
		bool flag=false;	//if all in and out unchanged, will be true
		std::vector<BBlock*> bblks=fblk->get_basic_blocks();
		for(int i=0;i<bblks.size();i++)
		{
			BBlock*	blk1=bblks[i];
			bit_set in, temp,temp1;
			for(int j=0;j<blk1->entry.size();j++)	//flow in=all the out defintion of the entry basic block
			{
				in+=_out[blk1->entry[j]];
			}
			temp=in;
			std::vector<Quadcode*> ins=blk1->get_all_ins();
			for(int k=0;k<ins.size();k++)
			{
				code_in[ins[k]]=temp;
				temp=code_out[ins[k]]+=code_gen[ins[k]]+(temp-code_kill[ins[k]]);	//qout=qgen+(qin-qkill)
			}
			temp1=_out[blk1];
			_in[blk1]=in;
			_out[blk1]+=gen[blk1]+(in-kill[blk1]);	//out=gen+(in-out)
			if(temp1!=_out[blk1])	//the out set does change, then cal again, until unchanged, flag will become false
				flag=true;
		}
		if(!flag) break;
	}
	//out put reach definition results
	std::ofstream file("t_reach.txt",std::ios::out);
	std::map<BBlock*, bit_set>::iterator it1=_out.begin();
	for(std::map<BBlock*, bit_set>::iterator it=_in.begin(); it!=_in.end();it++)
	{
		file<<*it->first;
		file<<it->second;
		file<<"out"<<std::endl;
		file<<it1->second;
		it1++;
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//web class, cal web according to the reach definition
Webs::Webs(FBlock* _blk, std::map<symbol*, bit_set> _use, std::map<symbol*, bit_set> _def, Reach_def* _r_def):fblk(_blk),use(_use),def(_def),r_def(_r_def)
{
	_analyse();
	mapping();
}
//get all webs
std::vector<sym_web> Webs::get_web_vec() const
{
	return web_vec;
}
//find the symbol according to the given web
symbol* Webs::get_symbol(web* _web)
{
	unsigned int index=web_index_map[_web];
	return web_vec[index].first;
}
//find the web according to the given symbol, the use symbol and definition of symbol was mapped into two different maps
web* Webs::get_net(symbol* s, unsigned int line, bool _use) const
{
	if(_use)
	{
		std::map<sym_line, unsigned int>::const_iterator it=sym_uweb_map.find(std::pair<symbol*, unsigned int>(s, line));
		if(it!=sym_uweb_map.end())
			return web_vec[it->second].second;
		else return NULL;
	}
	else
	{
		std::map<sym_line, unsigned int>::const_iterator it=sym_dweb_map.find(std::pair<symbol*, unsigned int>(s,line));
		if(it!=sym_dweb_map.end())
			return web_vec[it->second].second;
		else return NULL;
	}
}

unsigned int Webs::get_net_index(web* _web) const
{
	return web_index_map.find(_web)->second;
}
//if do not find, return false, this symbol do not have a web
bool Webs::get_net_index(symbol* sym, unsigned int line, bool _use, unsigned int& index) const
{
	if(_use)
	{
		std::map<sym_line, unsigned int>::const_iterator it=sym_uweb_map.find(sym_line(sym,line));
		if(it!=sym_uweb_map.end())
			return index=it->second,true;
		else return false;
	}
	else
	{
		std::map<sym_line, unsigned int>::const_iterator it=sym_dweb_map.find(sym_line(sym,line));
		if(it!=sym_dweb_map.end())
			return index=it->second,true;
		else return false;
	}
}

void Webs::_analyse()
{
	//first get the all chains. then merge it
	chains chain_map;
	for(std::map<symbol*, bit_set>::iterator sym_use=use.begin();sym_use!=use.end();sym_use++)
	{
		symbol* sym=sym_use->first;
		symbol::symbol_type typ=sym->get_sym_type();
		assert(typ!=symbol::CONST);
		std::vector<unsigned int>& uses=sym_use->second.get_numbers();
		for(std::vector<unsigned int>::iterator sym_use1=uses.begin();sym_use1!=uses.end();sym_use1++)
		{
			Quadcode* code=fblk->get_code(*sym_use1);
			Quadcode::Quad_type q_typ=code->get_quad_type();
			bool cal=!(q_typ==Quadcode::ENDF||q_typ==Quadcode::ENDP||q_typ==Quadcode::FUNC||q_typ==Quadcode::MAIN||q_typ==Quadcode::ENDM);
			if(cal)
			//calculate the reach definition of this code, the reach definition contains multiple symbols, & to get the 
			//definition of symbol we need. Possible multiple definition is active at this code point, then every chain indexed
			//by definition point need to have the use point we get 
			{	
				std::vector<unsigned int> sym_def=(r_def->get_in_r_def(code)&def[sym]).get_numbers();	//reach-> get_in(code)

				for(unsigned int j=0;j<sym_def.size();j++)
				{
					chain_map[sym_def[j]]+=*sym_use1;	//definition ..<use .....>
				}
			}
		}
	}
	//then merge it
	merge(chain_map);
}
void Webs::mapping()	//map all the symbol
{
	std::vector<Quadcode*> codes=fblk->get_all_ins();
	for(int i=0;i<codes.size();i++)
	{
		Quadcode* code=codes[i];
		if(code->get_quad_type()==Quadcode::SIND)	//the symbol of the array index should also be mapped
		{
			symbol* sym=code->get_src_arg()[0];
			symbol* index=code->get_src_arg()[1];
			assert(sym->get_sym_type()==symbol::ARRAY);
			sym->set_array_index(index,false,code->get_index());
			continue;
		}
		std::vector<symbol*> src_args=code->get_src_arg();
		for(int j=0;j<src_args.size();j++)	
		{
			mapping(src_args[j],code->get_index(),false);
			if(src_args[j]->get_sym_type()==symbol::ARRAY)	
			{	//if a code contains a use of an array then we also used the index of the array
				mapping(src_args[j]->get_array_symbol(true),code->get_index(),false);
			}
		}
		symbol* obj=code->get_obj_arg();
		if(obj!=NULL)
		{
			mapping(obj,code->get_index(),true);
			if(obj->get_sym_type()==symbol::ARRAY)
			{	//if a code defines an array, then we also used the index of the array, be sure not defined the index of the array
				mapping(obj->get_array_symbol(true),code->get_index(),false);
			}
		}
	}
	//out put the web information
	std::ofstream file("t_web.txt",std::ios::app);
	file<<"---------------------------------------------"<<std::endl;
	for(std::multimap<symbol*, web*>::iterator it=symbol_web_map.begin();it!=symbol_web_map.end();it++)
	{
		file<<*(it->first);	//out put symbol
		web* _web=it->second;
		for(std::vector<chain>::iterator it=_web->begin();it!=_web->end();it++)
		{
			file<<it->first<<std::endl;
			file<<it->second;
		}
	}
	file.close();
}
bool Webs::is_consisent_define(symbol* sym, unsigned int line, web* _web)
{
	return sym_dweb_map.find(sym_line(sym,line))==sym_dweb_map.end()||sym_dweb_map[sym_line(sym,line)]==web_index_map[_web];
}
//a symbol in a line contains only in one net, ie, the pair<symbol, line> do not appear
// in symbol_line_use_net_map, or it only has one value in symbol_line_use_net_map according to it
bool Webs::is_consisent_use(symbol* sym, unsigned int line, web* _web)
{
	return sym_uweb_map.find(sym_line(sym,line))==sym_uweb_map.end()||sym_uweb_map[sym_line(sym,line)]==web_index_map[_web];
}
void Webs::mapping(symbol* sym, unsigned int line, bool is_obj)	//map one symbol to the nets
{	//one symbol may have multiple webs
	std::multimap<symbol*, web*>::iterator upper=symbol_web_map.upper_bound(sym);
	std::multimap<symbol*, web*>::iterator lower=symbol_web_map.lower_bound(sym);
//	std::cout<<*sym;
//	if(upper!=lower) std::cout<<*sym;
	while(upper!=lower)
	{
		web* chains = lower->second;
		bool flag = false;
		for(int i=0;i<chains->size();i++)
		{
			if(!is_obj&&(*chains)[i].second.find(line))		//not object, must find the use line
			{
				assert(is_consisent_use(sym,line,chains));		
				sym_uweb_map[sym_line(sym,line)]=web_index_map[chains];
				flag=true;
				break;
			}
			else if(is_obj&&(*chains)[i].first==line)	//object, the define line must equal to the line in argument
			{
				assert(is_consisent_define(sym,line,chains));
				sym_dweb_map[sym_line(sym,line)]=web_index_map[chains];
				flag=true;
				break;
			}
		}
	
		if(flag) break;
		lower++;
	}
}
void Webs::merge(chains& chains_map)
{
	std::vector<bit_set> web_use;
	for(chains::iterator chain=chains_map.begin();chain!=chains_map.end();chain++)	//iterate over all chains
	{
		unsigned int chain_def=chain->first;	//indexed by chain definition point
		bit_set chain_uses=chain->second;
		symbol* obj=fblk->get_code(chain_def)->get_obj_arg();
		bool flag=true;
		for(int i=0;i<web_use.size();i++)	//the length of web_use is equal to the length of web_vec
		{
			if(web_vec[i].first!=obj) continue; //different obj_args, shouldn't merge, continue
			if(!(web_use[i]&chain_uses).empty())	//if two chains contains same use point, then merge it
			{
				web_vec[i].second->push_back(*chain);
				web_use[i]+=chain_uses;
				flag=false;		//two chains have been merged
				break;
			}
		}
		// no chains have been merged, we need to allocate one new web
		if(flag)
		{
			web* new_web=new web();
			new_web->push_back(*chain);
			web_index_map[new_web]=web_vec.size();
			web_use.push_back(chain_uses);

			std::pair<symbol*, web*> new_pair(obj,new_web);
			web_vec.push_back(new_pair);
			symbol_web_map.insert(new_pair);
		}
	}
}
//[12/7/2013 lenovo]-----------------------------------------------------------------------------------------------------------------------------------------------------------
//live variable analysis
Live_Var::Live_Var(FBlock* _fblk, std::map<symbol*, bit_set>& _use, std::map<symbol*, bit_set>& _define, Webs* _web):fblk(_fblk),use(_use),define(_define),webs(_web)
{
	_analyse();
	elimiate();
}
//get in live variable of a given code
bit_set Live_Var::get_in(Quadcode* code) const
{
	if(code_in.find(code)!=code_in.end())
	{
		return code_in.find(code)->second;
	}
	else return bit_set();
}
//get out live variable of a given code
bit_set Live_Var::get_out(Quadcode* code) const
{
	if(code_out.find(code)!=code_out.end())
	{
		return code_out.find(code)->second;
	}
	else return bit_set();
}
void Live_Var::_analyse()
{
	std::map<BBlock*, bit_set> in_map;
	std::map<BBlock*, bit_set> out_map;
	std::map<BBlock*, bit_set> def_map;
	std::map<BBlock*, bit_set> use_map;
	std::map<Quadcode*, bit_set> code_def_map;
	std::map<Quadcode*, bit_set> code_use_map;

	//get init use and define set
	std::vector<BBlock*> bblk=fblk->get_basic_blocks();
	for(int i=0;i<bblk.size();i++)
	{
		BBlock *_blk=bblk[i];
		bit_set def;
		bit_set use;
		std::set<unsigned int> defined;
		std::vector<Quadcode*> ins = _blk->get_all_ins();
		for(int j=0;j<ins.size();j++)
		{
			Quadcode* code=ins[j];
			if(code->get_quad_type()==Quadcode::SIND)	//should also contain the array index of an array
			{
				symbol* src=code->get_src_arg()[0];
				symbol* index=code->get_src_arg()[1];
				assert(src->get_sym_type()==symbol::ARRAY);
				src->set_array_index(index,false,code->get_index());
				continue;
			}
			std::vector<symbol*> src_arg=code->get_src_arg();
			Quadcode::Quad_type q_typ=code->get_quad_type();
			bool cal=!(q_typ==Quadcode::ENDF||q_typ==Quadcode::ENDP||q_typ==Quadcode::FUNC||q_typ==Quadcode::MAIN||q_typ==Quadcode::ENDM);
			if(cal)
			{
				for(int k=0;k<src_arg.size();k++)
				{
					define_or_use(src_arg[k], code, defined, use, code_use_map, true);
					if(src_arg[k]->get_sym_type()==symbol::ARRAY)	//if we used an array, then we also used the index
						define_or_use(src_arg[k]->get_array_symbol(true),code,defined,use,code_use_map,true);
				}
				symbol* obj=code->get_obj_arg();
				if(obj!=NULL)
				{
					define_or_use(obj, code, defined, def, code_def_map, false);
					if(obj->get_sym_type()==symbol::ARRAY)	//if we defined an array, we also used its index, make sure it is use
						define_or_use(obj->get_array_symbol(true),code,defined,use,code_use_map,true);
				}
			}
		}

		def_map[_blk]=def;
		use_map[_blk]=use;
//		in_map[_blk]=use-def;//should be good
	}
// 	std::map<BBlock*, bit_set>::iterator it1=use_map.begin();
//  	for(std::map<BBlock*, bit_set>::iterator it=def_map.begin();it!=def_map.end();it++)
// 	{
//  		std::cout<<*it->first;
//  		std::cout<<it->second;
// 		std::cout<<"use"<<std::endl;
// 		std::cout<<it1->second;
// 		it1++;
//  	}

	//iterate until unchanged
	while(1)
	{
		bool flag=false;
		std::vector<BBlock*> bblks=fblk->get_basic_blocks();
		for(int i=0;i<bblks.size();i++)
		{
			BBlock* basic_blk=bblks[i];
			bit_set out, temp,temp1;
			for(int j=0;j<basic_blk->exit.size();j++)
			{
				out+=in_map[basic_blk->exit[j]];	//out=sum of in_map[bblk->exit(j)]
			}
			temp=out;
			std::vector<Quadcode*> ins=basic_blk->get_all_ins();	//refine to code level
			for(int k=ins.size();k>0;k--)	//the live variable is calculated in reverse order. be sure
			{
				code_out[ins[k-1]]=temp;
				temp=code_in[ins[k-1]]=code_use_map[ins[k-1]]+(temp-code_def_map[ins[k-1]]); //q_in=q_use+(q_out-q_def)
			}
			temp1=in_map[basic_blk];
			out_map[basic_blk]=out;
			in_map[basic_blk]+=use_map[basic_blk]+(out-def_map[basic_blk]);	//in=use+(out-def)
			if(temp1!=in_map[basic_blk])
				flag=true;
		}
		if(!flag) break;
	}
}
void Live_Var::define_or_use(symbol* sym, Quadcode* code, std::set<unsigned int>& defined , 
							 bit_set& _set, std::map<Quadcode*, bit_set>& def_use, bool _use)
{
	assert(sym!=NULL);
	unsigned int index;
	unsigned int line = code->get_index();
//	if(!_use) std::cout<<*sym;
	if(webs->get_net_index(sym,line,_use,index))
	{
		if(defined.count(index)==0)
		{
			defined.insert(index);
			_set+=index;
		}
		def_use[code]+=index;
	}
}
//elimiate those code sequence which the variable it defines do not flow out of the code(will not be used after)
void Live_Var::elimiate()
{
	std::vector<Quadcode*> ins=fblk->get_all_ins();
	for(int i=0;i<ins.size();i++)
	{
		SymbolTable* curstbl;
		symbol* obj_args=ins[i]->get_obj_arg();
		Quadcode::Quad_type q_typ=ins[i]->get_quad_type();
		if(q_typ==Quadcode::FUNC) curstbl=ins[i]->get_obj_arg()->get_cur_scope();
//		if(obj_args!=NULL) if(obj_args->get_scope()!=curstbl) std::cout<<*obj_args;
		bool cal=!(q_typ==Quadcode::ENDF||q_typ==Quadcode::ENDP||q_typ==Quadcode::FUNC||q_typ==Quadcode::MAIN||q_typ==Quadcode::ENDM);
		if(cal&&obj_args!=NULL)	//the code defined a variable
		{
			//all global and reference will be considered as live variable whatsoever
			symbol::symbol_type s_typ=obj_args->get_sym_type();
			if(obj_args->get_scope()->get_name()=="@global"||s_typ==symbol::REFSYM||s_typ==symbol::ARRAY
				||s_typ==symbol::FUNCTION||obj_args->get_scope()!=curstbl) continue;
			unsigned int index;
			if(webs->get_net_index(obj_args,ins[i]->get_index(),false,index))
			{
				//if a variable is defined, but not coming out of this instruction,
				//ie, this definition is not used before it was redefined, so this instruction is useless
				if(!code_out[ins[i]].find(index))
					ins[i]->disable();
			}
			else	//the src arg is not in the web map
			{
				ins[i]->disable();
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//conflict graph, used for global variable allocation
C_graph::C_graph(FBlock* _blk, std::map<symbol*, bit_set>& _use, std::map<symbol*, bit_set>& _define, Reach_def* _r_def
				 , Webs* _webs , Live_Var* _l_var):fblk(_blk), use(_use), define(_define), r_def(_r_def), webs(_webs), l_var(_l_var)
{
	_get_reference();	//find the reference
	_bulid();
	
	std::vector<Quadcode*> ins=fblk->get_all_ins();	//find the current symbol table
	for(int i0=0;i0<ins.size();i0++)
	{
		if(ins[i0]->get_quad_type()==Quadcode::FUNC) { cur_stbl=ins[i0]->get_obj_arg()->get_cur_scope(); break;}
	}
	//out put the current code

	std::ofstream file("t_graph.txt",std::ios::app);
	std::vector<sym_web>& cc=webs->get_web_vec();
	for(int i=0;i<cc.size();i++)
	{
		for(int j=0;j<cc.size();j++)
		{
			file<<adjecent_graph[i][j]<<"\t";
		}
		file<<std::endl;
	}
	for(int i1=0;i1<cc.size();i1++)
	{
		file<<*(cc[i1].first);
		file<<"referentce "<<refs[cc[i1].second]<<std::endl;
		for(int k=0;k<cc[i1].second->size();k++)
		{
			std::vector<chain>* chains=cc[i1].second;
			file<<(*chains)[k].first;
			file<<(*chains)[k].second;
		}
	}
	file.close();
}
Webs* C_graph::get_Webs() const
{
	return webs;
}
void C_graph::_bulid()
{
//	pre_scan();
	std::vector<sym_web>& chains=webs->get_web_vec();	//sym_web::=<symbol*, web*>
// 	for(std::vector<sym_web>::iterator it=chains.begin();it!=chains.end();)
// 	{
// 		symbol* sym=it->first;
// 		if(refered_symbol.find(sym)!=refered_symbol.end())	//this chain was referred to
// 			chains.erase(it);
// 		else it++;
// 	}
	adjecent_graph.resize(chains.size());	//equal to the size of all web, a web is one basic unit for allocating regs
	for(int i=0;i<chains.size();i++)	//resize
	{
		adjecent_graph[i].resize(chains.size());
	}
	for(int j=0;j<chains.size();j++)
	{
		web* web1=chains[j].second;
		for(int k=j+1;k<chains.size();k++)
		{
			web* web2=chains[k].second;
			bool confict=false;

			//one web is in conflict with other if and only if one web is active in the definition point of another web
			for(std::vector<chain>::iterator chain1=web1->begin();chain1!=web1->end();chain1++)
			{
				for(std::vector<chain>::iterator chain2=web2->begin();chain2!=web2->end();chain2++)
				{
					//a variable is active in another's definition point
					if(is_conflict(fblk->get_code(chain2->first), j, *chain1)||is_conflict(fblk->get_code(chain1->first),k,*chain2))
						confict=true;
					break;
				}
				if(confict) break;	//find the conflict point, break;
			}
			if(confict)
			{
				adjecent_graph[j][k]=adjecent_graph[k][j]=true;
				continue;
			}
		}
	}
}
//warning: the reference here contains the symbol even invalidated in the previous procedure
void C_graph::_get_reference()
{
	std::vector<BBlock*> bblks=fblk->get_basic_blocks();
	for(int i=0;i<bblks.size();i++)
	{
		std::vector<Quadcode*> ins=bblks[i]->get_all_ins();
		for(int j=0;j<ins.size();j++)
		{
			std::vector<symbol*> src_args=ins[j]->get_src_arg();
			symbol* obj_arg=ins[j]->get_obj_arg();

			for(int k=0;k<src_args.size();k++)
			{
				web* web1=webs->get_net(src_args[k], ins[j]->get_index(),true);
				refs[web1]++;
			}
			if(obj_arg!=NULL)
			{
				web* web2=webs->get_net(obj_arg, ins[j]->get_index(),false);
				refs[web2]++;
			}
		}
	}
}
//whether a chain and a web is in conflict
bool C_graph::is_conflict(Quadcode* code, unsigned int web1, chain chain1)
{
	//the definition of chain1 must reach the definition point of chain2
	//also. the web(symbol) must be active in the code point of the program
	if(r_def->get_in_r_def(code).find(chain1.first)&&l_var->get_in(code).find(web1))
	{
		return true;
	}
	else return false;
}
//find a node in matrix with degree less than the required register number, used is the node move out of the graph

void C_graph::coloring(unsigned int reg_num)
{
	std::vector<sym_web> nodes=webs->get_web_vec();
	std::vector<bool> used(nodes.size());	//vector with node use regs
	std::vector<bool> not_use_reg(nodes.size());	//vector with node do not use regs
	std::vector<unsigned int> close_nodes;

	while(close_nodes.size()<nodes.size())	//move out nodes until only one node left
	{
		unsigned int index=find_degree_less_than(reg_num, used);	//find a index witch have degree less than reg_num
		if(index<adjecent_graph.size())		//the index was found
		{
			used[index]=true;	//moved out form the graph
			symbol::symbol_type typ=nodes[index].first->get_sym_type();
			//the symbol is function, or reference and array will not be assigned a global regs, also the outer variables will not be assigned
			if(typ==symbol::FUNCTION||typ==symbol::ARRAY||nodes[index].first->get_scope()!=cur_stbl)
				not_use_reg[index]=true;
			else
				not_use_reg[index]=false;
			close_nodes.push_back(index);	//keep the moved out nodes in an array
		}
		else	//the index was not found
		{
			unsigned int no_reg=choose_no_reg(nodes, used);		//choose one node do not have a global regs
			used[no_reg]=true;
			not_use_reg[no_reg]=true;
			close_nodes.push_back(no_reg);
		}
	}
	//color map<web*, unsigned int>, the color will be allocated bt web, basic units
	//out this , close_nodes.size()==nodes.size()
	for(int i=close_nodes.size();i>0;i--) //move the nodes back and allocate color
	{
		if(not_use_reg[close_nodes[i-1]])	//do not use global regs, color num=0
		{
			color_map[nodes[close_nodes[i-1]].second]=0;
			used[close_nodes[i-1]]=false;	//not use reg
		}
		else	//allocate it with  a color(regs)
		{
			color_map[nodes[close_nodes[i-1]].second]=allocate_color(close_nodes[i-1],reg_num, nodes,used);
		}
	}
}
//find a node with degree lower than the given reg_number
unsigned int C_graph::find_degree_less_than(unsigned int reg_num, bool_vec& used)
{
	for(int i=0;i<adjecent_graph.size();i++)
	{
		if(used[i]) continue;
		int num=0;
		for(int j=0;j<adjecent_graph.size();j++)
		{
			if(adjecent_graph[i][j]&&!used[j]) num++;
		}
		if(num<reg_num) return i;
	}
	return adjecent_graph.size();
}
//choose a node which do not have a register
unsigned int C_graph::choose_no_reg(std::vector<sym_web>& nodes, std::vector<bool>& used)
{
	unsigned int times, index;
	times=INT_MAX;
	bool find=false;
	for(int i=0;i<used.size();i++)
	{
		if(used[i]) continue;
		symbol::symbol_type typ=nodes[i].first->get_sym_type();
		if(typ==symbol::ARRAY||typ==symbol::REFSYM||typ==symbol::FUNCTION||nodes[i].first->get_scope()!=cur_stbl) return i;//return array and reference, also outer variables,since they do not use the global regs
		unsigned int ref_time=refs[nodes[i].second];	//the reference was calculated by web, basic allocation unit
		if(!find||times>ref_time)		//get one with minimum reference times
		{
			times=ref_time;
			find=true;
			index=i;
		}
	}
	return index;
}
//allocate color for one web
unsigned int C_graph::allocate_color(unsigned int index, unsigned int reg_num, std::vector<sym_web>& nodes, std::vector<bool>&used)
{	//total color reg_num+1,  because start with 1
	std::vector<bool> color(reg_num+1,true); // the available color array, the index of this array means the according color num
	for(int i=0;i<adjecent_graph[index].size();i++)//if the other index is put into the graph and is in conflict
	{
		if(used[i]&&adjecent_graph[index][i])
			color[color_map[nodes[i].second]]=false;	//the color in these position is not available
	}
	for(int j=1;j<color.size();j++)	//i is the return color, ranged from 1 to reg_num
	{
		if(color[j]) return j;	//return the first color
	}
	assert(0);
	return -1;
}
//color the conflict graph, this function will be called during the reg_allocation procedure
std::map<web*, unsigned int> C_graph::get_color(unsigned int reg_num)
{
	if(color_map.size()!=0)
		return color_map;
	else
	{
		coloring(reg_num);
		return color_map;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//data stream global class. control
Data_stream::Data_stream(InsCtrl* _ins):ins(_ins)
{
	_analyse();

//	ins->reorder_all_ins();
	std::ofstream file("block_s.txt",std::ios::out);
	std::vector<FBlock*> blo=ins->get_all_func_block();
	for(int i1=0;i1<blo.size();i1++)
	{
		file<<*blo[i1];
	}
	file.close();
}
//get the Live_var class of a given function block
Live_Var* Data_stream::get_live_variable(FBlock* fblk) const
{
	if(l_var.find(fblk)!=l_var.end())
	{
		return l_var.find(fblk)->second;
	}
	else {assert(0); return NULL;}
}
//get the Conflict graph class of a given function block
C_graph* Data_stream::get_conflict_graph(FBlock* fblk) const
{
	if(c_graph.find(fblk)!=c_graph.end())
	{
		return c_graph.find(fblk)->second;
	}
	else {assert(0); return NULL;}
}
// void Data_stream::pre_scan(FBlock* blk)
// {
// 	std::set<symbol*> refereed_sym;
// 	std::vector<Quadcode*> ins=blk->get_all_ins();
// 	for(int i=0;i<ins.size();i++)
// 	{
// 		if(ins[i]->get_quad_type()==Quadcode::SREF)
// 		{
// 			refereed_sym.insert(ins[i]->get_src_arg()[0]);
// 		}
// 	}
// 	for(std::map<symbol*, bit_set>::iterator it=definition.begin();it!=definition.end();)
// 	{
// 		if(refereed_sym.find(it->first)!=refereed_sym.end())
// 			definition.erase(it);
// 		else it++;
// 	}
// 	for(std::map<symbol*, bit_set>::iterator it1=use.begin();it1!=use.end();)
// 	{
// 		if(refereed_sym.find(it->first)!=refereed_sym.end())
// 			use.erase(it);
// 		else it++;
// 	}
// }
void Data_stream::_analyse()
{	//first init the define and use set
	std::vector<Quadcode*> codes=ins->get_all_ins();
	for(unsigned int i=0;i<codes.size();i++)
	{
		Quadcode* code=codes[i];
		SymbolTable* cur_stbl;
		if(code->get_quad_type()==Quadcode::FUNC)
			cur_stbl=code->get_obj_arg()->get_cur_scope();
		if(code->get_quad_type()==Quadcode::SIND)
		{
			symbol* obj=code->get_src_arg()[0];
			symbol* index=code->get_src_arg()[1];
			assert(obj->get_sym_type()==symbol::ARRAY);
			obj->set_array_index(index,false,code->get_index());
			continue;
		}
		symbol* obj=code->get_obj_arg();
		Quadcode::Quad_type q_typ=code->get_quad_type();
		symbol::symbol_type typ;
		if(obj!=NULL) typ=obj->get_sym_type();
		bool cal=!(q_typ==Quadcode::ENDF||q_typ==Quadcode::ENDP||q_typ==Quadcode::FUNC||q_typ==Quadcode::MAIN||q_typ==Quadcode::ENDM);

		if(obj!=NULL&&cal&&(typ==symbol::VARIABLE||typ==symbol::ARGUMENT||typ==symbol::ARRAY)
			&&obj->get_scope()==cur_stbl)
		{definition[obj]+=i;}//init definition

		if(obj!=NULL)
		{
			if(obj->get_sym_type()==symbol::ARRAY)
			{
				symbol* sym=obj->get_array_symbol(true);	//the index symbol of an array is always an local temp variable
	//			std::cout<<*code;
				use[sym]+=i;
			}
		}

		std::vector<symbol*>& src_args=code->get_src_arg();
		for(int j=0;j<src_args.size();j++)
		{
			symbol::symbol_type typ1=src_args[j]->get_sym_type();
			if((typ1==symbol::VARIABLE||typ1==symbol::ARGUMENT||typ1==symbol::ARRAY)
				&&src_args[j]->get_scope()->get_name()!="@global"&&src_args[j]->get_scope()==cur_stbl)
			{	use[src_args[j]]+=i; }
			if(src_args[j]->get_sym_type()==symbol::ARRAY)
			{
				symbol* sym=src_args[j]->get_array_symbol(true);
				use[sym]+=i;
			}
		}
	}
// 	for(std::map<symbol*, bit_set>::iterator it=definition.begin();it!=definition.end();it++)
// 	{
// 		std::cout<<*(it->first);
// 		std::cout<<it->second;
// 	}
	std::vector<FBlock*> fblks=ins->get_all_func_block();
	for(int i1=0;i1<fblks.size();i1++)
	{
		FBlock* fbl=fblks[i1];
		Reach_def *r_def=new Reach_def(fbl,use,definition);
		Webs* web=new Webs(fbl,use,definition,r_def);
		Live_Var* live_var=new Live_Var(fbl,use,definition,web);
		C_graph* g_graph=new C_graph(fbl,use,definition,r_def,web,live_var);
		this->c_graph[fblks[i1]]=g_graph;
		this->l_var[fblks[i1]]=live_var;
	}
}