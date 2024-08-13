/* / ------------------------------------------------------------------------ \ */
/* | Sandbox.cpp                                                              | */
/* | ------------------------------------------------------------------------ | */
/* |                        This file is part of:                             | */
/* |                           TURBINE ENGINE                                 | */
/* | ------------------------------------------------------------------------ | */
/* |                                                                          | */
/* | Copyright (C) 2022 Vcredent All rights reserved.                         | */
/* |                                                                          | */
/* | Licensed under the Apache License, Version 2.0 (the "License");          | */
/* | you may not use this file except in compliance with the License.         | */
/* |                                                                          | */
/* | You may obtain a copy of the License at                                  | */
/* |     http://www.apache.org/licenses/LICENSE-2.0                           | */
/* |                                                                          | */
/* | Unless required by applicable law or agreed to in writing, software      | */
/* | distributed under the License is distributed on an "AS IS" BASIS,        | */
/* | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied  | */
/* | See the License for the specific language governing permissions and      | */
/* | limitations under the License.                                           | */
/* |                                                                          | */
/* \ ------------------------------------------------------------------------ / */
#include <Runtime/Drivers/RenderDevice.h>
#include <Runtime/Window/Window.h>
#include <Runtime/Win32/RenderDeviceCotnextWin32.h>
#include <Runtime/Renderer/ScreenRender.h>
#include <NavUI/NavUI.h>

int main()
{
    Window *window = new Window("TurbineEngine", 1080, 1060);
    RenderDeviceContextWin32 *rdc = new RenderDeviceContextWin32(window);
    rdc->Initialize();
    RenderDevice *rd = rdc->CreateRenderDevice();
    ScreenRender *screen = new ScreenRender(rd);
    screen->initialize(window);

    NavUI::InitializeInfo initialize_info = {};
    initialize_info.window = (GLFWwindow *) screen->get_focused_window()->GetNativeWindow();
    initialize_info.Instance = rdc->GetInstance();
    initialize_info.PhysicalDevice = rdc->GetPhysicalDevice();
    initialize_info.Device = rdc->GetDevice();
    initialize_info.QueueFamily = rdc->GetQueueFamily();
    initialize_info.Queue = rdc->GetQueue();
    initialize_info.DescriptorPool = rd->GetDescriptorPool();
    initialize_info.RenderPass = screen->get_render_pass();
    initialize_info.MinImageCount = screen->get_image_buffer_count();
    initialize_info.ImageCount = screen->get_image_buffer_count();
    initialize_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    NavUI::Initialize(&initialize_info);

    while (!window->IsClose()) {
        window->PollEvents();

        VkCommandBuffer cmdBuffer;
        screen->cmd_begin_screen_render(&cmdBuffer);
        {
            NavUI::BeginNewFrame(cmdBuffer);
            static bool showDemoWindow = true;
            ImGui::ShowDemoWindow(&showDemoWindow);
            NavUI::EndNewFrame(cmdBuffer);
        }
        screen->cmd_end_screen_render(cmdBuffer);

    }

    NavUI::Terminate();
    delete screen;
    rdc->DestroyRenderDevice(rd);
    delete rdc;
    delete window;

    return 0;
}