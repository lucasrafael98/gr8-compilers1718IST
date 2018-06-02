#ifndef __GR8_IDENTITY_H__
#define __GR8_IDENTITY_H__

#include <cdk/ast/unary_expression_node.h>

namespace gr8 {

  /**
   * Class for describing identity operator nodes.
   */
  class identity_node: public cdk::unary_expression_node {
  public:
    identity_node(int lineno, expression_node *arg) :
        unary_expression_node(lineno, arg) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identity_node(this, level);
    }

  };

} // gr8

#endif