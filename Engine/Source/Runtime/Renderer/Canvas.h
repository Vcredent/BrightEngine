/* ======================================================================== */
/* Canvas.h                                                                 */
/* ======================================================================== */
/*                        This file is part of:                             */
/*                           COPILOT ENGINE                                 */
/* ======================================================================== */
/*                                                                          */
/* Copyright (C) 2022 TheSampson All rights reserved.                       */
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
#ifndef _RENDERER_SCENE_RENDER_H_
#define _RENDERER_SCENE_RENDER_H_

#include "Drivers/RenderDevice.h"
#include <Turbine/Math.h>

class Canvas
{
public:
    Canvas(RenderDevice *v_RenderDevice);
   ~Canvas();

    VkRenderPass GetRenderPass() { return renderPass; }
    void GetFinishedRenderColorAttachment(RenderDevice::Texture2D **ppFinishedRenderColorAttachment)
      { *ppFinishedRenderColorAttachment = resolve; }

    void BeginCanvasRendering(VkCommandBuffer *pCmdBuffer, uint32_t v_width, uint32_t v_height);
    void EndCanvasRendering();

private:
    void _Initialize();
    void _CreateTextureAttachments();
    void _CleanUpTextureAttachments();

    RenderDevice *rd;
    VkRenderPass renderPass;
    VkCommandBuffer cmdBuffer;
    VkQueue queue;
    VkSampler sampler;
    RenderDevice::Texture2D *color;
    RenderDevice::Texture2D *depth;
    RenderDevice::Texture2D *resolve;
    VkFramebuffer framebuffer;

    uint32_t w = 32.0f;
    uint32_t h = 32.0f;
    VkFormat surfaceFormat;
    VkFormat depthFormat;
    VkSampleCountFlagBits sampleCount;
};

#endif /* _RENDERER_SCENE_RENDER_H_ */
