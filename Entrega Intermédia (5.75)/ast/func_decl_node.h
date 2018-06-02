#ifndef __GR8_FUNCDECLNODE_H__
#define __GR8_FUNCDECLNODE_H__

#include <string>

namespace gr8 {

  /**
   * Class for describing function declaration nodes.
   */
  class func_decl_node: public cdk::typed_node {
    bool _isPrivate;
  	std::string *_id;
    cdk::sequence_node *_args;

  public:
    inline func_decl_node(int lineno, bool isPrivate, std::string *id, cdk::sequence_node *args, basic_type *type) :
        cdk::typed_node(lineno), _isPrivate(isPrivate), _id(id), _args(args) {
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
      sp->do_func_decl_node(this, level);
    }

  };

} // gr8

#endif
