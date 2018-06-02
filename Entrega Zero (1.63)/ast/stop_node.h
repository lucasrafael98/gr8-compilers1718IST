#ifndef __GR8_STOPNODE_H__
#define __GR8_STOPNODE_H__

namespace gr8 {

  /**
   * Class for describing "stop" (or break) nodes.
   */
  class stop_node: public cdk::basic_node {
    cdk::lvalue_node *_argument;

  public:
    inline stop_node(int lineno) :
        cdk::basic_node(lineno) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_stop_node(this, level);
    }

  };

} // gr8

#endif