#include <string>
#include <iostream>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void gr8::xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
void gr8::xml_writer::do_data_node(cdk::data_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<data_node size='" << node->size() << "'>" << std::endl;
  closeTag(node, lvl);
}
void gr8::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  process_literal(node, lvl);
}
void gr8::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}
void gr8::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++){
    node->node(i)->accept(this, lvl + 2);}
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void gr8::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_unary_expression(cdk::unary_expression_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_binary_expression(cdk::binary_expression_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
/* The commented method bodies below are commented out because these nodes don't exist in gr8. */
void gr8::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  //do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  //do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  //do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}  

//---------------------------------------------------------------------------

void gr8::xml_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
}

void gr8::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);

  node->lvalue()->accept(this, lvl);
  reset_new_symbol();

  node->rvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_evaluation_node(gr8::evaluation_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_tweet_node(gr8::tweet_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_post_node(gr8::post_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_read_node(gr8::read_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_if_node(gr8::if_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_if_else_node(gr8::if_else_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_sweep_node(gr8::sweep_node * const node, int lvl) {
  //TODO needs to be improved for next delivery
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);
  openTag("init", lvl + 2);
  node->init()->accept(this, lvl + 4);
  closeTag("init", lvl + 2);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("increment", lvl + 2);
  node->increment()->accept(this, lvl + 4);
  closeTag("increment", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_again_node(gr8::again_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  os() << std::string(lvl, ' ') << "<again_node cycleNumber='" << node->cycleNumber() << "'>" << std::endl;
  closeTag(node, lvl);
}

void gr8::xml_writer::do_stop_node(gr8::stop_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  os() << std::string(lvl, ' ') << "<stop_node cycleNumber='" << node->cycleNumber() << "'>" << std::endl;
  closeTag(node, lvl);
}

void gr8::xml_writer::do_return_node(gr8::return_node * const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);
  openTag("value", lvl);
  if(!(node->argument() == 0)){
    node->argument()->accept(this, lvl + 2);
  }
  closeTag("value", lvl);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_block_node(gr8::block_node * const node, int lvl){
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);
  openTag("decls", lvl + 2);
  node->decls()->accept(this, lvl + 4);
  closeTag("decls", lvl + 2);
  if(!node->instructs() == 0){
    openTag("instructions", lvl + 2);
    node->instructs()->accept(this, lvl);
    closeTag("instructions", lvl + 2);
  };
  if(!node->ret() == 0){
    openTag("return", lvl + 2);
    node->ret()->accept(this, lvl);
    closeTag("return", lvl + 2);
  }
  closeTag(node, lvl);
}

void gr8::xml_writer::do_indexing_node(gr8::indexing_node * const node, int lvl){
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);
  openTag("ptr", lvl + 2);
  node->ptr()->accept(this, lvl + 4);
  closeTag("ptr", lvl + 2);
  openTag("index", lvl + 2);
  node->index()->accept(this, lvl + 4);
  closeTag("index", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_var_decl_node(gr8::var_decl_node * const node, int lvl){
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  os() << std::string(lvl, ' ') << "<var_decl_node id='" << *(node->id()) << "'>" << std::endl;
  openTag("value", lvl);
  if(!node->argument() == 0){
    node->argument()->accept(this, lvl + 2);
  }
  closeTag("value", lvl);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_func_decl_node(gr8::func_decl_node * const node, int lvl){
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  os() << std::string(lvl, ' ') << "<func_decl_node id='" << *(node->id()) << "'>" << std::endl;
  openTag("functionArgs", lvl + 2);
  node->args()->accept(this, lvl + 4);
  closeTag("functionArgs", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_func_call_node(gr8::func_call_node * const node, int lvl){
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  os() << std::string(lvl, ' ') << "<func_call_node id='" << *(node->id()) << "'>" << std::endl;
  openTag("functionArgs", lvl + 2);
  node->args()->accept(this, lvl + 4);
  closeTag("functionArgs", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_func_def_node(gr8::func_def_node * const node, int lvl){
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  os() << std::string(lvl, ' ') << "<func_def_node id='" << *(node->id()) << "'>" << std::endl;
  openTag("functionArgs", lvl + 2);
  node->args()->accept(this, lvl + 4);
  closeTag("functionArgs", lvl + 2);
  openTag("body", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("body", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_identity_node(gr8::identity_node * const node, int lvl){
  do_unary_expression(node, lvl);
}

void gr8::xml_writer::do_address_node(gr8::address_node * const node, int lvl){
  //ASSERT_SAFE_EXPRESSIONS; commented because we don't want it to go to type checker!
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_alloc_node(gr8::alloc_node * const node, int lvl){
  do_unary_expression(node, lvl);
}

void gr8::xml_writer::do_null_ptr_node(gr8::null_ptr_node * const node, int lvl){
  openTag(node, lvl);
  closeTag(node, lvl);
}
