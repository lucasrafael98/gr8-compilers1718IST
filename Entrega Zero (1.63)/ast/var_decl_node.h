#ifndef __GR8_VARDECLNODE_H__
#define __GR8_VARDECLNODE_H__

#include <string>

namespace gr8 {

  /**
   * Class for describing variable declaration nodes.
   */
  class var_decl_node: public cdk::typed_node {
  	bool _isPublic;
  	bool _isUse;
  	std::string _id;
  	cdk::expression_node *_value;

  public:
    inline var_decl_node(int lineno, bool isPublic, bool isUse, std::string id, cdk::expression_node *value) :
        cdk::typed_node(lineno), _isPublic(isPublic), _isUse(isUse), _id(id), _value(value) {
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_var_decl_node(this, level);
    }

  };

} // gr8

#endif