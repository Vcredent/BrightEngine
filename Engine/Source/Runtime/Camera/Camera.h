/* ======================================================================== */
/* Camera.h                                                                 */
/* ======================================================================== */
/*                        This file is part of:                             */
/*                           COPILOT ENGINE                                 */
/* ======================================================================== */
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
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, e1ither express or implied */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
/*                                                                          */
/* ======================================================================== */
#ifndef _RENDER_CAMERA_H_
#define _RENDER_CAMERA_H_

#include <Turbine/Math.h>

class Camera
{
public:
    Camera(float v_Fov = 45.0f, float v_Near = 0.1f, float v_Far = 512.0f);
   ~Camera();

    Matrix4 GetViewMatrix() { return viewMatrix; }
    Matrix4 GetProjectionMatrix() { return projectionMatrix; }

    void SetFov(float v_Fov) { fov = v_Fov; }
    void SetNear(float v_Near) { near = v_Near; }
    void SetFar(float v_Far) { near = v_Far; }
    void SetAspectRatio(float v_Aspect) { aspect = v_Aspect; }

    void Update()
    {
        viewMatrix = glm::lookAt(position, position + direction, up);
        projectionMatrix = glm::perspective(fov, aspect, near, far);
    }

private:
    float fov;
    float aspect;
    float near;
    float far;

    Vector3 position;
    Vector3 direction;
    Vector3 up;

    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;
};

#endif /* _RENDER_CAMERA_H_ */
