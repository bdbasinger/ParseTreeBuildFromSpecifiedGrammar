/*********************************************************************
	Name: Brennan Basinger
	Purpose of File: Includes the node classes required to build the parse tree
*********************************************************************/

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"

using namespace std;

// Forward declaration of <expr> node
class ExprNode;
class BlockNode;
class ProgNode;
class CS_Node;

// Forward declaration of operator<< for ExprNode
ostream& operator<<(ostream& os, ExprNode& en);
ostream& operator<<(ostream& os, ProgNode& pn);
ostream& operator<<(ostream& os, BlockNode& bn);
ostream& operator<<(ostream& os, CS_Node& cn);


// Abstract class
// Base class for IdNode, IntLitNode, NestedExprNode.
class FactorNode
{
	public:
		// pure virtual method, makes the class Abstract
    	virtual void printTo(ostream &os) = 0;
    	// labeling the destructor as virtual allows
		// the subclass destructors to be called
		virtual ~FactorNode();
};
FactorNode::~FactorNode() {}
// Uses double dispatch to call the overloaded method printTo in the
// FactorNodes: IdNode, IntLitNode, and NestedExprNode
ostream& operator<<(ostream& os, FactorNode& fn)
{
	//os << "(Factor: ";
	fn.printTo(os);
	//os << ") ";
	return os;
}

// Abstract Class
class Statement
{
	public:
		// pure virtual method, makes the class Abstract
    	virtual void printTo(ostream &os) = 0;
		// labeling the destructor as virtual allows
		// the subclass destructors to be called
    	virtual ~Statement();
};
Statement::~Statement() {}
ostream& operator<<(ostream& os, Statement& sn)
{
	sn.printTo(os);
	return os;
}

// TermNode Class
class TermNode
{
	public:
    	FactorNode* _1stFactor = nullptr;
    	vector<int> restFactorOps;
    	vector<FactorNode*> restFactors;
   		~TermNode();
};
ostream& operator<<(ostream& os, TermNode& tnode)
{
	os << "term( " << *(tnode._1stFactor);
	int l = tnode.restFactorOps.size();
	for (int x=0; x<l; ++x)
	{
		int AndMultDiv = tnode.restFactorOps[x];
		switch (AndMultDiv)
		{
			case TOK_AND:
				os << "AND ";
				break;
			case TOK_MULTIPLY:
				os << "* ";
				break;
			case TOK_DIVIDE:
				os << "/ ";
				break;
		}
		os << *(tnode.restFactors[x]);
	}
	os << ") ";
	return os;
}
TermNode::~TermNode()
{
	if (_1stFactor != nullptr)
	{
		cout << "Deleting a termNode"<< endl;
	}
	delete _1stFactor;
	_1stFactor = nullptr;
	int l = restFactorOps.size();
	for (int x = 0; x < l; ++x)
	{
		delete restFactors[x];
		restFactors[x] = nullptr;
	}
}

// SimpleExpressionNode Class
class SimpleExpressionNode
{
	public:
    	TermNode* _1stTerm = nullptr;
    	vector<int> restTermOps;
    	vector<TermNode*> restTerms;
    	~SimpleExpressionNode();
};
ostream& operator<<(ostream& os, SimpleExpressionNode& simpNode)
{
	os << "simple_expression( ";
	os << *(simpNode._1stTerm);
	int l = simpNode.restTermOps.size();
	for (int x=0; x<l; ++x)
	{
		int OrPlusMinus = simpNode.restTermOps[x];
		switch (OrPlusMinus)
		{
			case TOK_OR:
				os << "OR ";
				break;
			case TOK_PLUS:
				os << "+ ";
				break;
			case TOK_MINUS:
				os << "- ";
				break;
		}
		os << *(simpNode.restTerms[x]);
	}
	os << ") ";
	return os;
}
SimpleExpressionNode::~SimpleExpressionNode()
{
	cout << "Deleting a simpleExpressionNode" << endl;
	delete _1stTerm;
	_1stTerm = nullptr;
	int l = restTermOps.size();
	for (int x=0; x<l; ++x)
	{
		delete restTerms[x];
		restTerms[x] = nullptr;
	}
}

// ExprNode Class
class ExprNode
{
	public:
    	SimpleExpressionNode* _1stSimple = nullptr;
    	vector<int> restTermOps;
    	vector<SimpleExpressionNode*> restOfSimpleTerms;
		~ExprNode();
};
ostream& operator<<(ostream& os, ExprNode& en)
{
	os << "(expression: ";
	os << *(en._1stSimple);
	int l = en.restTermOps.size();
	for (int x=0; x<l; ++x)
	{
		int equal_NotEqual_LessThan_GreaterThan = en.restTermOps[x];
		switch (equal_NotEqual_LessThan_GreaterThan)
		{
			case TOK_EQUALTO:
				os << "= ";
				break;
			case TOK_NOTEQUALTO:
				os << "<> ";
				break;
			case TOK_LESSTHAN:
				os << "< ";
				break;
			case TOK_GREATERTHAN:
				os << "> ";
				break;
		}
		os << *(en.restOfSimpleTerms[x]);
	}
	os << ")";
	return os;
}
ExprNode::~ExprNode()
{
	cout << "Deleting an expressionNode" << endl;
	delete _1stSimple;
	_1stSimple = nullptr;

	int l = restTermOps.size();
	for (int x=0; x<l; ++x)
	{
		delete restOfSimpleTerms[x];
		restOfSimpleTerms[x] = nullptr;
	}
}

// Nested ExprNode Class
class NestedExprNode : public FactorNode
{
	public:
   		ExprNode* expressionPTR = nullptr;

    	NestedExprNode(ExprNode* e);
    	void printTo(ostream& os);
    	~NestedExprNode();
};
NestedExprNode::NestedExprNode(ExprNode* e)
{
	expressionPTR = e;
}
void NestedExprNode::printTo(ostream& os)
{
	os << "nested_expression( ";
	os << *expressionPTR;
	os << " ) ";
}
NestedExprNode::~NestedExprNode()
{
	cout << "Deleting a factorNode" << endl;
	delete expressionPTR;
	expressionPTR = nullptr;
}


// IdNode Class
class IdNode : public FactorNode
{
	public:
    	string* id = nullptr;
    	IdNode(string name);
    	~IdNode();
    	void printTo(ostream &os);
};
IdNode::IdNode(string name)
{
	id = new string(name);
}
IdNode::~IdNode()
{
	cout << "Deleting a factorNode" << endl;
	delete id;
	id = nullptr;
}
void IdNode::printTo(ostream &os)
{
	os << "factor( : " << *id << " ) ";
}

// IntLitNode Class
class IntLitNode : public FactorNode
{
	public:
    	int int_literal = 0;
    	IntLitNode(string value);
    	~IntLitNode();
    	void printTo(ostream & os);
};
IntLitNode::IntLitNode(string value)
{
	int_literal = stoi(value);
}
IntLitNode::~IntLitNode()
{
	cout << "Deleting a factorNode" << endl;
}
void IntLitNode::printTo(ostream& os)
{
	os << "factor( " << int_literal << " ) ";
}

// FloatLitNode Class
class FloatLitNode : public FactorNode
{
	public:
		float f_lit = 0;
		FloatLitNode(string value);
		~FloatLitNode();
		void printTo(ostream & os);
};
FloatLitNode::FloatLitNode(string value)
{
	f_lit = std::stof(value);
}
FloatLitNode::~FloatLitNode()
{
	cout << "Deleting a factorNode" << endl;
}
void FloatLitNode::printTo(ostream& os)
{
	os << "factor( " << f_lit << " ) ";
}

// NoFactor Class
class NoFactor : public FactorNode
{
	public:
		bool tru;
    	FactorNode* fPTR = nullptr;
		NoFactor(FactorNode* en, bool tru);
    	void printTo(ostream & os);
    	~NoFactor();
};
// Constructor
NoFactor::NoFactor(FactorNode* fnode, bool x)
{
	tru = x;
	fPTR = fnode;
}
void NoFactor::printTo(ostream& os)
{
	os << "factor( ";
	if (tru)
	{
		os << "- ";
	}
	else
	{
		os << "NOT ";
	}
	os << *fPTR << ") ";
}
//Destructor
NoFactor::~NoFactor()
{
	cout << "Deleting a factorNode" << endl;
	delete fPTR;
	fPTR = nullptr;
}

// Assignment Class
class Assignment_S : public Statement
{
	public:
		string* n;
		ExprNode* expression;

		~Assignment_S();

		void printTo(ostream &os)
		{
			os << "Assignment " << *n << " := ";
			os << *expression << endl;
		}
};
Assignment_S::~Assignment_S()
{
	cout << "Deleting an assignmentNode" << endl;
	delete expression;
	expression = nullptr;
	delete n;
	n = nullptr;
}


// If Class
class If : public Statement
{
	public:
		bool _else_is_here;
		ExprNode* expression;
		Statement* _else;
		Statement* _then;
		~If();

		void printTo(ostream &os)
		{
        	os << "If " << *expression << endl;
			os << "%%%%%%%% True Statement %%%%%%%%" << endl;
			os << *_then << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;;
			if (_else_is_here)
			{
				os << "%%%%%%%% False Statement %%%%%%%%" << endl;
				os << *_else << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;;
			}
		}
};

If::~If()
{
	cout << "Deleting a ifNode" << endl;
	delete expression;
	expression = nullptr;
	delete _then;
	_then = nullptr;
	if (_else_is_here)
	{
		delete _else;
		_else = nullptr;
	}
}

// While CLass
class While : public Statement
{
	public:
		ExprNode* expression;
		Statement* bodyOfLoop;
		~While();
		void printTo(ostream &os)
		{
        os << "While " << *expression << endl;
		os << "%%%%%%%% Loop Body %%%%%%%%" << endl;
		os << *bodyOfLoop << "%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
		}
};

While::~While()
{
	cout << "Deleting a whileNode" << endl;
	delete expression;
	expression = nullptr;
	delete bodyOfLoop;
	bodyOfLoop = nullptr;
}

// Read Class
class Read : public Statement
{
	public:
		string* id;
		~Read();
		void printTo(ostream &os)
		{
			os << "Read Value " << *id << endl;
		}
};
Read::~Read()
{
	cout << "Deleting a readNode" << endl;
	delete id;
	id = nullptr;
}

// Write Class
class Write : public Statement
{
	public:
		string* id;
		bool tru;
		~Write();
		void printTo(ostream &os)
		{
			if(tru)
			{
				os << "Write String " << *id << endl;
			}
			else
			{
				os << "Write Value " << *id << endl;
			}
		}
};
Write::~Write()
{
	cout << "Deleting a writeNode" << endl;
	delete id;
	id = nullptr;
}

// Compound Statement Class
class CS_Node : public Statement
{
	public:
		vector<Statement*> _s;
		~CS_Node();
		void printTo(ostream &os)
		{
			os << *this;
		}
};
CS_Node::~CS_Node()
{
	cout << "Deleting a compoundNode" << endl;
	int l = _s.size();
	for (int x=0; x<l; ++x)
	{
		delete _s[x];
		_s[x] = nullptr;
	}
}
ostream& operator<<(ostream& os, CS_Node& cn)
{
	os << "Begin Compound Statement" << endl;
	int l = cn._s.size();
	for (int x=0; x<l; ++x)
	{
		os << *(cn._s[x]);
	}
	os << "End Compound Statement" << endl;
	return os;
}


// Block Node Class
class BlockNode
{
	public:
		Statement* compound;
		~BlockNode();
};
BlockNode::~BlockNode()
{
	cout << "Deleting a blockNode" << endl;
	delete compound;
	compound = nullptr;
}
ostream& operator<<(ostream& os, BlockNode& bn)
{
	os << *(bn.compound);
	return os;
}


//Program Node Class
class ProgNode
{
	public:
		BlockNode* blockNode = nullptr;
		string* n;
		~ProgNode();
};
ProgNode::~ProgNode()
{
	if(blockNode != nullptr)
	{
		cout << "Deleting a programNode" << endl;
	}
		delete blockNode;
		blockNode = nullptr;
		delete n;
		n = nullptr;
}
ostream& operator<<(ostream& os, ProgNode& pn)
{
	os << "Program Name ";
	os << *(pn.n);
	cout << endl;
	os << *(pn.blockNode);
	return os;
}


#endif /* PARSE_TREE_NODES_H */
