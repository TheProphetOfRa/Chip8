//
//  main.cpp
//  OpenGL RTT
//
//  Created by David Hodgkinson on 24/11/2014.
//  Copyright (c) 2014 David Hodgkinson. All rights reserved.
//
#include "OpenGLController.h"


#include "Node.h"

int main(int argc, const char * argv[]) {

    OpenGL::OpenGLController* controller = OpenGL::OpenGLController::sharedController();
    
    auto n = OpenGL::Node::Create();
    controller->AddNode(n);
    
    //application loop
    while (controller->Update())
    {
        controller->Draw();
    }
    
    controller->Destroy();
    
    return 0;
}
