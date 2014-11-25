//
//  main.cpp
//  OpenGL RTT
//
//  Created by David Hodgkinson on 24/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//
#include "OpenGLController.h"
#include "Triangle.h"

int main(int argc, const char * argv[]) {

    OpenGL::OpenGLController* controller = OpenGL::OpenGLController::sharedController();
    
    auto t = OpenGL::Triangle::Create();
    controller->addTriangle(t);
    
    //application loop
    while (controller->update())
    {
        controller->draw();
    }
    
    controller->Destroy();
    
    return 0;
}
