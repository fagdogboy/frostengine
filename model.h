#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <vector>
#include "math.h"
#include <sstream>
#include <fstream>
#include <strstream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <list>

#include "utility.h"

class model {
  
 public:

  //model data
  mesh model_data;
  
  float fThetaX = 0.0f;
  float fThetaY = 0.0f;
  float fThetaZ = 0.0f;

  float fPosition_X = 0.0f;
  float fPosition_Y = 0.0f;
  float fPosition_Z = 0.0f;

  //model functions
  mesh import_obj_mesh(std::string file_path) {
    
    mesh output;
    
    std::ifstream file(file_path);
    if(!file.is_open())
      std::cout << "imort mesh not found!!!" << std::endl;
    
    std::vector<vec3d> vertices;
    
    while(!file.eof()) {

      char line[1024];
      file.getline(line,1024);

      std::strstream s;

      s << line;

      char junk;

      if(line[0] == 'v') {

	vec3d vec;

	s >> junk >> vec.x >> vec.y >> vec.z;

        vertices.push_back(vec);

      }
      
      if(line[0] == 'f') {

	int f[3];

	s >> junk >> f[0] >> f[1] >> f[2];
	
	output.tris.push_back({ vertices[f[0] - 1], vertices[f[1] - 1], vertices[f[2] - 1] , /*texture*/ 0.0f, 1.0f, 0x000000 });

      }
      
    }
    
    return output;

  }

  model( mesh mesh_data_in,
              float thetaX = 0.0f, 
              float thetaY = 0.0f, 
              float thetaZ = 0.0f,
              float positionX = 0.0f, 
              float positionY = 0.0f, 
              float positionZ = 0.0f)
        : model_data(mesh_data_in),
          fThetaX(thetaX), 
          fThetaY(thetaY), 
          fThetaZ(thetaZ),
          fPosition_X(positionX), 
          fPosition_Y(positionY), 
          fPosition_Z(positionZ) {
    }  
};
