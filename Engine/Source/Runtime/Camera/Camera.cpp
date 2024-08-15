/* ======================================================================== */
/* Camera.cpp                                                               */
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
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, e1ither express or implied */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
/*                                                                          */
/* ======================================================================== */
#include "Camera.h"

Camera::Camera(float vFov, float vNear, float vFar)
    : fov(vFov), aspectRatio(1.0f), near(vNear), far(vFar)
{
    position = vec3(0.0f, 0.0f, -6.0f);
    front = vec3(0.0f, 0.0f, 0.0f);
    up = vec3(0.0f, 1.0f, 0.0f);
}

Camera::~Camera()
{
    /* do nothing... */
}

void Camera::Update()
{
    front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front.y = -glm::sin(glm::radians(pitch));
    front.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
    front = glm::normalize(front);

    viewMatrix = glm::lookAt(position, position + front, up);
    projectionMatrix = glm::perspective(fov, aspectRatio, near, far);
    projectionMatrix[1][1] *= -1;
}