%{
	#include <iostream>
	#include <cstdlib>
	#include "scanner.h"
	#include "interpreter.h"
	#include "parser.hpp"
	#include "location.hh"

	using namespace std;

	// Original yyterminate() macro returns int. Since we're using Bison 3 variants
	// as tokens, we must redefine it to change type from `int` to `Parser::semantic_type`
	#define yyterminate() ParserModule::Parser::make_END(ParserModule::location());

	// This will track current scanner location.
	// Action is called when length of the token is known.
	#define YY_USER_ACTION m_driver.increaseLocation(yyleng);
	
	// !!!WARNING!!!
	// Location API is used, but the location is not initialized, 'cause I'm lazy. When making
	// a token with make_{something} method you can pass detailed token location. Current location
	// is accessible with m_driver.location() method. All puzzle elements are there - just
	// pass location value in every action code block below. I'm going to waste more time writing
	// this excuse than putting this boilerplate below...
	//
	// Location class can be found in location.hh and posistion.hh files. It's just a bit too much
	// boilerplate for this small example. Bummer.
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Scanner"
%option prefix="ParserModule_"

%%
[ \t\n]+                        ;
"select"                        {
                                    return ParserModule::Parser::make_SELECT(ParserModule::location());
                                }
"insert"                        {
									//std::cout << "Got INSERT" << std::endl;
                                    return ParserModule::Parser::make_INSERT(ParserModule::location());
                                }
"delete"                        {
                                    return ParserModule::Parser::make_DELETE(ParserModule::location());
                                }
"name"                          {
									//std::cout << "Got first_name" << std::endl;
                                    return ParserModule::Parser::make_PARAM(yytext, ParserModule::location());
                                }
"surname"                       {
									//std::cout << "Got last_name" << std::endl;
                                    return ParserModule::Parser::make_PARAM(yytext, ParserModule::location());
                                }
"patronymic"                          {
									//std::cout << "Got city" << std::endl;
                                    return ParserModule::Parser::make_PARAM(yytext, ParserModule::location());
                                }
"gender"                         {
									//std::cout << "Got group" << std::endl;
                                    return ParserModule::Parser::make_PARAM(yytext, ParserModule::location());
                                }
"age"                           {
									//std::cout << "Got city" << std::endl;
                                    return ParserModule::Parser::make_PARAM(yytext, ParserModule::location());
                                }
"number"                          {
									//std::cout << "Got city" << std::endl;
                                    return ParserModule::Parser::make_PARAM(yytext, ParserModule::location());
                                }
"city"                          {
									//std::cout << "Got city" << std::endl;
                                    return ParserModule::Parser::make_PARAM(yytext, ParserModule::location());
                                }
"sec"                          {
									//std::cout << "Got city" << std::endl;
                                    return ParserModule::Parser::make_PARAM(yytext, ParserModule::location());
                                }
";"                             {
									//std::cout << "Got semicolon" << std::endl;
                                    return ParserModule::Parser::make_SEMICOLON(ParserModule::location());
                                }
"="                             {   //std::cout << "Got equal" << std::endl;
                                    return ParserModule::Parser::make_EQ(ParserModule::location());
                                }
"all"                           {	//std::cout << "Got all" << std::endl;
                                    return ParserModule::Parser::make_ALL(ParserModule::location());
                                }
<<EOF>>     					{	//std::cout << "EOF" << std::endl;
								   	return ParserModule::Parser::make_END(ParserModule::location()); }
[a-zA-Z0-9абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ]+                    {   //std::cout << "Got value = " << yytext << std::endl;
                                    return ParserModule::Parser::make_VALUE(yytext, ParserModule::location());
                                }

%%