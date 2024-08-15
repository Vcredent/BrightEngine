/* ------------------------------------------------------------------------ */
/* Sandbox.cpp                                                              */
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
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied  */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
/*                                                                          */
/* ------------------------------------------------------------------------ */
#include <Runtime/Win32/RenderDeviceCotnextWin32.h>
#include <ImGuiEx/ImGuiEx.h>
#include <Runtime/Renderer/Canvas.h>
#include <Runtime/Renderer/CoordinateAxisRender.h>
#include <Runtime/Camera/Camera.h>

Window *window;
RenderDeviceContextWin32 *rdc;
RenderDevice *rd;
Displayer *displayer;
Canvas *canvas;
CoordinateAxisRender *coordinateAxisLine;

namespace Sandbox
{
    void Initialize()
    {
        window = new Window("TurbineEngine", 1920, 1080);
        rdc = new RenderDeviceContextWin32(window);
        rd = rdc->CreateRenderDevice();
        displayer = new Displayer(rd, window);
        ImGuiEx::Initialize(displayer);
        canvas = new Canvas(rd);
        coordinateAxisLine = new CoordinateAxisRender(rd, canvas->GetRenderPass());
    }

    void Terminate()
    {
        delete coordinateAxisLine;
        delete canvas;
        ImGuiEx::Terminate();
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

    // static
    static RenderDevice::Texture2D *preview;
    static ImTextureID imPreview = NULL;
    static ImVec2 region = ImVec2(32.0f, 32.0f);

    Camera camera;

    while (!window->IsClose()) {
        window->PollEvents();

        camera.SetAspectRatio(region.x / region.y);
        camera.Update();

        canvas->BeginCanvasRendering(&cmdBuffer, region.x, region.y);
        {
            coordinateAxisLine->SetViewUniformBuffer(camera.GetViewMatrix(), camera.GetProjectionMatrix());
            coordinateAxisLine->CmdDrawCoordinateAxisRendering(cmdBuffer, region.x, region.y);
        }
        canvas->EndCanvasRendering();
        canvas->GetFinishedRenderColorAttachment(&preview);

        displayer->BeginDisplayRendering(&cmdBuffer);
        {
            ImGuiEx::BeginNewFrame(cmdBuffer);
            {
                static bool showDemoFlag = true;
                ImGui::ShowDemoWindow(&showDemoFlag);

                ImGuiEx::Begin("调试");
                {
                    Vector3 position = camera.GetPosition();
                    ImGuiEx::DragFloat3("位置", glm::value_ptr(position), 0.01f);
                    camera.SetPosition(position);
                    Vector3 direction = camera.GetDirection();
                    ImGuiEx::DragFloat3("方向", glm::value_ptr(direction), 0.01f);
                    camera.SetDirection(direction);
                }
                ImGuiEx::End();

                ImGuiEx::BeginViewport("场景");
                {
                    region = ImGui::GetContentRegionAvail();

                    if (imPreview)
                        ImGuiEx::DestroyImTextureID(imPreview);
                    imPreview = ImGuiEx::CreateImTextureID(preview);

                    ImGui::Image(imPreview, region);
                }
                ImGuiEx::EndViewport();
            }
            ImGuiEx::EndNewFrame(cmdBuffer);
        }
        displayer->EndDisplayRendering();
    }

    Sandbox::Terminate();

    return 0;
}