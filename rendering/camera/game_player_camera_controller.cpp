/* ======================================================================== */
/* game_player_camera_controller.cpp                                        */
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
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied  */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
/*                                                                          */
/* ======================================================================== */
#include "game_player_camera_controller.h"
#include <GLFW/glfw3.h>

void GamePlayerCameraController::on_event_mouse_button(int button, int action, int mods)
{

}

void GamePlayerCameraController::on_event_cursor(float x, float y)
{
    static float last_x = 0.0f, last_y = 0.0f;

    if (!continual_dragging_flag) {
        last_x = x;
        last_y = y;
        continual_dragging_flag = true;
    }

    float xoffset = x - last_x;
    float yoffset = last_y - y;
    last_x = x;
    last_y = y;

    float sensitivity = camera->get_sensitivity();
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    float yaw = camera->get_yaw();
    float pitch = camera->get_pitch();

    yaw += xoffset;
    pitch += yoffset;

    pitch = glm::clamp(-89.0f, pitch, 89.0f);

    camera->set_yaw(yaw);
    camera->set_pitch(pitch);
}

void GamePlayerCameraController::on_event_key(int key, int action)
{
    Vec3 position = camera->get_position();
    Vec3 front = camera->get_front();
    float speed = camera->get_speed();

    static double last_time = 0.0f;
    double current_time = glfwGetTime();
    float delta_time = current_time - last_time;

    if (action) {
        switch (key) {
            case EVENT_INPUT_KEY_W: position += speed * front * delta_time; break;
            case EVENT_INPUT_KEY_S: position -= speed * front * delta_time; break;
            case EVENT_INPUT_KEY_A: position -= glm::normalize(glm::cross(front, camera->get_camera_up())) * speed * delta_time; break;
            case EVENT_INPUT_KEY_D: position += glm::normalize(glm::cross(front, camera->get_camera_up())) * speed * delta_time; break;
        }
    }

    camera->set_position(position);
}

void GamePlayerCameraController::on_update_camera()
{
    camera->update();
}