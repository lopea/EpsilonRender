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

namespace Epsilon::Vulkan
{
    SwapChain::SwapChain(const Device &device, const Surface &screen, GLFWwindow *window)
    {
      logicalDevice_ = device.GetLogicalHandle();
      CreateSwapchainHandle(device, screen, window);
    }

    SwapChain::~SwapChain()
    {

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
        throw InitializationException("VULKAN ERROR: Could not create a swap chain!");

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
      if (vkCreateRenderPass(logicalDevice_, &renderPassInfo, nullptr, &renderPass_) != VK_SUCCESS)
        throw std::runtime_error("VULKAN ERROR: Cannot create the proper render pass!!");


    }
}