// $Id: post_node.h,v 1.2 2018/04/16 16:17:04 ist424785 Exp $ -*- c++ -*-
#ifndef __GR8_POSTNODE_H__
#define __GR8_POSTNODE_H__

#include <cdk/ast/expression_node.h>

namespace gr8 {

  /**
   * Class for describing post (print with a \n) nodes.
   */
  class post_node: public cdk::basic_node {
    cdk::expression_node *_argument;

  public:
    inline post_node(int lineno, cdk::expression_node *argument) :
        cdk::basic_node(lineno), _argument(argument) {
    }

  public:
    inline cdk::expression_node *argument() {
      return _argument;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_post_node(this, level);
    }

  };

} // gr8

#endif
