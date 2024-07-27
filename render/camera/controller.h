/* ======================================================================== */
/* controller.h                                                             */
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
#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_

#include "camera.h"
#include "event.h"

class CameraController {
public:
    CameraController(Camera *v_camera = NULL);
    ~CameraController();

    virtual void on_event_mouse_button(int button, int action, int mods);
    virtual void on_event_cursor(float x, float y);
    virtual void on_update_camera();

    V_FORCEINLINE
    inline void make_current_camera(Camera *v_camera) { camera = v_camera; }

protected:
    struct MouseEvent {
        int button;
        int action;
        int mods;
    };

    struct CursorEvent {
        float x;
        float y;
    };

    /* events */
    MouseEvent *mouse = NULL;
    CursorEvent *cursor = NULL;

    Camera *camera = NULL;
};

#endif /* _CAMERA_CONTROLLER_H_ */
