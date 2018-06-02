#ifndef __GR8_FUNCCALLNODE_H__
#define __GR8_FUNCCALLNODE_H__

#include <string>

namespace gr8 {

  /**
   * Class for describing function call nodes.
   */
  class func_call_node: public cdk::expression_node {
  	std::string *_id;
    cdk::sequence_node *_args;

  public:
    inline func_call_node(int lineno, std::string *id, cdk::sequence_node *args, basic_type *type) :
        cdk::expression_node(lineno), _id(id), _args(args) {
            cdk::typed_node::type(type);
    }

  public:
    inline cdk::sequence_node *args() {
      return _args;
    }
    inline std::string *id(){
      return _id;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_func_call_node(this, level);
    }

  };

} // gr8

#endif
