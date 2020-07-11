

#ifndef _RENDERER_H_
#define _RENDERER_H_

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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> 
#include <list> 

#include "containers.h"

GLuint SetupGLSL(char*); 

typedef struct
{
    GLuint program; 
    char *name; 
} Program; 

class Renderer
{
public:
    Renderer(){}; 
    ~Renderer(){}; 
    glm::mat4 lookAt(glm::vec3 eyepos, glm::vec3 coi, glm::vec3 viewup); 
    glm::mat4 perspective(float y_angle, float ratio, float near, float far); 
    glm::mat4 getViewMatrix(){return viewM;}; 
    glm::mat4 getProjectMatrix(){return projectionM;}; 
    void resetMatrices() {has_view = false; 
                       has_projection = false; 
                       has_view_projection= false; } 
    void useProgram(GLuint program) {glUseProgram(program); }; 
    bool useProgram(char* pname); 
    void addProgram(Program program) {programList.push_back(program);}; 
    void addContainer(Container* c) {objContainers.push_back(c); }; 
    void clearContainer(){objContainers.clear(); }; 
    void setPosPort(GLuint c) {pos_port = c; has_pos_port = true; }; 
    void setColorPort(GLuint c){color_port = c; has_color_port = true; }; 
    void setNormalPort(GLuint c){normal_port = c; has_normal_port = true; }; 
    void setTexCoordPort(GLuint c){texcoord_port = c;has_texcoord_port = true; }; 
    void setLocal2clipPort(GLuint m){
            local2clip_port = m; 
            has_local2clip_port = true; }

    void setModelViewProjectionM(glm::mat4 mvp) {
        model_view_projectionM = mvp; 
        has_model_view_projection = true; }
    void sendUMatrices(); 

    void render(Container&); 
    void batch_render(); 
   
    GLuint setupGLSL(char *shader) {return SetupGLSL(shader);}; 
protected: 
    int window_w, window_h; 
    int viewport_x, viewport_y, viewport_w, viewport_h; 
    std::list<Container*> objContainers; 
    glm::vec3 camera_pos, camera_coi, camera_viewup; 
    float camera_y_angle, camera_aspect, camera_near, camera_far;  
    glm::mat4 viewM, projectionM, view_projectionM, model_view_projectionM; 
    std::list<Program> programList; 
    bool has_view = false; 
    bool has_projection = false; 
    bool has_view_projection = false; 
    bool has_model_view_projection = false; 

    GLuint local2clip_port; 
    bool has_local2clip_port = false; 
    GLuint pos_port;   
    bool has_pos_port = false; 
    GLuint color_port; 
    bool has_color_port = false; 
    GLuint normal_port; 
    bool has_normal_port = false; 
    GLuint texcoord_port; 
    bool has_texcoord_port = false; 
}; 

#endif