/* ------------------------------------------------------------------------ */
/* ImGuiNav.cpp                                                             */
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
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, e1ither express or implied */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
/*                                                                          */
/* ------------------------------------------------------------------------ */
#include "ImGuiNav.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_vulkan.h"
#include <Turbine/Typedefs.h>

static GLFWwindow *_window = NULL;

void _DarkNavUITheme()
  {
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border]                 = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.00f, 0.46f, 1.00f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.25f, 0.25f, 0.25f, 0.52f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator]              = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding                     = ImVec2(8.00f, 8.00f);
    style.FramePadding                      = ImVec2(5.00f, 2.00f);
    style.CellPadding                       = ImVec2(6.00f, 6.00f);
    style.ItemSpacing                       = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing                  = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding                 = ImVec2(0.00f, 0.00f);
    style.IndentSpacing                     = 25;
    style.ScrollbarSize                     = 15;
    style.GrabMinSize                       = 16;
    style.WindowBorderSize                  = 1;
    style.ChildBorderSize                   = 1;
    style.PopupBorderSize                   = 1;
    style.FrameBorderSize                   = 1;
    style.TabBorderSize                     = 1;
    style.WindowRounding                    = 7;
    style.ChildRounding                     = 4;
    style.FrameRounding                     = 3;
    style.PopupRounding                     = 12;
    style.ScrollbarRounding                 = 9;
    style.GrabRounding                      = 3;
    style.LogSliderDeadzone                 = 4;
    style.TabRounding                       = 4;
  }

void _CheckDraggingCursor()
  {
    static bool     is_dragging = false;
    static ImGuiID  current_drag_item = 0x7FFFFFFF;

    ImGuiID item = ImGui::GetItemID();
    bool check_active = ImGui::IsItemActive() && ImGui::IsMouseDragging(0);

    if (item == current_drag_item && !check_active) {
        is_dragging = false;
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        current_drag_item = 0x7FFFFFFF;
        return;
    }

    if (!is_dragging && check_active) {
        is_dragging = true;
        current_drag_item = item;
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        return;
    }
  }

namespace ImGuiNav {

    void Initialize(ScreenRender *screen)
      {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        io.ConfigViewportsNoAutoMerge = true;
        io.ConfigViewportsNoTaskBarIcon = true;
        io.IniFilename = _RESOURCE("/naveditor.ini");

        // set default font.
        io.Fonts->AddFontFromFileTTF(_RESOURCE("/Fonts/Microsoft Yahei UI/Microsoft Yahei UI.ttf"), 18.0f,
                                     nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        io.FontDefault = io.Fonts->Fonts.back();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // default theme.
        _DarkNavUITheme();

        // Setup Platform/Renderer backends
          _window = (GLFWwindow *) screen->GetNativeWindow();
          ImGui_ImplGlfw_InitForVulkan(_window, true);

        RenderDevice *rd = screen->GetRenderDevice();
        RenderDeviceContext *rdc = rd->GetDeviceContext();

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = rdc->GetInstance();
        init_info.PhysicalDevice = rdc->GetPhysicalDevice();
        init_info.Device = rdc->GetDevice();
        init_info.QueueFamily = rdc->GetQueueFamily();
        init_info.Queue = rdc->GetQueue();
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = rd->GetDescriptorPool();
        init_info.RenderPass = screen->GetRenderPass();
        init_info.Subpass = 0;
        init_info.MinImageCount = screen->GetImageBufferCount();
        init_info.ImageCount = screen->GetImageBufferCount();
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        ImGui_ImplVulkan_Init(&init_info);
      }

    void Terminate()
      {
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplVulkan_Shutdown();
      }

    void BeginNewFrame(VkCommandBuffer U_MAYBE_UNUSED cmdBuffer)
      {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // docking
        ImGui::DockSpaceOverViewport();
      }

    void EndNewFrame(VkCommandBuffer cmdBuffer)
      {
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // Rendering
        ImGui::Render();
        ImDrawData* main_draw_data = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(main_draw_data, cmdBuffer);

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
      }

    bool Begin(const char *title, bool* p_open, ImGuiWindowFlags flags)
      {
        bool retval;
        ImGuiStyle &style = ImGui::GetStyle();
        ImVec2 padding = style.FramePadding;
        padding.y = padding.y + 5;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, padding);
        retval = ImGui::Begin(title, p_open, flags);
        ImGui::PopStyleVar();
        return retval;
      }

    void End()
      {
        ImGui::End();
      }

    void BeginViewport(const char *title)
      {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        Begin(title);
      }

    void EndViewport()
      {
        End();
        ImGui::PopStyleVar();
      }

    void DrawTexture(ImTextureID v_texture, const ImVec2& v_size)
      {
        ImGui::Image(v_texture, v_size);
      }

    ImTextureID AddTexture(VkSampler v_sampler, VkImageView v_image, VkImageLayout v_layout)
      {
        return ImGui_ImplVulkan_AddTexture(v_sampler, v_image, v_layout);
      }

    void RemoveTexture(ImTextureID v_texture)
      {
        ImGui_ImplVulkan_RemoveTexture((VkDescriptorSet) v_texture);
      }

}