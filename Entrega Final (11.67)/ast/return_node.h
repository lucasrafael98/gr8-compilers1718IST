#ifndef __GR8_RETURNNODE_H__
#define __GR8_RETURNNODE_H__

namespace gr8 {

  /**
   * Class for describing return nodes.
   */
  class return_node: public cdk::basic_node {
    cdk::expression_node *_args;

  public:
    inline return_node(int lineno) :
        cdk::basic_node(lineno) {
    }

    inline return_node(int lineno, cdk::expression_node *args) :
        cdk::basic_node(lineno), _args(args) {
    }

    inline cdk::expression_node *argument() {
      return _args;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }

  };

} // gr8

#endif