

#ifndef _CONTAINER_H_
#define _CONTAINER_H_

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

typedef struct 
{
  float location[4]; 
  float color[4]; 
  float normal[4]; 
  float tex2d_coord[2]; 
} Vertex; 

class Container 
{
public:
        Container(){}; 
        ~Container(){}; 
        virtual void draw() = 0; 
        virtual int initVBO() = 0; // initialize VBOs etc.  
        virtual void bindVBO() = 0; 
        virtual void clear()=0; 
        virtual int getSize() =0; 

        void setModelMatrix(glm::mat4 m) {model_matrix = m; }
        glm::mat4 getModelMatrix() { return model_matrix; } 
        GLuint getVBO() {return vbo;}
        bool sendPositions(GLuint pos_port) {
                c_pos_port = pos_port; 
                glVertexAttribPointer(pos_port,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0);  
                return true;      
        }
        bool sendColors(GLuint color_port) {
                c_color_port = color_port; 
                glVertexAttribPointer(color_port,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16);       
                return true;     
        }
        bool sendNormals(GLuint normal_port) {
                c_normal_port = normal_port; 
                glVertexAttribPointer(normal_port,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+32);       
                return true;  
        }
        bool sendTexCoords(GLuint texcoord_port) {
                c_texcoord_port = texcoord_port; 
                glVertexAttribPointer(texcoord_port,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);       
                return true;  
        }
protected:
        glm::mat4 model_matrix; // model (local 2 world) matrix 
        std::list<Vertex> vList;  // vertex list 
        std::list<GLuint> cList; 
        GLuint vbo; // OpenGL Vertex Buffer Object 
        GLuint indexVBO; 
        GLuint c_pos_port, c_color_port, c_normal_port, c_texcoord_port; 
        bool has_data = false; 
};

class PointsContainer : public Container 
{
public:
        PointsContainer(); 
        ~PointsContainer(); 
        void draw(); 
        int initVBO();
        void bindVBO(); 
        void clear() {if (has_data) vList.clear(); }  
        void addPoint(Vertex p); 
        void addPoints(Vertex* pArray, int size); 
        int getSize() {return vList.size();}
        void setPointSize(int s) {pSize = s;}; 
protected: 
        int pSize=1; 

}; 

class LinesContainer : public Container
{
public: 
        LinesContainer(); 
        ~LinesContainer(); 
        void draw(); 
        int initVBO(); 
        void bindVBO();    
        void clear() {if (has_data) vList.clear(); }  
        void addLine(Vertex p1, Vertex p2); 
        void addLines(Vertex* pArray, int size); 
        int getSize() {return (vList.size()/2);} 
protected: 
}; 


class TrianglesContainer : public Container
{
public: 
        TrianglesContainer(); 
        ~TrianglesContainer(); 
        void draw(); 
        int initVBO(); 
        void bindVBO(); 
        void clear() {if (has_data) vList.clear(); }  
        void addTriangle(Vertex p1, Vertex p2, Vertex p3); 
        void addTriangles(Vertex* pArray, int size); 
        int getSize() {return (vList.size()/3);} 
protected: 
}; 

class QuadsContainer : public Container
{
public: 
        QuadsContainer(); 
        ~QuadsContainer(); 
        void draw(); 
        int initVBO(); 
        void bindVBO(); 
        void clear() {if (has_data) vList.clear(); }  
        void addQuad(Vertex p1, Vertex p2, Vertex p3, Vertex p4); 
        void addQuads(Vertex* pArray, int size); 
        int getSize() {return (vList.size()/6);} 
protected:  
}; 

class MeshContainer : public Container
{
public: 
        MeshContainer(); 
        ~MeshContainer(); 
        void draw(); 
        int initVBO(); 
        void bindVBO(); 
        void clear() {if (has_data) vList.clear(); cList.clear();}  
        void addvList(Vertex* pArray, int size); 
        void addTriangle(GLuint c1, GLuint c2, GLuint c3); 
        void addcList(GLuint* cArray, int size); 
        int getSize() {return (vList.size());} 
        int getvSize() {return vList.size();}
        int getcSize() {return cList.size();} 
        GLuint getcVBO(){ return indexVBO;}
protected:   
}; 



#endif