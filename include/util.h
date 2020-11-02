#include <string>
#include <vector>
#include <sstream>

/**
 *  @brief  Split a string based on a separator
 *  @param str  The string to split
 *  @param separator  The separator to split by
 *  @return  The split as a vector of strings.
 */
std::vector<std::string> split(const std::string& str, const std::string& separator) {
  std::vector<std::string> out;
  
  int token_start = 0;

  size_t separator_start{};
  while ((separator_start = str.find(separator,token_start)) != std::string::npos){
    int token_length = separator_start - token_start;
    out.push_back(str.substr(token_start,token_length));
    token_start += token_length + separator.length();
  }
  out.push_back(str.substr(token_start));

  return out;
}

/**
 *  @brief  Split a string by whitespace
 *  @param str  The string to split
 *  @return  The split as a vector of strings.
 */
std::vector<std::string> split_whitespace(const std::string& str) {
  std::vector<std::string> out;
  
  std::istringstream iss(str);
  std::string token;
  while(iss >> token) out.push_back(token);
  
  return out;
}