/* ======================================================================== */
/* Camera.h                                                                 */
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
#ifndef _RUNTIME_CAMERA_H_
#define _RUNTIME_CAMERA_H_

#include <Turbine/Math.h>
#include <Turbine/Typedefs.h>

class Camera
{
public:
    Camera(float vFov = 45.0f, float vNear = 1024.0f, float vFar = 0.1f);
   ~Camera();

    V_FORCEINLINE void SetPosition(float x, float y, float z) { SetPosition(vec3(x, y, z)); }
    V_FORCEINLINE void SetPosition(const vec3 &vPosition) { position = vPosition; }
    V_FORCEINLINE void SetPitch(float vPitch) { pitch = vPitch; }
    V_FORCEINLINE void SetYaw(float vYaw) { yaw = vYaw; }
    V_FORCEINLINE void SetFov(float vFov) { fov = vFov; }
    V_FORCEINLINE void SetAspectRatio(float vAspectRatio) { aspectRatio = vAspectRatio; }
    V_FORCEINLINE void SetNear(float vNear) { near = vNear; }
    V_FORCEINLINE void SetFar(float vFar) { far = vFar; }

    V_FORCEINLINE vec3 GetPosition() { return position; }
    V_FORCEINLINE float GetPitch() { return pitch; }
    V_FORCEINLINE float GetYaw() { return yaw; }
    V_FORCEINLINE float GetFov() { return fov; }
    V_FORCEINLINE float GetAspectRatio() { return aspectRatio; }
    V_FORCEINLINE float GetNear() { return near; }
    V_FORCEINLINE float GetFar() { return far; }

    V_FORCEINLINE const mat4& GetViewMatrix() { return viewMatrix; }
    V_FORCEINLINE const mat4& GetProjectionMatrix() { return projectionMatrix; }

    void Update();

private:
    vec3 position;
    vec3 front;
    vec3 up;

    float pitch;
    float yaw;

    float fov;
    float aspectRatio;
    float near;
    float far;

    mat4 viewMatrix;
    mat4 projectionMatrix;
};

#endif /* _RUNTIME_CAMERA_H_ */
