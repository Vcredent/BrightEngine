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

// 示例数据结构
struct MyItem {
    int id;
    char name[32];
    float value;
    bool selected;
};

void ShowEditableTable() {
    // 创建示例数据
    static MyItem items[] = {
      { 1, "Item 1", 0.5f, false },
      { 2, "Item 2", 1.0f, true },
      { 3, "Item 3", 0.7f, false },
    };
    static int itemCount = sizeof(items) / sizeof(items[0]);

    // 开始表格
    if (ImGui::BeginTable("EditableTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        // 设置列标题
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Value");
        ImGui::TableSetupColumn("Selected");
        ImGui::TableHeadersRow();

        // 填充表格数据
        for (int row = 0; row < itemCount; row++) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", items[row].id);

            ImGui::TableSetColumnIndex(1);
            ImGui::InputText("##Name", items[row].name, IM_ARRAYSIZE(items[row].name));

            ImGui::TableSetColumnIndex(2);
            ImGui::SliderFloat("##Value", &items[row].value, 0.0f, 1.0f);

            ImGui::TableSetColumnIndex(3);
            ImGui::Checkbox("##Selected", &items[row].selected);
        }
        ImGui::EndTable();
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

//        canvas->BeginCanvasRendering(&cmdBuffer, region.x, region.y);
//        {
//            coordinateAxisLine->SetViewUniformBuffer(camera.GetViewMatrix(), camera.GetProjectionMatrix());
//            coordinateAxisLine->CmdDrawCoordinateAxisRendering(cmdBuffer, region.x, region.y);
//        }
//        canvas->EndCanvasRendering();
//        canvas->GetFinishedRenderColorAttachment(&preview);

        displayer->BeginDisplayRendering(&cmdBuffer);
        {
            ImGuiEx::BeginNewFrame(cmdBuffer);
            {
                static bool showDemoFlag = true;
                ImGui::ShowDemoWindow(&showDemoFlag);

                ImGui::Begin("Table data");
                {
                    ShowEditableTable();
                }
                ImGui::End();

            }
            ImGuiEx::EndNewFrame(cmdBuffer);
        }
        displayer->EndDisplayRendering();
    }

    Sandbox::Terminate();

    return 0;
}