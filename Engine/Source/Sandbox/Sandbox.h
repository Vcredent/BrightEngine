/* ------------------------------------------------------------------------ */
/* Sandbox.h                                                                */
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
#include <Runtime/Camera/Camera.h>
#include <ImGuiNav/ImGuiNav.h>
#include <Runtime/Renderer/Canvas.h>
#include <Runtime/Renderer/CoordinateAxisRender.h>

Window *window;
RenderDeviceContextWin32 *rdc;
RenderDevice *rd;
Displayer *displayer;
Canvas *canvas;
Camera *camera;
CoordinateAxisRender *coordinateAxis;

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
        camera = new Camera();
        coordinateAxis = new CoordinateAxisRender(rd, canvas->GetRenderPass());
    }

    void Terminate()
    {
        delete coordinateAxis;
        delete camera;
        delete canvas;
        ImGuiNav::Terminate();
        delete displayer;
        rdc->DestroyRenderDevice(rd);
        delete rdc;
        delete window;
    }

    void UpdateCamera()
    {
        // update camera
        static bool isDragging = false;
        static bool isFirst = true;
        static float lastX = 0.0f, lastY = 0.0f;
        static int presslist[1024] = { 0 };

        if (window->GetKey(GLFW_KEY_F))
        {
            isDragging = true;
            isFirst = true;
            window->HideCursor();
        }

        if (window->GetKey(GLFW_KEY_ESCAPE) && isDragging)
        {
            isDragging = false;
            window->ShowCursor();
        }

        if (isDragging)
        {
            float xpos, ypos;
            window->GetCursorPosition(&xpos, &ypos);

            // rotation
            if (isFirst)
            {
                lastX = xpos;
                lastY = ypos;
                isFirst = false;
            }

            float sensitivity = camera->GetSensitivity();
            float xoffset = (xpos - lastX) * sensitivity;
            float yoffset = (lastY - ypos) * sensitivity;

            lastX = xpos;
            lastY = ypos;

            float pitch = camera->GetPitch();
            float yaw = camera->GetYaw();

            pitch += yoffset;
            yaw += xoffset;
            pitch = glm::clamp(-89.0f, pitch, 89.0f);

            camera->SetPitch(pitch);
            camera->SetYaw(yaw);

            // move
            presslist[GLFW_KEY_W] = window->GetKey(GLFW_KEY_W);
            presslist[GLFW_KEY_S] = window->GetKey(GLFW_KEY_S);
            presslist[GLFW_KEY_A] = window->GetKey(GLFW_KEY_A);
            presslist[GLFW_KEY_D] = window->GetKey(GLFW_KEY_D);

            vec3 position = camera->GetPosition();
            vec3 front = camera->GetFront();
            float speed = camera->GetSpeed();

            static float lastTime = 0.0f;
            float currentTime = (float)  glfwGetTime();
            float deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            float velocity = speed * deltaTime;

            if (presslist[GLFW_KEY_W])
                position += velocity * front;
            if (presslist[GLFW_KEY_S])
                position -= velocity * front;
            if (presslist[GLFW_KEY_A])
                position -= glm::normalize(glm::cross(front, camera->GetUp())) * velocity;
            if (presslist[GLFW_KEY_D])
                position += glm::normalize(glm::cross(front, camera->GetUp())) * velocity;

            camera->SetPosition(position);
        }

        camera->Update();
    }

}