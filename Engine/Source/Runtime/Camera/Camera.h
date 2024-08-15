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
#include <Turbine/Typedefs.h>

class Camera
{
public:
    Camera(float v_Fov = 45.0f, float v_Near = 0.1f, float v_Far = 512.0f);
   ~Camera();

    V_FORCEINLINE Matrix4 GetViewMatrix() { return viewMatrix; }
    V_FORCEINLINE Matrix4 GetProjectionMatrix() { return projectionMatrix; }

    V_FORCEINLINE float GetFov() { return fov; }
    V_FORCEINLINE float GetNear() { return near; }
    V_FORCEINLINE float GetFar() { return near; }
    V_FORCEINLINE float GetAspectRatio() { return aspect; }
    V_FORCEINLINE Vector3 GetPosition() { return position; }
    V_FORCEINLINE Vector3 GetDirection() { return direction; }
    V_FORCEINLINE Vector3 GetUp() { return up; }

    V_FORCEINLINE void SetFov(float v_Fov) { fov = v_Fov; }
    V_FORCEINLINE void SetNear(float v_Near) { near = v_Near; }
    V_FORCEINLINE void SetFar(float v_Far) { near = v_Far; }
    V_FORCEINLINE void SetAspectRatio(float v_Aspect) { aspect = v_Aspect; }
    V_FORCEINLINE void SetPosition(Vector3 v_Position) { position = v_Position; }
    V_FORCEINLINE void SetDirection(Vector3 v_Direction) { direction = v_Direction; }
    V_FORCEINLINE void SetUp(Vector3 v_Up) { up = v_Up; }

    void Update()
    {
        viewMatrix = glm::lookAt(position, direction, up);
        projectionMatrix = glm::perspective(fov, aspect, near, far);
    }

private:
    float fov;
    float aspect;
    float near;
    float far;

    Vector3 position = Vector3(0.0f, 0.0f, -6.0f);
    Vector3 direction = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 up = Vector3(0.0f, 1.0f, 0.0f);

    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;
};

#endif /* _RENDER_CAMERA_H_ */
