#include <CExprI.h>

extern std::string
CExprInsertEscapeCodes(const std::string &str)
{
  uint i = 0;

  for (i = 0; i < str.size(); ++i)
    if (str[i] == '\\' || str[i] == '\"' || str[i] == '\'' || (str[i] != ' ' && ! isgraph(str[i])))
      break;

  if (i >= str.size())
    return "";

  std::string str1;

  for (i = 0; i < str.size(); ++i) {
    char c = str[i];

    if (c == ' ' || (c != '\\' && c != '\"' && c != '\'' && isgraph(c))) {
      str1 += c;

      continue;
    }

    str1 += '\\';

    switch (c) {
      case '\a': str1 += 'a'; break;
      case '\b': str1 += 'b'; break;
      case '\f': str1 += 'f'; break;
      case '\n': str1 += 'n'; break;
      case '\r': str1 += 'r'; break;
      case '\t': str1 += 't'; break;
      case '\v': str1 += 'v'; break;
      case '\"': str1 += '\"'; break;
      case '\'': str1 += '\''; break;
      case '\033': str1 += 'e'; break;
      default: {
        int digit1 = c/64;
        int digit2 = (c - digit1*64)/8;
        int digit3 = c - digit1*64 - digit2*8;

        str1 += '0' + digit1;
        str1 += '0' + digit2;
        str1 += '0' + digit3;

        break;
      }
    }
  }

  return str1;
}
