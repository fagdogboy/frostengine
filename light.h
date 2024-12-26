#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <vector>
#include "math.h"
#include <fstream>
#include <strstream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <list>

#include "utility.h"

class light {
public:

  unsigned int strength = 0.0f;
  float fThetaX = 0.0f;
  float fThetaY = 0.0f;
  float fThetaZ = 0.0f;
  
  float fPosition_X = 0.0f;
  float fPosition_Y = 0.0f;
  float fPosition_Z = 0.0f;
  
  light( unsigned int strength_in = 0,
              float thetaX = 0.0f, 
              float thetaY = 0.0f, 
              float thetaZ = 0.0f,
              float positionX = 0.0f, 
              float positionY = 0.0f, 
              float positionZ = 0.0f)
        : strength(strength_in),
          fThetaX(thetaX), 
          fThetaY(thetaY), 
          fThetaZ(thetaZ),
          fPosition_X(positionX), 
          fPosition_Y(positionY), 
          fPosition_Z(positionZ) {
    }  

  vec3d get_light_source() {

    vec3d to_output = {fPosition_X, fPosition_Y, fPosition_Z};
    
    return to_output;

  }
  
};
