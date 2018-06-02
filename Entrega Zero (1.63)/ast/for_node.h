// $Id: for_node.h,v 1.4 2018/03/22 19:19:52 ist424829 Exp $ -*- c++ -*-
#ifndef __GR8_FORNODE_H__
#define __GR8_FORNODE_H__

#include <cdk/ast/expression_node.h>

namespace gr8 {

  /**
   * Class for describing for-cycle nodes.
   */
  class for_node: public cdk::basic_node {
    cdk::lvalue_node *_value;
    cdk::expression_node *_init;
    cdk::expression_node *_condition;
    cdk::expression_node *_increment;

    cdk::basic_node *_block;

  public:
    inline for_node(int lineno, cdk::lvalue_node *val, cdk::expression_node *init, cdk::expression_node *condition, cdk::expression_node *increment, cdk::basic_node *block) :
        basic_node(lineno), _value(val), _init(init), _condition(condition), _increment(increment), _block(block) {
    }

  public:
    inline cdk::expression_node *init() {
      return _init;
    }
    inline cdk::expression_node *condition() {
      return _condition;
    }
    inline cdk::expression_node *increment() {
      return _increment;
    }
    inline cdk::basic_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_for_node(this, level);
    }

  };

} // gr8

#endif
