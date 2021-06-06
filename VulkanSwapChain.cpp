//
// Created by Javier on 6/4/2021.
//

#include <GLFW/glfw3.h>
#include <algorithm>
#include "VulkanSwapChain.h"
#include "VulkanDevice.h"
#include "VulkanSurface.h"
#include "SwapChainContext.h"
#include "VulkanQueueFamilies.h"
#include "VulkanInitializationException.h"
#include "VulkanShader.h"

namespace Epsilon::Vulkan
{
    SwapChain::SwapChain(Device &device, Surface &screen, GLFWwindow *window)
        : Device_(device), surface_(screen), windowHandle_(window)
    {
      //create the swapchain and connect it to the window
      Initialize();
    }


    SwapChain::~SwapChain()
    {
      Cleanup();
    }

    void SwapChain::CreateCommandPool(const Device &device, const Surface &screen)
    {
      QueueFamilyIndices queueFamilyIndices(device.GetPhysicalHandle(), screen.GetSurfaceHandle());

      //another struct on the list
      VkCommandPoolCreateInfo info{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};

      //store the graphics index in the family
      info.queueFamilyIndex = queueFamilyIndices.graphicsInd_.value();
      info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

      //create the command pool
      if (vkCreateCommandPool(Device_.GetLogicalHandle(), &info, nullptr, &commandPool_) != VK_SUCCESS)
        throw std::runtime_error("VULKAN ERROR: Failed to create a command buffer!");

    }

    VkPresentModeKHR SwapChain::GetSwapChainPresentMode(const std::vector<VkPresentModeKHR> &availableModes)
    {
      //iterate through all modes that are available
      for (VkPresentModeKHR currentMode : availableModes)
      {
        //only check for the mailbox mode if available
        if (currentMode == VK_PRESENT_MODE_MAILBOX_KHR)
          return currentMode;
      }

      //return the standard present mode
      return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::GetSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window)
    {
      //check if the device capabilities already contain valid info
      if (capabilities.currentExtent.width != UINT32_MAX)
        return capabilities.currentExtent;

      //get the framebuffer size for the window
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);

      //create a handle for the size of the window
      VkExtent2D extent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

      //clamp values just in case
      extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
      extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

      return extent;
    }


    void SwapChain::CreateFrameBuffers()
    {
      //create the size for the framebuffers
      frameBuffers_.resize(images_.size());

      //create every buffer for the swapchain
      for (size_t i = 0; i < views_.size(); i++)
      {
        //store all the attachments for the buffer
        VkImageView attachments[] = {views_[i]};

        //create the struct that give info to the create the struct
        VkFramebufferCreateInfo framebufferInfo{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};

        //populate the struct
        framebufferInfo.renderPass = renderPass_;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = extent_.width;
        framebufferInfo.height = extent_.height;
        framebufferInfo.layers = 1;

        //create the buffer
        if (vkCreateFramebuffer(Device_.GetLogicalHandle(), &framebufferInfo, nullptr, &frameBuffers_[i]) !=
            VK_SUCCESS)
          throw std::runtime_error("VULKAN ERROR: Failed to create a framebuffer!");
      }
    }

    void SwapChain::CreateCommandBuffers()
    {
      //resize the buffers based on the frame buffers
      commandBuffers_.resize(frameBuffers_.size());

      //please end my suffering with all these structs!!!!
      VkCommandBufferAllocateInfo info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
      info.commandPool = commandPool_;
      info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      info.commandBufferCount = static_cast<uint32_t>(commandBuffers_.size());

      //create the command buffer
      if (vkAllocateCommandBuffers(Device_.GetLogicalHandle(), &info, commandBuffers_.data()) != VK_SUCCESS)
        throw std::runtime_error("VULKAN ERROR: UNABLE TO ALLOCATE COMMANDBUFFERS!");

    }

    VkSurfaceFormatKHR SwapChain::GetSwapChainFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
    {
      //go through all the available formats
      for (const VkSurfaceFormatKHR &format : availableFormats)
      {
        //if the format is what we want, sent it
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
          return format;

      }

      //return something at this point
      return availableFormats[0];
    }

    void SwapChain::CreateSwapchainHandle(const Device &device, const Surface &surface, GLFWwindow *window)
    {
      SwapChainContext context(device.GetPhysicalHandle(), surface.GetSurfaceHandle());
      unsigned imageCount = context.capabilities.minImageCount + 1;
      if (context.capabilities.minImageCount > 0 && imageCount > context.capabilities.maxImageCount)
        imageCount = context.capabilities.maxImageCount;
      //create a struct for creating a swapchain
      VkSwapchainCreateInfoKHR info{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};

      auto surfaceFormat = GetSwapChainFormat(context.formats);
      //populate struct data
      info.surface = surface.GetSurfaceHandle();
      info.minImageCount = imageCount;
      info.imageFormat = surfaceFormat.format;
      info.imageColorSpace = surfaceFormat.colorSpace;
      info.imageExtent = GetSwapExtent(context.capabilities, window);
      info.imageArrayLayers = 1;
      info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

      //get the queue family info for the device
      QueueFamilyIndices indices(device.GetPhysicalHandle(), surface.GetSurfaceHandle());
      uint32_t queueFamily[] = {indices.graphicsInd_.value(), indices.presentInd_.value()};

      //set proper values based on how the queue families are set up
      if (indices.graphicsInd_ != indices.presentInd_)
      {
        info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        info.queueFamilyIndexCount = 2;
        info.pQueueFamilyIndices = queueFamily;
      } else
      {
        info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        info.queueFamilyIndexCount = 0;
        info.pQueueFamilyIndices = nullptr;
      }

      info.preTransform = context.capabilities.currentTransform;
      info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
      info.presentMode = GetSwapChainPresentMode(context.presents);
      info.clipped = true;
      info.oldSwapchain = VK_NULL_HANDLE;

      //create the swapchain
      if (vkCreateSwapchainKHR(device.GetLogicalHandle(), &info, nullptr, &handle_) != VK_SUCCESS)
        //something went terribly wrong
        throw InitializationException("Could not create a swap chain!");

      //get the swapchain images
      unsigned int imageSize;
      vkGetSwapchainImagesKHR(device.GetLogicalHandle(), handle_, &imageSize, nullptr);
      images_.resize(imageSize);
      vkGetSwapchainImagesKHR(device.GetLogicalHandle(), handle_, &imageSize, images_.data());


      extent_ = info.imageExtent;
      format_ = info.imageFormat;

    }

    void SwapChain::CreateImageViews(const Device &device)
    {
      //resize the image view to the amount of images in the swap chain
      views_.reserve(images_.size());

      //iterate through all the images in the swapchain
      for (auto &vkSwapChainImage : images_)
      {
        //we gotta create a new fucking struct, again...
        VkImageViewCreateInfo info{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};


        //set the image for the view
        info.image = vkSwapChainImage;

        //set the view type and the image format
        info.format = format_;

        //set the type of the image format
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;

        //allow the color channels to be interchangeable
        info.components.r = info.components.g = info.components.b = info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        //describe the image and what it does.
        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        info.subresourceRange.baseMipLevel = 0;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.layerCount = 1;

        VkImageView view;

        //create the image view for the swapchain
        if (vkCreateImageView(device.GetLogicalHandle(), &info, nullptr, &view) != VK_SUCCESS)
          //something went terribly wrong
          throw InitializationException("VULKAN ERROR: Cannot create an image view!");

        //add the view to the list
        views_.push_back(view);
      }
    }

    void SwapChain::CreateRenderPass()
    {
      //setup the color attachemnt for 1 image that will be used for rendering
      VkAttachmentDescription colorAttachment{};
      colorAttachment.format = format_;

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
      if (vkCreateRenderPass(Device_.GetLogicalHandle(), &renderPassInfo, nullptr, &renderPass_) != VK_SUCCESS)
        throw InitializationException("VULKAN ERROR: Cannot create the proper render pass!!");

    }

    void SwapChain::CreateSemaphores()
    {
      //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
      VkSemaphoreCreateInfo semaphoreInfo{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

      //REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
      VkFenceCreateInfo fenceInfo{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};

      //set the flag for fences
      fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;


      //initalize list
      imageAvailableSemaphore_.resize(framesPerFlight);
      renderFinishedSemaphore_.resize(framesPerFlight);
      fences_.resize(framesPerFlight);
      //create the semaphores and fences
      for (int i = 0; i < framesPerFlight; i++)
      {
        if (vkCreateSemaphore(Device_.GetLogicalHandle(), &semaphoreInfo, nullptr, &imageAvailableSemaphore_[i]) !=
            VK_SUCCESS ||
            vkCreateSemaphore(Device_.GetLogicalHandle(), &semaphoreInfo, nullptr, &renderFinishedSemaphore_[i]) !=
            VK_SUCCESS)
          throw InitializationException("Cannot create semaphores!!!");

        if (vkCreateFence(Device_.GetLogicalHandle(), &fenceInfo, nullptr, &fences_[i]) != VK_SUCCESS)
          throw InitializationException("Cannot create proper fences!!!");
      }

    }

    void SwapChain::Initialize()
    {
      CreateSwapchainHandle(Device_, surface_, windowHandle_);
      CreateImageViews(Device_);
      CreateRenderPass();
      CreateFrameBuffers();
      CreateCommandPool(Device_, surface_);
      CreateCommandBuffers();
      CreateSemaphores();
      initialized_ = true;
    }

    void SwapChain::Cleanup()
    {
      //destroy semaphores
      for (int i = 0; i < framesPerFlight; i++)
      {
        vkDestroySemaphore(Device_.GetLogicalHandle(), imageAvailableSemaphore_[i], nullptr);
        vkDestroySemaphore(Device_.GetLogicalHandle(), renderFinishedSemaphore_[i], nullptr);
        vkDestroyFence(Device_.GetLogicalHandle(), fences_[i], nullptr);
      }
      //destroy the command pool
      vkDestroyCommandPool(Device_.GetLogicalHandle(), commandPool_, nullptr);

      //destroy all framebuffers
      for (auto frameBuffer: frameBuffers_)
        vkDestroyFramebuffer(Device_.GetLogicalHandle(), frameBuffer, nullptr);

      //cleanup render pass
      vkDestroyRenderPass(Device_.GetLogicalHandle(), renderPass_, nullptr);

      //remove all the views associated with the swapchain
      for (auto imageView: views_)
        vkDestroyImageView(Device_.GetLogicalHandle(), imageView, nullptr);

      //destroy the swapchain
      vkDestroySwapchainKHR(Device_.GetLogicalHandle(), handle_, nullptr);
    }

    void SwapChain::Present()
    {

      //wait until the current frame is ready for presentation


      //get the next image


      //this is fucking torture
      VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};

      //store a location of the necessary semaphore in a sendable format
      VkSemaphore waitSemaphores[] = {imageAvailableSemaphore_[currentFrame_]};

      //store the stage of the semaphore
      VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

      //populate struct
      submitInfo.waitSemaphoreCount = 1;

      //send wait requirements
      submitInfo.pWaitSemaphores = waitSemaphores;
      submitInfo.pWaitDstStageMask = waitStages;

      //send command buffers
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &commandBuffers_[currentFrame_];

      //create the done timer in a sendable format
      VkSemaphore signalSemaphore[] = {renderFinishedSemaphore_[currentFrame_]};
      submitInfo.signalSemaphoreCount = 1;
      submitInfo.pSignalSemaphores = signalSemaphore;
      vkResetFences(Device_.GetLogicalHandle(), 1, &fences_[currentFrame_]);
      //create send the render request to the queue
      if (vkQueueSubmit(Device_.GetGraphicsQueue(), 1, &submitInfo, fences_[currentFrame_]) != VK_SUCCESS)
        throw std::runtime_error("VULKAN ERROR: Failed to submit the command buffer!!!");

      VkPresentInfoKHR presentinfo{VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};

      presentinfo.waitSemaphoreCount = 1;
      presentinfo.pWaitSemaphores = signalSemaphore;

      VkSwapchainKHR swapchains[] = {handle_};

      presentinfo.swapchainCount = 1;

      presentinfo.pSwapchains = swapchains;
      presentinfo.pImageIndices = &imageIndex;

      //show the value to the swapchain
      vkQueuePresentKHR(Device_.GetPresentQueue(), &presentinfo);

      currentFrame_ = (++currentFrame_) % framesPerFlight;




    }

    void SwapChain::ClearFrame()
    {
      //if i had a cent for every struct i've made in this project, i would have enough to buy a gun so i can end my misery
      VkCommandBufferBeginInfo cbinfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
      cbinfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

      //start the buffer for recording
      vkWaitForFences(Device_.GetLogicalHandle(), 1, &fences_[currentFrame_], true, UINT64_MAX);

      vkResetCommandBuffer(commandBuffers_[currentFrame_], 0);
      vkAcquireNextImageKHR(Device_.GetLogicalHandle(), handle_, UINT32_MAX, imageAvailableSemaphore_[currentFrame_],
                            nullptr, &imageIndex);


      if (vkBeginCommandBuffer(commandBuffers_[currentFrame_], &cbinfo) != VK_SUCCESS)
        throw std::runtime_error("VULKAN ERROR: FAILED TO INITIALIZE COMMANDBUFFER!!!");

      //start running the render pass

      //another struct, another cent
      VkRenderPassBeginInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
      renderPassInfo.renderPass = renderPass_;
      renderPassInfo.framebuffer = frameBuffers_[imageIndex];

      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = extent_;

      //set the clear color for the texture
      VkClearValue clearColor = {0, 0, 1, 1};

      renderPassInfo.clearValueCount = 1;
      renderPassInfo.pClearValues = &clearColor;

      //set the render pass
      vkCmdBeginRenderPass(commandBuffers_[currentFrame_], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void SwapChain::RenderShader(VulkanShader *shader)
    {
      vkCmdBindPipeline(commandBuffers_[currentFrame_], VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline());
      vkCmdDraw(commandBuffers_[currentFrame_], 3, 1, 0, 0);

    }

    void SwapChain::FinishFrame()
    {
      vkCmdEndRenderPass(commandBuffers_[currentFrame_]);

      if (vkEndCommandBuffer(commandBuffers_[currentFrame_]) != VK_SUCCESS)
        throw std::runtime_error("Failed to record command buffer!!!");
    }

}