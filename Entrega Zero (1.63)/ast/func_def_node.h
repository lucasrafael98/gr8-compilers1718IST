#ifndef __GR8_FUNCDEFNODE_H__
#define __GR8_FUNCDEFNODE_H__

#include <string>

namespace gr8 {

  /**
   * Class for describing function definition nodes.
   */
  class func_def_node: public cdk::typed_node {
  	std::string _id;
    cdk::sequence_node *_args;
    gr8::block_node *_body;
    cdk::expression_node *_return;

  public:
    inline func_def_node(int lineno, std::string id, cdk::sequence_node *args, gr8::block_node *body, cdk::expression_node *retn) :
        cdk::typed_node(lineno), _id(id), _args(args), _body(body), _return(retn) {
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_func_def_node(this, level);
    }

  };

} // gr8

#endif