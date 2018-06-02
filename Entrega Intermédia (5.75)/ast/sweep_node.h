// $Id: sweep_node.h,v 1.1 2018/04/14 16:37:56 ist424785 Exp $ -*- c++ -*-
#ifndef __GR8_SWEEPNODE_H__
#define __GR8_SWEEPNODE_H__

#include <cdk/ast/expression_node.h>

namespace gr8 {

  /**
   * Class for describing for-cycle nodes.
   */
  class sweep_node: public cdk::basic_node {
    cdk::lvalue_node *_value;
    cdk::expression_node *_init;
    cdk::expression_node *_condition;
    cdk::expression_node *_increment;

    cdk::basic_node *_block;

  public:
    inline sweep_node(int lineno, cdk::lvalue_node *val, cdk::expression_node *init, cdk::expression_node *condition, cdk::expression_node *increment, cdk::basic_node *block) :
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
      sp->do_sweep_node(this, level);
    }

  };

} // gr8

#endif
