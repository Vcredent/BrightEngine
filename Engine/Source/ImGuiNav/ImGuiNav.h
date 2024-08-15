/* ------------------------------------------------------------------------ */
/* ImGuiNav.h                                                               */
/* ------------------------------------------------------------------------ */
/*                        This file is part of:                             */
/*                            BRIGHT ENGINE                                 */
/* ------------------------------------------------------------------------ */
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
/* ------------------------------------------------------------------------ */
#ifndef _PILOTUI_H_
#define _PILOTUI_H_

#include "imgui/imgui.h"
#include <Runtime/Renderer/Displayer.h>
#include <GLFW/glfw3.h>

namespace ImGuiNav
{
    // create and destroy
    void Initialize(Displayer *displayer);
    void Terminate();

    // api
    void BeginNewFrame(VkCommandBuffer cmdBuffer);
    void EndNewFrame(VkCommandBuffer cmdBuffer);
    bool Begin(const char *title, bool* p_open = NULL, ImGuiWindowFlags flags = 0);
    void End();
    void BeginViewport(const char *title);
    void EndViewport();
    void DrawTexture(ImTextureID v_texture, const ImVec2& v_size);

    // widgets
    void DragFloat(const char *label, float *v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f");
    void DragFloat2(const char *label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f");
    void DragFloat3(const char *label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f");
    void DragFloat4(const char *label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f");

    // vulkan
    ImTextureID AddTexture(RenderDevice::Texture2D *texture);
    void RemoveTexture(ImTextureID v_texture);

    // private
    void _DragScalarN(const char *label, float *v, int v_number, float v_speed, float v_min, float v_max, const char *format);
}

#endif /* _PILOTUI_H_ */
