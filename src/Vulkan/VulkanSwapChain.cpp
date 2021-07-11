//
// Created by Javier on 6/4/2021.
//

#include <GLFW/glfw3.h>
#include <algorithm>
#include "VulkanSwapChain.h"
#include "VulkanDevice.h"
#include "VulkanSurface.h"
#include "../SwapChainContext.h"
#include "VulkanQueueFamilies.h"
#include "VulkanException.h"
#include "VulkanShader.h"

namespace Epsilon::Vulkan
{
    SwapChain::SwapChain(Device &device, Surface &screen, GLFWwindow *window)
        : Device_(device), surface_(screen), windowHandle_(window)
    {
      //create the swapchain and connect it to the window
      Initialize();
      DescriptorPool_ = new DescriptorPool(device);
    }


    SwapChain::~SwapChain()
    {
      delete DescriptorPool_;

      Cleanup();
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
        framebufferInfo.renderPass = renderPass_->GetHandle();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = extent_.width;
        framebufferInfo.height = extent_.height;
        framebufferInfo.layers = 1;

        //create the buffer
        if (vkCreateFramebuffer(Device_.GetLogicalHandle(),
                                &framebufferInfo, nullptr, &frameBuffers_[i]) !=VK_SUCCESS)
          throw std::runtime_error("VULKAN ERROR: Failed to create a framebuffer!");
      }
    }


    VkSurfaceFormatKHR SwapChain::GetSwapChainFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
    {
      //go through all the available formats
      for (const VkSurfaceFormatKHR &format : availableFormats)
      {
        //if the format is what we want, sent it
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
          return format;

      }

      //return something at this point
      return availableFormats[0];
    }

    void SwapChain::CreateSwapchainHandle(const Device &device, const Surface &surface, GLFWwindow *window)
    {
      SwapChainContext context(device.GetPhysicalHandle(), surface.GetSurfaceHandle());
      minImageCount_ = context.capabilities.minImageCount + 1;
      //create a struct for creating a swapchain
      VkSwapchainCreateInfoKHR info{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};

      auto surfaceFormat = GetSwapChainFormat(context.formats);
      //populate struct data
      info.surface = surface.GetSurfaceHandle();
      info.minImageCount = minImageCount_;
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

      vkGetSwapchainImagesKHR(device.GetLogicalHandle(), handle_, &imageCount, nullptr);
      images_.resize(imageCount);
      vkGetSwapchainImagesKHR(device.GetLogicalHandle(), handle_, &imageCount, images_.data());


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
      renderPass_ = new RenderPass(Device_, format_);

      CreateFrameBuffers();

      commandPool_ = new CommandPool(Device_,
                                     QueueFamilyIndices(Device_.GetPhysicalHandle(), surface_.GetSurfaceHandle())
                                     .graphicsInd_.value());

      commandBuffers_ = commandPool_->CreateCommandBuffers(VK_COMMAND_BUFFER_LEVEL_PRIMARY, framesPerFlight);

      CreateSemaphores();

    }

    void SwapChain::Cleanup()
    {
      //wait until data is not used anymore
      vkDeviceWaitIdle(Device_.GetLogicalHandle());


      //destroy semaphores
      for (int i = 0; i < framesPerFlight; i++)
      {
        vkDestroySemaphore(Device_.GetLogicalHandle(), imageAvailableSemaphore_[i], nullptr);
        vkDestroySemaphore(Device_.GetLogicalHandle(), renderFinishedSemaphore_[i], nullptr);
        vkDestroyFence(Device_.GetLogicalHandle(), fences_[i], nullptr);
      }

      //destroy all framebuffers
      for (auto frameBuffer: frameBuffers_)
        vkDestroyFramebuffer(Device_.GetLogicalHandle(), frameBuffer, nullptr);

      delete renderPass_;
      //remove all the views associated with the swapchain
      for (auto imageView: views_)
        vkDestroyImageView(Device_.GetLogicalHandle(), imageView, nullptr);
      commandBuffers_.clear();

      delete commandPool_;

      //destroy the swapchain
      vkDestroySwapchainKHR(Device_.GetLogicalHandle(), handle_, nullptr);
      frameBuffers_.clear();
      views_.clear();
      fences_.clear();

    }

    void SwapChain::Present()
    {
      if(allowRendering)
      {
        //this is fucking torture
        VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};

        //store a location of the necessary semaphore in a sendable format
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore_[currentFrame_]};

        VkCommandBuffer commandBuffer[] = {commandBuffers_[currentFrame_].GetHandle()};
        //store the stage of the semaphore
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        //populate struct
        submitInfo.waitSemaphoreCount = 1;

        //send wait requirements
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        //send command buffers
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = commandBuffer;

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
        VkResult result = vkQueuePresentKHR(Device_.GetPresentQueue(), &presentinfo);

        //check if the swap chain needs to be recreated due to a window change
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || needsRefresh)
        {
          //get the size of the new window
          int width, height;
          glfwGetFramebufferSize(windowHandle_, &width, &height);

          //check if rendering is allowed
          allowRendering = !(width == 0 || height == 0);

          //update the swap chain if rendering is continuing
          if (allowRendering)
            Refresh();
        } else if (result != VK_SUCCESS)
          throw RuntimeException("Unable to present swapchain image!");

        //start the buffer for recording
        vkWaitForFences(Device_.GetLogicalHandle(), 1, &fences_[currentFrame_], true, UINT64_MAX);

        currentFrame_++;
        currentFrame_ %= framesPerFlight;
      }
      else if (needsRefresh)
      {
        //get the size of the new window
        int width, height;
        glfwGetFramebufferSize(windowHandle_, &width, &height);

        //check if rendering is allowed
        allowRendering = !(width == 0 || height == 0);

        //update the swap chain if rendering is continuing
        if (allowRendering)
          Refresh();
      }

    }

    void SwapChain::ClearFrame()
    {
      //dont do anything if the swapchain cant render
      if (!allowRendering)
        return;

      commandBuffers_[currentFrame_].Reset();

      vkAcquireNextImageKHR(Device_.GetLogicalHandle(), handle_, UINT32_MAX, imageAvailableSemaphore_[currentFrame_],
                            nullptr, &imageIndex);


      //start recording the render pass
      commandBuffers_[currentFrame_].BeginRecording();

      //update the renderpass values
      renderPass_->SetExtent(extent_);
      renderPass_->SetOffset({0,0});
      renderPass_->SetFrameBuffer(frameBuffers_[imageIndex]);
      renderPass_->SetClearColorValue({{0,0,0,1}});
      renderPass_->StartRendering(commandBuffers_[currentFrame_]);

    }

    void SwapChain::RenderShader(vkShader *shader)
    {
      //dont do anything if swapchain cant render
      if (!allowRendering)
        return;

      vkCmdBindPipeline(commandBuffers_[currentFrame_].GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, shader->GetPipeline());
      vkCmdDraw(commandBuffers_[currentFrame_].GetHandle(), 3, 1, 0, 0);

    }

    void SwapChain::FinishFrame()
    {
      //dont render anything if the swapchain cant present anything
      if (!allowRendering)
        return;

      renderPass_->EndRendering();

      commandBuffers_[currentFrame_].EndRecording();
    }

    void SwapChain::Refresh()
    {
      //clear all data for the swap chain
      Cleanup();

      //recreate it with the new values
      Initialize();

      //clear refresh flag
      needsRefresh = false;
    }

    void SwapChain::MarkForRefresh()
    {
      needsRefresh = true;
    }

}