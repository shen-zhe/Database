#ifndef SCANNER_H
#define SCANNER_H


#if ! defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer ParserModule_FlexLexer // the trick with prefix; no namespace here :(
#include <FlexLexer.h>
#endif


#undef YY_DECL
#define YY_DECL ParserModule::Parser::symbol_type ParserModule::Scanner::get_next_token()

#include "parser.hpp" // this is needed for symbol_type

namespace ParserModule {


class Interpreter; 
    
class Scanner : public yyFlexLexer {
public:
        Scanner(Interpreter &driver) : m_driver(driver) {}
	virtual ~Scanner() {}
	virtual ParserModule::Parser::symbol_type get_next_token();
        
private:
    Interpreter &m_driver;
};

}

#endif