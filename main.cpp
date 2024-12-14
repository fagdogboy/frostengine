#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <vector>
#include "math.h"

struct vec3d {

  float x,y,z;
  
};

struct triangle {
  
  vec3d p[3];
  
};

struct mesh {
  
  std::vector<triangle> tris;
  
};

struct mat4x4 {
  
  float m[4][4] = { 0 };
  
};

void multiply_matrix_vector(vec3d &i, vec3d &o, mat4x4 &m) {

  o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
  o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
  o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];

  float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];
  
  if (w != 0.0f) {

    o.x /= w; o.y /= w; o.z /= w;

  }

  return;
}

class XlibApp {
public:
  XlibApp(int width, int height);
  ~XlibApp();
  void draw_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
  void run();
  void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
  void render_screen();
  
private:
  Display* display;
  Window window;
  GC gc;
  Window root;
  int screen;
  unsigned int width;
  unsigned int height;
  float fTheta;


  mesh meshCube;
    
  float fNear = 0.1f;
  
  float fFar = 1000.0f;
  
  float fFov = 90.0f;

  mat4x4 matProj;
  mat4x4 matRotZ;
  mat4x4 matRotX;

  float fAspectRatio = (float)height / (float)width;
  
  float fFovRad = 1.0f / tanf( fFov * 0.5f / 180.0f * 3.14159f );
  
};

XlibApp::XlibApp(int width, int height) : width(width), height(height) {
    display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cerr << "Cannot open display" << std::endl;
        exit(1);
    }

    root = DefaultRootWindow(display);
    
    screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, root, 10, 10, width, height, 1,
                                  BlackPixel(display, screen), WhitePixel(display, screen));

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    gc = XCreateGC(display, window, 0, nullptr);

    meshCube.tris = {
      
      // SOUTH
      { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
      { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },
      
      // EAST                                                      
      { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
      { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },
      
      // NORTH                                                     
      { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
      { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },
      
      // WEST                                                      
      { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
      { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },
      
      // TOP                                                       
      { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
      { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },
      
      // BOTTOM                                                    
      { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
      { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
      
    };

}

XlibApp::~XlibApp() {
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
}

void XlibApp::draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {

    XSetForeground(display, gc, BlackPixel(display, DefaultScreen(display)));

    printf("printing Coordinates: (%d, %d), (%d, %d), (%d, %d)\n", x1, y1, x2, y2, x3, y3);

    XDrawLine(display, window, gc, x1, y1, x2, y2);
    XDrawLine(display, window, gc, x2, y2, x3, y3); 
    XDrawLine(display, window, gc, x3, y3, x1, y1);


    XFlush(display);
}

void XlibApp::draw_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {

    Colormap colormap = DefaultColormap(display, screen);
    XColor color;
    color.red = (r << 8);
    color.green = (g << 8);
    color.blue = (b << 8);
    XAllocColor(display, colormap, &color);

    XSetForeground(display, gc, color.pixel);
    XDrawPoint(display, window, gc, x, y);
    XFlush(display);

    }

void XlibApp::render_screen() {

  XSetForeground(display, DefaultGC(display, screen), WhitePixel(display, screen));
  XFillRectangle(display, window, DefaultGC(display, screen), 0, 0, 800, 600);
  
  matProj.m[0][0] = fAspectRatio * fFovRad;
  matProj.m[1][1] = fFovRad;
  matProj.m[2][2] = fFar / (fFar - fNear);
  matProj.m[3][2] = ( -fFar * fNear ) / ( fFar - fNear );
  matProj.m[2][3] = 1.0f;
  matProj.m[3][3] = 0.0f;    
  
  matRotZ.m[0][0] = cosf(fTheta);
  matRotZ.m[0][1] = sinf(fTheta);
  matRotZ.m[1][0] = -sinf(fTheta);
  matRotZ.m[1][1] = cosf(fTheta);
  matRotZ.m[2][2] = 1;
  matRotZ.m[3][3] = 1;
  
  matRotX.m[0][0] = 1;
  matRotX.m[1][1] = cosf(fTheta * 0.5f);
  matRotX.m[1][2] = sinf(fTheta * 0.5f);
  matRotX.m[2][1] = -sinf(fTheta * 0.5f);
  matRotX.m[2][2] = cosf(fTheta * 0.5f);
  matRotX.m[3][3] = 1;
  
  
  for(auto tri : meshCube.tris) {
    
    triangle triProjected;
    triangle triTranslated;
    triangle triRotatedZ;
    triangle triRotatedX;
    triangle triRotatedZX;
    
    multiply_matrix_vector(tri.p[0],triRotatedZ.p[0], matRotZ);
    multiply_matrix_vector(tri.p[1],triRotatedZ.p[1], matRotZ);
    multiply_matrix_vector(tri.p[2],triRotatedZ.p[2], matRotZ);
    
    multiply_matrix_vector(triRotatedZ.p[0],triRotatedZX.p[0], matRotX);
    multiply_matrix_vector(triRotatedZ.p[1],triRotatedZX.p[1], matRotX);
    multiply_matrix_vector(triRotatedZ.p[2],triRotatedZX.p[2], matRotX);
    
    triTranslated = triRotatedZX;
    triTranslated.p[0].z =  triRotatedZX.p[0].z + 2.0f;
    triTranslated.p[1].z =  triRotatedZX.p[1].z + 2.0f;
    triTranslated.p[2].z =  triRotatedZX.p[2].z + 2.0f;
    
    multiply_matrix_vector(triTranslated.p[0],triProjected.p[0], matProj);
    multiply_matrix_vector(triTranslated.p[1],triProjected.p[1], matProj);
    multiply_matrix_vector(triTranslated.p[2],triProjected.p[2], matProj);
    
    triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
    triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
    triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
    
    triProjected.p[0].x *= 0.5f * (float)width;
    triProjected.p[0].y *= 0.5f * (float)height;
    
    triProjected.p[1].x *= 0.5f * (float)width;
    triProjected.p[1].y *= 0.5f * (float)height;
    
    triProjected.p[2].x *= 0.5f * (float)width;
    triProjected.p[2].y *= 0.5f * (float)height;
    
    draw_triangle(triProjected.p[0].x, triProjected.p[0].y,
		  triProjected.p[1].x, triProjected.p[1].y,
		  triProjected.p[2].x, triProjected.p[2].y);
    
  }

  XFlush(display);
   
}    

void XlibApp::run() {
    XEvent event;
    while (true) {
        XNextEvent(display, &event);
        if (event.type == Expose) {

	  int x, y; 
	  unsigned int border_width, depth;
	  
	  if (XGetGeometry(display, window, &root, &x, &y, &width, &height, &border_width, &depth)) {
	    std::cout << "Window size: " << width << "x" << height << std::endl;
	  } else {
	    std::cerr << "Failed to get window geometry" << std::endl;
	  }

	  render_screen();
	  
	}
        if (event.type == KeyPress) {
	  KeySym key = XLookupKeysym(&event.xkey, 0);
	  if (key == XK_w) {

	    fTheta += 0.1;
	    
	    std::cout << "fTheta incremented: " << fTheta << std::endl;

	  } else if (key == XK_s) {

	    fTheta -= 0.1; 

	    std::cout << "fTheta decremented: " << fTheta << std::endl;

	  } else if (key == XK_Escape) {
	    break; 
	  }

	  render_screen();
	  
        }
    }
}

int main() {
    XlibApp app(800, 600);
    app.run();
    return 0;
}
