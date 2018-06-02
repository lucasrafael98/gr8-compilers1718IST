#ifndef __GR8_MEMADDRESSNODE_H__
#define __GR8_MEMADDRESSNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace gr8 {

  /**
   * Class for describing addressing nodes.
   */
  class mem_address_node: public cdk::unary_expression_node {

  public:
    inline mem_address_node(int lineno, cdk::expression_node *arg) :
        cdk::unary_expression_node(lineno, arg) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_mem_address_node(this, level);
    }

  };

} // gr8

#endif