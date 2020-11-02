// author: Ebrahim Ebrahim

#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

/**
 *  @brief  Split a string based on a separator
 *  @param str  The string to split
 *  @param separator  The separator to split by
 *  @return  The split as a vector of strings
 */
std::vector<std::string> split(const std::string& str,
                               const std::string& separator) {
  std::vector<std::string> out;

  int token_start = 0;

  size_t separator_start{};
  while ((separator_start = str.find(separator, token_start)) !=
         std::string::npos) {
    int token_length = separator_start - token_start;
    out.push_back(str.substr(token_start, token_length));
    token_start += token_length + separator.length();
  }
  out.push_back(str.substr(token_start));

  return out;
}

/**
 *  @brief  Split a string by whitespace
 *  @param str  The string to split
 *  @return  The split as a vector of strings
 */
std::vector<std::string> split_whitespace(const std::string& str) {
  std::vector<std::string> out;

  std::istringstream iss(str);
  std::string token;
  while (iss >> token) out.push_back(token);

  return out;
}

/**
 *  @brief  Strip leading and trailing characters from a string
 *  @param str  The string to strip
 *  @param remove  The set of characters to strip away
 *  @return  A stripped copy of the string
 */
std::string strip(const std::string& str,
                  const std::unordered_set<char>& remove = {' '}) {
  std::string result;
  result.reserve(str.size());
  std::string buffer;
  std::istringstream input_string{str};
  char c;

  while (input_string.get(c))
    if (remove.find(c) ==
        remove.end()) {  // if c is not one of the characters we are stripping
      result.push_back(c);
      break;
    }

  while (input_string.get(c)) {
    if (remove.find(c) == remove.end()) {
      if (!buffer.empty()) {
        result += buffer;
        buffer.clear();
      }
      result.push_back(c);
    } else
      buffer.push_back(c);
  }
  if (remove.find(c) == remove.end()) result += buffer;

  return result;
}