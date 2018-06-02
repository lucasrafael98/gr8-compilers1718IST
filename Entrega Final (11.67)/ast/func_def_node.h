#ifndef __GR8_FUNCDEFNODE_H__
#define __GR8_FUNCDEFNODE_H__

#include <string>

namespace gr8 {

  /**
   * Class for describing function definition nodes.
   */
  class func_def_node: public cdk::typed_node {
    int _accessMode; // for determining whether it's local/global/extern (0/1/2).
  	std::string *_id;
    cdk::sequence_node *_args;
    gr8::block_node *_body;

  public:
    inline func_def_node(int lineno, int accessMode, std::string *id, cdk::sequence_node *args, gr8::block_node *body, basic_type *type) :
        cdk::typed_node(lineno), _accessMode(accessMode), _id(id), _args(args), _body(body){
            cdk::typed_node::type(type);
    }

  public:
    inline cdk::sequence_node *args() {
      return _args;
    }
    inline gr8::block_node *block() {
      return _body;
    }
    inline std::string *id(){
      return _id;
    }
    inline int accessMode(){
      return _accessMode;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_func_def_node(this, level);
    }

  };

} // gr8

#endif
