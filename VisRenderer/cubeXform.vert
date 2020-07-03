
attribute vec4 position; 
attribute vec4 color1; 
attribute vec4 normal; 

uniform mat4 local2clip; 

varying vec4 pcolor; 

void main(){
     
      pcolor = color1; 
      
      gl_Position = local2clip * position; 

}
