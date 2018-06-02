#ifndef __GR8_STOPNODE_H__
#define __GR8_STOPNODE_H__

namespace gr8 {

  /**
   * Class for describing "stop" (or break) nodes.
   */
  class stop_node: public cdk::basic_node {
    int _cycleNumber;

  public:
    inline stop_node(int lineno) :
        cdk::basic_node(lineno), _cycleNumber(1) {
    }

    inline stop_node(int lineno, int cycleNumber) :
        cdk::basic_node(lineno), _cycleNumber(cycleNumber) {
    }

    inline int cycleNumber(){
      return _cycleNumber;
    }
    
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_stop_node(this, level);
    }

  };

} // gr8

#endif