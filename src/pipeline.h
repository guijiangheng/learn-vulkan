#pragma once

#include <string>
#include <vector>

namespace lve {

class Pipeline {
 public:
  Pipeline(std::string vertFilepath, std::string fragFilepath);

 private:
  static std::vector<char> readFile(std::string filepath);
  void create(std::string vertFilepath, std::string fragFilepath);
};

}  // namespace lve
