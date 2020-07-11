#include "containers.h"


PointsContainer::PointsContainer()
{

}; 

PointsContainer::~PointsContainer()
{
}
void PointsContainer::addPoint(Vertex p) 
{
    vList.push_back(p); 
    has_data = true; 
}
void PointsContainer::addPoints(Vertex *pArray, int size)
{
    for (int i=0; i<size; i++){
        vList.push_back(pArray[i]); 
    }
    has_data = true; 
}
int PointsContainer::initVBO()
{
    glGenBuffers(1, &vbo);   // create an interleaved VBO object
    glBindBuffer(GL_ARRAY_BUFFER, vbo);   // bind the first handle 
    Vertex *pArray = new Vertex[vList.size()];
    int i = 0; 
    for(std::list<Vertex>::const_iterator
				pIter = vList.begin(); 
			pIter!= vList.end(); 
			pIter++, i++)
            {
                pArray[i] = *pIter; 
            } 
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vList.size(), pArray, GL_STATIC_DRAW); // allocate space and copy the position data over
    glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up   
    return(1); 
}

void PointsContainer::bindVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
}; 

void PointsContainer::draw() 
{
    glPointSize(6); 
    glDrawArrays(GL_POINTS, 0, getSize());    
}

LinesContainer::LinesContainer()
{

}; 

LinesContainer::~LinesContainer()
{
}
void LinesContainer::addLine(Vertex p1, Vertex p2) 
{
    vList.push_back(p1); 
    vList.push_back(p2); 
    has_data = true; 
}
void LinesContainer::addLines(Vertex *pArray, int size)
{
    for (int i=0; i<size; i++){
        vList.push_back(pArray[i*2]); 
        vList.push_back(pArray[i*2+1]); 
    }
    has_data = true; 
}
int LinesContainer::initVBO()
{
    glGenBuffers(1, &vbo);   // create an interleaved VBO object
    glBindBuffer(GL_ARRAY_BUFFER, vbo);   // bind the first handle 
    Vertex *pArray = new Vertex[vList.size()];
    int i = 0; 
    for(std::list<Vertex>::const_iterator
				pIter = vList.begin(); 
			pIter!= vList.end(); 
			pIter++, i++)
            {
                pArray[i] = *pIter; 
            } 
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vList.size(), pArray, GL_STATIC_DRAW); // allocate space and copy the position data over
    glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up   
    return(1); 
}

void LinesContainer::bindVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
}

void LinesContainer::draw() 
{
    glDrawArrays(GL_LINES, 0, getSize()*2);    
}

TrianglesContainer::TrianglesContainer()
{

}; 

TrianglesContainer::~TrianglesContainer()
{
}
void TrianglesContainer::addTriangle(Vertex p1, Vertex p2, Vertex p3) 
{
    vList.push_back(p1); 
    vList.push_back(p2); 
    vList.push_back(p3); 
    has_data = true; 
}
void TrianglesContainer::addTriangles(Vertex *pArray, int size)
{
    for (int i=0; i<size; i++){
        vList.push_back(pArray[i*3]); 
        vList.push_back(pArray[i*3+1]); 
        vList.push_back(pArray[i*3+2]); 
    }
    has_data = true; 
}
int TrianglesContainer::initVBO()
{
    glGenBuffers(1, &vbo);   // create an interleaved VBO object
    glBindBuffer(GL_ARRAY_BUFFER, vbo);   // bind the first handle 
    Vertex *pArray = new Vertex[vList.size()];
    int i = 0; 
    for(std::list<Vertex>::const_iterator
				pIter = vList.begin(); 
			pIter!= vList.end(); 
			pIter++, i++)
            {
                pArray[i] = *pIter; 
            } 
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vList.size(), pArray, GL_STATIC_DRAW); // allocate space and copy the position data over
    glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up   
    return(1); 
}
void TrianglesContainer::bindVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
}
void TrianglesContainer::draw() 
{
    glDrawArrays(GL_TRIANGLES, 0, getSize()*3);    
}

QuadsContainer::QuadsContainer()
{

}; 

QuadsContainer::~QuadsContainer()
{
}
void QuadsContainer::addQuad(Vertex p1, Vertex p2, Vertex p3, Vertex p4) 
{
    vList.push_back(p1); 
    vList.push_back(p3); 
    vList.push_back(p2); 
    vList.push_back(p1); 
    vList.push_back(p4); 
    vList.push_back(p3); 
    has_data = true; 
}
void QuadsContainer::addQuads(Vertex *pArray, int size)
{
    for (int i=0; i<size; i++){
        vList.push_back(pArray[i*4]); 
        vList.push_back(pArray[i*4+2]); 
        vList.push_back(pArray[i*4+1]); 
        vList.push_back(pArray[i*4]); 
        vList.push_back(pArray[i*4+3]); 
        vList.push_back(pArray[i*4+2]);
    }
    has_data = true; 
}
int QuadsContainer::initVBO()
{
    glGenBuffers(1, &vbo);   // create an interleaved VBO object
    glBindBuffer(GL_ARRAY_BUFFER, vbo);   // bind the first handle 
    Vertex *pArray = new Vertex[vList.size()];
    int i = 0; 
    for(std::list<Vertex>::const_iterator
				pIter = vList.begin(); 
			pIter!= vList.end(); 
			pIter++, i++)
            {
                pArray[i] = *pIter; 
            } 
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vList.size(), pArray, GL_STATIC_DRAW); // allocate space and copy the position data over
    glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up   
    return(1); 
}

void QuadsContainer::bindVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
}
void QuadsContainer::draw() 
{
    glDrawArrays(GL_TRIANGLES, 0, getSize()*6);    
}

MeshContainer::MeshContainer()
{

}; 

MeshContainer::~MeshContainer()
{
}
void MeshContainer::addvList(Vertex *pArray, int size)
{
    for (int i=0; i<size; i++){
        vList.push_back(pArray[i]); 
    }
    has_data = true; 
}
void MeshContainer::addTriangle(GLuint c1, GLuint c2, GLuint c3)
{
    cList.push_back(c1); 
    cList.push_back(c2); 
    cList.push_back(c3);
}; 

void MeshContainer::addcList(GLuint *cArray, int size)
{
    for (int i=0; i<size/3; i++){
        cList.push_back(cArray[3*i]); 
        cList.push_back(cArray[3*i+1]); 
        cList.push_back(cArray[3*i+2]); 
    }
    has_data = true; 
}

int MeshContainer::initVBO()
{
    glGenBuffers(1, &vbo);   // create an interleaved VBO object
    glBindBuffer(GL_ARRAY_BUFFER, vbo);   // bind the first handle 
    Vertex *pArray = new Vertex[vList.size()];
    int i = 0; 
    for(std::list<Vertex>::const_iterator pIter = vList.begin(); 
			pIter!= vList.end(); 
			pIter++, i++)
    {
        pArray[i] = *pIter; 
    } 
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vList.size(), pArray, GL_STATIC_DRAW); // allocate space and copy the position data over
    glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up 

    glGenBuffers(1, &indexVBO); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO); 
    GLuint *cArray = new GLuint[cList.size()];
    i = 0; 
    for(std::list<GLuint>::const_iterator pIter = cList.begin(); 
			pIter!= cList.end(); 
			pIter++, i++)
    {
        cArray[i] = *pIter; 
    } 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*cList.size(), cArray, GL_STATIC_DRAW);  // load the index data 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up 

    return(1); 
}
void MeshContainer::bindVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);  
}; 

void MeshContainer::draw() 
{
    glDrawElements(GL_TRIANGLES, getcSize(), GL_UNSIGNED_INT, (char*) NULL+0);    
}