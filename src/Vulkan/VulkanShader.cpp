//
// Created by Javier on 5/31/2021.
//

#include "VulkanShader.h"
#include <fstream>
#include <vector>
#include "VulkanMeshHelper.h"

VkShaderModule CreateModule(const std::vector<char> &data, VkDevice device)
{
  VkShaderModuleCreateInfo shaderInfo{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
  shaderInfo.codeSize = data.size();
  shaderInfo.pCode = reinterpret_cast<const uint32_t *>(data.data());

  VkShaderModule result;

  if (vkCreateShaderModule(device, &shaderInfo, nullptr, &result) != VK_SUCCESS)
    throw std::runtime_error("VULKAN SHADER ERROR: Cannot create a proper shader module!!!");

  return result;

}

namespace Epsilon::Vulkan
{
    vkShader::vkShader(const std::string &vertFileLocation, const std::string &fragFileLocation,
                       Vulkan::SwapChain& swapChain) : pipeline_(nullptr),
                                                                        logicalDevice_(swapChain.GetDevice())
    {
      std::ifstream vertShader(vertFileLocation, std::ios::binary), fragShader(fragFileLocation, std::ios::binary);

      //check if files are valuable
      if (!vertShader.is_open())
        throw std::runtime_error("VULKAN SHADER ERROR: Cannot open Vertex File: " + vertFileLocation);

      if (!fragShader.is_open())
        throw std::runtime_error("VULKAN SHADER ERROR: Cannot open Fragment File: " + fragFileLocation);

      //get all the data to an array
      std::vector<char> vertData((std::istreambuf_iterator<char>(vertShader)),
                                 (std::istreambuf_iterator<char>()));

      std::vector<char> fragData((std::istreambuf_iterator<char>(fragShader)),
                                 (std::istreambuf_iterator<char>()));


      VkShaderModule vertexModule = CreateModule(vertData, logicalDevice_);
      VkShaderModule fragmentModule = CreateModule(fragData, logicalDevice_);

      VkPipelineShaderStageCreateInfo vertShaderStageInfo{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
      vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
      vertShaderStageInfo.module = vertexModule;
      vertShaderStageInfo.pName = "VertMain";

      VkPipelineShaderStageCreateInfo fragShaderStageInfo{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
      fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
      fragShaderStageInfo.module = fragmentModule;
      fragShaderStageInfo.pName = "FragMain";

      VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

      ////////////////////////////////////////////////////
      /// Vertex Input Setup
      ////////////////////////////////////////////////////
      VkPipelineVertexInputStateCreateInfo vertexInfo{VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};

      //get the information of a vertex
      auto attribdescriptions = GetVertexDescription();
      auto bindingDescriptions = GetVertexBindingDescription();
      //set the vertex info
      //set the spacing between data and whether the data is per-vertex or per-instance
      vertexInfo.vertexBindingDescriptionCount = 1;
      vertexInfo.pVertexBindingDescriptions = &bindingDescriptions;

      //set the number of attributes passed to the vertex
      vertexInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribdescriptions.size());
      vertexInfo.pVertexAttributeDescriptions = attribdescriptions.data();
      ////////////////////////////////////////////////////
      /// Input Assembly
      ////////////////////////////////////////////////////
      //what type of geometry will be rendered when it comes to presentation?
      VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo
          {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};

      //set the topology(how meshes and the like are going to be rendered) Filled in this case
      inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

      //TODO: change this to true when using more advanced rendering!
      inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

      ////////////////////////////////////////////////////
      /// Viewport Setup
      ////////////////////////////////////////////////////

      //create a dynamic state
      VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};

      //we want to keep the viewport and the scissor for this shader dynamic so that it can be changed without recreating the pipeline
      VkDynamicState DynamicStates[] { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

      //set the states to the info struct
      dynamicStateCreateInfo.dynamicStateCount = std::size(DynamicStates);
      dynamicStateCreateInfo.pDynamicStates = DynamicStates;

      ////////////////////////////////////////////////////
      /// Rasterizer Setup
      ////////////////////////////////////////////////////

      //so many fucking structs ffs
      VkPipelineRasterizationStateCreateInfo rasterizer{VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};

      //dont clamp fragment's depth value when out of bounds, just remove it
      rasterizer.depthClampEnable = VK_FALSE;

      //we want to render something lmao
      rasterizer.rasterizerDiscardEnable = VK_FALSE;

      //fill triangles when rendering
      rasterizer.polygonMode = VK_POLYGON_MODE_FILL;

      //even though we dont use it, set the line width
      rasterizer.lineWidth = 1.0f;

      //set cull modes
      rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
      rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

      //have no bias when it comes depth tests
      rasterizer.depthBiasEnable = VK_FALSE;

      ////////////////////////////////////////////////////
      /// Multisampling Setup
      ////////////////////////////////////////////////////

      //setup the multsampling values
      VkPipelineMultisampleStateCreateInfo multisampling{VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
      multisampling.sampleShadingEnable = VK_FALSE;
      multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

      ////////////////////////////////////////////////////
      /// Color Blending Setup
      ////////////////////////////////////////////////////

      //setup the values for mixing fragment colors
      VkPipelineColorBlendAttachmentState colorBlendAttachmentState{};

      colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_G_BIT |
                                                 VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_A_BIT;
      colorBlendAttachmentState.blendEnable = VK_FALSE;

      //create another handle for the color state
      VkPipelineColorBlendStateCreateInfo colorBlend{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
      //disable any blending for now
      colorBlend.logicOpEnable = VK_FALSE;

      //attach the attachment to the handle
      colorBlend.attachmentCount = 1;
      colorBlend.pAttachments = &colorBlendAttachmentState;

      ////////////////////////////////////////////////////
      /// Final pipeline layout
      ////////////////////////////////////////////////////

      //create an empty one for now
      VkPipelineLayoutCreateInfo layoutInfo{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};

      if (vkCreatePipelineLayout(logicalDevice_, &layoutInfo, nullptr, &layout_) != VK_SUCCESS)
        throw std::runtime_error("VULKAN ERROR: Pipeline layout could not be created!!!");

      VkGraphicsPipelineCreateInfo graphicsPipelineInfo{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
      graphicsPipelineInfo.pStages = shaderStages;
      graphicsPipelineInfo.stageCount = 2;
      graphicsPipelineInfo.subpass = 0;
      graphicsPipelineInfo.pVertexInputState = &vertexInfo;
      graphicsPipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
      graphicsPipelineInfo.pMultisampleState = &multisampling;
      graphicsPipelineInfo.pColorBlendState = &colorBlend;
      graphicsPipelineInfo.layout = layout_;
      graphicsPipelineInfo.pRasterizationState = &rasterizer;
      graphicsPipelineInfo.renderPass =swapChain.GetRenderPass(); //context.GetWindowRenderPass();
      graphicsPipelineInfo.pDynamicState = &dynamicStateCreateInfo;

      if (vkCreateGraphicsPipelines(logicalDevice_, nullptr, 1, &graphicsPipelineInfo, nullptr, &pipeline_) !=
          VK_SUCCESS)
        throw std::runtime_error("VULKAN ERROR: Failed to create a proper pipeline!!!!");

      vkDestroyShaderModule(logicalDevice_, vertexModule, nullptr);
      vkDestroyShaderModule(logicalDevice_, fragmentModule, nullptr);
    }


    vkShader::~vkShader()
    {
      vkDestroyPipeline(logicalDevice_, pipeline_, nullptr);
      vkDestroyPipelineLayout(logicalDevice_, layout_, nullptr);
    }

    SpecificationType vkShader::GetShaderType() const
    {
      return SpecificationType::Vulkan;
    }

}