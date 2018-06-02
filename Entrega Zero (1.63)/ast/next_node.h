#ifndef __GR8_NEXTNODE_H__
#define __GR8_NEXTNODE_H__

namespace gr8 {

  /**
   * Class for describing "next" (or continue) nodes.
   */
  class next_node: public cdk::basic_node {
    cdk::lvalue_node *_argument;

  public:
    inline next_node(int lineno) :
        cdk::basic_node(lineno) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_next_node(this, level);
    }

  };

} // gr8

#endif