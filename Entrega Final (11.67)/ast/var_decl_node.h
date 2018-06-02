#ifndef __GR8_VARDECLNODE_H__
#define __GR8_VARDECLNODE_H__

#include <string>

namespace gr8 {

  /**
   * Class for describing variable declaration nodes.
   */
  class var_decl_node: public cdk::typed_node {
  	int _accessMode; // for determining whether it's local/global/extern (0/1/2).
  	std::string *_id;
  	cdk::expression_node *_value;

  public:
    inline var_decl_node(int lineno, int accessMode, std::string *id, cdk::expression_node *value, basic_type* type) :
        cdk::typed_node(lineno), _accessMode(accessMode), _id(id), _value(value){
	cdk::typed_node::type(type);
    }

  public:
    inline std::string *id(){
      return _id;
    }

    inline cdk::expression_node *argument() {
      return _value;
    }
    inline int accessMode(){
      return _accessMode;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_var_decl_node(this, level);
    }

  };

} // gr8

#endif
