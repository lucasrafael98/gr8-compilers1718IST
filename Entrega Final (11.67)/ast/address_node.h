#ifndef __GR8_ADDRESSNODE_H__
#define __GR8_ADDRESSNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace gr8 {

  /**
   * Class for describing addressing nodes.
   */
  class address_node: public cdk::expression_node {
    cdk::lvalue_node *_lvalue;

  public:
    inline address_node(int lineno, cdk::lvalue_node *lvalue) :
        cdk::expression_node(lineno), _lvalue(lvalue) {
    }

  public:
    inline cdk::lvalue_node *argument() {
      return _lvalue;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_address_node(this, level);
    }

  };

} // gr8

#endif
