#include <string>
#include "targets/stack_counter.h"
#include <iostream>
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void gr8::stack_counter::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void gr8::stack_counter::do_nil_node(cdk::nil_node * const node, int lvl) {}
void gr8::stack_counter::do_data_node(cdk::data_node * const node, int lvl) {}
void gr8::stack_counter::do_double_node(cdk::double_node * const node, int lvl) {}
void gr8::stack_counter::do_not_node(cdk::not_node * const node, int lvl) {}
void gr8::stack_counter::do_and_node(cdk::and_node * const node, int lvl) {}
void gr8::stack_counter::do_or_node(cdk::or_node * const node, int lvl) {}
void gr8::stack_counter::do_integer_node(cdk::integer_node * const node, int lvl) {}
void gr8::stack_counter::do_string_node(cdk::string_node * const node, int lvl) {}
void gr8::stack_counter::do_neg_node(cdk::neg_node * const node, int lvl) {}
void gr8::stack_counter::do_add_node(cdk::add_node * const node, int lvl) {}
void gr8::stack_counter::do_sub_node(cdk::sub_node * const node, int lvl) {}
void gr8::stack_counter::do_mul_node(cdk::mul_node * const node, int lvl) {}
void gr8::stack_counter::do_div_node(cdk::div_node * const node, int lvl) {}
void gr8::stack_counter::do_mod_node(cdk::mod_node * const node, int lvl) {}
void gr8::stack_counter::do_lt_node(cdk::lt_node * const node, int lvl) {}
void gr8::stack_counter::do_gt_node(cdk::gt_node * const node, int lvl) {}
void gr8::stack_counter::do_eq_node(cdk::eq_node * const node, int lvl) {}
void gr8::stack_counter::do_le_node(cdk::le_node * const node, int lvl) {}
void gr8::stack_counter::do_ge_node(cdk::ge_node * const node, int lvl) {}
void gr8::stack_counter::do_ne_node(cdk::ne_node * const node, int lvl) {}
void gr8::stack_counter::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  std::string id = node->name();
  if(!_symtab.find_local(id) && _symtab.find(id)){
    _count += _symtab.find(id)->type()->size();
    _symtab.insert(_func + id, _symtab.find(id));
  }
}
void gr8::stack_counter::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  node->lvalue()->accept(this,lvl);
}
void gr8::stack_counter::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  node->lvalue()->accept(this, lvl);
}
void gr8::stack_counter::do_evaluation_node(gr8::evaluation_node * const node, int lvl) {}
void gr8::stack_counter::do_tweet_node(gr8::tweet_node * const node, int lvl) {}
void gr8::stack_counter::do_post_node(gr8::post_node * const node, int lvl) {}
void gr8::stack_counter::do_read_node(gr8::read_node * const node, int lvl) {}

void gr8::stack_counter::do_if_node(gr8::if_node * const node, int lvl) {
  node->block()->accept(this, lvl);
}

void gr8::stack_counter::do_if_else_node(gr8::if_else_node * const node, int lvl) {
  node->thenblock()->accept(this, lvl);
  node->elseblock()->accept(this, lvl);
}

void gr8::stack_counter::do_sweep_node(gr8::sweep_node * const node, int lvl) {
  node->block()->accept(this, lvl);
}

void gr8::stack_counter::do_again_node(gr8::again_node * const node, int lvl) {}
void gr8::stack_counter::do_return_node(gr8::return_node * const node, int lvl) {}
void gr8::stack_counter::do_stop_node(gr8::stop_node * const node, int lvl) {}

void gr8::stack_counter::do_block_node(gr8::block_node * const node, int lvl){
  node->decls()->accept(this, lvl);
  if(node->instructs())
    node->instructs()->accept(this, lvl);
}

void gr8::stack_counter::do_indexing_node(gr8::indexing_node * const node, int lvl){}
void gr8::stack_counter::do_var_decl_node(gr8::var_decl_node * const node, int lvl){
  _count += node->type()->size();
}

void gr8::stack_counter::do_func_decl_node(gr8::func_decl_node * const node, int lvl){}
void gr8::stack_counter::do_func_call_node(gr8::func_call_node * const node, int lvl){}

void gr8::stack_counter::do_func_def_node(gr8::func_def_node * const node, int lvl){
  node->block()->accept(this, lvl);
}

void gr8::stack_counter::do_identity_node(gr8::identity_node * const node, int lvl){}
void gr8::stack_counter::do_address_node(gr8::address_node * const node, int lvl){}
void gr8::stack_counter::do_alloc_node(gr8::alloc_node * const node, int lvl){}
void gr8::stack_counter::do_null_ptr_node(gr8::null_ptr_node * const node, int lvl){}