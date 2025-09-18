//
// Created by raysh on 9/17/2025.
//

#pragma once

#include <string>
#include <vector>

namespace GEngine {

    class GamePipeline {
      public:
        GamePipeline(const std::string& vertfilepath, const std::string& fragfilepath);

        private:
          static std::vector<char> readFile(const std::string& filepath);
          void createGraphicsPipeline(const std::string& vertfilepath, const std::string& fragfilepath);


      };

}


