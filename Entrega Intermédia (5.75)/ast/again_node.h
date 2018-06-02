#ifndef __GR8_AGAINNODE_H__
#define __GR8_AGAINNODE_H__

namespace gr8 {

  /**
   * Class for describing "again" (or continue) nodes.
   */
  class again_node: public cdk::basic_node {
    int _cycleNumber;

  public:
    inline again_node(int lineno) :
        cdk::basic_node(lineno) {
    }

     inline again_node(int lineno, int cycleNumber) :
        cdk::basic_node(lineno), _cycleNumber(cycleNumber) {
    }

    inline int cycleNumber(){
      return _cycleNumber;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_again_node(this, level);
    }

  };

} // gr8

#endif