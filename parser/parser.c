/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 2.0
 */

#include <stdlib.h>
#include <stdio.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void)
{
  Token *tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType)
{
  if (lookAhead->tokenType == tokenType)
  {
    printToken(lookAhead);
    scan();
    if (lookAhead->lineNo > currentToken->lineNo)
    {
      printf("\n");
    }
  }
  else
    missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void)
{
  // assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  // assert("Program parsed!");
}

void compileBlock(void)
{
  // assert("Parsing a Block ....");
  if (lookAhead->tokenType == KW_CONST)
  {
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileBlock2();
  }
  else
    compileBlock2();
  // assert("Block parsed!");
}

void compileBlock2(void)
{
  if (lookAhead->tokenType == KW_TYPE)
  {
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileBlock3();
  }
  else
    compileBlock3();
}

void compileBlock3(void)
{
  if (lookAhead->tokenType == KW_VAR)
  {
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileBlock4();
  }
  else
    compileBlock4();
}

void compileBlock4(void)
{
  compileSubDecls();
  compileBlock5();
}

void compileBlock5(void)
{
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

void compileConstDecls(void)
{
  // TODO
  while (lookAhead->tokenType == TK_IDENT)
    compileConstDecl();
}

void compileConstDecl(void)
{
  // TODO
  eat(TK_IDENT);
  eat(SB_EQ);
  compileConstant();
  eat(SB_SEMICOLON);
}

void compileTypeDecls(void)
{
  // TODO
  while (lookAhead->tokenType == TK_IDENT)
    compileTypeDecl();
}

void compileTypeDecl(void)
{
  // TODO
  eat(TK_IDENT);
  eat(SB_EQ);
  compileType();
  eat(SB_SEMICOLON);
}

void compileMultiVar(void)
{
  eat(TK_IDENT);
  if (lookAhead->tokenType == SB_EQ)
  {
    eat(SB_EQ);
    eat(TK_NUMBER);
  }
  if (lookAhead->tokenType == SB_COMMA)
  {
    eat(SB_COMMA);
    compileMultiVar();
  }
}

void compileVarDecls(void)
{
  // TODO
  while (lookAhead->tokenType == TK_IDENT)
    compileVarDecl();
}

void compileVarDecl(void)
{
  // TODO
  // For test
  compileMultiVar();
  eat(SB_COLON);
  compileType();
  eat(SB_SEMICOLON);
}

void compileSubDecls(void)
{
  // assert("Parsing subtoutines ....");
  // TODO
  while (lookAhead->tokenType == KW_PROCEDURE || lookAhead->tokenType == KW_FUNCTION)
  {
    if (lookAhead->tokenType == KW_PROCEDURE)
      compileProcDecl();
    else if (lookAhead->tokenType == KW_FUNCTION)
      compileFuncDecl();
  }
  // assert("Subtoutines parsed ....");
}

void compileFuncDecl(void)
{
  // assert("Parsing a function ....");
  // TODO
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParams();
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  // assert("Function parsed ....");
}

void compileProcDecl(void)
{
  // assert("Parsing a procedure ....");
  // TODO
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParams();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  // assert("Procedure parsed ....");
}

void compileUnsignedConstant(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  default:
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileConstant(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case SB_PLUS:
    eat(SB_PLUS);
    compileConstant2();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileConstant2();
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  default:
    compileConstant2();
    break;
  }
}

void compileConstant2(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  default:
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileType(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case KW_ARRAY:
    eat(KW_ARRAY);
    eat(SB_LSEL);
    eat(TK_NUMBER);
    eat(SB_RSEL);
    eat(KW_OF);
    compileType();
    break;
  default:
    compileBasicType();
    break;
  }
}

void compileBasicType(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case KW_INTEGER:
    eat(KW_INTEGER);
    break;
  case KW_CHAR:
    eat(KW_CHAR);
    break;
  default:
    error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileParams(void)
{
  // TODO
  if (lookAhead->tokenType == SB_LPAR)
  {
    eat(SB_LPAR);
    compileParam();
    compileParams2();
    eat(SB_RPAR);
  }
}

void compileParams2(void)
{
  if (lookAhead->tokenType == SB_SEMICOLON)
  {
    eat(SB_SEMICOLON);
    compileParam();
    compileParams2();
  }
}

void compileParam(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case KW_VAR:
    eat(KW_VAR);
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
    break;
  default:
    error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileStatements(void)
{
  // TODO
  compileStatement();
  compileStatements2();
}

void compileStatements2(void)
{
  // TODO
  if (lookAhead->tokenType == SB_SEMICOLON)
  {
    eat(SB_SEMICOLON);
    compileStatement();
    compileStatements2();
  }
}

void compileMultiAssign(void)
{
  eat(SB_LPAR);
  while (lookAhead->tokenType != SB_RPAR)
  {
    eat(TK_IDENT);
    if (lookAhead->tokenType == SB_COMMA)
      eat(SB_COMMA);
  }
  eat(SB_RPAR);
  eat(SB_ASSIGN);
  eat(SB_LPAR);
  while (lookAhead->tokenType != SB_RPAR)
  {
    compileExpression();
    if (lookAhead->tokenType == SB_COMMA)
    {
      eat(SB_COMMA);
    }
  }
  eat(SB_RPAR);
}

void compileStatement(void)
{
  // I have some bug here
  switch (lookAhead->tokenType)
  {
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
  case SB_LPAR:
    compileMultiAssign();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileConditionAssign(void)
{
  switch (lookAhead->tokenType)
  {
  case SB_GT:
    eat(SB_GT);
    break;
  case SB_LT:
    eat(SB_LT);
    break;
  case SB_LE:
    eat(SB_LE);
    break;
  case SB_GE:
    eat(SB_GE);
    break;
  default:
    break;
  }
  eat(TK_IDENT);
  eat(SB_QUESTION);
  compileExpression();
  eat(SB_COLON);
  compileExpression();
}

void compileAssignSt(void)
{
  // assert("Parsing an assign statement ....");
  // TODO
  eat(TK_IDENT);
  if (lookAhead->tokenType == SB_LSEL)
    compileIndexes();
  eat(SB_ASSIGN);
  compileExpression();
  // assert("Assign statement parsed ....");
}

void compileCallSt(void)
{
  // assert("Parsing a call statement ....");
  // TODO
  eat(KW_CALL);
  eat(TK_IDENT);
  compileArguments();
  // assert("Call statement parsed ....");
}

void compileGroupSt(void)
{
  // assert("Parsing a group statement ....");
  // TODO
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  // assert("Group statement parsed ....");
}

void compileIfSt(void)
{
  // assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE)
    compileElseSt();
  // assert("If statement parsed ....");
}

void compileElseSt(void)
{
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void)
{
  // assert("Parsing a while statement ....");
  // TODO
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  // assert("While statement pased ....");
}

void compileAnotherFor(void)
{
  eat(SB_LPAR);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  eat(TK_NUMBER);
  eat(SB_SEMICOLON);
  eat(TK_IDENT);
  switch (lookAhead->tokenType)
  {
  case SB_LT:
    eat(SB_LT);
    break;
  case SB_LE:
    eat(SB_LE);
    break;
  case SB_GT:
    eat(SB_GT);
    break;
  case SB_GE:
    eat(SB_GE);
    break;
  default:
    eat(SB_LT);
    break;
  }
  eat(TK_NUMBER);
  eat(SB_SEMICOLON);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(SB_RPAR);
}

void compileForSt(void)
{
  // assert("Parsing a for statement ....");
  // TODO
  eat(KW_FOR);
  if (lookAhead->tokenType == SB_LPAR)
    compileAnotherFor();
  else
  {
    eat(TK_IDENT);
    eat(SB_ASSIGN);
    compileExpression();
    eat(KW_TO);
    compileExpression();
    eat(KW_DO);
  }
  compileStatement();
  // assert("For statement parsed ....");
}

void compileArguments(void)
{
  // TODO
  if (lookAhead->tokenType == SB_LPAR)
  {
    eat(SB_LPAR);
    compileExpression();
    compileArguments2();
    eat(SB_RPAR);
  }
}

void compileArguments2(void)
{
  // TODO
  if (lookAhead->tokenType == SB_COMMA)
  {
    eat(SB_COMMA);
    compileExpression();
    compileArguments2();
  }
}

void compileCondition(void)
{
  // TODO
  compileExpression();
  compileCondition2();
}

void compileCondition2(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case SB_EQ:
    eat(SB_EQ);
    break;
  case SB_NEQ:
    eat(SB_NEQ);
    break;
  case SB_LE:
    eat(SB_LE);
    break;
  case SB_LT:
    eat(SB_LT);
    break;
  case SB_GE:
    eat(SB_GE);
    break;
  case SB_GT:
    eat(SB_GT);
    break;
  default:
    break;
  }
  compileExpression();
}

void compileExpression(void)
{
  // assert("Parsing an expression");
  // TODO
  switch (lookAhead->tokenType)
  {
  case SB_PLUS:
    eat(SB_PLUS);
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    break;
  // Condition assign
  default:
    break;
  }
  compileExpression2();
  // assert("Expression parsed");
}

void compileExpression2(void)
{
  // TODO
  compileTerm();
  compileExpression3();
}

void compileExpression3(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case SB_PLUS:
    eat(SB_PLUS);
    compileTerm();
    compileExpression3();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileTerm();
    compileExpression3();
    break;
  default:
    break;
  }
}

void compileTerm(void)
{
  // TODO
  compileFactor();
  compileTerm2();
}

void compileTerm2(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case SB_TIMES:
    eat(SB_TIMES);
    compileFactor();
    compileTerm2();
    break;
  case SB_SLASH:
    eat(SB_SLASH);
    compileFactor();
    compileTerm2();
    break;
  default:
    break;
  }
}

void compileFactor(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    switch (lookAhead->tokenType)
    {
    case SB_LSEL:
      compileIndexes();
      break;
    case SB_LPAR:
      compileArguments();
      break;
    // For condition assign
    default:
      break;
    }
    break;
  case SB_LPAR:
    eat(SB_LPAR);
    compileExpression();
    eat(SB_RPAR);
    break;
  default:
    error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

// For another array type
void compileAnotherArray(void)
{
  while (lookAhead->tokenType != SB_RSEL)
  {
    compileExpression();
    if (lookAhead->tokenType == SB_COMMA)
      eat(SB_COMMA);
  }
}

void compileIndexes(void)
{
  // TODO
  if (lookAhead->tokenType == SB_LSEL)
  {
    eat(SB_LSEL);
    // compileExpression();
    compileAnotherArray();
    eat(SB_RSEL);
    compileIndexes();
  }
}

int compile(char *fileName)
{
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;
}
