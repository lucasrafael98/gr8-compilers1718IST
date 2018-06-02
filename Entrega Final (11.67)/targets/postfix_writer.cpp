#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/stack_counter.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void gr8::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void gr8::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  _pf.DOUBLE(node->value()); // push a double
}
// Postfix's logical operations are bitwise, so to be perfectly accurate, jumps must be used.
void gr8::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->argument()->accept(this, lvl); // determine the value
  _pf.INT(0);
  _pf.EQ();
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  _pf.INT(1);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  _pf.INT(0);
  _pf.LABEL(mklbl(lbl2));
}
void gr8::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->left()->accept(this, lvl);
  _pf.INT(0);
  _pf.EQ();
  _pf.JNZ(mklbl(lbl1 = ++_lbl));
  node->right()->accept(this, lvl);
  _pf.INT(0);
  _pf.EQ();
  _pf.JNZ(mklbl(lbl1));
  _pf.INT(1);
  _pf.LABEL(mklbl(lbl1));
}
void gr8::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->left()->accept(this, lvl);
  _pf.INT(0);
  _pf.EQ();
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->right()->accept(this, lvl);
  _pf.INT(0);
  _pf.EQ();
  _pf.JNZ(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  _pf.INT(1);
  _pf.LABEL(mklbl(lbl2));
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  _pf.INT(node->value()); // push an integer
}

void gr8::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters

  /* leave the address on the stack */
  _pf.TEXT(); // return to the TEXT segment
  _pf.ADDR(mklbl(lbl1)); // the string to be printed
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if(node->argument()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DNEG();
  else
    _pf.NEG(); // 2-complement
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if(node->right()->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();
  node->right()->accept(this, lvl);
  if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();
  if(node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DADD();
  else if(node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_INT)
    (new indexing_node(node->lineno(), node->left(), node->right()))->accept(this, lvl);
  else if(node->right()->type()->name() == basic_type::TYPE_POINTER && node->left()->type()->name() == basic_type::TYPE_INT)
    (new indexing_node(node->lineno(), node->right(), node->left()))->accept(this, lvl);
  else
    _pf.ADD();
}
void gr8::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if(node->right()->type()->name() == basic_type::TYPE_POINTER && node->left()->type()->name() == basic_type::TYPE_POINTER){
    cdk::lvalue_node* left = (dynamic_cast<cdk::rvalue_node*>(node->left())) ?
        dynamic_cast<cdk::rvalue_node*>(node->left())->lvalue() : nullptr;
    cdk::lvalue_node* right = (dynamic_cast<cdk::rvalue_node*>(node->right())) ?
        dynamic_cast<cdk::rvalue_node*>(node->right())->lvalue() : nullptr;
    left->accept(this, lvl);
    right->accept(this, lvl);
    _pf.SUB();
    _pf.INT(node->left()->type()->_subtype->size());
    _pf.DIV();
    return;
  }
  node->left()->accept(this, lvl);
  if(node->right()->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();
  node->right()->accept(this, lvl);
  if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();
  if(node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DSUB();
  else if(node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() == basic_type::TYPE_INT)
    (new indexing_node(node->lineno(), node->left(), node->right()))->accept(this, lvl);
  else if(node->right()->type()->name() == basic_type::TYPE_POINTER && node->left()->type()->name() == basic_type::TYPE_INT)
    (new indexing_node(node->lineno(), node->right(), node->left()))->accept(this, lvl);
  else
    _pf.SUB();
}
void gr8::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if(node->right()->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();
  node->right()->accept(this, lvl);
  if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();
  if(node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DMUL();
  else
    _pf.MUL();
}
void gr8::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if(node->right()->type()->name() == basic_type::TYPE_DOUBLE && node->left()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();
  node->right()->accept(this, lvl);
  if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT)
    _pf.I2D();
  if(node->left()->type()->name() == basic_type::TYPE_DOUBLE || node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DDIV();
  else
    _pf.DIV();
}
void gr8::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void gr8::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->left()->accept(this, lvl);
  if(node->left()->type()->name() == basic_type::TYPE_INT &&
     node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();
  node->right()->accept(this, lvl);
  if(node->right()->type()->name() == basic_type::TYPE_INT &&
     node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();
  
  if(node->right()->type()->name() == basic_type::TYPE_DOUBLE ||
     node->left()->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DCMP();
    _pf.INT(0);
    _pf.LT();
    _pf.DUP64();
    _pf.JZ(mklbl(lbl1 = ++_lbl));
    _pf.INT(1);
    _pf.JMP(mklbl(lbl2 = ++_lbl));
    _pf.LABEL(mklbl(lbl1));
    _pf.INT(0);
    _pf.LABEL(mklbl(lbl2));
  }
  else _pf.LT();
}
void gr8::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->left()->accept(this, lvl);
  if(node->left()->type()->name() == basic_type::TYPE_INT &&
     node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();
  node->right()->accept(this, lvl);
  if(node->right()->type()->name() == basic_type::TYPE_INT &&
     node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();
  
  if(node->right()->type()->name() == basic_type::TYPE_DOUBLE ||
     node->left()->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DCMP();
    _pf.INT(0);
    _pf.GT();
    _pf.DUP64();
    _pf.JZ(mklbl(lbl1 = ++_lbl));
    _pf.INT(1);
    _pf.JMP(mklbl(lbl2 = ++_lbl));
    _pf.LABEL(mklbl(lbl1));
    _pf.INT(0);
    _pf.LABEL(mklbl(lbl2));
  }
  else _pf.GT();
}
void gr8::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->left()->accept(this, lvl);
  if(node->left()->type()->name() == basic_type::TYPE_INT &&
     node->right()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();
  node->right()->accept(this, lvl);
  if(node->right()->type()->name() == basic_type::TYPE_INT &&
     node->left()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.I2D();
  
  if(node->right()->type()->name() == basic_type::TYPE_DOUBLE ||
     node->left()->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.DCMP();
    _pf.INT(0);
    _pf.EQ();
    _pf.DUP64();
    _pf.JZ(mklbl(lbl1 = ++_lbl));
    _pf.INT(1);
    _pf.JMP(mklbl(lbl2 = ++_lbl));
    _pf.LABEL(mklbl(lbl1));
    _pf.INT(0);
    _pf.LABEL(mklbl(lbl2));
  }
  else _pf.EQ();
}
void gr8::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  //EMPTY: Operation does not exist in gr8!
}
void gr8::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  //EMPTY: Operation does not exist in gr8!
}
void gr8::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  //EMPTY: Operation does not exist in gr8!
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  const std::string &id = node->name();
  if(_symtab.find_local(id))
    _pf.LOCAL(_symtab.find_local(id)->offset());
  else if(_symtab.find(id)){
    std::shared_ptr<gr8::symbol> symbol = _symtab.find_local(_currentFunc + id);
    if(symbol && symbol->accessMode() == 0){
      if(symbol->offset())
        _pf.LOCAL(symbol->offset());
      else{
        symbol->offset(_currentOffset);
        _currentOffset -= symbol->type()->size();
        _pf.LOCAL(symbol->offset());
      }
    }
    else
      _pf.ADDR(node->name());
  }
}

void gr8::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  if(node->lvalue()->type()->name() == basic_type::TYPE_INT || 
     node->lvalue()->type()->name() == basic_type::TYPE_STRING || 
     node->lvalue()->type()->name() == basic_type::TYPE_POINTER)
    _pf.LDINT();
  else if(node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.LDDOUBLE();
}

void gr8::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // determine the new value
  if(node->type()->name() == basic_type::TYPE_DOUBLE){
    if(node->rvalue()->type()->name() != basic_type::TYPE_DOUBLE)
      _pf.I2D();
    node->lvalue()->accept(this, lvl); // where to store the value
    _pf.STDOUBLE();
  }
  else{
    node->lvalue()->accept(this, lvl); // where to store the value
    _pf.STINT();
  }
}

//---------------------------------------------------------------------------

/*void gr8::postfix_writer::do_program_node(gr8::program_node * const node, int lvl) {
  // Note that Simple didn't have functions. Thus, it didn't need
  // a function node. However, it had to start in the main function.
  // program_node (representing the whole program) doubled as a
  // main function node.

  // generate the main function (RTS mandates that its name be "_main")
  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL("_main");
  _pf.ENTER(0);  // Simple didn't implement local variables

  node->statements()->accept(this, lvl);

  // end the main function
  _pf.INT(0);
  _pf.STFVAL32();
  _pf.LEAVE();
  _pf.RET();

  // these are just a few library function imports
  _pf.EXTERN("readi");
  _pf.EXTERN("printi");
  _pf.EXTERN("prints");
  _pf.EXTERN("println");
}*/

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_evaluation_node(gr8::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.TRASH(4); // delete the evaluated value
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else if (node->argument()->type()->name() == basic_type::TYPE_POINTER) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.TRASH(8); // delete the evaluated value's address
  } else if (node->argument()->type()->name() == basic_type::TYPE_UNSPEC) {
    ;
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void gr8::postfix_writer::do_tweet_node(gr8::tweet_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd"); //TODO is this the func to call? postfix ref doesn't state anything about it 
    _pf.TRASH(8); // delete the printed value
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void gr8::postfix_writer::do_post_node(gr8::post_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd"); //TODO is this the func to call? postfix ref doesn't state anything about it 
    _pf.TRASH(8); // delete the printed value
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  _pf.CALL("println"); // print a newline
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_read_node(gr8::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->type()->name() == basic_type::TYPE_INT){
    _pf.CALL("readi");
    _pf.LDFVAL32();
  }
  else if (node->type()->name() == basic_type::TYPE_DOUBLE){
    _pf.CALL("readd"); //TODO is this the func to call?
    _pf.LDFVAL64();
  }
}

//---------------------------------------------------------------------------

/*void gr8::postfix_writer::do_while_node(gr8::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  _pf.LABEL(mklbl(lbl1 = ++_lbl));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl2 = ++_lbl));
  node->block()->accept(this, lvl);
  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl2));
}*/

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_if_node(gr8::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_if_else_node(gr8::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl);
  _pf.LABEL(mklbl(lbl2));
}

void gr8::postfix_writer::do_sweep_node(gr8::sweep_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2, lbl3, lbl4, lbl5, lbl6, lbl7, lbl8;
  lbl1 = ++_lbl;                          // SWEEP_CONDITION DECREASING LABEL
  _cycleEnds.push_back(lbl2 = ++_lbl);    // SWEEP_END LABEL
  _cycleStarts.push_back(lbl3 = ++_lbl);  // SWEEP_INCR LABEL
  lbl4 = ++_lbl;                          // SWEEP_CONDITION INCREASING LABEL
  lbl5 = ++_lbl;                          // NEGATIVE INCR CHECK LABEL
  lbl6 = ++_lbl;                          // POSITIVE INCR CHECK LABEL
  lbl7 = ++_lbl;                          // CHECK BEGIN LABEL
  lbl8 = ++_lbl;                          // CHECK BEGIN BLOCK LABEL

  //init var setting
  (new cdk::assignment_node(node->lineno(), node->value(), node->init()))->accept(this, lvl);

  _pf.LABEL(mklbl(lbl7));
  (new cdk::sub_node(node->lineno(), node->init(), node->condition()))->accept(this, lvl);
  _pf.DUP32(); 
  _pf.INT(0);
  _pf.EQ();
  _pf.JNZ(mklbl(lbl8));
  _pf.INT(0);
  _pf.GT();
  _pf.DUP32();
  _pf.JZ(mklbl(lbl6));
  _pf.JNZ(mklbl(lbl5));

  _pf.LABEL(mklbl(lbl5));
  (new cdk::lt_node(node->lineno(), node->increment(), new cdk::integer_node(node->lineno(), 0)))->accept(this, lvl);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl1));
  _pf.JZ(mklbl(lbl2));

  _pf.LABEL(mklbl(lbl6));
  _pf.TRASH(4);
  (new cdk::gt_node(node->lineno(), node->increment(), new cdk::integer_node(node->lineno(), 0)))->accept(this, lvl);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl4));
  _pf.JZ(mklbl(lbl2));

  //checking end state
  _pf.LABEL(mklbl(lbl1));
  _pf.TRASH(4);
  (new cdk::not_node(node->lineno(), new cdk::lt_node(node->lineno(), new cdk::rvalue_node(node->lineno(),node->value()), \
    node->condition())))->accept(this, lvl);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl2));
  _pf.JNZ(mklbl(lbl8));

  _pf.LABEL(mklbl(lbl4));
  _pf.TRASH(4);
  (new cdk::not_node(node->lineno(), new cdk::gt_node(node->lineno(), new cdk::rvalue_node(node->lineno(),node->value()), \
    node->condition())))->accept(this, lvl);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl2));
  _pf.TRASH(4);

  _pf.LABEL(mklbl(lbl8));
  node->block()->accept(this, lvl);

  //increment variable
  _pf.LABEL(mklbl(lbl3));
  cdk::add_node* add_expr = new cdk::add_node(node->lineno(), new cdk::rvalue_node(node->lineno(),node->value()), node->increment());
  (new cdk::assignment_node(node->lineno(), node->value(), add_expr))->accept(this, lvl);

  _pf.JMP(mklbl(lbl7));
  _pf.LABEL(mklbl(lbl2));
  _cycleStarts.pop_back();
  _cycleEnds.pop_back();
}


void gr8::postfix_writer::do_return_node(gr8::return_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  std::shared_ptr<gr8::symbol> symbol = _symtab.find((_currentFunc == "_main")? "covfefe" : _currentFunc);
  if(node->argument()){
    node->argument()->accept(this, lvl);
    if(symbol->type()->name() == basic_type::TYPE_DOUBLE && node->argument()->type()->name() == basic_type::TYPE_INT)
      _pf.I2D();
    if(symbol->type()->name() == basic_type::TYPE_DOUBLE)
      _pf.STFVAL64();
    else
      _pf.STFVAL32();
  }
  if(_currentFunc != "")
    _pf.JMP(_currentFunc + "end");
}

void gr8::postfix_writer::do_again_node(gr8::again_node * const node, int lvl) {
  _pf.JMP(mklbl(_cycleStarts.at(_cycleStarts.size() - node->cycleNumber())));
}

void gr8::postfix_writer::do_stop_node(gr8::stop_node * const node, int lvl) {
  _pf.JMP(mklbl(_cycleEnds.at(_cycleEnds.size() - node->cycleNumber())));
}

void gr8::postfix_writer::do_block_node(gr8::block_node* const node, int lvl){
  node->decls()->accept(this, lvl);
  if(node->instructs())
    node->instructs()->accept(this, lvl);
  if (node->ret())
    node->ret()->accept(this, lvl);
}

void gr8::postfix_writer::do_indexing_node(gr8::indexing_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  node->ptr()->accept(this, lvl);
  node->index()->accept(this, lvl);
  _pf.INT(node->ptr()->type()->_subtype->size());
  _pf.MUL();
  _pf.ADD();
}

void gr8::postfix_writer::do_var_decl_node(gr8::var_decl_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  std::string id = *(node->id());
  std::shared_ptr<gr8::symbol> symbol = _symtab.find_local(id);

  if(_funcArgs){
    symbol->offset(_currentArgOffset);
    _currentArgOffset += symbol->type()->size();
    return;
  }

  if (symbol->accessMode() == 2){
    _pf.EXTERN(id);
    return;
  }
  else if (symbol->type()->name() == basic_type::TYPE_STRING){
    if(node->argument()){
      int lbl1;

      /* generate the string */
      _pf.RODATA(); // strings are DATA readonly
      _pf.ALIGN(); // make sure we are aligned
      _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
      cdk::string_node* init_node = dynamic_cast<cdk::string_node*>(node->argument());
      _pf.SSTRING(init_node->value()); // output string characters

      _pf.DATA();
      _pf.ALIGN();
      _pf.LABEL(id);
      _pf.SADDR(mklbl(lbl1));
    }
    else{
      _pf.DATA();
      _pf.ALIGN();
      _pf.LABEL(id);
      _pf.SINT(0);
    }
    _pf.TEXT();
    _pf.ALIGN();
    return;
  }
  if(_currentFunc == ""){
    if (!node->argument())
      _pf.BSS();
    else
      _pf.DATA();

    _pf.ALIGN();

    if (symbol->accessMode() == 1) 
      _pf.GLOBAL(id, std::string("OBJ"));

    if(_currentFunc == "")
      _pf.LABEL(id);
  }

  if(_currentFunc != ""){ 
    symbol->offset(_currentOffset);
    _currentOffset -= symbol->type()->size();
  }

  if(node->argument()){
    if(_currentFunc == ""){
      if(symbol->type()->name() == basic_type::TYPE_INT){
        cdk::integer_node* init_node = dynamic_cast<cdk::integer_node*>(node->argument());
        _pf.SINT(init_node->value());
        if(!(symbol->accessMode()))
          symbol->value(init_node->value());
      }
      else if(symbol->type()->name() == basic_type::TYPE_DOUBLE){
        if(node->argument()->type()->name() == basic_type::TYPE_DOUBLE){
          cdk::double_node* init_node = dynamic_cast<cdk::double_node*>(node->argument());
          _pf.SDOUBLE(init_node->value());
          if(!(symbol->accessMode()))
            symbol->value(init_node->value());
        }
        else{
          cdk::integer_node* init_node = dynamic_cast<cdk::integer_node*>(node->argument());
          _pf.SDOUBLE(init_node->value());
          if(!(symbol->accessMode()))
            symbol->value(init_node->value());
        }
      }
      _pf.TEXT();
      _pf.ALIGN();
    }
    else{
      //_pf.SALLOC(symbol->type()->size());
      _pf.TEXT();
      _pf.ALIGN();
      node->argument()->accept(this, lvl);
      if(node->argument()->type()->name() == basic_type::TYPE_DOUBLE)
        _pf.DUP64();
      else
        _pf.DUP32();

      if(_currentFunc != "")
        _pf.LOCAL(symbol->offset());
      else
        _pf.ADDR(*(node->id()));

      if(symbol->type()->name() == basic_type::TYPE_DOUBLE)
        _pf.STDOUBLE();
      else if (symbol->type()->name() == basic_type::TYPE_INT ||
                symbol->type()->name() == basic_type::TYPE_POINTER ||
                symbol->type()->name() == basic_type::TYPE_STRING)
        _pf.STINT(); // store the value at address
      _pf.TRASH(node->type()->size());
    }
  }
  else if(_currentFunc == ""){
    _pf.SALLOC(symbol->type()->size());
    _pf.TEXT();
    _pf.ALIGN();
  }
}

void gr8::postfix_writer::do_func_decl_node(gr8::func_decl_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  std::string id = *(node->id());
  // RTS library functions should be extern
  if(id == "argc" || id == "argv" || id == "envp")
    _pf.EXTERN(id);
  else{
    _pf.TEXT();
    _pf.ALIGN();
    _pf.GLOBAL(id, std::string("FUNC"));
  }
}

void gr8::postfix_writer::do_func_call_node(gr8::func_call_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  // function args are put in the stack in reverse order
  std::string id = *(node->id());
  std::shared_ptr<gr8::symbol> symbol = _symtab.find(id);
  int toTrash = 0;
  for (int i = node->args()->size() - 1; i > -1; i--) {
    node->args()->node(i)->accept(this, lvl);
    toTrash += dynamic_cast<cdk::expression_node*>(node->args()->node(i))->type()->size();
    if(symbol->arg(i) == basic_type::TYPE_DOUBLE &&
       dynamic_cast<cdk::expression_node*>(node->args()->node(i))->type()->name() == basic_type::TYPE_INT){
      _pf.I2D();
      toTrash += 4;
    }
  }
  _pf.CALL(*(node->id()));
  _pf.TRASH(toTrash);
  if(node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.LDFVAL64();
  else
    _pf.LDFVAL32();
}

void gr8::postfix_writer::do_func_def_node(gr8::func_def_node * const node, int lvl){
  // No need to put the function on symtab again if it was declared!
  if(!_symtab.find(*(node->id()))){
    ASSERT_SAFE_EXPRESSIONS;
  }
  _symtab.push();
  //"covfefe" is the main func name, so it must be recalled "_main" for the RTS library.
  std::string id_temp;
  if(*(node->id()) == "covfefe"){
    id_temp = std::string("_main");
    _pf.EXTERN("readi");
    _pf.EXTERN("readd");
    _pf.EXTERN("printi");
    _pf.EXTERN("printd");
    _pf.EXTERN("prints");
    _pf.EXTERN("println");
  }
  else{
    id_temp = *(node->id());
  }

  const std::string id = id_temp;
  _currentFunc = id;
  _funcArgs = true;
  node->args()->accept(this, lvl);
  _funcArgs = false;

  stack_counter counter = stack_counter(_compiler, _symtab, _currentFunc);
  node->accept(&counter, lvl);

  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL(id, _pf.FUNC());
  _pf.LABEL(id);
  _pf.ENTER(counter.count());

  node->block()->accept(this, lvl);

  _pf.LABEL(id + "end");
  _pf.LEAVE();
  _pf.RET();
  _symtab.pop();
  _currentFunc = "";
  _currentOffset = -4;
  _currentArgOffset = 8;
}

void gr8::postfix_writer::do_identity_node(gr8::identity_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  // identity is... itself, nothing else needed here.
  node->argument()->accept(this, lvl);
}

void gr8::postfix_writer::do_address_node(gr8::address_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  // this only takes lvals, which are by themselves addresses; nothing else needed here.
  node->argument()->accept(this, lvl);
}

void gr8::postfix_writer::do_alloc_node(gr8::alloc_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.INT(node->type()->_subtype->size());
  _pf.MUL();
  _pf.ALLOC();
  _pf.SP();
}

void gr8::postfix_writer::do_null_ptr_node(gr8::null_ptr_node * const node, int lvl){
  ASSERT_SAFE_EXPRESSIONS;
  _pf.INT(0);
}