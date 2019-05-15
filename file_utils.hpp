#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <fstream>
#include <sstream>

class FileUtils
{
 public:
  static std::string readFile(const std::string& filename)
  {
    std::ifstream in(filename);
    std::istreambuf_iterator<char> begin(in);
    std::istreambuf_iterator<char> end;
    std::string str(begin, end);
    in.close();
    return str;
  }

  /*
  static std::string readFile(const std::string& filename)
  {
    std::ifstream in(filename);
    std::ostringstream tmp;
    tmp << in.rdbuf();
    in.close();
    return tmp.str();
  }
  */
};
#endif  // !FILE_UTILS_HPP