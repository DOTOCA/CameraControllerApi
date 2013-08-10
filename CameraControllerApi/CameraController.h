//
//  CameraController.h
//  CameraControllerApi
//
//  Created by Tobias Scheck on 10.08.13.
//  Copyright (c) 2013 scheck-media. All rights reserved.
//

#ifndef __CameraControllerApi__CameraController__
#define __CameraControllerApi__CameraController__

#include <iostream>
#include <string>
#include <exception>
#include <gphoto2/gphoto2-camera.h>

namespace CameraControllerApi {
    class CameraController {     
    public:
        bool camera_found();
        
        CameraController();
        ~CameraController();
        int capture(const char *filename);
        
    private:
        Camera *_camera;
        GPContext *_ctx;
        bool _camera_found;
        
        static GPContextErrorFunc _error_callback(GPContext *context, const char *text, void *data);
        static GPContextMessageFunc _message_callback(GPContext *context, const char *text, void *data);
    };
}

#endif /* defined(__CameraControllerApi__CameraController__) */