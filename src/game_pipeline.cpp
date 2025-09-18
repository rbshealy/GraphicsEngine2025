//
// Created by raysh on 9/17/2025.
//

#include "game_pipeline.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace GEngine{

    GamePipeline::GamePipeline(const std::string& vertfilepath, const std::string& fragfilepath){
      createGraphicsPipeline(vertfilepath, fragfilepath);
      }

    std::vector<char> GamePipeline::readFile(const std::string& filepath){
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};

        if (!file.is_open()){
          throw std::runtime_error("failed to open " + filepath);
        }

        size_t filesize = file.tellg();
        std::vector<char> buffer(filesize);

        file.seekg(0);
        file.read(buffer.data(),filesize);

        file.close();
        return buffer;
      };

    void GamePipeline::createGraphicsPipeline(const std::string& vertfilepath, const std::string& fragfilepath){
      auto vertCode = readFile(vertfilepath);
      auto fragCode = readFile(fragfilepath);

      std::cout << "Vertex Shader Code Size: " << vertCode.size() << std::endl;
      std::cout << "Fragment Shader Code Size: " << fragCode.size() << std::endl;
      }

}

