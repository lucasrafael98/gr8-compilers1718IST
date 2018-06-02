#ifndef __GR8_BLOCKNODE_H__
#define __GR8_BLOCKNODE_H__

#include <cdk/ast/basic_node.h>

namespace gr8 {

  /**
   * Class for describing block nodes.
   */
  class block_node: public cdk::basic_node {
    cdk::sequence_node *_decls, *_instructs;
    cdk::basic_node *_return;

  public:
    inline block_node(int lineno, cdk::sequence_node *decls, cdk::sequence_node *instructs, cdk::basic_node *retn) :
        cdk::basic_node(lineno), _decls(decls), _instructs(instructs), _return(retn) {
    }

  public:
    inline cdk::sequence_node *decls() {
      return _decls;
    }

    inline cdk::sequence_node *instructs() {
      return _instructs;
    }

    inline cdk::basic_node *ret(){
      return _return;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_block_node(this, level);
    }

  };

} // gr8

#endif
