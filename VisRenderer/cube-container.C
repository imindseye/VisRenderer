
////////////////////////////////////////////////////////////////////////////////////////////////
// 
// The purpose of this program is to use vertex buffer objects (VBOs) to define a simple 3D cube
// Note that we will not use glBegin/End any more since they have been deprecated in OpenGL 3.x 
// 
// A very simple shader (cube_simple.{vert,frag}) is uesd in this example
//
// In this example, the triangles vertices and colors are listed in separate arrays and VBOs. We also 
// use a triangle index array (GL_ELEMENT_ARRAY_BUFFER) to assemble triangles 
// vertices. As a result, we use glDrawArrays() to draw. 
//
// Simple 3D viewing is set up, using the OpenGL mathematics (glm) library. This is to replace the 
// OpenGL 2.x methods (GL_MODELVIEW, GL_PROJECTIon, etc) that have been deprecated. 
//
// Han-Wei Shen, July 2012 @ Ohio State University 
// hwshen@cse.ohio-state.edu 
// 
////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __APPLE__
#include <GL/glew.h> 
#include <GLUT/glut.h> 
#include <OpenGL/gl.h> 
#else 
#include <GL/glew.h> 
#include <GL/glut.h> 
#include <GL/gl.h> 
#endif

#include<glm/glm.hpp>
#include<glm/gtx/transform.hpp>

//#include<glm/GLM_GTX_inverse.h>
//#include<glm/GLM_GTX_matrix_projection.h>
//#include<glm/GLM_GTX_transform.h> 
//#include<glm/GLM_GTX_transform2.h> 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> 

#include "containers.h"
#include "renderer.h"

#define DtoR(d) d/360.0 * 3.1415926 *2

Vertex bbx_verts[8];       // cube vertices 
GLuint bbx_indices[6*6];   // cube face indices: 6 faces, 6 indices each face 

int num_tiles; 

glm::mat4 inv_modelview; 

GLuint programObject;
GLuint SetupGLSL(char*); 

GLuint vao; 

bool fill_mode = true; 
bool toggle_grid = false; 
bool toggle_rays = false; 

int width, height; 
int tile_width, tile_height;  

/////////////////////////////////
// glut mouse control 
// 
int xform_mode = 0; 
#define XFORM_NONE    0 
#define XFORM_ROTATE  1
#define XFORM_SCALE 2 

int press_x, press_y; 
int release_x, release_y; 
float x_angle = 0.0; 
float y_angle = 0.0; 
float scale_size = 1; 

float camera_angle = 90.0; 

 PointsContainer Tiles; 
 LinesContainer Rays; 
 TrianglesContainer Tris; 
 QuadsContainer Quads; 
 MeshContainer BBX; 

Renderer renderer; 

///////////////////////////////////////////////////////////////////////////
//
//   solve t for the equation  target = x0 + t (x1-x0)
//
float intersection(float x0, float x1, float target) {
  return ((target-x0)/(x1-x0)); 
}
////////////////////////////////////////////////////////////////////////////
//
//  calculate the intersectino of 1D ray: P = P1 + t (P2-P1) 
//  with two 1D  faces ( F=0 && F= 1) of parallel box  
//
void ray_faces_intersection(float p1, float p2, float &tmin, float &tmax)
{
      float ta = 10000000; 
      float tb = -10000000; 
      float t1 = intersection(p1, p2, 0.0);   
      if (t1 < ta) ta = t1; 
      if (t1 > tb) tb  = t1; 
      t1 = intersection(p1, p2, 1.0);  
      if (t1 < ta) ta = t1; 
      if (t1 > tb) tb = t1; 
      if (ta< tb) 
        { 
          float tmp = ta; 
          ta = tb; tb= tmp; 
        } 
      tmin = tb; tmax = ta;  
}
////////////////////////////////////////////////////////////////////////////
//
//  Ray-Box intesection
//  ray: p = p1 + t (p2-p1)
//  box: axis aligned unit box 
//
bool ray_box_intersection(glm::vec4 p1, glm::vec4 p2, glm::vec4 &pin, glm::vec4 &pout)
{
 bool miss =  false; 
    glm::vec4 pa; 
    glm::vec4 pb; 

    float Dx = p2[0]-p1[0]; 
    float Dy = p2[1]-p1[1]; 
    float Dz = p2[2]-p1[2]; 
    if ((Dx ==0) && (p1[0] < 0 || p1[0]>1)) miss = true; 
    if ((Dy ==0) && (p1[1] < 0 || p1[1]>1)) miss = true; 
    if ((Dz ==0) && (p1[2] < 0 || p1[2]>1)) miss = true; 

    if (!miss) {
      float ta, tb; 
      float tax, tbx, tay, tby, taz, tbz; 
      float t1; 

      ray_faces_intersection(p1[0], p2[0], tbx, tax); 
      ray_faces_intersection(p1[1], p2[1], tby, tay); 
    
      if (tax < tay) ta = tax; 
       else ta = tay; 
      if (tbx > tby) tb = tbx; 
      else tb = tby; 

      ray_faces_intersection(p1[2], p2[2], tbz, taz); 

      if (taz < ta) ta = taz;      
      if (tbz > tb) tb = tbz;  

      if (ta<tb) miss = true;  
      else {
        miss = false; 
        pin = p1+tb*(p2-p1); 
        pout = p1+ta*(p2-p1); 
      }
    } 
    return(miss); 
}


///////////////////////////////////////////////////////////////////////////////////
//
//  Define the centers of the screen space tiles, in eye space  
//
void InitTilesGeometry()
{
int udim = (int)(width/(float) tile_width); 
int vdim = (int)(height/(float) tile_height); 
float z_offset = -1; // on the near plane  

float du = 2.0* tanf(DtoR(camera_angle/2.0))/width; 
float dv = 2.0* tanf(DtoR(camera_angle/2.0))/height; 
Tiles.clear(); 
for (int i = 0; i < udim; i++)
  for (int j = 0; j< vdim; j++) {

    float x =  (i+0.5)*tile_width*du - tanf(DtoR(camera_angle/2.0));
    float y =  (j+0.5)*tile_height*dv -tanf(DtoR(camera_angle/2.0));  
    int idx = i*udim + j;  

    glm::vec4 p1 = inv_modelview * glm::vec4(x,y,z_offset,1.0); 

    Vertex tile; 
    tile.location[0] = p1[0]; tile.location[1] = p1[1]; 
    tile.location[2] = p1[2]; tile.location[3] = p1[3]; 
    tile.color[0] = 1.0; tile.color[1] = 1.0;   
    tile.color[2] = 0.0; tile.color[3] = 1.0;
    Tiles.addPoint(tile); 

  }
}
////////////////////////////////////////////////////////////////
//
//       Given an image of width and height pixels, where the image is placed 
//       at one unit away from the perspective camear (OpenGL default), with the full view angle of "camera_angle", 
//       calculate the view rays from the camera through the centers of image tiles, and the 
//       intersections with a unit box in front of the camera  
//
void InitRaysGeometry()
{
int udim = (int)(width/(float) tile_width); 
int vdim = (int)(height/(float) tile_height); 
float z_offset = -1; // on the near plane 

float du = 2.0* tanf(DtoR(camera_angle/2.0))/width; 
float dv = 2.0* tanf(DtoR(camera_angle/2.0))/height; 
Rays.clear(); 
for (int i = 0; i < udim; i++)
  for (int j = 0; j< vdim; j++) {

    float x =  (i+0.5)*tile_width*du - tanf(DtoR(camera_angle/2.0));
    float y =  (j+0.5)*tile_height*dv -tanf(DtoR(camera_angle/2.0)); 
    int idx = (i*udim + j)*2;  
    
    glm::vec4 p1 = inv_modelview * glm::vec4(x,y,z_offset,1.0); 
    glm::vec4 p2 = inv_modelview * glm::vec4(x,y,z_offset-1, 1.0); 

    glm::vec4 pIn, pOut; 
    bool miss = ray_box_intersection(p1, p2, pIn, pOut); 
    if (miss) {p2 = p1; }
    else {
      p1 = pIn; p2 = pOut; 
    }

   Vertex LP1, LP2; 
    
    LP1.location[0] = p1[0]; LP1.location[1] = p1[1]; 
    LP1.location[2] = p1[2]; LP1.location[3] = p1[3]; 
    LP1.color[0] = 0.0;  LP1.color[1] = 0.0;   
    LP1.color[2] = 1.0;  LP1.color[3] = 1.0;

    LP2.location[0] = p2[0]; LP2.location[1] = p2[1];  
    LP2.location[2] = p2[2]; LP2.location[3] = p2[3]; 
    LP2.color[0] = 1.0;  LP2.color[1] = 0.0;   
    LP2.color[2] = 0.0;  LP2.color[3] = 1.0;

    Rays.addLine(LP1, LP2); 
  }
}

void InitTrisGeometry() 
{
  Vertex P0, P1, P2, P3; 
  Tris.clear(); 
  P0.location[0] = 0; P0.location[1] = 0;  P0.location[2] = 1.5;  P0.location[3] = 1; 
  P1.location[0] = 0; P1.location[1] = 1;  P1.location[2] = 1.5;  P1.location[3] = 1; 
  P2.location[0] = 1; P2.location[1] = 1;  P2.location[2] = 1.5;  P2.location[3] = 1; 
  P3.location[0] = 1; P3.location[1] = 0;  P3.location[2] = 1.5;  P3.location[3] = 1; 

  P0.color[0] = 0; P0.color[1] = 0;  P0.color[2] = 0;  P0.color[3] = 1;   
  P1.color[0] = 0; P1.color[1] = 1;  P1.color[2] = 0;  P1.color[3] = 1; 
  P2.color[0] = 1; P2.color[1] = 1;  P2.color[2] = 0;  P2.color[3] = 1; 
  P3.color[0] = 1; P3.color[1] = 0;  P3.color[2] = 0;  P3.color[3] = 1; 

  //Tris.addTriangle(P0,P1,P2); 
  //Tris.addTriangle(P0,P2,P3); 

  Vertex p[6]; 
  p[0] = P0; p[1] = P1; p[2] = P2; p[3] = P0; p[4] = P2; p[5] = P3; 

  Tris.addTriangles(p, 2);

}

void InitQuadsGeometry() 
{
  Vertex P0, P1, P2, P3; 
  Quads.clear(); 
  P0.location[0] = 0; P0.location[1] = 0;  P0.location[2] = -.5;  P0.location[3] = 1; 
  P1.location[0] = 0; P1.location[1] = 1;  P1.location[2] = -.5;  P1.location[3] = 1; 
  P2.location[0] = 1; P2.location[1] = 1;  P2.location[2] = -.5;  P2.location[3] = 1; 
  P3.location[0] = 1; P3.location[1] = 0;  P3.location[2] = -.5;  P3.location[3] = 1; 

  P0.color[0] = 0; P0.color[1] = 0;  P0.color[2] = 0;  P0.color[3] = 1;   
  P1.color[0] = 0; P1.color[1] = 1;  P1.color[2] = 0;  P1.color[3] = 1; 
  P2.color[0] = 1; P2.color[1] = 1;  P2.color[2] = 0;  P2.color[3] = 1; 
  P3.color[0] = 1; P3.color[1] = 0;  P3.color[2] = 0;  P3.color[3] = 1; 

  Quads.addQuad(P0, P1, P2, P3);  
}

//////////////////////////////////////////////////////////////////////////////////
//
//  A 1x1x1 BBX with the origin at its lower left coner 
// 
void InitBbxGeometry()
{
  BBX.clear(); 
  // negative z faces vertices 
  bbx_verts[0].location[0] = 0; bbx_verts[0].location[1] = 0;  bbx_verts[0].location[2] = 1;  bbx_verts[0].location[3] = 1; 
  bbx_verts[1].location[0] = 0; bbx_verts[1].location[1] = 1;  bbx_verts[1].location[2] = 1;  bbx_verts[1].location[3] = 1; 
  bbx_verts[2].location[0] = 1; bbx_verts[2].location[1] = 1;  bbx_verts[2].location[2] = 1;  bbx_verts[2].location[3] = 1; 
  bbx_verts[3].location[0] = 1; bbx_verts[3].location[1] = 0;  bbx_verts[3].location[2] = 1;  bbx_verts[3].location[3] = 1; 

  // positive  z faces vertices 
  bbx_verts[4].location[0] = 0; bbx_verts[4].location[1] = 0;  bbx_verts[4].location[2] = 0;  bbx_verts[4].location[3] = 1; 
  bbx_verts[5].location[0] = 0; bbx_verts[5].location[1] = 1;  bbx_verts[5].location[2] = 0;  bbx_verts[5].location[3] = 1; 
  bbx_verts[6].location[0] = 1; bbx_verts[6].location[1] = 1;  bbx_verts[6].location[2] = 0;  bbx_verts[6].location[3] = 1; 
  bbx_verts[7].location[0] = 1; bbx_verts[7].location[1] = 0;  bbx_verts[7].location[2] = 0;  bbx_verts[7].location[3] = 1; 

  bbx_verts[0].color[0] = 0; bbx_verts[0].color[1] = 0;  bbx_verts[0].color[2] = 0;  bbx_verts[0].color[3] = 1;   
  bbx_verts[1].color[0] = 0; bbx_verts[1].color[1] = 1;  bbx_verts[1].color[2] = 0;  bbx_verts[1].color[3] = 1; 
  bbx_verts[2].color[0] = 1; bbx_verts[2].color[1] = 1;  bbx_verts[2].color[2] = 0;  bbx_verts[2].color[3] = 1; 
  bbx_verts[3].color[0] = 1; bbx_verts[3].color[1] = 0;  bbx_verts[3].color[2] = 0;  bbx_verts[3].color[3] = 1; 

  bbx_verts[4].color[0] = 0; bbx_verts[4].color[1] = 0;  bbx_verts[4].color[2] = 1;  bbx_verts[4].color[3] = 1; 
  bbx_verts[5].color[0] = 0; bbx_verts[5].color[1] = 1;  bbx_verts[5].color[2] = 1;  bbx_verts[5].color[3] = 1; 
  bbx_verts[6].color[0] = 1; bbx_verts[6].color[1] = 1;  bbx_verts[6].color[2] = 1;  bbx_verts[6].color[3] = 1; 
  bbx_verts[7].color[0] = 1; bbx_verts[7].color[1] = 0;  bbx_verts[7].color[2] = 1;  bbx_verts[7].color[3] = 1; 

  BBX.addvList(bbx_verts, 8); 

  // create cube vertex indices. 

  // negative z face 
  bbx_indices[0] = 0;   bbx_indices[1] = 1;   bbx_indices[2] = 2; 
  bbx_indices[3] = 2;   bbx_indices[4] = 3;   bbx_indices[5] = 0; 
  // positive z face 
  bbx_indices[6] = 4;   bbx_indices[7] = 5;   bbx_indices[8] = 6; 
  bbx_indices[9] = 6;   bbx_indices[10] = 7;   bbx_indices[11] =4;
  // negative y face 
  bbx_indices[12] = 4;   bbx_indices[13] = 0;   bbx_indices[14] = 3; 
  bbx_indices[15] = 3;   bbx_indices[16] = 7;   bbx_indices[17] = 4;  
  // positive y face 
  bbx_indices[18] = 5;   bbx_indices[19] = 1;   bbx_indices[20] = 2; 
  bbx_indices[21] = 2;   bbx_indices[22] = 6;   bbx_indices[23] = 5;  
  // negative x face 
  bbx_indices[24] = 4;   bbx_indices[25] = 5;   bbx_indices[26] = 1; 
  bbx_indices[27] = 1;   bbx_indices[28] = 0;   bbx_indices[29] = 4;  
  // positive x face 
  bbx_indices[30] = 7;   bbx_indices[31] = 6;   bbx_indices[32] = 2; 
  bbx_indices[33] = 2;   bbx_indices[34] = 3;   bbx_indices[35] = 7;  

  BBX.addcList(bbx_indices, 36);  

}

////////////////////////////////////////////////////////////////////////////////////

bool toggle_cast= true; 

void display() 
{ 
  glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects 
  glClearColor(0,0,0,1); 
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

  if (!fill_mode)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   char *s= "basic"; 
  //glUseProgram(programObject);
  //renderer.useProgram(programObject);  
  renderer.useProgram(s); 

  GLuint c0 = glGetAttribLocation(programObject, "position"); 
  GLuint c1 = glGetAttribLocation(programObject, "color1"); 
  GLuint m1 = glGetUniformLocation(programObject, "local2clip"); 
  glEnableVertexAttribArray(c0); 
  glEnableVertexAttribArray(c1); 
 
  glm::mat4 model = glm::mat4(1.0f); 
  model = glm::scale(model, glm::vec3(scale_size, scale_size, scale_size)); 
  model = glm::rotate(model, x_angle, glm::vec3(0.0f, 1.0f, 0.0f)); 
  model = glm::rotate(model, y_angle, glm::vec3(1.0f, 0.0f, 0.0f)); 
  model = glm::translate(model, glm::vec3(-.5, -.5, -.5)); 
  glm::mat4 view = renderer.lookAt(glm::vec3(0.0, 0.0, 2.0), 
			       glm::vec3(0.0, 0.0, 0.0), 
			       glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 projection = renderer.perspective(camera_angle,1.0f,.1f,100.0f);
  glm::mat4 modelview = view * model; 
  glm::mat4 modelview_projection = projection * modelview; 
  inv_modelview = glm::inverse(modelview);

  if (toggle_cast == true) {
    InitTilesGeometry();  
    Tiles.initVBO(); 
    InitRaysGeometry();  
    Rays.initVBO(); 
    glm::vec4 ur = glm::vec4(0,0,-1,0); 
    glm::vec4 ray = inv_modelview * ur; 
    float mag = sqrt(ray[0]*ray[0] + ray[1]*ray[1] + ray[2]*ray[2]); 
    printf("Ray direction in local space: [%f %f %f %f] %f\n", ray[0]/mag, ray[1]/mag, ray[2]/mag, ray[3], mag); 
    toggle_cast = !toggle_cast; 
  }  

renderer.setLocal2clipPort(m1); 
renderer.setPosPort(c0); 
renderer.setColorPort(c1); 

BBX.setModelMatrix(model); 
Tiles.setModelMatrix(model); 
Rays.setModelMatrix(model); 
Tris.setModelMatrix(model); 
Quads.setModelMatrix(model); 

//renderer.setModelViewProjectionM(modelview_projection); 
//renderer.sendUMatrices(); 

//renderer.batch_render(); 
renderer.render(BBX); 
if (toggle_rays)
  renderer.render(Rays); 
if (toggle_grid)  {
  renderer.render(Tiles); 
  renderer.render(Tris); 
  renderer.render(Quads); 
}
  glutSwapBuffers(); 
  glUseProgram(0); 
} 

//////////////////////////////////////////////////////////////////////////////////
//
//    GLUT stuff 
//

void mymouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN) {
    press_x = x; press_y = y; 
    if (button == GLUT_LEFT_BUTTON)
      xform_mode = XFORM_ROTATE; 
	 else if (button == GLUT_RIGHT_BUTTON) 
      xform_mode = XFORM_SCALE; 
  }
  else if (state == GLUT_UP) {
	  xform_mode = XFORM_NONE; 
  }
}
void mymotion(int x, int y)
{
    if (xform_mode==XFORM_ROTATE) {
      x_angle += (x - press_x)/5.0; 
      if (x_angle > 180) x_angle -= 360; 
      else if (x_angle <-180) x_angle += 360; 
      press_x = x; 
	   
      y_angle += (y - press_y)/5.0; 
      if (y_angle > 180) y_angle -= 360; 
      else if (y_angle <-180) y_angle += 360; 
      press_y = y; 
    }
	else if (xform_mode == XFORM_SCALE){
      float old_size = scale_size;
      scale_size *= (1+ (y - press_y)/60.0); 
      if (scale_size <0) scale_size = old_size; 
      press_y = y; 
    }
    glutPostRedisplay(); 
}
void mykey(unsigned char key, int x, int y)
{
  if (key == 'w') fill_mode = !fill_mode; 
  if (key == 'g') toggle_grid = !toggle_grid; 
  if (key == 'r') toggle_rays = !toggle_rays; 
  if (key == 'c') toggle_cast = !toggle_cast; 
	if (key == 'q') exit(1); 
	glutPostRedisplay(); 
}
///////////////////////////////////////////////////////////////////////////////
//
int main(int argc, char** argv) 
{ 
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH); 

  width = height = 600; 
  tile_width = tile_height = 60; 

  num_tiles = int(width/tile_width)* int(height/tile_height); 

  glutInitWindowSize(width,height); 

  glutCreateWindow("shader cube"); 
  glutDisplayFunc(display); 
  glutMouseFunc(mymouse); 
  glutMotionFunc(mymotion);
  glutKeyboardFunc(mykey); 

  // initialize GLEW 

  GLenum err = glewInit(); 

  if ( err != GLEW_OK)  printf(" Error initializing GLEW! \n"); 
  else printf("Initializing GLEW succeeded!\n"); 

  Program p; 
  programObject = p.program = renderer.setupGLSL("cubeXform"); 
  p.name = "basic";
  renderer.addProgram(p); 

  InitBbxGeometry();   BBX.initVBO(); 
  InitTrisGeometry();  Tris.initVBO(); 
  InitQuadsGeometry();  Quads.initVBO(); 

  renderer.addContainer(&BBX); 
  renderer.addContainer(&Tiles); 
  renderer.addContainer(&Rays); 
  renderer.addContainer(&Tris); 
  renderer.addContainer(&Quads); 

  glutMainLoop(); 
} 
