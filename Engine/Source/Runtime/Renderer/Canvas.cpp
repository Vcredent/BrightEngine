/* ======================================================================== */
/* Canvas.cpp                                                               */
/* ======================================================================== */
/*                        This file is part of:                             */
/*                           COPILOT ENGINE                                 */
/* ======================================================================== */
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
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, e1ither express or implied */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
/*                                                                          */
/* ======================================================================== */
#include "Canvas.h"
#include <array>

Canvas::Canvas(RenderDevice *vRenderDevice)
    : rd(vRenderDevice)
{
    _Initialize();
}

Canvas::~Canvas()
{
    _CleanUpTextureAttachments();
    rd->DestroySampler(sampler);
    rd->DestroyRenderPass(renderPass);
    rd->FreeCommandBuffer(cmdBuffer);
}

void Canvas::SetViewport(uint32_t vW, uint32_t vH)
{
    if (vW <= 0 || vH <= 0)
        return;

    w = vW;
    h = vH;

    if (color->width != vW || color->height != vH)
    {
        _CleanUpTextureAttachments();
        _CreateTextureAttachments();
    }
}

void Canvas::BeginCanvasRendering(VkCommandBuffer *pCmdBuffer)
{
    *pCmdBuffer = cmdBuffer;

    rd->BeginCommandBuffer(cmdBuffer, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT);

    std::array<VkClearValue, 3> clearValues = {};
    clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
    clearValues[1].depthStencil = { 1.0f, 0 };
    clearValues[2].color = { 0.1f, 0.1f, 0.1f, 1.0f };;

    VkRect2D rect = {};
    rect.offset = { 0, 0 };
    rect.extent = { w, h };
    rd->CmdBeginRenderPass(cmdBuffer, renderPass, std::size(clearValues), std::data(clearValues), framebuffer, &rect);
}

void Canvas::EndCanvasRendering()
{
    rd->CmdEndRenderPass(cmdBuffer);
    rd->EndCommandBuffer(cmdBuffer);

    // submit
    rd->CmdBufferSubmit(cmdBuffer,
                        0, nullptr,
                        0, nullptr,
                           nullptr,
                           queue,
                           nullptr);
}

void Canvas::_Initialize()
{
    rd->AllocateCommandBuffer(&cmdBuffer);

    std::array<VkAttachmentDescription, 3> attachments;

    RenderDeviceContext *rdc = rd->GetDeviceContext();
    surfaceFormat = rd->GetSurfaceFormat();
    sampleCount = rd->GetMSAASamples();
    std::vector<VkFormat> expectDepthFormats = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
    depthFormat = rdc->FindSupportedFormat(expectDepthFormats ,VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    queue = rdc->GetQueue();

    // msaa
    attachments[0].flags = no_flag_bits;
    attachments[0].format = surfaceFormat;
    attachments[0].samples = sampleCount;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // depth
    attachments[1].flags = no_flag_bits;
    attachments[1].format = depthFormat;
    attachments[1].samples = sampleCount;
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // depth
    attachments[2].flags = no_flag_bits;
    attachments[2].format = surfaceFormat;
    attachments[2].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[2].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[2].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[2].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkAttachmentReference colorRef = {
      .attachment = 0,
      .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    VkAttachmentReference depthRef = {
      .attachment = 1,
      .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };

    VkAttachmentReference resolveRef = {
      .attachment = 2,
      .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorRef;
    subpassDescription.pDepthStencilAttachment = &depthRef;
    subpassDescription.pResolveAttachments = &resolveRef;

    VkSubpassDependency subpassDependency = {};
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask =  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    rd->CreateRenderPass(std::size(attachments), std::data(attachments), 1, &subpassDescription, 1, &subpassDependency, &renderPass);

    RenderDevice::SamplerCreateInfo samplerCreateInfo;
    rd->CreateSampler(&samplerCreateInfo, &sampler);
    _CreateTextureAttachments();
}

void Canvas::_CreateTextureAttachments()
{
    RenderDevice::TextureCreateInfo textureCreateInfo = {
      .width = w,
      .height = h,
    };

    // color
    textureCreateInfo.samples = sampleCount;
    textureCreateInfo.format = surfaceFormat;
    textureCreateInfo.aspect_mask = VK_IMAGE_ASPECT_COLOR_BIT;
    textureCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    color = rd->CreateTexture(&textureCreateInfo);

    // depth
    textureCreateInfo.samples = sampleCount;
    textureCreateInfo.format = depthFormat;
    textureCreateInfo.aspect_mask = VK_IMAGE_ASPECT_DEPTH_BIT;
    textureCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    depth = rd->CreateTexture(&textureCreateInfo);

    // resolve
    textureCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    textureCreateInfo.format = surfaceFormat;
    textureCreateInfo.aspect_mask = VK_IMAGE_ASPECT_COLOR_BIT;
    textureCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    resolve = rd->CreateTexture(&textureCreateInfo);
    rd->BindTextureSampler(resolve, sampler);

    // barrier
    VkCommandBuffer barrierCmdBuffer;
    rd->BeginCommandBufferOneTime(&barrierCmdBuffer);
    RenderDevice::PipelineMemoryBarrier barrier = {
      .image = {
        .texture = resolve,
        .old_image_layout = VK_IMAGE_LAYOUT_UNDEFINED,
        .new_image_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .src_access_mask = 0,
        .dst_access_mask = VK_ACCESS_HOST_READ_BIT | VK_ACCESS_HOST_WRITE_BIT,
      }
    };
    rd->CmdPipelineBarrier(barrierCmdBuffer, &barrier);
    rd->EndCommandBufferOneTime(barrierCmdBuffer);

    // framebuffer
    VkImageView attachments[] = {
      color->image_view,
      depth->image_view,
      resolve->image_view,
    };
    rd->CreateFramebuffer(w, h, ARRAY_SIZE(attachments), attachments, renderPass, &framebuffer);
}

void Canvas::_CleanUpTextureAttachments()
{
    rd->DestroyTexture(color);
    rd->DestroyTexture(depth);
    rd->DestroyTexture(resolve);
    rd->DestroyFramebuffer(framebuffer);
}
