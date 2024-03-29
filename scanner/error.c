/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

// Thong bao loi
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

// Nhan vao ma loi, so dong, vi tri
void error(ErrorCode err, int lineNo, int colNo)
{
  switch (err)
  {
  case ERR_ENDOFCOMMENT:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_ENDOFCOMMENT);
    break;
  case ERR_IDENTTOOLONG:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_IDENTTOOLONG);
    break;
  case ERR_INVALIDCHARCONSTANT:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDCHARCONSTANT);
    break;
  case ERR_INVALIDSYMBOL:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDSYMBOL);
    break;
  case ERR_INVALIDIDENT:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDIDENT);
    break;
  case ERR_NUMBERTOOBIG:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_NUMBERTOOBIG);
    break;
  }
  exit(-1);
}
