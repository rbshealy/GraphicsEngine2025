//
// Created by raysh on 9/17/2025.
//

#pragma once

#include "game_device.h"

#include <string>
#include <vector>

namespace GEngine {

    struct PipelineConfigInfo {
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        PipelineConfigInfo() = default;

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class GamePipeline {
        public:
            GamePipeline(GameDevice &device, const std::string& vertfilepath, const std::string& fragfilepath, const PipelineConfigInfo& configInfo);
            ~GamePipeline();

            GamePipeline(const GamePipeline&) = delete;
            GamePipeline& operator=(const GamePipeline&) = delete;

            void bind(VkCommandBuffer commandBuffer);
            static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);


        private:
            static std::vector<char> readFile(const std::string& filepath);
            void createGraphicsPipeline(const std::string& vertfilepath, const std::string& fragfilepath,  const PipelineConfigInfo& configInfo);

            void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

            GameDevice& GameDevice;
            VkPipeline graphicsPipeline;
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;

      };

}


