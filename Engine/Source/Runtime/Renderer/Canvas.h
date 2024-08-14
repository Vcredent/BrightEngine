/* ======================================================================== */
/* Canvas.h                                                                 */
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
#ifndef _RENDERER_SCENE_RENDER_H_
#define _RENDERER_SCENE_RENDER_H_

#include "Drivers/RenderDevice.h"
#include <Turbine/Math.h>

class Canvas
{
public:
    Canvas(RenderDevice *vRenderDevice);
   ~Canvas();

    void SetViewport(float vW, float vH) { w = vW; h = vH; }
    void CmdBeginCanvasRendering(VkCommandBuffer *pCmdBuffer);
    void CmdEndCanvasRendering(VkCommandBuffer cmdBuffer);

private:
    void _Initialize();
    void _CreateTextureAttachments();
    void _CleanUpTextureAttachments();

    RenderDevice *rd;
    VkRenderPass renderPass;
    RenderDevice::Texture2D *msaa;
    RenderDevice::Texture2D *depth;
    RenderDevice::Texture2D *resolve;
    VkFramebuffer framebuffer;

    float w;
    float h;
};

#endif /* _RENDERER_SCENE_RENDER_H_ */
