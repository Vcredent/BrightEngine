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
#include "Sandbox.h"

namespace Sandbox
{
    void DrawCanvas()
    {
        VkCommandBuffer cmdBuffer;
        canvas->CmdBeginCanvasRendering(&cmdBuffer);
        canvas->CmdEndCanvasRendering(cmdBuffer);
    }

    void DrawEditor()
    {
        VkCommandBuffer cmdBuffer;
        displayer->CmdBeginDisplayRendering(&cmdBuffer);
        ImGuiNav::BeginNewFrame(cmdBuffer);
        {
            static bool showDemoWindow = true;
            ImGui::ShowDemoWindow(&showDemoWindow);

            // draw scene.
            ImGuiNav::BeginViewport("场景");
            {
                ImVec2 region = ImGui::GetContentRegionAvail();
                camera->SetAspectRatio(region.x / region.y);
            }
            ImGuiNav::EndViewport();
        }
        ImGuiNav::EndNewFrame(cmdBuffer);
        displayer->CmdEndDisplayRendering(cmdBuffer);
    }
}

int main()
{
    Sandbox::Initialize();

    while (!window->IsClose())
    {
        window->PollEvents();
        Sandbox::UpdateCamera();
        Sandbox::DrawCanvas();
        Sandbox::DrawEditor();
    }

    Sandbox::Terminate();

    return 0;
}