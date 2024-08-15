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
    static ImVec2 canvasRegion = ImVec2(32.0f, 32.0f);
    static RenderDevice::Texture2D *finishedRenderTexture;
    static ImTextureID preview = NULL;

    void DrawCanvas()
    {
        camera->SetAspectRatio(canvasRegion.x / canvasRegion.y);
        canvas->SetViewport(canvasRegion.x, canvasRegion.y);

        VkCommandBuffer cmdBuffer;
        canvas->CmdBeginCanvasRendering(&cmdBuffer);
        {
            coordinateAxis->SetViewUniformBuffer(camera->GetViewMatrix(), camera->GetProjectionMatrix());
            coordinateAxis->CmdDrawCoordinateAxisRendering(cmdBuffer, canvasRegion.x, canvasRegion.y);
        }
        canvas->CmdEndCanvasRendering();
        canvas->GetFinishedRenderColorAttachment(&finishedRenderTexture);
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
                canvasRegion = ImGui::GetContentRegionAvail();
                if (preview)
                    ImGuiNav::RemoveTexture(preview);

                preview = ImGuiNav::AddTexture(finishedRenderTexture);
                ImGui::Image(preview, canvasRegion);
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