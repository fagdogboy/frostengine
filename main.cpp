#define FILL_COLOR 0xAA22BB
#define BORDER_COLOR 0x000000


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

  int sgn(int x)
  {
    if (x > 0) return +1;
    if (x < 0) return -1;
    return 0;
  }

  //this is a nightmare
  void draw_bresenham_line(unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y, unsigned long color) {

    XSetForeground(display,gc,color);
    
    int x, y, t, dx, dy, incx, incy, pdx, pdy, ddx, ddy, deltaslowdirection, deltafastdirection, err;
    
    dx = end_x - start_x;
    dy = end_y - start_y;
    
    incx = sgn(dx);
    incy = sgn(dy);

    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    
    if (dx > dy) {
      
        pdx = incx; pdy = 0;
        ddx = incx; ddy = incy;
        deltaslowdirection = dy;
	deltafastdirection = dx;

    } else {

      pdx = 0;    pdy = incy;
      ddx = incx; ddy = incy;
      deltaslowdirection = dx;
      deltafastdirection = dy;   

    }
    
    x = start_x;
    y = start_y;
    err = deltafastdirection / 2;

    XDrawPoint(display,window,gc,x,y);
    
    for(t = 0; t < deltafastdirection; ++t)  {

      err -= deltaslowdirection;

      if(err < 0) {

	err += deltafastdirection;
	x += ddx;
	y += ddy;

      } else {

	x += pdx;
	y += pdy;

      }

      XDrawPoint(display,window,gc,x,y);

    }
  }

  // i mog nimma :(
  void draw_filled_in_triangle (unsigned int x1, unsigned int x2, unsigned int x3, unsigned int y1, unsigned int y2, unsigned int y3 ) {

    printf("Debugging Parameters, draw_filled_tri and:\n");
    printf("x1: %u, y1: %u\n", x1, y1);
    printf("x2: %u, y2: %u\n", x2, y2);
    printf("x3: %u, y3: %u\n", x3, y3);
    
    float fx1 = x1;
    float fx2 = x2;
    float fx3 = x3;
    float fy1 = y1;
    float fy2 = y2;
    float fy3 = y3;
    
    std::cout << "drawing filled tri" << std::endl;
    
    unsigned int s_buf;
    
    if(fy1 > fy2) { s_buf = fy1; fy1 = fy2 ; fy2 = s_buf; }
    if(fy2 > fy3) { s_buf = fy2; fy2 = fy3 ; fy3 = s_buf; }
    
    if (fy2 == fy1)
      {
	std::cout << "bottom triangle! " << std::endl;
	
	fill_bottom_triangle(fx1,fy1,fx2,fy2,fx3,fy3);
      }
    
    if (fy2 == fy3)
      {
	std::cout << "top triangle! " << std::endl;
		
	fill_top_triangle(fx1,fy1,fx2,fy2,fx3,fy3);
      }
    
    float fx4,fy4;

    //    if(x1 == 0.0f || x2 == 0.0f || x3 == 0.0f || y1 == 0.0f || y2 == 0.0f || y3 == 0.0f) 
    
    fx4 = (fx1 + ((float)(fy2 - fy1) / (float)(fy3 - fy1)) * (fx3 - fx1));
    fy4 = fy2;

    fill_bottom_triangle(fx1,fy1,fx4,fy4,fx3,fy3);
    fill_top_triangle(fx1,fy1,fx4,fy4,fx3,fy3);
    
  }

  void draw_horizontal_line(unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2, unsigned long color) {

    XSetForeground(display,gc,color);
    
    XDrawLine(display,window,gc,x1,y1,x2,y2);

  }

  void tmp_draw_filled_tri(unsigned int v1x, unsigned int v2x, unsigned int v3x, unsigned int v1y, unsigned int v2y, unsigned int v3y, unsigned long color) {

    XSetForeground(display,gc,color);
    
    XPoint triangle[3];
    triangle[0].x = v1x; triangle[0].y = v1y;
    triangle[1].x = v2x; triangle[1].y = v2y;
    triangle[2].x = v3x; triangle[2].y = v3y;
    
    XFillPolygon(display,window,gc, triangle, 3, 2, 0);

  }
  
  void fill_top_triangle(unsigned int v1x, unsigned int v1y, unsigned int v2x, unsigned int v2y, unsigned int v3x, unsigned int v3y) {

    float fv1x = (float) v1x;
    float fv1y = (float) v1y;
    float fv2x = (float) v2x;
    float fv2y = (float) v2y;    
    float fv3x = (float) v3x;
    float fv3y = (float) v3y;
    
    printf("Parameters received:\n");
    printf("v1x: %u, v1y: %u\n", v1x, v1y);
    printf("v2x: %u, v2y: %u\n", v2x, v2y);
    printf("v3x: %u, v3y: %u\n", v3x, v3y);
    
    float invslope1,invslope2;
    
    if (fv3y != fv1y) {
      invslope1 = (fv3x - fv1x) / (fv3y - fv1y);
    } else {
      fv1y = fv1y + 0.1f;
      invslope1 = (fv3x - fv1x) / (fv3y - fv1y);
    }
    
    if (fv3y != fv2y) {
      invslope2 = (fv3x - fv2x) / (fv3y - fv2y);
    } else {
      fv1y = fv1y + 0.1f;
      invslope2 = (fv3x - fv2x) / (fv3y - fv2y);
    }
        
    float curx1 = fv3x;
    float curx2 = fv3x;
    
    for (int scanlineY = fv3y; scanlineY > fv1y; scanlineY--)
      {
	
	draw_horizontal_line((unsigned int)curx1, (unsigned int)scanlineY, (unsigned int)curx2, (unsigned int)scanlineY, FILL_COLOR);
	curx1 -= invslope1;
	curx2 -= invslope2;
	
      }
  }
  
  void fill_bottom_triangle(unsigned int v1x,unsigned int v1y,unsigned int v2x,unsigned int v2y,unsigned int v3x,unsigned int v3y) {

    float fv1x = (float) v1x;
    float fv1y = (float) v1y;
    float fv2x = (float) v2x;
    float fv2y = (float) v2y;    
    float fv3x = (float) v3x;
    float fv3y = (float) v3y;

    float invslope1,invslope2;
    if(fv2y != fv1y) {
      invslope1 = (fv2x - fv1x) / (fv2y - fv1y);
    } else {
      fv1y = fv1y + 0.1f;
      invslope1 = (fv2x - fv1x) / (fv2y - fv1y);
    }

    if(fv3y !=fv1y) {
      invslope2 = (fv3x - fv1x) / (fv3y - fv1y);
    } else {
      fv1y = fv1y + 0.1f;
      invslope2 = (fv3x - fv1x) / (fv3y - fv1y);
    }
    
    float curx1 = fv1x;
    float curx2 = fv1x;
    
    for (int scanlineY = fv1y; scanlineY <= fv2y; scanlineY++)
      {	
	draw_horizontal_line((unsigned int)curx1, (unsigned int)scanlineY, (unsigned int)curx2, (unsigned int)scanlineY, FILL_COLOR);
	curx1 += invslope1;
	curx2 += invslope2;
      }
  }
  
  XlibApp(int width, int height) : width(width), height(height) {
    display = XOpenDisplay(nullptr);
    if (display == nullptr) {
      std::cerr << "Cannot open display" << std::endl;
      exit(1);
    }
    
    root = DefaultRootWindow(display);
    
    screen = DefaultScreen(display);

    window = XCreateSimpleWindow(display,
				 root,
				 10,
				 10,
				 width,
				 height,
				 1,				 
				 BlackPixel(display, screen),
				 WhitePixel(display, screen)
				 );
    
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);
    
    gc = XCreateGC(display, window, 0, nullptr);
    
    meshCube.tris = {
      
      { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
      { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },
      
      { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
      { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },
      
      { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
      { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },
            
      { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
      { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },
      
      { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
      { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },
      
      { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
      { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
      
    };
    
  }
  
  ~XlibApp() {
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
  }
  
  void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    
    XSetForeground(display, gc, BlackPixel(display, DefaultScreen(display)));
    
    printf("rendering Coordinates: (%d, %d), (%d, %d), (%d, %d)\n", x1, y1, x2, y2, x3, y3);
    
    draw_bresenham_line(x1, y1, x2, y2, BORDER_COLOR);
    draw_bresenham_line(x2, y2, x3, y3, BORDER_COLOR); 
    draw_bresenham_line(x3, y3, x1, y1, BORDER_COLOR);
    XFlush(display);
  }
  
  void draw_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    
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
  
  void render_screen() {
    
    XSetForeground(display, DefaultGC(display, screen), WhitePixel(display, screen));
    XFillRectangle(display, window, DefaultGC(display, screen), 0, 0, height, width);
    
    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = ( -fFar * fNear ) / ( fFar - fNear );
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;    
    
    matRotZ.m[0][0] = cosf(fThetaY);
    matRotZ.m[0][1] = sinf(fThetaY);
    matRotZ.m[1][0] = -sinf(fThetaY);
    matRotZ.m[1][1] = cosf(fThetaY);
    matRotZ.m[2][2] = 1;
    matRotZ.m[3][3] = 1;
    
    matRotX.m[0][0] = 1;
    matRotX.m[1][1] = cosf(fThetaX * 0.5f);
    matRotX.m[1][2] = sinf(fThetaX * 0.5f);
    matRotX.m[2][1] = -sinf(fThetaX * 0.5f);
    matRotX.m[2][2] = cosf(fThetaX * 0.5f);
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

      vec3d normal, vec1, vec2;

      vec1.x = triTranslated.p[1].x - triTranslated.p[0].x;
      vec1.y = triTranslated.p[1].y - triTranslated.p[0].y;
      vec1.z = triTranslated.p[1].z - triTranslated.p[0].z;
      
      vec2.x = triTranslated.p[2].x - triTranslated.p[0].x;
      vec2.y = triTranslated.p[2].y - triTranslated.p[0].y;
      vec2.z = triTranslated.p[2].z - triTranslated.p[0].z;

      normal.x = vec1.y * vec2.z - vec1.z * vec2.y;
      normal.y = vec1.z * vec2.x - vec1.x * vec2.z;
      normal.z = vec1.x * vec2.y - vec1.y * vec2.x;

      // yeah its normal to normally normalize our normal vector :nerd:
      float l = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
      normal.x /= l; normal.y /= l; normal.z /= l;

      if(normal.x * (triTranslated.p[0].x - camera.x) + 
	 normal.y * (triTranslated.p[0].y - camera.y) +
	 normal.z * (triTranslated.p[0].z - camera.z) < 0.0f)
	{
	  
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
	  
	  tmp_draw_filled_tri((unsigned int)triProjected.p[0].x,
			      (unsigned int)triProjected.p[1].x,
			      (unsigned int)triProjected.p[2].x,
			      (unsigned int)triProjected.p[0].y,
			      (unsigned int)triProjected.p[1].y,
			      (unsigned int)triProjected.p[2].y,
			      0xAA11AA);
	  
	  draw_triangle(triProjected.p[0].x, triProjected.p[0].y,
			triProjected.p[1].x, triProjected.p[1].y,
			triProjected.p[2].x, triProjected.p[2].y);
	}      
    }
    
    XFlush(display);
    
  }    
  
  void run() {
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
	  
	  fThetaX -= 0.1;
	  
	  std::cout << "X incremented: " << fThetaX << std::endl;
	  
	} else if (key == XK_a) {
	  
	  fThetaY -= 0.1;
	  
	  std::cout << "Y incremented: " << fThetaY << std::endl;
	  
	} else if (key == XK_s) {
	  
	  fThetaX += 0.1;
	  
	  std::cout << "X decremented: " << fThetaX << std::endl;
	  
	} else if (key == XK_d) {
	  
	  fThetaY += 0.1;
	  
	  std::cout << "Y decremented: " << fThetaY << std::endl;
	  
	} else if (key == XK_f) {
	  
	 
	  
	} else if (key == XK_x) {
	  break; 
	}
	
	render_screen();
	
      }
    }
  }
  
  
private:
  Display* display;
  Window window;
  GC gc;
  Window root;
  int screen;
  
  unsigned int width;
  unsigned int height;
  
  float fThetaX = 0.0f;
  float fThetaY = 0.0f;

  float fPosition_X;

  float fPosition_Y;

  float fPosition_Z;
  
  mesh meshCube;
    
  float fNear = 0.1f;
  
  float fFar = 1000.0f;
  
  float fFov = 90.0f;

  vec3d camera;
  
  mat4x4 matProj;
  mat4x4 matRotZ;
  mat4x4 matRotX;
  
  float fAspectRatio = (float)height / (float)width;
  
  float fFovRad = 1.0f / tanf( fFov * 0.5f / 180.0f * 3.14159f );
  
};

int main() {
    XlibApp app(800, 600);
    app.run();
    return 0;
}
