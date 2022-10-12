
    /*********************************************************************
       Name: Brennan Basinger
       Purpose of File: Necessary driver.cpp file for building a parse tree from given input
     *********************************************************************/


#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include <string>
#include <set>
#include <vector>

using namespace std;

#include "lexer.h"
#include "productions.h"
#include "parse_tree_nodes.h"

extern "C" {
    // Instantiate global variables
    extern FILE *yyin; // input stream
    extern FILE *yyout; // output stream
    extern int yyleng; // length of current lexeme
    extern char *yytext; // text of current lexeme
    extern int yylineno; // line number for current lexeme
    extern int yylex(); // the generated lexical analyzer
}

// Needed global variables
extern int nextToken; // token returned from yylex

set<string> symbolTable; // Symbol Table

void printThis(int x)
{
    if(x==1)
        cout << endl << endl << "*** In order traversal of parse tree ***" << endl;
    else
        cout << endl << endl << "*** Delete the parse tree ***" << endl;
}

//*****************************************************************************
// The main processing loop
int main(int argc, char* argv[])
{
    // Set the input stream
    if (argc > 1)
    {
        printf("INFO: Using the %s file for input\n", argv[1]);
        yyin = fopen(argv[1], "r");
    }
    else
    {
        printf("INFO: Using the sample.pas file for input\n");
        yyin = fopen("sample.pas", "r");
    }
    if (!yyin)
    {
        printf("ERROR: input file not found\n");
        return EXIT_FAILURE;
    }

    // Set the output stream
    yyout = stdout;
    // Get the first token
    nextToken = yylex();

    ProgNode* prog = new ProgNode();

    // Fire up the parser!
    try {

        program(prog);  // Process <program> production

        if (nextToken != TOK_EOF)
            throw "end of file expected, but there is more here!";

    } catch (char const *errmsg) {
        cout << endl << "***ERROR:" << endl;
        cout << "On line number " << yylineno << ", near " << yytext << ", error type ";
        cout << errmsg << endl;
        return EXIT_FAILURE;
    }

    // Tell the world about our success!!
    cout << endl << "=== GO BULLDOGS! Your parse was successful! ===" << endl;
    // Print out the symbol table
    cout << endl << "User Defined Symbols:" << endl;
    set<string>::iterator it_erator;
    for (it_erator = symbolTable.begin(); it_erator != symbolTable.end(); ++it_erator)
    {
        cout << *it_erator << endl;
    }

    printThis(1);
    cout << *prog;
    printThis(2);

    delete prog;
    prog = nullptr;

    return EXIT_SUCCESS;
}
