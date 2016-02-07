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

  bool skipExpression(const std::string &str, uint &i, const std::string &echars="");

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

  bool isStringOp (CExprOpType op) const;
  bool isBooleanOp(CExprOpType op) const;

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
  CExprTokenType  lastPTokenType = CExprTokenType::UNKNOWN;

  while (true) {
    CStrUtil::skipSpace(line, &i);
    if (i >= line.size()) break;

    if      (CExprOperator::isOperatorChar(line[i])) {
      CExprOpType lastOpType =
       (lastPToken.isValid() && lastPToken->type() == CExprTokenType::OPERATOR ?
        lastPToken->getOperator() : CExprOpType::UNKNOWN);

      if (lastPTokenType == CExprTokenType::UNKNOWN || lastOpType != CExprOpType::UNKNOWN) {
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

      if (ptoken.isValid() && ptoken->type() == CExprTokenType::OPERATOR) {
        if      (ptoken->getOperator() == CExprOpType::OPEN_RBRACKET) {
          stack.addToken(ptoken);

          if (! parseLine(stack, line, i))
            return false;

          lastPToken     = CExprTokenBaseP();
          lastPTokenType = CExprTokenType::VALUE;

          continue;
        }
        else if (ptoken->getOperator() == CExprOpType::CLOSE_RBRACKET) {
          stack.addToken(ptoken);

          return true;
        }
      }

#ifdef GNUPLOT_EXPR
      if (ptoken.isValid() && ptoken->type() == CExprTokenType::OPERATOR &&
          ptoken->getOperator() == CExprOpType::LOGICAL_NOT &&
          lastPToken.isValid() && lastPToken->type() == CExprTokenType::INTEGER) {
        ptoken = createOperatorToken(CExprOpType::FACTORIAL);
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
skipExpression(const std::string &line, uint &i, const std::string &echars)
{
  CExprTokenType lastTokenType = CExprTokenType::UNKNOWN;
  CExprOpType    lastOpType    = CExprOpType::UNKNOWN;

  uint i1   = i;
  uint len1 = line.size();

  while (true) {
    CStrUtil::skipSpace(line, &i);
    if (i >= len1) break;

    CExprTokenType lastTokenType1 = lastTokenType;
    CExprOpType    lastOpType1    = lastOpType;

    lastTokenType = CExprTokenType::UNKNOWN;
    lastOpType    = CExprOpType::UNKNOWN;

    if      (CExprOperator::isOperatorChar(line[i])) {
      if (line[i] == ',' || echars.find(line[i]) != std::string::npos)
        break;

      int i2 = i;

      if (lastTokenType1 == CExprTokenType::OPERATOR && lastOpType1 != CExprOpType::UNKNOWN) {
        if ((line[i] == '-' || line[i] == '+') && i < len1 - 1 && isdigit(line[i + 1])) {
          if (! skipNumber(line, &i)) {
            break;
          }

          lastTokenType = CExprTokenType::REAL;
        }
        else {
          lastOpType = skipOperator(line, &i);

          if (lastOpType != CExprOpType::UNKNOWN) {
            if (lastTokenType1 == CExprTokenType::STRING && ! isStringOp(lastOpType)) {
              i = i2;
              break;
            }

            lastTokenType = CExprTokenType::OPERATOR;
          }
        }
      }
      else {
        lastOpType = skipOperator(line, &i);

        if (lastOpType != CExprOpType::UNKNOWN) {
          if (lastTokenType1 == CExprTokenType::STRING && ! isStringOp(lastOpType)) {
            i = i2;
            break;
          }

          lastTokenType = CExprTokenType::OPERATOR;
        }
      }

#ifdef GNUPLOT_EXPR
      if (lastTokenType == CExprTokenType::UNKNOWN && line[i] == '.') {
        if (! skipNumber(line, &i)) {
          break;
        }

        lastTokenType = CExprTokenType::REAL;
      }
#endif

      if (lastTokenType == CExprTokenType::OPERATOR) {
        if      (lastTokenType == CExprTokenType::OPERATOR &&
                 lastOpType == CExprOpType::OPEN_RBRACKET) {
          while (i < len1) {
            if (! skipExpression(line, i, ",)")) {
              i = i1; return false;
            }

            // check for comma separated expression list
            CStrUtil::skipSpace(line, &i);

            if (i >= len1 || line[i] != ',')
              break;

            lastTokenType = CExprTokenType::OPERATOR;
            lastOpType    = CExprOpType::COMMA;

            ++i;
          }

          CStrUtil::skipSpace(line, &i);

          if (i >= len1 || line[i] != ')') {
            i = i1; return false;
          }

          lastTokenType = CExprTokenType::VALUE;

          ++i;

          continue;
        }
        else if (lastTokenType == CExprTokenType::OPERATOR &&
                 lastOpType == CExprOpType::CLOSE_RBRACKET) {
          i = i2;

          break;
        }
        else if (lastTokenType == CExprTokenType::OPERATOR &&
                 lastOpType == CExprOpType::OPEN_SBRACKET) {
          if (! skipExpression(line, i, ":]")) { // also ends on ',' ?
            i = i1; return false;
          }

          // check for ':"
          CStrUtil::skipSpace(line, &i);

          if (i < len1 && line[i] == ':') {
            ++i;

            if (! skipExpression(line, i, "]")) { // also ends on ',' ?
              i = i1; return false;
            }
          }

          CStrUtil::skipSpace(line, &i);

          if (i >= len1 || line[i] != ']') {
            i = i1; return false;
          }

          lastTokenType = CExprTokenType::VALUE;

          ++i;

          continue;
        }
        else if (lastTokenType == CExprTokenType::OPERATOR &&
                 lastOpType == CExprOpType::CLOSE_SBRACKET) {
          i = i2;

          break;
        }
        else if (lastTokenType == CExprTokenType::OPERATOR &&
                 lastOpType == CExprOpType::QUESTION) {
          if (! skipExpression(line, i, ":")) {
            i = i1; return false;
          }

          CStrUtil::skipSpace(line, &i);

          if (i >= len1 || line[i] != ':') {
            i = i1; return false;
          }

          ++i;

          CStrUtil::skipSpace(line, &i);

          if (! skipExpression(line, i)) {
            i = i1; return false;
          }

          lastTokenType = CExprTokenType::VALUE;

          continue;
        }
        else if (lastTokenType == CExprTokenType::OPERATOR && lastOpType == CExprOpType::COLON) {
          i = i2;

          break;
        }
      }
    }
#ifdef GNUPLOT_EXPR
    else if (CExprOperator::isOperatorStr(line[i])) {
      int i2 = i;

      CExprOpType lastOpType2 = skipOperatorStr(line, &i);

      if (lastOpType2 == CExprOpType::UNKNOWN) {
        if (lastTokenType1 != CExprTokenType::UNKNOWN &&
            lastTokenType1 != CExprTokenType::OPERATOR)
          break;

        skipIdentifier(line, &i);

        lastTokenType = CExprTokenType::IDENTIFIER;
      }
      else {
        if (lastTokenType1 == CExprTokenType::STRING && ! isStringOp(lastOpType2)) {
          i = i2;
          break;
        }

        lastOpType    = lastOpType2;
        lastTokenType = CExprTokenType::OPERATOR;
      }
    }
#endif
    else if (isNumber(line, i)) {
      if (lastTokenType1 != CExprTokenType::UNKNOWN &&
          lastTokenType1 != CExprTokenType::OPERATOR)
        break;

      if (! skipNumber(line, &i)) {
        i = i1; return false;
      }

      lastTokenType = CExprTokenType::REAL;
    }
    else if (line[i] == '_' || isalpha(line[i])) {
      if (lastTokenType1 != CExprTokenType::UNKNOWN &&
          lastTokenType1 != CExprTokenType::OPERATOR)
        break;

      skipIdentifier(line, &i);

      lastTokenType = CExprTokenType::IDENTIFIER;
    }
    else if (line[i] == '\'' || line[i] == '\"') {
      if (lastTokenType1 != CExprTokenType::UNKNOWN &&
          lastTokenType1 != CExprTokenType::OPERATOR)
        break;

      if (! skipString(line, &i)) {
        i = i1; return false;
      }

      if (lastTokenType1 == CExprTokenType::OPERATOR && isBooleanOp(lastOpType1))
        lastTokenType = CExprTokenType::INTEGER;
      else
        lastTokenType = CExprTokenType::STRING;
    }
#ifdef GNUPLOT_EXPR
    else if (line[i] == '{') {
      if (lastTokenType1 != CExprTokenType::UNKNOWN &&
          lastTokenType1 != CExprTokenType::OPERATOR)
        break;

      if (! skipComplex(line, &i)) {
        i = i1; return false;
      }

      lastTokenType = CExprTokenType::COMPLEX;
    }
#endif
    else {
      break;
    }

    if (lastTokenType == CExprTokenType::UNKNOWN) {
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
  return (op == CExprOpType::STR_CONCAT || op == CExprOpType::STR_EQUAL ||
          op == CExprOpType::STR_NOT_EQUAL);
}

bool
CExprParseImpl::
isBooleanOp(CExprOpType op) const
{
  return (op == CExprOpType::LESS         || op == CExprOpType::LESS_EQUAL ||
          op == CExprOpType::GREATER      || op == CExprOpType::GREATER_EQUAL ||
          op == CExprOpType::EQUAL        || op == CExprOpType::NOT_EQUAL ||
          op == CExprOpType::APPROX_EQUAL || op == CExprOpType::STR_EQUAL);
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
#ifndef GNUPLOT_EXPR
      if      (str[*i] == '\\' && *i < str.size() - 1)
        (*i)++;
      else if (str[*i] == '\'')
        break;
#else
      // no escapes inside single quote for gnuplot
      if (str[*i] == '\'')
        break;
#endif

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

  if (id == CExprOpType::UNKNOWN)
    return CExprTokenBaseP();

  return createOperatorToken(id);
}

CExprOpType
CExprParseImpl::
skipOperator(const std::string &str, uint *i)
{
  CExprOpType id = CExprOpType::UNKNOWN;

  switch (str[*i]) {
    case '(':
      (*i)++;
      id = CExprOpType::OPEN_RBRACKET;
      break;
    case ')':
      (*i)++;
      id = CExprOpType::CLOSE_RBRACKET;
      break;
    case '!':
      (*i)++;

      if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CExprOpType::NOT_EQUAL;
      }
      else
        id = CExprOpType::LOGICAL_NOT;

      break;
    case '~':
      (*i)++;

      if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CExprOpType::APPROX_EQUAL;
      }
      else
        id = CExprOpType::BIT_NOT;

      break;
    case '*':
      (*i)++;

      if      (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CExprOpType::TIMES_EQUALS;
      }
      else if (*i < str.size() && str[*i] == '*') {
        (*i)++;
        id = CExprOpType::POWER;
      }
      else
        id = CExprOpType::TIMES;

      break;
    case '/':
      (*i)++;

      if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CExprOpType::DIVIDE_EQUALS;
      }
      else
        id = CExprOpType::DIVIDE;

      break;
    case '%':
      (*i)++;

      if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CExprOpType::MODULUS_EQUALS;
      }
      else
        id = CExprOpType::MODULUS;

      break;
    case '+':
      (*i)++;

      if      (*i < str.size() && str[*i] == '+') {
        (*i)++;
        id = CExprOpType::INCREMENT;
      }
      else if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CExprOpType::PLUS_EQUALS;
      }
      else
        id = CExprOpType::PLUS;

      break;
    case '-':
      (*i)++;

#ifndef GNUPLOT_EXPR
      if      (*i < str.size() && str[*i] == '-') {
        (*i)++;
        id = CExprOpType::DECREMENT;
      }
      else if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CExprOpType::MINUS_EQUALS;
      }
      else
#endif
        id = CExprOpType::MINUS;

      break;
    case '<':
      (*i)++;

      if      (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CExprOpType::LESS_EQUAL;
      }
      else if (*i < str.size() && str[*i] == '<') {
        (*i)++;

        if (*i < str.size() && str[*i] == '=') {
          (*i)++;
          id = CExprOpType::BIT_LSHIFT_EQUALS;
        }
        else
          id = CExprOpType::BIT_LSHIFT;
      }
      else
        id = CExprOpType::LESS;

      break;
    case '>':
      (*i)++;

      if      (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CExprOpType::GREATER_EQUAL;
      }
      else if (*i < str.size() && str[*i] == '>') {
        (*i)++;

        if (*i < str.size() && str[*i] == '=') {
          (*i)++;
          id = CExprOpType::BIT_RSHIFT_EQUALS;
        }
        else
          id = CExprOpType::BIT_RSHIFT;
      }
      else
        id = CExprOpType::GREATER;

      break;
    case '=':
      (*i)++;

      if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CExprOpType::EQUAL;
      }
      else
        id = CExprOpType::EQUALS;

      break;
    case '&':
      (*i)++;

      if      (*i < str.size() && str[*i] == '&') {
        (*i)++;
        id = CExprOpType::LOGICAL_AND;
      }
      else if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CExprOpType::BIT_XOR_EQUALS;
      }
      else
        id = CExprOpType::BIT_AND;

      break;
    case '^':
      (*i)++;

      if (*i < str.size() && str[*i + 1] == '=') {
        (*i)++;
        id = CExprOpType::BIT_XOR_EQUALS;
      }
      else
        id = CExprOpType::BIT_XOR;

      break;
    case '|':
      (*i)++;

      if      (*i < str.size() && str[*i] == '|') {
        (*i)++;
        id = CExprOpType::LOGICAL_OR;
      }
      else if (*i < str.size() && str[*i] == '=') {
        (*i)++;
        id = CExprOpType::BIT_OR_EQUALS;
      }
      else
        id = CExprOpType::BIT_OR;

      break;
    case '?':
      (*i)++;
      id = CExprOpType::QUESTION;
      break;
    case ':':
      (*i)++;
      id = CExprOpType::COLON;
      break;
#ifdef GNUPLOT_EXPR
    case '[':
      (*i)++;
      id = CExprOpType::OPEN_SBRACKET;
      break;
    case ']':
      (*i)++;
      id = CExprOpType::CLOSE_SBRACKET;
      break;
    case '.': {
      uint i1 = (*i) + 1;

      if (i1 < str.size() && isdigit(str[i1]))
        return CExprOpType::UNKNOWN;

      (*i)++;

      id = CExprOpType::STR_CONCAT;

      break;
    }
#endif
    case ',':
      (*i)++;
      id = CExprOpType::COMMA;
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

  if (op == CExprOpType::UNKNOWN)
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

  CExprOpType id = CExprOpType::UNKNOWN;

  if      (identifier == "eq")
    id = CExprOpType::STR_EQUAL;
  else if (identifier == "ne")
    id = CExprOpType::STR_NOT_EQUAL;
  else {
    *i = j;
    id = CExprOpType::UNKNOWN;
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
