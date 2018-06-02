#ifndef __GR8_STACK_COUNTER_H__
#define __GR8_STACK_COUNTER_H__

#include <string>
#include <iostream>
#include <cdk/symbol_table.h>
#include "targets/basic_ast_visitor.h"
#include "targets/symbol.h"

namespace gr8{
	class stack_counter: public basic_ast_visitor{
		cdk::symbol_table<gr8::symbol> &_symtab;
		int _count = 0;
		std::string _func;

    public:
		  // do not edit these lines
		#define __IN_VISITOR_HEADER__
		#include "ast/visitor_decls.h"       // automatically generated
		#undef __IN_VISITOR_HEADER__
		  // do not edit these lines: end
    	inline int count(){
    		return _count;
    	}

    	stack_counter(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<gr8::symbol> &symtab, std::string func) :
        	basic_ast_visitor(compiler), _symtab(symtab), _func(func){
    	}
	
    	~stack_counter(){}
	};
}
#endif