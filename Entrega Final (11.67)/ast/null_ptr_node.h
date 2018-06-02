#ifndef __GR8_NULLPTRNODE_H__
#define __GR8_NULLPTRNODE_H__

namespace gr8 {

  /**
   * Class for describing null pointer nodes.
   */
  class null_ptr_node: public cdk::expression_node {

  public:
    inline null_ptr_node(int lineno) :
        cdk::expression_node(lineno) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_null_ptr_node(this, level);
    }

  };

} // gr8

#endif