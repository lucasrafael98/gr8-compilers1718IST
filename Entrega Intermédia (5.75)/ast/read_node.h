// $Id: read_node.h,v 1.2 2018/03/21 15:45:16 ist424829 Exp $ -*- c++ -*-
#ifndef __GR8_READNODE_H__
#define __GR8_READNODE_H__

#include <cdk/ast/lvalue_node.h>

namespace gr8 {

  /**
   * Class for describing read nodes.
   */
  class read_node: public cdk::expression_node {
    cdk::lvalue_node *_argument;

  public:
    inline read_node(int lineno) :
        cdk::expression_node(lineno) {
    }

  public:
    inline cdk::lvalue_node *argument() {
      return _argument;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_read_node(this, level);
    }

  };

} // gr8

#endif
