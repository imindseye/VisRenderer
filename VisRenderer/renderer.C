
#include "renderer.h"

#define D2R(d) d/360.0 * 3.1415926 *2

glm::mat4 Renderer::lookAt(glm::vec3 eyepos, glm::vec3 coi, glm::vec3 viewup)
{
     camera_pos = eyepos; 
     camera_coi = coi; 
     camera_viewup = viewup; 
     viewM = glm::lookAt(eyepos, coi, viewup);
     has_view= true; 
     if (has_projection) {
         view_projectionM = projectionM * viewM; 
         has_view_projection = true; 
     }
     return viewM; 
}; 

glm::mat4 Renderer::perspective(float yangle, float ratio, float near, float far)
{
    camera_y_angle = yangle; 
    camera_aspect = ratio; 
    camera_near = near; camera_far = far; 
    projectionM = glm::perspective((float)(D2R(yangle)),ratio,near,far); 
    has_projection = true; 
    if (has_view) {
        view_projectionM = projectionM * viewM; 
        has_view_projection= true; 
    }
    return projectionM; 
}; 


bool Renderer::useProgram(char *name)
{
    for(std::list<Program>::const_iterator pIter = programList.begin(); 
			pIter!= programList.end(); 
			pIter++)
        {
                Program p= *pIter; 
                printf("name =%s %s\n", p.name, name); 
                if (p.name == name) {
                    printf("got .....\n"); 
                    glUseProgram(p.program); 
                    return(true); 
                }
                
        }
    return(false); 
 }

 void Renderer::sendUMatrices() {
    glUniformMatrix4fv(local2clip_port, 1, GL_FALSE, &model_view_projectionM[0][0]); 
 }

 void Renderer::render(Container& obj)  {
    glm::mat4 m = obj.getModelMatrix(); 
    model_view_projectionM = view_projectionM * m; 
    sendUMatrices(); 
    obj.bindVBO(); 
    obj.sendPositions(pos_port); 
    obj.sendColors(color_port); 
    obj.draw(); 
 }

 void Renderer::batch_render() 
 {
    for(std::list<Container*>::const_iterator pIter = objContainers.begin(); 
			pIter!= objContainers.end(); 
			pIter++){
                Container* obj = *pIter; 
                glm::mat4 m = obj->getModelMatrix(); 
                model_view_projectionM = view_projectionM * m; 
                sendUMatrices(); 
                obj->bindVBO(); 
                obj->sendPositions(pos_port); 
                obj->sendColors(color_port); 
                obj->draw(); 
            }
 }; 




