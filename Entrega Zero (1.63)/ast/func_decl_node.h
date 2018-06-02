#ifndef __GR8_FUNCDECLNODE_H__
#define __GR8_FUNCDECLNODE_H__

#include <string>

namespace gr8 {

  /**
   * Class for describing function declaration nodes.
   */
  class func_decl_node: public cdk::typed_node {
    bool _isUse;
    bool _isPublic;
  	std::string _id;
    cdk::sequence_node *_args;

  public:
    inline func_decl_node(int lineno, bool isUse, bool isPublic, std::string id, cdk::sequence_node *args) :
        cdk::typed_node(lineno), _isUse(isUse), _isPublic(isPublic), _id(id), _args(args) {
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_func_decl_node(this, level);
    }

  };

} // gr8

#endif