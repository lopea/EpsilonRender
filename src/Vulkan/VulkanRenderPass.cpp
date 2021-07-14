//
// Created by Javier on 6/10/2021.
//

#include "VulkanRenderPass.h"
#include "VulkanException.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"

namespace Epsilon::Vulkan
{

    RenderPass::RenderPass(Device &device, VkFormat format)
     : handle_(nullptr), deviceHandle_(device), info_{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO}, buffer_(nullptr)
    {
      //setup the color attachemnt for 1 image that will be used for rendering
      VkAttachmentDescription colorAttachment{};
      colorAttachment.format = format;

      //only one sample since multisampling is not enabled
      colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

      //clear all data before rendering
      colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;

      //keep data when we are done rendering
      colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

      //we dont do anything with the stencil buffer, so we dont care about what happens to it
      colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

      //we dont care how the layout of the render pass starts off
      colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

      //we want the renderpass to be used for swap chain purposes
      colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

      //create a sub pass for actual rendering (we can be a little more complicated once we do more advanced rendering)
      VkAttachmentReference colorAttachmentRef{};

      //connect the sub pass to the 0th index in the pass
      colorAttachmentRef.attachment = 0;

      //we want to optimize this subpass as much as possible
      colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

      //we need to define the subpass that we want to create
      VkSubpassDescription subpass{};

      //describe that the subpass is used for graphics
      subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

      //attach the subpass to the description
      //NOTE: you can add more than one subpass for rendering!!!!!
      subpass.colorAttachmentCount = 1;
      subpass.pColorAttachments = &colorAttachmentRef;

      //Create a dependency
      VkSubpassDependency dependency{};

      //define a subpass as implicit to the render pass
      dependency.srcSubpass = VK_SUBPASS_EXTERNAL;

      //set the dependancy to the only subpass
      dependency.srcSubpass = 0;


      //set when the subpass should wait
      dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      dependency.srcAccessMask = 0;

      //set when the subpass should wait when the swap chain is currently getting written
      dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

      //another stupid struct to create the render pass
      VkRenderPassCreateInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};

      //only one color attachment
      renderPassInfo.attachmentCount = 1;
      renderPassInfo.pAttachments = &colorAttachment;

      //set the subpass for rendering
      renderPassInfo.pSubpasses = &subpass;
      renderPassInfo.subpassCount = 1;

      //add the dependency to the renderpass
      renderPassInfo.pDependencies = &dependency;
      renderPassInfo.dependencyCount = 1;

      //create the render pass
      if (vkCreateRenderPass(device.GetLogicalHandle(), &renderPassInfo, nullptr, &handle_) != VK_SUCCESS)
        throw InitializationException("VULKAN ERROR: Cannot create the proper render pass!!");


      //setup values for starting the renderpass

      //setup clear color formats
      clearColorValues.resize( 1);

      //setup renderpass handle
      info_.renderPass = handle_;
    }

    RenderPass::~RenderPass()
    {
      //destroy all memory from the render pass
      vkDestroyRenderPass(deviceHandle_.GetLogicalHandle(), handle_, nullptr);
    }

    void RenderPass::StartRendering(CommandBuffer &commandBuffer)
    {
      info_.clearValueCount = static_cast<uint32_t>(clearColorValues.size());
      info_.pClearValues = clearColorValues.data();

      //start recording the render pass
      vkCmdBeginRenderPass(commandBuffer.GetHandle(),&info_, VK_SUBPASS_CONTENTS_INLINE);
      buffer_ = &commandBuffer;

      //update the size of the pipelines

      VkViewport viewport{};
      viewport.width = static_cast<float>(info_.renderArea.extent.width);
      viewport.height = static_cast<float>(info_.renderArea.extent.height);
      viewport.minDepth = 0;
      viewport.maxDepth = 1;

      vkCmdSetViewport(commandBuffer.GetHandle(), 0, 1, &viewport);

      vkCmdSetScissor(commandBuffer.GetHandle(), 0, 1, &info_.renderArea);

    }

    RenderPass::RenderPass(Device &device, const VkRenderPassCreateInfo &info) : handle_(nullptr), deviceHandle_(device)
    {
      //create the render pass
      if (vkCreateRenderPass(device.GetLogicalHandle(), &info, nullptr, &handle_) != VK_SUCCESS)
        throw InitializationException("VULKAN ERROR: Cannot create the proper render pass!!");
    }

    void RenderPass::EndRendering()
    {
      if(!buffer_)
        throw RuntimeException("Render pass ran with out a command buffer ?");

      //stop the accepting any rendering requests
      vkCmdEndRenderPass(buffer_->GetHandle());

      //dont need the buffer anymore so clear it
      buffer_ = nullptr;
    }

    void RenderPass::SetExtent(const VkExtent2D &extent)
    {
      info_.renderArea.extent = extent;
    }
    void RenderPass::SetOffset(const VkOffset2D &offset)
    {
      info_.renderArea.offset = offset;
    }

    void RenderPass::SetClearColorValue(const VkClearValue &value, uint32_t index)
    {
      clearColorValues[index] = value;
    }

    void RenderPass::SetFrameBuffer(VkFramebuffer const &buffer)
    {
      info_.framebuffer = buffer;
    }


}