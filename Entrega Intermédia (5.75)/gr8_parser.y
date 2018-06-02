%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#include <iostream>
#define LINE               compiler->scanner()->lineno()
#define yylex()            compiler->scanner()->scan()
#define yyerror(s)         compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                   i;	/* integer value */
  double                d;  /* double value */ 
  bool                  b;
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  basic_type           *type;
  gr8::block_node      *block;
  cdk::literal_node<std::string>    *lit; 
};

%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tIDENTIFIER tSTRING
%token <type> tINIT
%token tIF tPRINT tREAD tUSE tPUBLIC tPOINTER tDEF tVFUNC tFUNC tON tAS tDO tUSES
%token tFOR tRETURN tTHEN tELIF tBREAK tCONTINUE tSWEEP tFROM tBY tASSIGN
%token tTO tINDEXNUM tINDEXARR tPRINTLN tINTDECL tSTRDECL tDBLDECL tELSE tALLOC tNULL

%left '|'
%left '&'
%nonassoc '!' tLVAL
%left '='
%left '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY '?' tALLOC
%nonassoc tEXPR
%nonassoc '(' ')'

%type <node> iter cond var elif decl func instr vdecl vdef fdecl fdef vdec vdefl vdecq vdefq varl
%type <sequence> decls elifs exprs insts vdecs varls vars
%type <expression> expr attr
%type <lvalue> lval
%type <type> type smals yuges fakes
%type <block> bloc blocf
%type <b> qual
%type <lit> strs

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file : decls        {compiler->ast($1);}
     ;

decls:              { $$ = new cdk::sequence_node(LINE); }
     | decls decl   { $$ = new cdk::sequence_node(LINE, $2, $1); }
     ;

decl : var          { $$ = $1; }
     | func         { $$ = $1; }
     ;

vars:              { $$ = new cdk::sequence_node(LINE); }
     | vars var    { $$ = new cdk::sequence_node(LINE, $2, $1); }
     ;

var  : vdecl ';'       { $$ = $1; }
     | vdef  ';'       { $$ = $1; }
     ;

/* local variables. */
varls:                 { $$ = new cdk::sequence_node(LINE); }
     | varls varl      { $$ = new cdk::sequence_node(LINE, $2, $1); }
     ;

varl : vdecl ';'       { $$ = $1; }
     | vdefl ';'       { $$ = $1; }
     ;

/* variable declarations. */
vdecs: vdec            { $$ = new cdk::sequence_node(LINE, $1); }
     | vdecs ',' vdec  { $$ = new cdk::sequence_node(LINE, $3, $1); }
     ;

vdec : vdecl           { $$ = $1; }
     | vdecq           { $$ = $1; }
     ;

/* local var declaration. */
vdecl: type tIDENTIFIER                               { $$ = new gr8::var_decl_node(LINE, false, $2, nullptr, $1); }
     ;

/* non-local var declaration. */
vdecq: qual type tIDENTIFIER                          { $$ = new gr8::var_decl_node(LINE, $1, $3, nullptr, $2); }
     ;

/* both types of var definition. */
vdef : vdefl           { $$ = $1; }
     | vdefq           { $$ = $1; }
     ;

/* local var definition. */
vdefl: type tIDENTIFIER '(' tINIT expr ')'            { $$ = new gr8::var_decl_node(LINE, false, $2, $5, $1); }
     ;

/* non-local var definition. */
vdefq: qual type tIDENTIFIER '(' tINIT expr ')'       { $$ = new gr8::var_decl_node(LINE, $1, $3, $6, $2); }
     ;

func : fdecl ';'    { $$ = $1; }
     | fdef         { $$ = $1; }
     ;

fdef : tDEF qual type tFUNC tIDENTIFIER tAS blocf                   { $$ = new gr8::func_def_node(LINE, $2, $5, new cdk::sequence_node(LINE), $7, $3); }
     | tDEF qual type tFUNC tIDENTIFIER tON vdecs tAS blocf         { $$ = new gr8::func_def_node(LINE, $2, $5, $7, $9, $3); }
     | tDEF type tFUNC tIDENTIFIER tAS blocf                        { $$ = new gr8::func_def_node(LINE, false, $4, new cdk::sequence_node(LINE), $6, $2); }
     | tDEF type tFUNC tIDENTIFIER tON vdecs tAS blocf              { $$ = new gr8::func_def_node(LINE, false, $4, $6, $8, $2); }
     | tDEF qual tVFUNC tIDENTIFIER tAS blocf                       { $$ = new gr8::func_def_node(LINE, $2, $4, new cdk::sequence_node(LINE), $6, new basic_type()); }
     | tDEF qual tVFUNC tIDENTIFIER tON vdecs tAS blocf             { $$ = new gr8::func_def_node(LINE, $2, $4, $6, $8, new basic_type()); }
     | tDEF tVFUNC tIDENTIFIER tAS blocf                            { $$ = new gr8::func_def_node(LINE, false, $3, new cdk::sequence_node(LINE), $5, new basic_type()); }
     | tDEF tVFUNC tIDENTIFIER tON vdecs tAS blocf                  { $$ = new gr8::func_def_node(LINE, false, $3, $5, $7, new basic_type()); }
     ;

fdecl: qual type tFUNC tIDENTIFIER                                  { $$ = new gr8::func_decl_node(LINE, $1, $4, new cdk::sequence_node(LINE), $2); }
     | qual type tFUNC tIDENTIFIER tUSES vdecs                      { $$ = new gr8::func_decl_node(LINE, $1, $4, $6, $2); }
     | type tFUNC tIDENTIFIER                                       { $$ = new gr8::func_decl_node(LINE, false, $3, new cdk::sequence_node(LINE), $1); }
     | type tFUNC tIDENTIFIER tUSES vdecs                           { $$ = new gr8::func_decl_node(LINE, false, $3, $5, $1); }
     | qual tVFUNC tIDENTIFIER                                      { $$ = new gr8::func_decl_node(LINE, $1, $3, new cdk::sequence_node(LINE), new basic_type()); }
     | qual tVFUNC tIDENTIFIER tUSES vdecs                          { $$ = new gr8::func_decl_node(LINE, $1, $3, $5, new basic_type()); }
     | tVFUNC tIDENTIFIER                                           { $$ = new gr8::func_decl_node(LINE, false, $2, new cdk::sequence_node(LINE), new basic_type()); }
     | tVFUNC tIDENTIFIER tUSES vdecs                               { $$ = new gr8::func_decl_node(LINE, false, $2, $4, new basic_type()); }
     ;

/* defines a function body (there can't be any non-local vars inside functions!) */
blocf: '{' varls insts '}'                  { $$ = new gr8::block_node(LINE, $2, $3, nullptr); }
     | '{' varls insts tRETURN ';' '}'      { $$ = new gr8::block_node(LINE, $2, $3, new gr8::return_node(LINE)); }
     | '{' varls insts tRETURN expr ';' '}' { $$ = new gr8::block_node(LINE, $2, $3, new gr8::return_node(LINE, $5)); }
     | '{' varls '}'                        { $$ = new gr8::block_node(LINE, $2, nullptr, nullptr); }
     | '{' varls tRETURN ';' '}'            { $$ = new gr8::block_node(LINE, $2, nullptr, new gr8::return_node(LINE)); }
     | '{' varls tRETURN expr ';' '}'       { $$ = new gr8::block_node(LINE, $2, nullptr, new gr8::return_node(LINE, $4)); }
     ;

/*defines a normal indentation block. */
bloc : '{' vars insts '}'                  { $$ = new gr8::block_node(LINE, $2, $3, nullptr); }
     | '{' vars insts tRETURN ';' '}'      { $$ = new gr8::block_node(LINE, $2, $3, new gr8::return_node(LINE)); }
     | '{' vars insts tRETURN expr ';' '}' { $$ = new gr8::block_node(LINE, $2, $3, new gr8::return_node(LINE, $5)); }
     | '{' vars '}'                        { $$ = new gr8::block_node(LINE, $2, nullptr, nullptr); }
     | '{' vars tRETURN ';' '}'            { $$ = new gr8::block_node(LINE, $2, nullptr, new gr8::return_node(LINE)); }
     | '{' vars tRETURN expr ';' '}'       { $$ = new gr8::block_node(LINE, $2, nullptr, new gr8::return_node(LINE, $4)); }
     ;

qual : tUSE                                 { $$ = true; }
     | tPUBLIC                              { $$ = true; }
     ;

insts: instr                                { $$ = new cdk::sequence_node(LINE, $1); }
     | insts instr                          { $$ = new cdk::sequence_node(LINE, $2, $1); }
     ;

instr: exprs ';'                            { $$ = $1; }
     | attr ';'                             { $$ = $1; }
     | tPRINTLN expr ';'                    { $$ = new gr8::post_node(LINE, $2); }
     | tPRINT expr ';'                      { $$ = new gr8::tweet_node(LINE, $2); }
     | tCONTINUE ';'                        { $$ = new gr8::again_node(LINE); }
     | tCONTINUE tINTEGER ';'               { $$ = new gr8::again_node(LINE, $2); }
     | tBREAK ';'                           { $$ = new gr8::stop_node(LINE); }
     | tBREAK tINTEGER ';'                  { $$ = new gr8::stop_node(LINE, $2); }
     | cond                                 { $$ = $1; }
     | iter                                 { $$ = $1; }
     | bloc                                 { $$ = $1; }
     ;

type : tINTDECL                             { $$ = new basic_type(4, basic_type::TYPE_INT); }
     | smals tPOINTER                       { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $1; }
     | tDBLDECL                             { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
     | yuges tPOINTER                       { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $1; }
     | tSTRDECL                             { $$ = new basic_type(4, basic_type::TYPE_STRING); }
     | fakes tSTRDECL                       { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $1; }
     ;

smals: tINTDECL                             { $$ = new basic_type(4, basic_type::TYPE_POINTER); }
     | smals tINTDECL                       { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $1; }
     ;

yuges: tDBLDECL                             { $$ = new basic_type(4, basic_type::TYPE_POINTER); }       
     | yuges tDBLDECL                       { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $1; }
     ;

fakes: tPOINTER                             { $$ = new basic_type(4, basic_type::TYPE_POINTER); }
     | fakes tPOINTER                       { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $1; }
     ;

attr : tASSIGN expr tTO lval                { $$ = new cdk::assignment_node(LINE, $4, $2); }
     ;

cond : tIF expr tTHEN bloc                  { $$ = new gr8::if_node(LINE, $2, $4); }
     | tIF expr tTHEN bloc tELSE bloc       { $$ = new gr8::if_else_node(LINE, $2, $4, $6); }
     | tIF expr tTHEN bloc elifs            { $$ = new gr8::if_else_node(LINE, $2, $4, $5); }
     ;

elifs: elif                                 { $$ = new cdk::sequence_node(LINE, $1); }
     | elifs elif                           { $$ = new cdk::sequence_node(LINE, $2, $1); }
     ;

elif : tELIF expr tTHEN bloc                { $$ = new gr8::if_node(LINE, $2, $4); }
     | tELIF expr tTHEN bloc tELSE bloc     { $$ = new gr8::if_else_node(LINE, $2, $4, $6); }
     ;

iter : tSWEEP lval tFROM expr tTO expr tDO bloc             { $$ = new gr8::sweep_node(LINE, $2, $4, $6, new cdk::integer_node(LINE, 1), $8); }
     | tSWEEP lval tFROM expr tTO expr tBY expr tDO bloc    { $$ = new gr8::sweep_node(LINE, $2, $4, $6, $8, $10); }
     ;

exprs: expr                     { $$ = new cdk::sequence_node(LINE, $1); }
     | exprs ',' expr           { $$ = new cdk::sequence_node(LINE, $3, $1); }
     ;

expr : tINTEGER                             { $$ = new cdk::integer_node(LINE, $1); }
     | strs                                 { $$ = $1; }
     | tDOUBLE                              { $$ = new cdk::double_node(LINE, $1); }
     | lval %prec tLVAL                     { $$ = new cdk::rvalue_node(LINE, $1); }
     | expr tALLOC                          { $$ = new gr8::alloc_node(LINE, $1); }
     | lval '?'                             { $$ = new gr8::address_node(LINE, $1); }
     | '!' expr                             { $$ = new cdk::not_node(LINE, $2); }
     | '-' expr %prec tUNARY                { $$ = new cdk::neg_node(LINE, $2); }
     | '+' expr %prec tUNARY                { $$ = new gr8::identity_node(LINE, $2); }
     | expr '+' expr                        { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr                        { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr                        { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr                        { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr                        { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '<' expr                        { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr                        { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr '=' expr                        { $$ = new cdk::eq_node(LINE, $1, $3); }
     | expr '&' expr                        { $$ = new cdk::and_node(LINE, $1, $3); }
     | expr '|' expr                        { $$ = new cdk::or_node(LINE, $1, $3); }
     | '(' expr ')'                         { $$ = $2; }
     | tUSE exprs tFOR tIDENTIFIER          { $$ = new gr8::func_call_node(LINE, $4, new cdk::sequence_node(LINE, $2), new basic_type());}
     | tDO tIDENTIFIER                      { $$ = new gr8::func_call_node(LINE, $2, new cdk::sequence_node(LINE), new basic_type()); }
     | tREAD                                { $$ = new gr8::read_node(LINE); }
     | tNULL                                { $$ = new gr8::null_ptr_node(LINE); }
     ;

lval : tIDENTIFIER                                     { $$ = new cdk::identifier_node(LINE, $1); }
     | tINDEXNUM expr tINDEXARR expr %prec tEXPR       { $$ = new gr8::indexing_node(LINE, $4, $2); }
     ;

strs : tSTRING                             { $$ = new cdk::string_node(LINE, $1); }
     | strs tSTRING                        { $$ = new cdk::string_node(LINE, $1->value() + *$2); delete $1; }
     ;

%%
