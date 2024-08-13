/* ------------------------------------------------------------------------ */
/* Displayer.cpp                                                            */
/* ------------------------------------------------------------------------ */
/*                        This file is part of:                             */
/*                            BRIGHT ENGINE                                 */
/* ------------------------------------------------------------------------ */
/*                                                                          */
/* Copyright (C) 2022 Vcredent All rights reserved.                         */
/*                                                                          */
/* Licensed under the Apache License, Version 2.0 (the "License");          */
/* you may not use this file except in compliance with the License.         */
/*                                                                          */
/* You may obtain a copy of the License at                                  */
/*     http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                          */
/* Unless required by applicable law or agreed to in writing, software      */
/* distributed under the License is distributed on an "AS IS" BASIS,        */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied  */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
/*                                                                          */
/* ------------------------------------------------------------------------ */
#include "Displayer.h"
#include <algorithm>

Displayer::Displayer(RenderDevice *vRenderDevice, Window *vWindow)
    : rd(vRenderDevice), currentFocusedWindow(vWindow)
{
    instance = rd->GetDeviceContext()->GetInstance();
    physicalDevice = rd->GetDeviceContext()->GetPhysicalDevice();
    device = rd->GetDeviceContext()->GetDevice();
    queueFamily = rd->GetDeviceContext()->GetQueueFamily();
    commandPool = rd->GetDeviceContext()->GetCommandPool();
    queue = rd->GetDeviceContext()->GetQueue();

    _Initialize();
}

Displayer::~Displayer()
{
    vkDestroySemaphore(device, displayWindow->imageAvailableSemaphore, allocation_callbacks);
    vkDestroySemaphore(device, displayWindow->renderFinishedSemaphore, allocation_callbacks);
    vkDestroySwapchainKHR(device, displayWindow->swapchain, allocation_callbacks);
    vkDestroyRenderPass(device, displayWindow->renderPass, allocation_callbacks);
    _CleanUpSwapChain();
    vkDestroySurfaceKHR(instance, displayWindow->surface, allocation_callbacks);
    free(displayWindow);
}

void Displayer::CmdBeginDisplayRendering(VkCommandBuffer *p_cmd_buffer)
{
    _UpdateSwapChain();
    vkAcquireNextImageKHR(device, displayWindow->swapchain, UINT64_MAX, displayWindow->imageAvailableSemaphore, nullptr, &acquireNextIndex);

    VkCommandBuffer cmdBuffer;
    cmdBuffer = displayWindow->swapchainResources[acquireNextIndex].cmdBuffer;
    rd->BeginCommandBuffer(cmdBuffer, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT);

    VkClearValue clear_color = {
            0.10f, 0.10f, 0.10f, 1.0f
    };

    VkRect2D rect = {};
    rect.extent = { displayWindow->width, displayWindow->height };
    rd->CmdBeginRenderPass(cmdBuffer, displayWindow->renderPass, 1, &clear_color, displayWindow->swapchainResources[acquireNextIndex].framebuffer, &rect);

    *p_cmd_buffer = cmdBuffer;
}

void Displayer::CmdEndDisplayRendering(VkCommandBuffer cmdBuffer)
{
    rd->CmdEndRenderPass(cmdBuffer);
    rd->EndCommandBuffer(cmdBuffer);

    VkPipelineStageFlags mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    rd->CmdBufferSubmit(cmdBuffer, 1, &displayWindow->imageAvailableSemaphore, 1, &displayWindow->renderFinishedSemaphore, &mask, queue, VK_NULL_HANDLE);
    rd->Present(queue, displayWindow->swapchain, acquireNextIndex, displayWindow->renderFinishedSemaphore);
}

void Displayer::_Initialize()
{
    VkResult U_ASSERT_ONLY err;

    /* imalloc display displayWindow struct and set surface */
    displayWindow = (DisplayWindow *) imalloc(sizeof(DisplayWindow));
    currentFocusedWindow->CreateVulkanSurfaceKHR(instance, allocation_callbacks, &displayWindow->surface);

    VkSurfaceCapabilitiesKHR capabilities;
    err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, displayWindow->surface, &capabilities);
    assert(!err);

    /* pick surface format */
    uint32_t format_count = 0;
    err = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, displayWindow->surface, &format_count, nullptr);
    assert(!err);

    VkSurfaceFormatKHR *surface_formats_khr = (VkSurfaceFormatKHR *) imalloc(sizeof(VkSurfaceFormatKHR) * format_count);
    err = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, displayWindow->surface, &format_count, surface_formats_khr);
    assert(!err);

    VkSurfaceFormatKHR surface_format = PickSurfaceFormat(surface_formats_khr, format_count);
    displayWindow->format = surface_format.format;
    displayWindow->colorSpace = surface_format.colorSpace;

    free(surface_formats_khr);

    /* image buffer count */
    uint32_t desired_buffer_count = 3;
    desired_buffer_count = std::clamp(desired_buffer_count, capabilities.minImageCount, capabilities.maxImageCount);
    displayWindow->imageBufferCount = desired_buffer_count;

    displayWindow->compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    displayWindow->presentMode = VK_PRESENT_MODE_FIFO_KHR;

    VkSemaphoreCreateInfo semaphore_create_info = {};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    err = vkCreateSemaphore(device, &semaphore_create_info, allocation_callbacks, &displayWindow->imageAvailableSemaphore);
    assert(!err);

    err = vkCreateSemaphore(device, &semaphore_create_info, allocation_callbacks, &displayWindow->renderFinishedSemaphore);
    assert(!err);

    _CreateSwapChain();
}

void Displayer::_CreateSwapChain()
{
    VkResult U_ASSERT_ONLY err;
    VkSwapchainKHR oldSwappchain;

    oldSwappchain = displayWindow->swapchain;

    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, displayWindow->surface, &capabilities);
    displayWindow->width = capabilities.currentExtent.width;
    displayWindow->height = capabilities.currentExtent.height;

    if (!oldSwappchain) {
        // attachment
        VkAttachmentDescription color_attachment = {
                /* flags */ no_flag_bits,
                /* format */ displayWindow->format,
                /* samples */ VK_SAMPLE_COUNT_1_BIT,
                /* loadOp */VK_ATTACHMENT_LOAD_OP_CLEAR,
                /* storeOp */ VK_ATTACHMENT_STORE_OP_STORE,
                /* stencilLoadOp */ VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                /* stencilStoreOp */ VK_ATTACHMENT_STORE_OP_DONT_CARE,
                /* initialLayout */ VK_IMAGE_LAYOUT_UNDEFINED,
                /* finalLayout */ VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        };

        VkAttachmentReference color_attachment_reference = {};
        color_attachment_reference.attachment = 0;
        color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_reference;

        VkSubpassDependency subpass_dependency = {};
        subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpass_dependency.dstSubpass = 0;
        subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpass_dependency.srcAccessMask = 0;
        subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        // create render pass
        VkRenderPassCreateInfo render_pass_create_info = {};
        render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_create_info.attachmentCount = 1;
        render_pass_create_info.pAttachments = &color_attachment;
        render_pass_create_info.subpassCount = 1;
        render_pass_create_info.pSubpasses = &subpass;
        render_pass_create_info.dependencyCount = 1;
        render_pass_create_info.pDependencies = &subpass_dependency;

        err = vkCreateRenderPass(device, &render_pass_create_info, allocation_callbacks, &displayWindow->renderPass);
        assert(!err);
    }

    /* create swap chain */
    VkSwapchainCreateInfoKHR swappchainCreateInfo = {
            /* sType */ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            /* pNext */ nextptr,
            /* flags */ no_flag_bits,
            /* surface */ displayWindow->surface,
            /* minImageCount */ displayWindow->imageBufferCount,
            /* imageFormat */ displayWindow->format,
            /* imageColorSpace */ displayWindow->colorSpace,
            /* imageExtent */ capabilities.currentExtent,
            /* imageArrayLayers */ 1,
            /* imageUsage */ VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            /* imageSharingMode */ VK_SHARING_MODE_EXCLUSIVE,
            /* queueFamilyIndexCount */ 1,
            /* pQueueFamilyIndices */ &queueFamily,
            /* preTransform */ capabilities.currentTransform,
            /* compositeAlpha */ displayWindow->compositeAlpha,
            /* presentMode */ displayWindow->presentMode,
            /* clipped */ VK_TRUE,
            /* oldSwapchain */ oldSwappchain,
    };

    err = vkCreateSwapchainKHR(device, &swappchainCreateInfo, allocation_callbacks, &displayWindow->swapchain);
    assert(!err);

    if (oldSwappchain)
        vkDestroySwapchainKHR(device, oldSwappchain, allocation_callbacks);

    /* initialize swap chain resources */
    displayWindow->swapchainResources = (SwapchainResource *) imalloc(sizeof(SwapchainResource) * displayWindow->imageBufferCount);

    std::vector<VkImage> swap_chain_images;
    swap_chain_images.resize(displayWindow->imageBufferCount);
    err = vkGetSwapchainImagesKHR(device, displayWindow->swapchain, &displayWindow->imageBufferCount, std::data(swap_chain_images));
    assert(!err);

    for (uint32_t i = 0; i < displayWindow->imageBufferCount; i++) {
        displayWindow->swapchainResources[i].image = swap_chain_images[i];

        VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
                /* sType */ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                /* pNext */ nextptr,
                /* commandPool */ commandPool,
                /* level */ VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                /* commandBufferCount */ 1
        };

        vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &(displayWindow->swapchainResources[i].cmdBuffer));

        VkImageViewCreateInfo imageViewCreateInfo = {
                /* sType */ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                /* pNext */ nextptr,
                /* flags */ no_flag_bits,
                /* image */ displayWindow->swapchainResources[i].image,
                /* viewType */ VK_IMAGE_VIEW_TYPE_2D,
                /* format */ displayWindow->format,
                /* components */
                    {
                        .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                        .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                        .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                        .a = VK_COMPONENT_SWIZZLE_IDENTITY,
                    },
                /* subresourceRange */
                    {
                        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                        .baseMipLevel = 0,
                        .levelCount = 1,
                        .baseArrayLayer = 0,
                        .layerCount = 1,
                    },
        };

        err = vkCreateImageView(device, &imageViewCreateInfo, allocation_callbacks, &(displayWindow->swapchainResources[i].image_view));
        assert(!err);

        VkImageView framebuffer_attachments[] = { displayWindow->swapchainResources[i].image_view };

        VkFramebufferCreateInfo framebuffer_create_info = {
                /* sType */ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                /* pNext */ nextptr,
                /* flags */ no_flag_bits,
                /* renderPass */ displayWindow->renderPass,
                /* attachmentCount */ ARRAY_SIZE(framebuffer_attachments),
                /* pAttachments */ framebuffer_attachments,
                /* width */ displayWindow->width,
                /* height */ displayWindow->height,
                /* layers */ 1,
        };

        err = vkCreateFramebuffer(device, &framebuffer_create_info, allocation_callbacks, &(displayWindow->swapchainResources[i].framebuffer));
        assert(!err);
    }
}

void Displayer::_CleanUpSwapChain()
{
    for (uint32_t i = 0; i < displayWindow->imageBufferCount; i++) {
        vkFreeCommandBuffers(device, commandPool, 1, &displayWindow->swapchainResources[i].cmdBuffer);
        vkDestroyFramebuffer(device, displayWindow->swapchainResources[i].framebuffer, allocation_callbacks);
        vkDestroyImageView(device, displayWindow->swapchainResources[i].image_view, allocation_callbacks);
    }

    free(displayWindow->swapchainResources);
}

void Displayer::_UpdateSwapChain()
{
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, displayWindow->surface, &capabilities);
    VkExtent2D extent = capabilities.currentExtent;

    /* is update */
    if ((extent.width != displayWindow->width || extent.height != displayWindow->height) && (extent.width != 0 || extent.height != 0)) {
        vkDeviceWaitIdle(device);
        _CleanUpSwapChain();
        _CreateSwapChain();
    }
}
