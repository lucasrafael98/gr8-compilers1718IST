%option c++ prefix="gr8_scanner_" outfile="gr8_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <stack>
#include <vector>
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "gr8_parser.tab.h"

// don't change this
#define yyerror LexerError

//For indentation processing:
bool shouldEnd = false;
std::vector<int> st;
std::stack<char> indent;
int braces_toClose = 0;
int count_indents(const char* str){
	int count = 0;
	for(int i = 0; str[i] != '\0' && (str[i] == ' ' || str[i] == '\t'); i++){
		if(str[i] == ' ')
			count++;
		if(str[i] == '\t')
			count += 8 - (count % 8);
	}
	return count;
}
%}

ASCII 		(51[0-3])|(50[0-6])|([0-4]?[0-6]?[0-6])
FLOAT 		((([0-9]+"."[0-9]*)|([0-9]*"."[0-9]+))("E""-"?[0-9]+)?)|([0-9]+"E""-"?[0-9]+)

%x X_STRING X_COMMENT X_NOINDENT X_CLOSE
%%

						if(!indent.empty()){
						 	char toReturn = indent.top();
						 	indent.pop();
						 	return toReturn;
					 	}

"!!".*$                 ; /* ignore comments */

^\n 					; /* ignore empty lines */
^[ \t]*$				; /* ignore empty lines */

^.*$					{
							if(st.empty()){ //case for first line
								if(count_indents(yytext) != 0){ //we don't want the first line to be indented improperly!
									yyerror("Wrong indentation on 1st line");
								}
								st.push_back(0);
								yyless(0);
							}
							else{
								int cnt = count_indents(yytext);
								if(cnt > st.back()){
									st.push_back(cnt);
									yyless(0);
									return'{';
								}
								else if(cnt == st.back()){
									yyless(0);
									return ';'; 
								}
								else if(cnt < st.back()){
									while(cnt != st.back()){
										if(cnt < st.back()){
											st.pop_back();
											indent.push('}');
										}
										else if(cnt > st.back()){
											yyerror("Wrong indentation!");
										}
									}
									yyless(0);
									return ';';
								}
							}
						}

"..."$					yy_push_state(X_NOINDENT);
<X_NOINDENT>^[ \t]*		yy_pop_state();
<X_NOINDENT>^.			yy_pop_state(); yyless(0);
<X_NOINDENT>\n 			;



"small"					return tINTDECL;
"huge"					return tDBLDECL; 	/*yuge!*/
"news"					return tSTRDECL;
"fake"					return tPOINTER;

"use"					return tUSE;
"public"				return tPUBLIC;
"define"				return tDEF;
"procedure"				return tVFUNC;		/* V means "void" here, since it's a procedure. */
"function"				return tFUNC;
"on"					return tON;
"as"					return tAS;
"do"					return tDO;
"uses"					return tUSES; 		
"for"					return tFOR;
"return"				return tRETURN;

"plus"					return '+';
"minus"					return '-';
"times"					return '*';
"over"					return '/';
"modulus"				return '%';
"not"					return '!';
"and"					return '&';
"or"					return '|';
"above"					return '>';
"below"					return '<';
"equals"				return '=';

"if"					return tIF;
"then"					return tTHEN;
"else"					return tELSE;
"elsif"					return tELIF;
"stop"					return tBREAK;
"again"					return tCONTINUE;
"sweeping"				return tSWEEP;
"from"					return tFROM;
"by"					return tBY;

"initially"				return tINIT;
"assign"				return tASSIGN;
"to"					return tTO;
"cell"					return tINDEXNUM; 	/* index number */
"at"					return tINDEXARR; 	/* array to be indexed: "cell 2 at vec" = "vec[2]". */ 
"input"					return tREAD;
"tweet"					return tPRINT;
"post"					return tPRINTLN;	/* println because it's similar to Java's println(). */
"objects" 				return tALLOC;
"null" 					return tNULL;


[A-Za-z][A-Za-z0-9-]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

\"                    	yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"           	yy_pop_state(); return tSTRING;
<X_STRING>"~"\"         *yylval.s += yytext + 1;
<X_STRING>"~~"			*yylval.s += '~';
<X_STRING>"~n"			*yylval.s += '\n';
<X_STRING>"~t"			*yylval.s += '\t';
<X_STRING>"~r"			*yylval.s += '\r';
<X_STRING>"~0"			*yylval.s += '\0';
<X_STRING>"~"{ASCII}	*yylval.s += (char) strtol((yytext+1), nullptr, 7);
<X_STRING>"~".			yyerror("illegal character");
<X_STRING>" ..."\n 		; /* ignore newline */
<X_STRING>.            	*yylval.s += yytext;
<X_STRING>\n           	yyerror("newline in string");

"<<"				   	yy_push_state(X_COMMENT);
<X_COMMENT>"<<"	   	   	yy_push_state(X_COMMENT);
<X_COMMENT>">>"  	   	yy_pop_state();
<X_COMMENT>.|\n	   		; /*ignore comments */

[0-6]+                 	{
							long i = strtol(yytext, nullptr, 7);
							if(i != (int) strtol(std::to_string(i).c_str(), nullptr, 10))
								yyerror("Integer overflow");
							else
								yylval.i = i;
						   	return tINTEGER;
						}

{FLOAT}					yylval.d = strtod(yytext, nullptr); return tDOUBLE;

[~(),?]       			return *yytext; /* single chars that should be passed to yacc as-is.*/

<X_CLOSE><<EOF>>		{ 	
							if(!indent.empty()){
							 	char toReturn = indent.top();
							 	indent.pop();
							 	yy_pop_state();
							 	yy_push_state(X_CLOSE);
							 	return toReturn;
						 	}
						 	else{
								yy_pop_state();
								shouldEnd = true;
							}
						}

<<EOF>>					{
							if(shouldEnd){
								return 0;
							}
							int cnt = 0;
							if(cnt == st.back()){
								shouldEnd = true;
								return ';'; 
							}
							if(cnt < st.back()){
								while(cnt != st.back()){
									if(cnt < st.back()){
										st.pop_back();
										indent.push('}');
									}
									else if(cnt > st.back()){
										yyerror("Wrong indentation!");
									}
								}
								indent.push(';');
							}
							yy_push_state(X_CLOSE);
						}

\n|[ \t]+               	; /* ignore whitespace */

[7-9]+ 					yyerror("Non-Base 7 character");

.                      	yyerror("Unknown character");

%%
