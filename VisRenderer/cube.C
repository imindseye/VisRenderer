
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

typedef struct 
{
  float location[4]; 
  float color[4]; 
} Vertex; 

Vertex verts[8];       // cube vertices 

GLubyte tindices[6*6];   // cube face indices: 6 faces, 6 indices each face 

GLuint vboHandle[1];   // a VBO that contains interleaved positions and colors 
GLuint indexVBO; 

GLuint programObject;
GLuint SetupGLSL(char*); 

  GLuint vao; 

bool fill_mode = true; 

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



//////////////////////////////////////////////////////////////////////////////////
//
//  Just hard code the geometry 
// 
void InitGeometry()
{
  // negative z faces vertices 
  verts[0].location[0] = -0.5; verts[0].location[1] = -0.5;  verts[0].location[2] = -0.5;  verts[0].location[3] = 1; 
  verts[1].location[0] = -0.5; verts[1].location[1] = 0.5;  verts[1].location[2] = -0.5;  verts[1].location[3] = 1; 
  verts[2].location[0] = 0.5; verts[2].location[1] = 0.5;  verts[2].location[2] = -0.5;  verts[2].location[3] = 1; 
  verts[3].location[0] = 0.5; verts[3].location[1] = -0.5;  verts[3].location[2] = -0.5;  verts[3].location[3] = 1; 

  // positive  z faces vertices 
  verts[4].location[0] = -0.5; verts[4].location[1] = -0.5;  verts[4].location[2] = 0.5;  verts[4].location[3] = 1; 
  verts[5].location[0] = -0.5; verts[5].location[1] = 0.5;  verts[5].location[2] = 0.5;  verts[5].location[3] = 1; 
  verts[6].location[0] = 0.5; verts[6].location[1] = 0.5;  verts[6].location[2] = 0.5;  verts[6].location[3] = 1; 
  verts[7].location[0] = 0.5; verts[7].location[1] = -0.5;  verts[7].location[2] = 0.5;  verts[7].location[3] = 1; 

  verts[0].color[0] = 0; verts[0].color[1] = 0;  verts[0].color[2] = 1;  verts[0].color[3] = 1;   
  verts[1].color[0] = 0; verts[1].color[1] = 0;  verts[1].color[2] = 1;  verts[1].color[3] = 1; 
  verts[2].color[0] = 1; verts[2].color[1] = 1;  verts[2].color[2] = 0;  verts[2].color[3] = 1; 
  verts[3].color[0] = 1; verts[3].color[1] = 1;  verts[3].color[2] = 0;  verts[3].color[3] = 1; 

  verts[4].color[0] = 1; verts[4].color[1] = 1;  verts[4].color[2] = 0;  verts[4].color[3] = 1; 
  verts[5].color[0] = 1; verts[5].color[1] = 1;  verts[5].color[2] = 0;  verts[5].color[3] = 1; 
  verts[6].color[0] = 0; verts[6].color[1] = 0;  verts[6].color[2] = 1;  verts[6].color[3] = 1; 
  verts[7].color[0] = 0; verts[7].color[1] = 0;  verts[7].color[2] = 1;  verts[7].color[3] = 1; 

  // create cube vertex indices. 

  // negative z face 
  tindices[0] = 0;   tindices[1] = 1;   tindices[2] = 2; 
  tindices[3] = 2;   tindices[4] = 3;   tindices[5] = 0; 
  // positive z face 
  tindices[6] = 4;   tindices[7] = 5;   tindices[8] = 6; 
  tindices[9] = 6;   tindices[10] = 7;   tindices[11] =4;
  // negative y face 
  tindices[12] = 4;   tindices[13] = 0;   tindices[14] = 3; 
  tindices[15] = 3;   tindices[16] = 7;   tindices[17] = 4;  
  // positive y face 
  tindices[18] = 5;   tindices[19] = 1;   tindices[20] = 2; 
  tindices[21] = 2;   tindices[22] = 6;   tindices[23] = 5;  
  // negative x face 
  tindices[24] = 4;   tindices[25] = 5;   tindices[26] = 1; 
  tindices[27] = 1;   tindices[28] = 0;   tindices[29] = 4;  
  // positive x face 
  tindices[30] = 7;   tindices[31] = 6;   tindices[32] = 2; 
  tindices[33] = 2;   tindices[34] = 3;   tindices[35] = 7;  
}

////////////////////////////////////////////////////////////////////////////////////

void display() 
{ 
  glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects 
  glClearColor(0,0,0,1); 
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

  if (!fill_mode)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
  glUseProgram(programObject);

  GLuint c0 = glGetAttribLocation(programObject, "position"); 
  GLuint c1 = glGetAttribLocation(programObject, "color1"); 

  GLuint m1 = glGetUniformLocation(programObject, "local2clip"); 

  glEnableVertexAttribArray(c0); 
  glEnableVertexAttribArray(c1); 

  glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO); 

  // Tells OpenGL how to walk through the VBOs, i.e., how the data are packed 
  // number of coordinates per vertex (4 here), type of the coordinates, 
  // stride between consecutive vertices, and pointers to the first coordinate

  glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0); 
  glVertexAttribPointer(c1,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16); 

  //
  //  glBindVertexArray(vao); 
  // 

  // Now we are ready to draw, using the triangle indices in the buffer 
  //
  // The following code uses GLM, the OpenGL mathematics library. This is to replace 
  // the OpenGL2.x matrix functions which have been deprecated. 
  // 
  // 
  // use the GLM helper functions to construct projection and modelview matrices 
  glm::mat4 projection = glm::perspective(1.0f,1.0f,.1f,100.0f); 

  glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), 
			       glm::vec3(0.0, 0.0, 0.0), 
			       glm::vec3(0.0, 1.0, 0.0)); 

  glm::mat4 model = glm::mat4(1.0f); 
  model = glm::rotate(model, x_angle, glm::vec3(0.0f, 1.0f, 0.0f)); 
  model = glm::rotate(model, y_angle, glm::vec3(1.0f, 0.0f, 0.0f)); 
  model = glm::scale(model, glm::vec3(scale_size, scale_size, scale_size)); 

  // construct the modelview and modelview projection matrices 
  glm::mat4 modelview = view * model; 
  glm::mat4 modelview_projection = projection * modelview; 

  // pass the modelview_projection matrix to the shader as a uniform 
  glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]); 

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (char*) NULL+0); 

  glutSwapBuffers(); 

  glUseProgram(0); 
} 
//////////////////////////////////////////////////////////////////////////////////
//
// create VBO objects and send the triangle vertices/colors to the graphics card
// 
void InitVBO() 
{
  glGenBuffers(1, vboHandle);   // create an interleaved VBO object
  glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);   // bind the first handle 

  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*8, verts, GL_STATIC_DRAW); // allocate space and copy the position data over
  glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up 

  glGenBuffers(1, &indexVBO); 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO); 
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*36, tindices, GL_STATIC_DRAW);  // load the index data 

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up 

  //---------- try to use vertex array object 

  /*
  // vertex array does not seem to work for my macbook pro
  // The main program is that you need to use  glGenVertexArrayAPPLE() to make it work, which is 
  // not platform independent 
  // 
  glGenVertexArrays(1, &vao);     // need to use this on mac: glGenVertexArraysAPPLE(1, &vao); 
  glBindVertexArray(vao); 

  GLuint c0 = glGetAttribLocation(programObject, "position"); 
  GLuint c1 = glGetAttribLocation(programObject, "color1"); 

  glEnableVertexAttribArray(c0); 
  glEnableVertexAttribArray(c1); 

  glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]); 

  // Tells OpenGL how to walk through the VBOs, i.e., how the data are packed 
  // number of coordinates per vertex (4 here), type of the coordinates, 
  // stride between consecutive vertices, and pointers to the first coordinate

  glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0); 
  glVertexAttribPointer(c1,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16); 

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO); 
  */
  // by now, we moved the position and color data over to the graphics card. There will be no redundant data copy at drawing time 
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
	if (key == 'q') exit(1); 
	glutPostRedisplay(); 
}
///////////////////////////////////////////////////////////////////////////////
//
int main(int argc, char** argv) 
{ 
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH); 
  glutInitWindowSize(600,600); 

  glutCreateWindow("shader cube"); 
  glutDisplayFunc(display); 
  glutMouseFunc(mymouse); 
  glutMotionFunc(mymotion);
  glutKeyboardFunc(mykey); 

  // initialize GLEW 

  GLenum err = glewInit(); 

  if ( err != GLEW_OK)  printf(" Error initializing GLEW! \n"); 
  else printf("Initializing GLEW succeeded!\n"); 

  programObject = SetupGLSL("cubeXform");  //create shaders - assume the shaders are cubeXform.vert and cubeXform.frag

  InitGeometry(); 
  InitVBO(); 

  glutMainLoop(); 
} 
