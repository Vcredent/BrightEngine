/* ------------------------------------------------------------------------ */
/* Sandbox.cpp                                                              */
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
#include <Runtime/Win32/RenderDeviceCotnextWin32.h>
#include <ImGuiNav/ImGuiNav.h>
#include <Runtime/Renderer/Canvas.h>

Window *window;
RenderDeviceContextWin32 *rdc;
RenderDevice *rd;
Displayer *displayer;
Canvas *canvas;

namespace Sandbox
{
    void Initialize()
    {
        window = new Window("TurbineEngine", 1080, 1060);
        rdc = new RenderDeviceContextWin32(window);
        rd = rdc->CreateRenderDevice();
        displayer = new Displayer(rd, window);
        ImGuiNav::Initialize(displayer);
        canvas = new Canvas(rd);
    }

    void Terminate()
    {
        delete canvas;
        ImGuiNav::Terminate();
        delete displayer;
        rdc->DestroyRenderDevice(rd);
        delete rdc;
        delete window;
    }
}

int main()
{
    Sandbox::Initialize();
    VkCommandBuffer cmdBuffer;

    while (!window->IsClose())
    {
        window->PollEvents();

        displayer->BeginDisplayRendering(&cmdBuffer);
        {
            ImGuiNav::BeginNewFrame(cmdBuffer);
            {
                static bool showDemoFlag = true;
                ImGui::ShowDemoWindow(&showDemoFlag);

                ImGuiNav::BeginViewport("场景");
                {

                }
                ImGuiNav::EndViewport();
            }
            ImGuiNav::EndNewFrame(cmdBuffer);
        }
        displayer->EndDisplayRendering(cmdBuffer);

    }

    Sandbox::Terminate();

    return 0;
}