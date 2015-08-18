#include <CExprI.h>
#include <cstdio>

class CExprParseImpl {
 public:
  CExprParseImpl() { }
 ~CExprParseImpl() { }

  CExprTokenStack parseFile(const std::string &filename);
  CExprTokenStack parseFile(FILE *fp);
  CExprTokenStack parseFile(CFile &file);
  CExprTokenStack parseLine(const std::string &str);

  bool skipExpression(const std::string &str, uint &i);

 private:
  bool parseLine(CExprTokenStack &stack, const std::string &line);
  bool parseLine(CExprTokenStack &stack, const std::string &line, uint &i);

  void parseError(const std::string &msg, const std::string &line, uint i);

  bool isNumber(const std::string &str, uint i);

  CExprTokenBaseP readNumber     (const std::string &str, uint *i);
  CExprTokenBaseP readString     (const std::string &str, uint *i);
  CExprTokenBaseP readComplex    (const std::string &str, uint *i);
  CExprTokenBaseP readOperator   (const std::string &str, uint *i);
#ifdef GNUPLOT_EXPR
  CExprTokenBaseP readOperatorStr(const std::string &str, uint *i);
#endif
  CExprTokenBaseP readIdentifier (const std::string &str, uint *i);
#if 0
  CExprTokenBaseP readUnknown(const std::string &str, uint *i);
#endif

  bool        skipNumber     (const std::string &str, uint *i);
  bool        skipString     (const std::string &str, uint *i);
  bool        skipComplex    (const std::string &str, uint *i);
  CExprOpType skipOperator   (const std::string &str, uint *i);
#ifdef GNUPLOT_EXPR
  CExprOpType skipOperatorStr(const std::string &str, uint *i);
#endif
  void        skipIdentifier (const std::string &str, uint *i);

  bool readStringChars    (const std::string &str, uint *i, bool process, std::string &str1);
  bool readComplexChars   (const std::string &str, uint *i, std::complex<double> &c);
  void readIdentifierChars(const std::string &str, uint *i, std::string &identifier);

#if 0
  CExprTokenBaseP createUnknownToken();
#endif
  CExprTokenBaseP createIdentifierToken(const std::string &str);
  CExprTokenBaseP createOperatorToken  (CExprOpType op);
  CExprTokenBaseP createIntegerToken   (long);
  CExprTokenBaseP createRealToken      (double);
  CExprTokenBaseP createStringToken    (const std::string &str);
  CExprTokenBaseP createComplexToken   (const std::complex<double> &c);

  std::string replaceEscapeCodes(const std::string &str);

  bool isStringOp(CExprOpType op) const;

 private:
  CExprTokenStack tokenStack_;
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

CExprTokenStack
CExprParse::
parseFile(const std::string &filename)
{
  return impl_->parseFile(filename);
}

CExprTokenStack
CExprParse::
parseFile(FILE *fp)
{
  return impl_->parseFile(fp);
}

CExprTokenStack
CExprParse::
parseLine(const std::string &line)
{
  return impl_->parseLine(line);
}

bool
CExprParse::
skipExpression(const std::string &line, uint &i)
{
  return impl_->skipExpression(line, i);
}

//-----------

CExprTokenStack
CExprParseImpl::
parseFile(const std::string &filename)
{
  CFile file(filename);

  return parseFile(file);
}

CExprTokenStack
CExprParseImpl::
parseFile(FILE *fp)
{
  CFile file(fp);

  return parseFile(file);
}

CExprTokenStack
CExprParseImpl::
parseFile(CFile &file)
{
  std::vector<std::string> lines;

  file.toLines(lines);

  tokenStack_.clear();

  uint num_lines = lines.size();

  for (uint i = 0; i < num_lines; i++)
    if (! parseLine(tokenStack_, lines[i]))
      return CExprTokenStack();

  return tokenStack_;
}

CExprTokenStack
CExprParseImpl::
parseLine(const std::string &line)
{
  tokenStack_.clear();

  bool flag = parseLine(tokenStack_, line);

  if (! flag)
    return CExprTokenStack();

  return tokenStack_;
}

bool
CExprParseImpl::
parseLine(CExprTokenStack &stack, const std::string &line)
{
  uint i = 0;

  return parseLine(stack, line, i);
}

bool
CExprParseImpl::
parseLine(CExprTokenStack &stack, const std::string &line, uint &i)
{
  CExprTokenBaseP ptoken;
  CExprTokenBaseP lastPToken;
  CExprTokenType  lastPTokenType = CEXPR_TOKEN_UNKNOWN;

  while (true) {
    CStrUtil::skipSpace(line, &i);
    if (i >= line.size()) break;

    if      (CExprOperator::isOperatorChar(line[i])) {
      CExprOpType lastOpType =
       (lastPToken.isValid() && lastPToken->type() == CEXPR_TOKEN_OPERATOR ?
        lastPToken->getOperator() : CEXPR_OP_UNKNOWN);

      if (lastPTokenType == CEXPR_TOKEN_UNKNOWN || lastOpType != CEXPR_OP_UNKNOWN) {
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

      if (ptoken.isValid() && ptoken->type() == CEXPR_TOKEN_OPERATOR) {
        if      (ptoken->getOperator() == CEXPR_OP_OPEN_RBRACKET) {
          stack.addToken(ptoken);

          if (! parseLine(stack, line, i))
            return false;

          lastPToken     = CExprTokenBaseP();
          lastPTokenType = CEXPR_TOKEN_VALUE;

          continue;
        }
        else if (ptoken->getOperator() == CEXPR_OP_CLOSE_RBRACKET) {
          stack.addToken(ptoken);

          return true;
        }
      }

#ifdef GNUPLOT_EXPR
      if (ptoken.isValid() && ptoken->type() == CEXPR_TOKEN_OPERATOR &&
          ptoken->getOperator() == CEXPR_OP_LOGICAL_NOT &&
          lastPToken.isValid() && lastPToken->type() == CEXPR_TOKEN_INTEGER) {
        ptoken = createOperatorToken(CEXPR_OP_FACTORIAL);
      }
#endif
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
#ifdef GNUPLOT_EXPR
    else if (line[i] == '{')
      ptoken = readComplex(line, &i);
#endif
    else {
      parseError("Invalid Character", line, i);
      return false;
    }

    if (! ptoken.isValid()) {
      parseError("Invalid Token", line, i);
      return false;
    }

    stack.addToken(ptoken);

    lastPToken     = ptoken;
    lastPTokenType = ptoken->type();
  }

  return true;
}

bool
CExprParseImpl::
skipExpression(const std::string &line, uint &i)
{
  CExprTokenType lastTokenType = CEXPR_TOKEN_UNKNOWN;
  CExprOpType    lastOpType    = CEXPR_OP_UNKNOWN;

  uint i1   = i;
  uint len1 = line.size();

  while (true) {
    CStrUtil::skipSpace(line, &i);
    if (i >= len1) break;

    CExprTokenType lastTokenType1 = lastTokenType;
    CExprOpType    lastOpType1    = lastOpType;

    lastTokenType = CEXPR_TOKEN_UNKNOWN;
    lastOpType    = CEXPR_OP_UNKNOWN;

    if      (CExprOperator::isOperatorChar(line[i])) {
      if (line[i] == ',' || line[i] == ')')
        break;

      if (lastTokenType1 == CEXPR_TOKEN_OPERATOR && lastOpType1 != CEXPR_OP_UNKNOWN) {
        if ((line[i] == '-' || line[i] == '+') && i < len1 - 1 && isdigit(line[i + 1])) {
          if (! skipNumber(line, &i)) {
            break;
          }

          lastTokenType = CEXPR_TOKEN_REAL;
        }
        else {
          int i2 = i;

          lastOpType = skipOperator(line, &i);

          if (lastOpType != CEXPR_OP_UNKNOWN) {
            if (lastTokenType1 == CEXPR_TOKEN_STRING && ! isStringOp(lastOpType)) {
              i = i2;
              break;
            }

            lastTokenType = CEXPR_TOKEN_OPERATOR;
          }
        }
      }
      else {
        int i2 = i;

        lastOpType = skipOperator(line, &i);

        if (lastOpType != CEXPR_OP_UNKNOWN) {
          if (lastTokenType1 == CEXPR_TOKEN_STRING && ! isStringOp(lastOpType)) {
            i = i2;
            break;
          }

          lastTokenType = CEXPR_TOKEN_OPERATOR;
        }
      }

#ifdef GNUPLOT_EXPR
      if (lastTokenType == CEXPR_TOKEN_UNKNOWN && line[i] == '.') {
        if (! skipNumber(line, &i)) {
          break;
        }

        lastTokenType = CEXPR_TOKEN_REAL;
      }
#endif

      if (lastTokenType == CEXPR_TOKEN_OPERATOR) {
        if      (lastTokenType == CEXPR_TOKEN_OPERATOR && lastOpType == CEXPR_OP_OPEN_RBRACKET) {
          while (i < len1) {
            if (! skipExpression(line, i)) {
              i = i1; return false;
            }

            // check for comma separated expression list
            CStrUtil::skipSpace(line, &i);

            if (i >= len1 || line[i] != ',')
              break;

            lastTokenType = CEXPR_TOKEN_OPERATOR;
            lastOpType    = CEXPR_OP_COMMA;

            ++i;
          }

          CStrUtil::skipSpace(line, &i);

          if (i >= len1 || line[i] != ')') {
            i = i1; return false;
          }

          lastTokenType = CEXPR_TOKEN_VALUE;

          ++i;

          continue;
        }
        else if (lastTokenType == CEXPR_TOKEN_OPERATOR && lastOpType == CEXPR_OP_CLOSE_RBRACKET) {
          break;
        }
      }
    }
#ifdef GNUPLOT_EXPR
    else if (CExprOperator::isOperatorStr(line[i])) {
      int i2 = i;

      CExprOpType lastOpType2 = skipOperatorStr(line, &i);

      if (lastOpType2 == CEXPR_OP_UNKNOWN) {
        if (lastTokenType1 != CEXPR_TOKEN_UNKNOWN && lastTokenType1 != CEXPR_TOKEN_OPERATOR)
          break;

        skipIdentifier(line, &i);

        lastTokenType = CEXPR_TOKEN_IDENTIFIER;
      }
      else {
        if (lastTokenType1 == CEXPR_TOKEN_STRING && ! isStringOp(lastOpType2)) {
          i = i2;
          break;
        }

        lastOpType    = lastOpType2;
        lastTokenType = CEXPR_TOKEN_OPERATOR;
      }
    }
#endif
    else if (isNumber(line, i)) {
      if (lastTokenType1 != CEXPR_TOKEN_UNKNOWN && lastTokenType1 != CEXPR_TOKEN_OPERATOR)
        break;

      if (! skipNumber(line, &i)) {
        i = i1; return false;
      }

      lastTokenType = CEXPR_TOKEN_REAL;
    }
    else if (line[i] == '_' || isalpha(line[i])) {
      if (lastTokenType1 != CEXPR_TOKEN_UNKNOWN && lastTokenType1 != CEXPR_TOKEN_OPERATOR)
        break;

      skipIdentifier(line, &i);

      lastTokenType = CEXPR_TOKEN_IDENTIFIER;
    }
    else if (line[i] == '\'' || line[i] == '\"') {
      if (lastTokenType1 != CEXPR_TOKEN_UNKNOWN && lastTokenType1 != CEXPR_TOKEN_OPERATOR)
        break;

      if (! skipString(line, &i)) {
        i = i1; return false;
      }

      lastTokenType = CEXPR_TOKEN_STRING;
    }
#ifdef GNUPLOT_EXPR
    else if (line[i] == '{') {
      if (lastTokenType1 != CEXPR_TOKEN_UNKNOWN && lastTokenType1 != CEXPR_TOKEN_OPERATOR)
        break;

      if (! skipComplex(line, &i)) {
        i = i1; return false;
      }

      lastTokenType = CEXPR_TOKEN_COMPLEX;
    }
#endif
    else {
      break;
    }

    if (lastTokenType == CEXPR_TOKEN_UNKNOWN) {
      break;
    }
  }

  return true;
}

void
CExprParseImpl::
parseError(const std::string &msg, const std::string &line, uint i)
{
  std::stringstream ostr;

  ostr << msg << " \"" << line.substr(0, i - 1) << "#" << line[i] << "#" <<
          (i < line.size() ? line.substr(i + 1) : "") << std::endl;

  for (uint j = 0; j < i + msg.size() + 1; j++)
    ostr << " ";

  ostr << "^";

  CExprInst->errorMsg(ostr.str());
}

bool
CExprParseImpl::
isStringOp(CExprOpType op) const
{
  return (op == CEXPR_OP_STR_CONCAT || op == CEXPR_OP_STR_EQUAL || op == CEXPR_OP_STR_NOT_EQUAL);
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

CExprTokenBaseP
CExprParseImpl::
readNumber(const std::string &str, uint *i)
{
  long   integer = 0;
  double real    = 0.0;
  bool   is_int  = false;

  if (! CExprStringToNumber(str, i, integer, real, is_int))
    return CExprTokenBaseP();

  if (is_int)
    return createIntegerToken(integer);
  else
    return createRealToken(real);
}

bool
CExprParseImpl::
skipNumber(const std::string &str, uint *i)
{
  long   integer = 0;
  double real    = 0.0;
  bool   is_int  = false;

  if (! CExprStringToNumber(str, i, integer, real, is_int))
    return false;

  return true;
}

CExprTokenBaseP
CExprParseImpl::
readString(const std::string &str, uint *i)
{
  std::string str1;

  if (! readStringChars(str, i, true, str1))
    return CExprTokenBaseP();

  return createStringToken(str1);
}

bool
CExprParseImpl::
skipString(const std::string &str, uint *i)
{
  std::string str1;

  return readStringChars(str, i, false, str1);
}

bool
CExprParseImpl::
readStringChars(const std::string &str, uint *i, bool process, std::string &str1)
{
  if      (str[*i] == '\'') {
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
      return false;

    str1 = str.substr(j, *i - j);

    (*i)++;
  }
  else if (str[*i] == '\"') {
    (*i)++;

    uint j = *i;

    str1 += str[*i];

    while (*i < str.size()) {
      if      (str[*i] == '\\' && *i < str.size() - 1)
        (*i)++;
      else if (str[*i] == '\"')
        break;

      (*i)++;
    }

    if (*i >= str.size())
      return false;

    str1 = str.substr(j, *i - j);

    (*i)++;

    if (process)
      str1 = replaceEscapeCodes(str1);
  }
  else
    assert(false);

  return true;
}

std::string
CExprParseImpl::
replaceEscapeCodes(const std::string &str)
{
  int len = str.size();

  bool has_escape = false;

  for (int i = 0; i < len - 1; ++i)
    if (str[i] == '\\') {
      has_escape = true;
      break;
    }

  if (! has_escape)
    return str;

  std::string str1;

  int i = 0;

  while (i < len - 1) {
    if (str[i] != '\\') {
      str1 += str[i++];
      continue;
    }

    ++i;

    switch (str[i]) {
      case 'a':
        str1 += '\a';
        break;
      case 'b':
        str1 += '\b';
        break;
      case 'e':
        str1 += '\033';
        break;
      case 'f':
        str1 += '\f';
        break;
      case 'n':
        str1 += '\n';
        break;
      case 'r':
        str1 += '\r';
        break;
      case 't':
        str1 += '\t';
        break;
      case 'v':
        str1 += '\v';
        break;
      case 'x':
        if (i < len - 1 && ::isxdigit(str[i + 1])) {
          int hex_value = 0;

          ++i;

          if      (::isdigit(str[i]))
            hex_value += (str[i] - '0');
          else if (::islower(str[i]))
            hex_value += (str[i] - 'a' + 10);
          else
            hex_value += (str[i] - 'A' + 10);

          if (i < len - 1 && ::isxdigit(str[i + 1])) {
            hex_value *= 16;

            ++i;

            if      (::isdigit(str[i]))
              hex_value += (str[i] - '0');
            else if (::islower(str[i]))
              hex_value += (str[i] - 'a' + 10);
            else
              hex_value += (str[i] - 'A' + 10);
          }

          str1 += hex_value;
        }
        else {
          str1 += '\\';
          str1 += str[i++];
        }

        break;
      case '\\':
        str1 += '\\';
        break;
      case '0':
        if (i < len - 1 && CStrUtil::isodigit(str[i + 1])) {
          int oct_value = 0;

          ++i;

          oct_value += (str[i] - '0');

          if (i < len - 1 && CStrUtil::isodigit(str[i + 1])) {
            oct_value *= 8;

            ++i;

            oct_value += (str[i] - '0');
          }

          if (i < len - 1 && CStrUtil::isodigit(str[i + 1])) {
            oct_value *= 8;

            ++i;

            oct_value += (str[i] - '0');
          }

          str1 += oct_value;
        }
        else {
          str1 += '\\';
          str1 += str[i];
        }

        break;
      case '\'':
      case '\"':
        str1 += str[i];
        break;
      default:
        str1 += '\\';
        str1 += str[i];
        break;
    }

    ++i;
  }

  if (i < len)
    str1 += str[i++];

  return str1;
}

CExprTokenBaseP
CExprParseImpl::
readComplex(const std::string &str, uint *i)
{
  std::complex<double> c;

  if (! readComplexChars(str, i, c))
    return CExprTokenBaseP();

  return createComplexToken(c);
}

bool
CExprParseImpl::
skipComplex(const std::string &str, uint *i)
{
  std::complex<double> c;

  return readComplexChars(str, i, c);
}

bool
CExprParseImpl::
readComplexChars(const std::string &str, uint *i, std::complex<double> &c)
{
  if (*i >= str.size() || str[*i] != '{')
    return false;

  (*i)++;

  CStrUtil::skipSpace(str, i);

  long   integer1 = 0;
  double real1    = 0.0;
  bool   is_int1  = false;

  if (! CExprStringToNumber(str, i, integer1, real1, is_int1))
    return false;

  CStrUtil::skipSpace(str, i);

  if (*i >= str.size() || str[*i] != ',')
    return false;

  (*i)++;

  CStrUtil::skipSpace(str, i);

  long   integer2 = 0;
  double real2    = 0.0;
  bool   is_int2  = false;

  if (! CExprStringToNumber(str, i, integer2, real2, is_int2))
    return false;

  CStrUtil::skipSpace(str, i);

  if (*i >= str.size() || str[*i] != '}')
    return false;

  (*i)++;

  c = std::complex<double>(real1, real2);

  return true;
}

CExprTokenBaseP
CExprParseImpl::
readOperator(const std::string &str, uint *i)
{
  CExprOpType id = skipOperator(str, i);

  if (id == CEXPR_OP_UNKNOWN)
    return CExprTokenBaseP();

  return createOperatorToken(id);
}

CExprOpType
CExprParseImpl::
skipOperator(const std::string &str, uint *i)
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

#ifndef GNUPLOT_EXPR
      if      (*i < str.size() && str[*i] == '-') {
        (*i)++;
        id = CEXPR_OP_DECREMENT;
      }
      else if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CEXPR_OP_MINUS_EQUALS;
      }
      else
#endif
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
        return CEXPR_OP_UNKNOWN;

      (*i)++;

      id = CEXPR_OP_STR_CONCAT;

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

  return id;
}

#ifdef GNUPLOT_EXPR
CExprTokenBaseP
CExprParseImpl::
readOperatorStr(const std::string &str, uint *i)
{
  CExprOpType op = skipOperatorStr(str, i);

  if (op == CEXPR_OP_UNKNOWN)
    return CExprTokenBaseP();

  return createOperatorToken(op);
}

CExprOpType
CExprParseImpl::
skipOperatorStr(const std::string &str, uint *i)
{
  uint j = *i;

  while (*i < str.size() && isalpha(str[*i]))
    (*i)++;

  std::string identifier = str.substr(j, *i - j);

  CExprOpType id = CEXPR_OP_UNKNOWN;

  if      (identifier == "eq")
    id = CEXPR_OP_STR_EQUAL;
  else if (identifier == "ne")
    id = CEXPR_OP_STR_NOT_EQUAL;
  else {
    *i = j;
    id = CEXPR_OP_UNKNOWN;
  }

  return id;
}
#endif

CExprTokenBaseP
CExprParseImpl::
readIdentifier(const std::string &str, uint *i)
{
  std::string identifier;

  readIdentifierChars(str, i, identifier);

  return createIdentifierToken(identifier);
}

void
CExprParseImpl::
skipIdentifier(const std::string &str, uint *i)
{
  std::string identifier;

  readIdentifierChars(str, i, identifier);
}

void
CExprParseImpl::
readIdentifierChars(const std::string &str, uint *i, std::string &identifier)
{
  uint j = *i;

  while (*i < str.size() && (str[*i] == '_' || isalnum(str[*i])))
    (*i)++;

  identifier = str.substr(j, *i - j);
}

#if 0
CExprTokenBaseP
CExprParseImpl::
readUnknown(const std::string &str, uint *i)
{
  while (*i < str.size() && ! isspace(str[*i]))
    (*i)++;

  return createUnknownToken();
}

CExprTokenBaseP
CExprParseImpl::
createUnknownToken()
{
  return CExprTokenBaseP(CExprTokenMgrInst->createUnknownToken());
}
#endif

CExprTokenBaseP
CExprParseImpl::
createIdentifierToken(const std::string &identifier)
{
  return CExprTokenBaseP(CExprTokenMgrInst->createIdentifierToken(identifier));
}

CExprTokenBaseP
CExprParseImpl::
createOperatorToken(CExprOpType id)
{
  return CExprTokenBaseP(CExprTokenMgrInst->createOperatorToken(id));
}

CExprTokenBaseP
CExprParseImpl::
createIntegerToken(long integer)
{
  return CExprTokenBaseP(CExprTokenMgrInst->createIntegerToken(integer));
}

CExprTokenBaseP
CExprParseImpl::
createRealToken(double real)
{
  return CExprTokenBaseP(CExprTokenMgrInst->createRealToken(real));
}

CExprTokenBaseP
CExprParseImpl::
createStringToken(const std::string &str)
{
  return CExprTokenBaseP(CExprTokenMgrInst->createStringToken(str));
}

CExprTokenBaseP
CExprParseImpl::
createComplexToken(const std::complex<double> &c)
{
  return CExprTokenBaseP(CExprTokenMgrInst->createComplexToken(c));
}
