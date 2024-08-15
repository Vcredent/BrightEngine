/* ======================================================================== */
/* CoordinateAxisRender.cpp                                                 */
/* ======================================================================== */
/*                        This file is part of:                             */
/*                            BRIGHT ENGINE                                 */
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
#include "CoordinateAxisRender.h"

CoordinateAxisRender::CoordinateAxisRender(RenderDevice *vRenderDevice, VkRenderPass vRenderPass)
    : rd(vRenderDevice)
{
    uniformBuffer = rd->CreateBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, sizeof(ViewUniformBuffer));

    VkDescriptorSetLayoutBinding binds[] = {
      { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, VK_NULL_HANDLE}
    };

    rd->CreateDescriptorSetLayout(ARRAY_SIZE(binds), binds, &descriptor_set_layout);
    rd->AllocateDescriptorSet(descriptor_set_layout, &descriptor_set);
    rd->UpdateDescriptorSetBuffer(uniformBuffer, 0, descriptor_set);

    RenderDevice::ShaderInfo shader_info = {
      "CoordinateAxis",
      "CoordinateAxis",
      0, NULL,
      0, NULL,
      1, &descriptor_set_layout,
    };

    RenderDevice::PipelineCreateInfo create_info = {
      /* render_pass= */ vRenderPass,
      /* polygon= */ VK_POLYGON_MODE_FILL,
      /* topology= */ VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
      /* cull_mode= */ VK_CULL_MODE_BACK_BIT,
      /* samples= */ rd->GetMSAASamples(),
      /* line_width= */ 64.0f,
    };

    pipeline = rd->CreateGraphicsPipeline(&create_info, &shader_info);
}

CoordinateAxisRender::~CoordinateAxisRender()
{
    rd->DestroyPipeline(pipeline);
    rd->FreeDescriptorSet(descriptor_set);
    rd->DestroyDescriptorSetLayout(descriptor_set_layout);
    rd->DestroyBuffer(uniformBuffer);
}

void CoordinateAxisRender::SetViewUniformBuffer(mat4 view, mat4 projection)
{
    viewUniformBuffer.viewMatrix = view;
    viewUniformBuffer.projectionMatrix = projection;
    rd->WriteBuffer(uniformBuffer, 0, sizeof(ViewUniformBuffer), &viewUniformBuffer);
}

void CoordinateAxisRender::CmdDrawCoordinateAxisRendering(VkCommandBuffer cmdBuffer, uint32_t w, uint32_t h)
{
    rd->CmdBindPipeline(cmdBuffer, pipeline);
    rd->CmdSetViewport(cmdBuffer, w, h);
    rd->CmdBindDescriptorSet(cmdBuffer, pipeline, descriptor_set);
    vkCmdSetLineWidth(cmdBuffer, 2.0f);
    rd->CmdDraw(cmdBuffer, 4);
}