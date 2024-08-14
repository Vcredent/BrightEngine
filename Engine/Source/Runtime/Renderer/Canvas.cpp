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
    rd->DestroyRenderPass(renderPass);
}

void Canvas::CmdBeginCanvasRendering(VkCommandBuffer *pCmdBuffer)
{
    VkCommandBuffer cmdBuffer;
    rd->AllocateCommandBuffer(&cmdBuffer);
    *pCmdBuffer = cmdBuffer;

    rd->BeginCommandBuffer(cmdBuffer, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT);
}

void Canvas::CmdEndCanvasRendering(VkCommandBuffer cmdBuffer)
{
    rd->EndCommandBuffer(cmdBuffer);
}

void Canvas::_Initialize()
{
    std::array<VkAttachmentDescription, 3> attachments;

    RenderDeviceContext *rdc = rd->GetDeviceContext();
    VkFormat surfaceFormat = rd->GetSurfaceFormat();
    VkSampleCountFlagBits sampleCount = rd->GetMSAASamples();
    std::vector<VkFormat> expectDepthFormats = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
    VkFormat depthFormat = rdc->FindSupportedFormat(expectDepthFormats ,VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

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

    VkAttachmentReference msaaRef = {
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
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &msaaRef;
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

    SetViewport(32.0f, 32.0f);
    _CreateTextureAttachments();
}

void Canvas::_CreateTextureAttachments()
{

}

void Canvas::_CleanUpTextureAttachments()
{

}
