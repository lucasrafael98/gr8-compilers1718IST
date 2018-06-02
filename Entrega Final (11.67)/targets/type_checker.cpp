#include <string>
#include "targets/type_checker.h"
#include <iostream>
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void gr8::type_checker::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++){
    node->node(i)->accept(this, lvl);
  } 
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void gr8::type_checker::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void gr8::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}
void gr8::type_checker::do_not_node(cdk::not_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}
void gr8::type_checker::do_and_node(cdk::and_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void gr8::type_checker::do_or_node(cdk::or_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

//---------------------------------------------------------------------------

void gr8::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in argument of unary expression");

  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT && node->argument()->type()->name() != basic_type::TYPE_DOUBLE) throw std::string("wrong type in argument of unary expression");

  node->type(node->argument()->type());
}

//---------------------------------------------------------------------------

void gr8::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_UNSPEC) node->left()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->left()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_UNSPEC) node->right()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->right()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in right argument of binary expression");

  // in Simple, expressions were always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_UNSPEC) node->left()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->left()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_DOUBLE && 
    node->left()->type()->name() != basic_type::TYPE_POINTER) 
    throw std::string("wrong type in left argument of add expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_UNSPEC) node->right()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->right()->type()->name() != basic_type::TYPE_INT && node->right()->type()->name() != basic_type::TYPE_DOUBLE && 
    node->right()->type()->name() != basic_type::TYPE_POINTER) 
    throw std::string("wrong type in right argument of add expression");

  if((node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_INT) ||
      (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT) ||
      (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE) ||
      (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT))
    node->type(node->left()->type());
  else if((node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_POINTER) ||
            (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE))
    node->type(node->right()->type());
  else
    throw std::string("wrong types in add expression");
}
void gr8::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_UNSPEC) node->left()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->left()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_DOUBLE && 
    node->left()->type()->name() != basic_type::TYPE_POINTER) 
    throw std::string("wrong type in left argument of sub expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_UNSPEC) node->right()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->right()->type()->name() != basic_type::TYPE_INT && node->right()->type()->name() != basic_type::TYPE_DOUBLE && 
    node->right()->type()->name() != basic_type::TYPE_POINTER) 
    throw std::string("wrong type in right argument of sub expression");

  if((node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_INT) ||
      (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT) ||
      (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE) ||
      (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT))
    node->type(node->left()->type());
  else if((node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_POINTER) ||
            (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE))
    node->type(node->right()->type());
  else if((node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_POINTER) &&
            node->left()->type()->_subtype->name() == node->right()->type()->_subtype->name())
    node->type(new basic_type(4, basic_type::TYPE_INT));        
  else
    throw std::string("wrong types in sub expression");
}
void gr8::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_UNSPEC) node->left()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->left()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("wrong type in left argument of multiply expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_UNSPEC) node->right()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->right()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_DOUBLE) 
    throw std::string("wrong type in right argument of multiply expression");

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(4, basic_type::TYPE_DOUBLE));
  else 
    node->type(new basic_type(4, basic_type::TYPE_INT));
}
void gr8::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_UNSPEC) node->left()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->left()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("wrong type in left argument of division expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_UNSPEC) node->right()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->right()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_DOUBLE) 
    throw std::string("wrong type in right argument of division expression");

  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(4, basic_type::TYPE_DOUBLE));
  else 
    node->type(new basic_type(4, basic_type::TYPE_INT));
}
void gr8::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void gr8::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_UNSPEC) node->left()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->left()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("wrong type in left argument of less than expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_UNSPEC) node->right()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->right()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_DOUBLE) 
    throw std::string("wrong type in right argument of less than expression");

  node->type(new basic_type(4, basic_type::TYPE_INT));
}
void gr8::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_UNSPEC) node->left()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->left()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("wrong type in left argument of greater than expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_UNSPEC) node->right()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->right()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_DOUBLE) 
    throw std::string("wrong type in right argument of greater than expression");

  node->type(new basic_type(4, basic_type::TYPE_INT));
}
void gr8::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() == basic_type::TYPE_UNSPEC) node->left()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->left()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_DOUBLE && node->left()->type()->name() != basic_type::TYPE_POINTER) 
    throw std::string("wrong type in left argument of equals expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() == basic_type::TYPE_UNSPEC) node->right()->type(new basic_type(4, basic_type::TYPE_INT));
  if (node->right()->type()->name() != basic_type::TYPE_INT && node->right()->type()->name() != basic_type::TYPE_DOUBLE && node->right()->type()->name() != basic_type::TYPE_POINTER) 
    throw std::string("wrong type in right argument of equals expression");

  node->type(new basic_type(4, basic_type::TYPE_INT));
}
void gr8::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  //EMPTY: Operation does not exist in gr8!
}
void gr8::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  //EMPTY: Operation does not exist in gr8!
}
void gr8::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  //EMPTY: Operation does not exist in gr8!
}
//---------------------------------------------------------------------------

void gr8::type_checker::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<gr8::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
  } else {
    throw id;
  }
}

void gr8::type_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void gr8::type_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl);
  node->rvalue()->accept(this, lvl);
  gr8::alloc_node* alloc = dynamic_cast<gr8::alloc_node*>(node->rvalue());
  if(node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC){
    if(node->lvalue()->type()->name() == basic_type::TYPE_STRING || node->lvalue()->type()->name() == basic_type::TYPE_POINTER)
      throw std::string("can't assign input to string/pointer");
    else 
      node->rvalue()->type(node->lvalue()->type());
  }
  //checking if the rval is an alloc
  else if(node->rvalue()->type()->name() == basic_type::TYPE_POINTER && alloc){
    if(node->lvalue()->type()->name() != basic_type::TYPE_POINTER)
      throw std::string("can't alloc and assign to a non-pointer type");
    else
      alloc->type()->_subtype = node->lvalue()->type()->_subtype;
  }
  else if (node->lvalue()->type()->name() != node->rvalue()->type()->name() &&
      !(node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE && node->rvalue()->type()->name() == basic_type::TYPE_INT))
    throw std::string("wrong types in assignment");

  node->type(node->lvalue()->type());
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_evaluation_node(gr8::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void gr8::type_checker::do_tweet_node(gr8::tweet_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if(node->argument()->type()->name() == basic_type::TYPE_UNSPEC)
    node->argument()->type(new basic_type(4, basic_type::TYPE_INT));
  else if (node->argument()->type()->name() == basic_type::TYPE_POINTER)
    throw std::string("wrong type in post expression");
}

void gr8::type_checker::do_post_node(gr8::post_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if(node->argument()->type()->name() == basic_type::TYPE_UNSPEC)
    node->argument()->type(new basic_type(4, basic_type::TYPE_INT));
  else if (node->argument()->type()->name() == basic_type::TYPE_POINTER)
    throw std::string("wrong type in post expression");
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_read_node(gr8::read_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type());
}

void gr8::type_checker::do_if_node(gr8::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  node->block()->accept(this, lvl + 4);
}

void gr8::type_checker::do_if_else_node(gr8::if_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  node->thenblock()->accept(this, lvl + 4);
  node->elseblock()->accept(this, lvl + 4);
}

void gr8::type_checker::do_sweep_node(gr8::sweep_node * const node, int lvl) {
  node->init()->accept(this, lvl + 4);
  node->condition()->accept(this, lvl + 4);
  node->increment()->accept(this, lvl + 4);
  node->block()->accept(this, lvl + 4);
}

void gr8::type_checker::do_again_node(gr8::again_node * const node, int lvl) {
  if (node->cycleNumber() < 1)
    throw std::string("0 or negative cycle number in again instruction");

}

void gr8::type_checker::do_return_node(gr8::return_node * const node, int lvl) {
  if(node->argument()){
    node->argument()->accept(this, lvl);
    if(_parent->new_symbol()->type()->name() != node->argument()->type()->name() &&
        !(_parent->new_symbol()->type()->name() == basic_type::TYPE_DOUBLE && node->argument()->type()->name() == basic_type::TYPE_INT))
      throw std::string("wrong type for return");
  }
  else{
    if(_parent->new_symbol()->type()->name() != basic_type::TYPE_UNSPEC)
      throw std::string("empty return on non-void function");
  }
}

void gr8::type_checker::do_stop_node(gr8::stop_node * const node, int lvl) {
  if (node->cycleNumber() < 1)
    throw std::string("0 or negative cycle number in stop instruction");
}

void gr8::type_checker::do_block_node(gr8::block_node * const node, int lvl){
  node->decls()->accept(this, lvl + 4);
  if(node->instructs())
    node->instructs()->accept(this, lvl + 4);
  if (node->ret())
    node->ret()->accept(this, lvl + 4);
}

void gr8::type_checker::do_indexing_node(gr8::indexing_node * const node, int lvl){
  ASSERT_UNSPEC;
  node->ptr()->accept(this, lvl + 2);
  if (node->ptr()->type()->name() != basic_type::TYPE_POINTER)
    throw std::string("wrong type in pointer argument of indexing expression");

  node->index()->accept(this, lvl + 2);
  if (node->index()->type()->name() != basic_type::TYPE_INT) 
    throw std::string("wrong type in index argument of indexing expression");

  node->type(node->ptr()->type()->subtype());
}

void gr8::type_checker::do_var_decl_node(gr8::var_decl_node * const node, int lvl){
  std::string id = *(node->id());
  std::shared_ptr<gr8::symbol> symbol =  std::make_shared<gr8::symbol>(node->type(), id, 0, node->accessMode(), false);
  if (!_symtab.insert(id, symbol))
    throw id + " redeclared";
 
  if (node->argument()) {
    node->argument()->accept(this, lvl+2);
    gr8::alloc_node* alloc = dynamic_cast<gr8::alloc_node*>(node->argument());
    gr8::read_node* read = dynamic_cast<gr8::read_node*>(node->argument());
    if ((node->argument()->type()) && node->type()->name() != node->argument()->type()->name() && 
      !(node->argument()->type()->name() == basic_type::TYPE_UNSPEC) &&
      !(node->type()->name() == basic_type::TYPE_DOUBLE && node->argument()->type()->name() == basic_type::TYPE_INT))
      throw std::string("wrong type for initializer");
    // checking if the arg is an input expr
    else if(node->argument()->type()->name() == basic_type::TYPE_UNSPEC && read){
      if(node->type()->name() == basic_type::TYPE_STRING || node->type()->name() == basic_type::TYPE_POINTER)
        throw std::string("can't assign input to string/pointer");
      else 
        node->argument()->type(node->type());
    }
    //checking if the arg is an alloc
    else if(node->argument()->type()->name() == basic_type::TYPE_POINTER && alloc){
      if(node->type()->name() != basic_type::TYPE_POINTER)
        throw std::string("can't alloc and assign to a non-pointer type");
      else
        alloc->type()->_subtype = node->type()->_subtype;
    }
  }
  //_parent->set_new_symbol(symbol);
}

void gr8::type_checker::do_func_decl_node(gr8::func_decl_node * const node, int lvl){
  std::string id = *(node->id());
  std::shared_ptr<gr8::symbol> symbol =  std::make_shared<gr8::symbol>(node->type(), id, 0, node->accessMode(), true);
  node->args()->accept(this, lvl+2);
  // Placing the args on the function's symbol
  for(size_t i = 0; i != node->args()->size(); i++){
    symbol->addArg(((cdk::expression_node*)node->args()->node(i))->type()->name());
  }
  if (!_symtab.insert(id, symbol))
    throw id + " redeclared";
  _parent->set_new_symbol(symbol);
}

void gr8::type_checker::do_func_call_node(gr8::func_call_node * const node, int lvl){
  const std::string id = *(node->id());
  std::shared_ptr<gr8::symbol> symbol = _symtab.find(id);
  if (symbol != nullptr) {
    if(!symbol->isFunction())
      throw std::string("not calling a function");
    node->type(symbol->type());
    node->args()->accept(this, lvl+2);
    // Checking if call args are consistent with the function decl:
    if(symbol->args()->size() != node->args()->size())
      throw std::string("invalid number of args for function");
    else if(symbol->args()->size() != 0){
      for(size_t i = 0; i != node->args()->size(); i++){ 
        // Each arg must be the same type, with the exception where def/decl has a double and the call has an int.
        if((((cdk::expression_node*)node->args()->node(i))->type()->name() != symbol->arg(i)) &&
            !(symbol->arg(i) == basic_type::TYPE_DOUBLE &&
             ((cdk::expression_node*)node->args()->node(i))->type()->name() == basic_type::TYPE_INT))
          throw std::string("different args than function");
      }
    }
  } else {
    throw id + " undeclared";
  }
}

void gr8::type_checker::do_func_def_node(gr8::func_def_node * const node, int lvl){
  std::string id = *(node->id());
  std::shared_ptr<gr8::symbol> symbol =  std::make_shared<gr8::symbol>(node->type(), id, 0, node->accessMode(), true);
  // Placing the args on the function's symbol
  for(size_t i = 0; i != node->args()->size(); i++){
    symbol->addArg(((cdk::expression_node*)node->args()->node(i))->type()->name());
  }
  _symtab.insert(id, symbol);
  _parent->set_new_symbol(symbol);
}

void gr8::type_checker::do_identity_node(gr8::identity_node * const node, int lvl){
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if(node->argument()->type()->name() == basic_type::TYPE_UNSPEC)
    node->argument()->type(new basic_type(4, basic_type::TYPE_INT));
  else if (node->argument()->type()->name() != basic_type::TYPE_INT && node->argument()->type()->name() != basic_type::TYPE_DOUBLE) throw std::string("wrong type in argument of unary expression");

  node->type(node->argument()->type());
}

void gr8::type_checker::do_address_node(gr8::address_node * const node, int lvl){
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if(node->argument()->type()->name() == basic_type::TYPE_UNSPEC)
    node->argument()->type(new basic_type(4, basic_type::TYPE_INT));
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
  node->type()->_subtype = node->argument()->type();
}

void gr8::type_checker::do_alloc_node(gr8::alloc_node * const node, int lvl){
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if(node->argument()->type()->name() == basic_type::TYPE_UNSPEC)
    node->argument()->type(new basic_type(4, basic_type::TYPE_INT));
  else if (node->argument()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in argument of allocation expression");

  node->type(new basic_type(4, basic_type::TYPE_POINTER));
}

void gr8::type_checker::do_null_ptr_node(gr8::null_ptr_node * const node, int lvl){
  node->type(new basic_type());
}