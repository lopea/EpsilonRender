//
// Created by Javier on 6/7/2021.
//

#ifndef EPSILONRENDERER_VULKANBUFFER_H
#define EPSILONRENDERER_VULKANBUFFER_H

#include <vulkan/vulkan.h>
#include "VulkanCommandPool.h"

namespace Epsilon::Vulkan
{
    //forward declarations
    class Device;
    class SwapChain;

    // A block of memory for Vulkan related purposes
    class Buffer
    {
    public:

        /*!
         * Buffer Constructor
         * Create a new buffer that will be used for vulkan related operations
         * @param device reference to the device that will use this buffer for vulkan operations
         * @param chain reference to the swap buffer configuration that contains the main command pool, which is needed
         *              for proper copy operations.
         * @param size the size of the buffer in bytes
         * @param usage shows how the buffer will be used
         * @param memFlags how the buffer will be allocated
         */
        Buffer(Device &device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memFlags,
               CommandPool &pool);

        /*!
         * Add data from CPU memory and add it Buffer
         * @param data reference to the CPU memory
         * @param size the size of the memory to set
         * @param offset the size in byte on how far from the start of the buffer's data do you want to set the data
         */
        void SetData(void* data, VkDeviceSize size, uint64_t offset = 0);

        /*!
         * @return Vulkan handle associated with this buffer
         */
        [[nodiscard]] VkBuffer GetBufferHandle() const { return handle_; }

        /*
         * @return Vulkan handle associated with the memory of this buffer
         */
        [[nodiscard]] VkDeviceMemory GetMemoryHandle() const { return memoryHandle_; }

        /*!
         * Copy the data from another buffer to this one
         * @param other the buffer to copy the data from
         */
        void CopyBuffer(const Buffer& other);

        /*!
         * Buffer Destructor
         * Ensures all buffer data has been freed properly
         */
        ~Buffer();

        //avoid any copying
        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;
    private:
        //! store reference to the device that handles all vulkan operations
        Device& device_;

        //! store Command Pool to be able to execute commands for copying
        CommandPool &commandPool_;

        //! store the location of the buffer that vulkan gives us
        VkBuffer handle_{};

        //! store the location of memory the buffer is referring to
        VkDeviceMemory memoryHandle_{};

        //! the size of the buffer in bytes
        VkDeviceSize size_;

        //! how the buffer is used throughout its lifetime
        VkBufferUsageFlags usage_;

        //! how the memory will be allocated by the buffer
        VkMemoryPropertyFlags memFlags_;

        /*!
         * Find if the current device supports the memory flags
         * @param typeFilter all the supported flags that are used for this device
         * @param properties the properties to find in the device
         * @return property flag if successful, throws an Initialization exception if the device
         * cannot support the memory properties given.
         */
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    };
}

#endif //EPSILONRENDERER_VULKANBUFFER_H
