#include <CExprI.h>
#include <cstdio>

class CExprParseImpl {
 public:
  CExprParseImpl() { }
 ~CExprParseImpl() { }

  CExprPTokenStack parseFile(const std::string &filename);
  CExprPTokenStack parseFile(FILE *fp);
  CExprPTokenStack parseFile(CFile &file);
  CExprPTokenStack parseLine(const std::string &str);

 private:
  bool           parseLine(CExprPTokenStack &stack, const std::string &line);
  bool           parseLine(CExprPTokenStack &stack, const std::string &line, uint &i);
  void           parseError(const std::string &msg, const std::string &line, uint i);

  bool           isNumber(const std::string &str, uint i);
  CExprPTokenPtr readNumber(const std::string &str, uint *i);
  CExprPTokenPtr readString(const std::string &str, uint *i);
  CExprPTokenPtr readOperator(const std::string &str, uint *i);
#ifdef GNUPLOT_EXPR
  CExprPTokenPtr readOperatorStr(const std::string &str, uint *i);
#endif
  CExprPTokenPtr readIdentifier(const std::string &str, uint *i);
#if 0
  CExprPTokenPtr readUnknown(const std::string &str, uint *i);
#endif

#if 0
  CExprPTokenPtr createUnknownToken();
#endif
  CExprPTokenPtr createIdentifierToken(const std::string &str);
  CExprPTokenPtr createOperatorToken(CExprOperatorPtr);
  CExprPTokenPtr createIntegerToken(long);
  CExprPTokenPtr createRealToken(double);
  CExprPTokenPtr createStringToken(const std::string &str);

 private:
  CExprPTokenStack ptoken_stack_;
};

//-----------

CExprParse::
CExprParse()
{
  impl_ = new CExprParseImpl;
}

CExprParse::
~CExprParse()
{
}

CExprPTokenStack
CExprParse::
parseFile(const std::string &filename)
{
  return impl_->parseFile(filename);
}

CExprPTokenStack
CExprParse::
parseFile(FILE *fp)
{
  return impl_->parseFile(fp);
}

CExprPTokenStack
CExprParse::
parseLine(const std::string &line)
{
  return impl_->parseLine(line);
}

//-----------

CExprPTokenStack
CExprParseImpl::
parseFile(const std::string &filename)
{
  CFile file(filename);

  return parseFile(file);
}

CExprPTokenStack
CExprParseImpl::
parseFile(FILE *fp)
{
  CFile file(fp);

  return parseFile(file);
}

CExprPTokenStack
CExprParseImpl::
parseFile(CFile &file)
{
  std::vector<std::string> lines;

  file.toLines(lines);

  ptoken_stack_.clear();

  uint num_lines = lines.size();

  for (uint i = 0; i < num_lines; i++)
    if (! parseLine(ptoken_stack_, lines[i]))
      return CExprPTokenStack();

  return ptoken_stack_;
}

CExprPTokenStack
CExprParseImpl::
parseLine(const std::string &line)
{
  ptoken_stack_.clear();

  bool flag = parseLine(ptoken_stack_, line);

  if (! flag)
    return CExprPTokenStack();

  return ptoken_stack_;
}

bool
CExprParseImpl::
parseLine(CExprPTokenStack &stack, const std::string &line)
{
  uint i = 0;

  return parseLine(stack, line, i);
}

bool
CExprParseImpl::
parseLine(CExprPTokenStack &stack, const std::string &line, uint &i)
{
  CExprPTokenPtr ptoken;
  CExprPTokenPtr lastPToken;

  CStrUtil::skipSpace(line, &i);

  while (i < line.size()) {
    CStrUtil::skipSpace(line, &i);

    if      (CExprOperator::isOperatorChar(line[i])) {
      CExprOpType lastOpType =
       (lastPToken.isValid() && lastPToken->getType() == CEXPR_PTOKEN_OPERATOR ?
        lastPToken->getOperator()->getType() : CEXPR_OP_UNKNOWN);

      if (lastOpType != CEXPR_OP_UNKNOWN) {
        if ((line[i] == '-' || line[i] == '+') && i < line.size() - 1 && isdigit(line[i + 1]))
          ptoken = readNumber(line, &i);
        else
          ptoken = readOperator(line, &i);
      }
      else
        ptoken = readOperator(line, &i);

#ifdef GNUPLOT_EXPR
      if (! ptoken.isValid() && line[i] == '.')
        ptoken = readNumber(line, &i);
#endif

      if (ptoken.isValid() && ptoken->getType() == CEXPR_PTOKEN_OPERATOR) {
        if      (ptoken->getOperator()->getType() == CEXPR_OP_OPEN_RBRACKET) {
          stack.addToken(ptoken);

          if (! parseLine(stack, line, i))
            return false;

          lastPToken = CExprPTokenPtr();

          continue;
        }
        else if (ptoken->getOperator()->getType() == CEXPR_OP_CLOSE_RBRACKET) {
          stack.addToken(ptoken);

          return true;
        }
      }
    }
#ifdef GNUPLOT_EXPR
    else if (CExprOperator::isOperatorStr(line[i])) {
      ptoken = readOperatorStr(line, &i);

      if (! ptoken.isValid())
        ptoken = readIdentifier(line, &i);
    }
#endif
    else if (isNumber(line, i))
      ptoken = readNumber(line, &i);
    else if (line[i] == '_' || isalpha(line[i]))
      ptoken = readIdentifier(line, &i);
    else if (line[i] == '\'' || line[i] == '\"')
      ptoken = readString(line, &i);
    else {
      parseError("Invalid Character", line, i);
      return false;
    }

    if (! ptoken.isValid()) {
      parseError("Invalid Token", line, i);
      return false;
    }

    stack.addToken(ptoken);

    lastPToken = ptoken;
  }

  return true;
}

void
CExprParseImpl::
parseError(const std::string &msg, const std::string &line, uint i)
{
  std::cerr << msg << " \"" << line.substr(0, i - 1) << "#" << line[i] << "#" <<
               line.substr(i + 1) << std::endl;

  for (uint j = 0; j < i; j++)
    std::cerr << " ";

  std::cerr << "^" << std::endl;
}

bool
CExprParseImpl::
isNumber(const std::string &str, uint i)
{
  if (i >= str.size())
    return false;

  if (isdigit(str[i]))
    return true;

  if (str[i] == '.') {
    i++;

    if (i >= str.size())
      return false;

    if (isdigit(str[i]))
      return true;

    return false;
  }

  if (str[i] == '+' || str[i] == '-') {
    i++;

    return isNumber(str, i);
  }

  return false;
}

CExprPTokenPtr
CExprParseImpl::
readNumber(const std::string &str, uint *i)
{
  long   integer = 0;
  double real    = 0.0;
  bool   is_int  = false;

  if (! CExprParseUtil::stringToNumber(str, i, integer, real, is_int))
    return CExprPTokenPtr();

  if (is_int)
    return createIntegerToken(integer);
  else
    return createRealToken(real);
}

CExprPTokenPtr
CExprParseImpl::
readString(const std::string &str, uint *i)
{
  std::string str1;

  if (str[*i] == '\'') {
    (*i)++;

    uint j = *i;

    while (*i < str.size()) {
      if      (str[*i] == '\\' && *i < str.size() - 1)
        (*i)++;
      else if (str[*i] == '\'')
        break;

      (*i)++;
    }

    if (*i >= str.size())
      return CExprPTokenPtr();

    str1 = str.substr(j, *i - j);

    (*i)++;
  }
  else {
    (*i)++;

    uint j = *i;

    while (*i < str.size()) {
      if      (str[*i] == '\\' && *i < str.size() - 1)
        (*i)++;
      else if (str[*i] == '\"')
        break;

      (*i)++;
    }

    if (*i >= str.size())
      return CExprPTokenPtr();

    str1 = str.substr(j, *i - j);

    (*i)++;
  }

  str1 = CStrUtil::replaceEscapeCodes(str1);

  return createStringToken(str1);
}

CExprPTokenPtr
CExprParseImpl::
readOperator(const std::string &str, uint *i)
{
  CExprOpType id = CEXPR_OP_UNKNOWN;

  switch (str[*i]) {
    case '(':
      (*i)++;
      id = CEXPR_OP_OPEN_RBRACKET;
      break;
    case ')':
      (*i)++;
      id = CEXPR_OP_CLOSE_RBRACKET;
      break;
    case '!':
      (*i)++;

      if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CEXPR_OP_NOT_EQUAL;
      }
      else
        id = CEXPR_OP_LOGICAL_NOT;

      break;
    case '~':
      (*i)++;

      if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CEXPR_OP_APPROX_EQUAL;
      }
      else
        id = CEXPR_OP_BIT_NOT;

      break;
    case '*':
      (*i)++;

      if      (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CEXPR_OP_TIMES_EQUALS;
      }
      else if (*i < str.size() && str[*i] == '*') {
        (*i)++;
        id = CEXPR_OP_POWER;
      }
      else
        id = CEXPR_OP_TIMES;

      break;
    case '/':
      (*i)++;

      if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CEXPR_OP_DIVIDE_EQUALS;
      }
      else
        id = CEXPR_OP_DIVIDE;

      break;
    case '%':
      (*i)++;

      if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CEXPR_OP_MODULUS_EQUALS;
      }
      else
        id = CEXPR_OP_MODULUS;

      break;
    case '+':
      (*i)++;

      if      (*i < str.size() && str[*i] == '+') {
        (*i)++;
        id = CEXPR_OP_INCREMENT;
      }
      else if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CEXPR_OP_PLUS_EQUALS;
      }
      else
        id = CEXPR_OP_PLUS;

      break;
    case '-':
      (*i)++;

      if      (*i < str.size() && str[*i] == '-') {
        (*i)++;
        id = CEXPR_OP_DECREMENT;
      }
      else if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CEXPR_OP_MINUS_EQUALS;
      }
      else
        id = CEXPR_OP_MINUS;

      break;
    case '<':
      (*i)++;

      if      (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CEXPR_OP_LESS_EQUAL;
      }
      else if (*i < str.size() && str[*i] == '<') {
        (*i)++;

        if (*i < str.size() && str[*i] == '=') {
          (*i)++;
          id = CEXPR_OP_BIT_LSHIFT_EQUALS;
        }
        else
          id = CEXPR_OP_BIT_LSHIFT;
      }
      else
        id = CEXPR_OP_LESS;

      break;
    case '>':
      (*i)++;

      if      (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CEXPR_OP_GREATER_EQUAL;
      }
      else if (*i < str.size() && str[*i] == '>') {
        (*i)++;

        if (*i < str.size() && str[*i] == '=') {
          (*i)++;
          id = CEXPR_OP_BIT_RSHIFT_EQUALS;
        }
        else
          id = CEXPR_OP_BIT_RSHIFT;
      }
      else
        id = CEXPR_OP_GREATER;

      break;
    case '=':
      (*i)++;

      if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CEXPR_OP_EQUAL;
      }
      else
        id = CEXPR_OP_EQUALS;

      break;
    case '&':
      (*i)++;

      if      (*i < str.size() && str[*i] == '&') {
        (*i)++;
        id = CEXPR_OP_LOGICAL_AND;
      }
      else if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CEXPR_OP_BIT_XOR_EQUALS;
      }
      else
        id = CEXPR_OP_BIT_AND;

      break;
    case '^':
      (*i)++;

      if (*i < str.size() && str[*i + 1] == '=') {
        (*i)++;
        id = CEXPR_OP_BIT_XOR_EQUALS;
      }
      else
        id = CEXPR_OP_BIT_XOR;

      break;
    case '|':
      (*i)++;

      if      (*i < str.size() && str[*i] == '|') {
        (*i)++;
        id = CEXPR_OP_LOGICAL_OR;
      }
      else if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CEXPR_OP_BIT_OR_EQUALS;
      }
      else
        id = CEXPR_OP_BIT_OR;

      break;
    case '?':
      (*i)++;
      id = CEXPR_OP_QUESTION;
      break;
    case ':':
      (*i)++;
      id = CEXPR_OP_COLON;
      break;
#ifdef GNUPLOT_EXPR
    case '[':
      (*i)++;
      id = CEXPR_OP_OPEN_SBRACKET;
      break;
    case ']':
      (*i)++;
      id = CEXPR_OP_CLOSE_SBRACKET;
      break;
    case '.': {
      uint i1 = (*i) + 1;

      if (i1 < str.size() && isdigit(str[i1]))
        return CExprPTokenPtr();

      (*i)++;

      id = CEXPR_OP_CONCAT;

      break;
    }
#endif
    case ',':
      (*i)++;
      id = CEXPR_OP_COMMA;
      break;
    default:
      break;
  }

  if (id == CEXPR_OP_UNKNOWN)
    return CExprPTokenPtr();

  CExprOperatorPtr op = CExprInst->getOperator(id);

  if (! op.isValid())
    return CExprPTokenPtr();

  return createOperatorToken(op);
}

#ifdef GNUPLOT_EXPR
CExprPTokenPtr
CExprParseImpl::
readOperatorStr(const std::string &str, uint *i)
{
  uint j = *i;

  while (*i < str.size() && isalpha(str[*i]))
    (*i)++;

  std::string identifier = str.substr(j, *i - j);

  CExprOpType id = CEXPR_OP_UNKNOWN;

  if      (identifier == "eq")
    id = CEXPR_OP_EQUAL;
  else if (identifier == "ne")
    id = CEXPR_OP_NOT_EQUAL;
  else {
    *i = j;
    return CExprPTokenPtr();
  }

  CExprOperatorPtr op = CExprInst->getOperator(id);

  return createOperatorToken(op);
}
#endif

CExprPTokenPtr
CExprParseImpl::
readIdentifier(const std::string &str, uint *i)
{
  uint j = *i;

  while (*i < str.size() && (str[*i] == '_' || isalnum(str[*i])))
    (*i)++;

  std::string identifier = str.substr(j, *i - j);

  return createIdentifierToken(identifier);
}

#if 0
CExprPTokenPtr
CExprParseImpl::
readUnknown(const std::string &str, uint *i)
{
  while (*i < str.size() && ! isspace(str[*i]))
    (*i)++;

  return createUnknownToken();
}

CExprPTokenPtr
CExprParseImpl::
createUnknownToken()
{
  return CExprPToken::createUnknownToken();
}
#endif

CExprPTokenPtr
CExprParseImpl::
createIdentifierToken(const std::string &identifier)
{
  return CExprPToken::createIdentifierToken(identifier);
}

CExprPTokenPtr
CExprParseImpl::
createOperatorToken(CExprOperatorPtr op)
{
  return CExprPToken::createOperatorToken(op);
}

CExprPTokenPtr
CExprParseImpl::
createIntegerToken(long integer)
{
  return CExprPToken::createIntegerToken(integer);
}

CExprPTokenPtr
CExprParseImpl::
createRealToken(double real)
{
  return CExprPToken::createRealToken(real);
}

CExprPTokenPtr
CExprParseImpl::
createStringToken(const std::string &str)
{
  return CExprPToken::createStringToken(str);
}

//------

void
CExprPToken::
printQualified(std::ostream &os) const
{
  switch (type_) {
    case CEXPR_PTOKEN_IDENTIFIER:
      os << "<identifier>";
      break;
    case CEXPR_PTOKEN_OPERATOR:
      os << "<operator>";
      break;
    case CEXPR_PTOKEN_INTEGER:
      os << "<integer>";
      break;
    case CEXPR_PTOKEN_REAL:
      os << "<real>";
      break;
    case CEXPR_PTOKEN_STRING:
      os << "<string>";
      break;
    default:
      os << "<-?->";
      break;
  }

  base_->print(os);
}

//------

void
CExprPTokenStack::
print(std::ostream &os) const
{
  uint len = stack_.size();

  for (uint i = 0; i < len; ++i) {
    if (i > 0) os << " ";

    stack_[i]->printQualified(os);
  }
}

//------

bool
CExprParseUtil::
stringToNumber(const std::string &str, uint *i, long &integer, double &real, bool &is_int)
{
  uint i1 = *i;

  if (str[*i] == '+' || str[*i] == '-')
    (*i)++;

  if (*i >= str.size())
    return false;

  if (*i < str.size() - 2 && str[*i] == '0' &&
      (str[*i + 1] == 'x' || str[*i + 1] == 'X') && isxdigit(str[*i + 2])) {
    (*i)++;
    (*i)++;

    uint j = *i;

    while (*i < str.size() && isxdigit(str[*i]))
      (*i)++;

    std::string str1 = str.substr(j, *i - j);

    long unsigned integer1;

    sscanf(str1.c_str(), "%lx", &integer1);

    integer = integer1;
    real    = integer1;
    is_int  = true;

    return true;
  }

  //uint j = *i;

  while (*i < str.size() && isdigit(str[*i]))
    (*i)++;

  bool point_found = (*i < str.size() && str[*i] == '.');

  if (point_found) {
    (*i)++;

    //j = *i;

    while (*i < str.size() && isdigit(str[*i]))
      (*i)++;
  }

  bool exponent_found = (*i < str.size() && (str[*i] == 'e' || str[*i] == 'E'));

  if (exponent_found) {
    uint i2 = *i;

    (*i)++;

    if (*i < str.size() && (str[*i] == '+' || str[*i] == '-'))
      (*i)++;

    if (*i < str.size() && isdigit(str[*i])) {
      //j = *i;

      while (*i < str.size() && isdigit(str[*i]))
        (*i)++;
    }
    else {
      *i = i2;

      exponent_found = false;
    }
  }

  std::string str1 = str.substr(i1, *i - i1);

  if (! point_found && ! exponent_found) {
    if (*i < str.size() && (str[*i] == 'l' || str[*i] == 'L' || str[*i] == 'u' || str[*i] == 'U'))
      (*i)++;
  }

  CExprPTokenPtr ptoken;

  if (point_found || exponent_found) {
    real    = CStrUtil::toReal(str1);
    integer = real;
    is_int  = false;

    return true;
  }

  //------

  bool octal = false;

  if (str1[0] == '0') {
    octal = true;

    for (uint j = 1; j < str1.size(); ++j)
      if (str1[j] < '0' || str1[j] > '7') {
        octal = false;
        break;
      }
  }

  if (octal) {
    long unsigned integer1;

    sscanf(str1.c_str(), "%lo", &integer1);

    integer = integer1;
    real    = integer1;
    is_int  = true;

    return true;
  }

  long integer1;

  sscanf(str1.c_str(), "%ld", &integer1);

  integer = integer1;
  real    = integer1;
  is_int  = true;

  return true;
}
