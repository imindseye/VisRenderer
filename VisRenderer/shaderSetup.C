

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <fcntl.h>
#include <math.h>
#include <unistd.h>

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLUT/glut.h> 
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else 
#include <GL/glew.h>
#include <GL/glut.h> 
#include <GL/gl.h>
#include <GL/glext.h>
#endif

///////////////////////////////////////////////////////////////
// GLSL setup code 
///////////////////////////////////////////////////////////////

GLuint VertexShaderObject = 0;
GLuint FragmentShaderObject = 0;

GLchar *vertexShaderSource;
GLchar *fragmentShaderSource;

/////////////////////////////////////
//GLSL specific types and variables


typedef enum {
    EVertexShader,
    EFragmentShader,
} EShaderType;


/////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
//
//  Routine to check the capability of your graphics card 
//
void check_graphics() 
{
  const GLubyte *renderer = glGetString(GL_RENDERER); 
  const GLubyte *vendor = glGetString(GL_VENDOR); 
  const GLubyte *version = glGetString(GL_VERSION); 
  const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION); 
  GLint major, minor; 
  glGetIntegerv(GL_MAJOR_VERSION, &major); 
  glGetIntegerv(GL_MINOR_VERSION, &minor); 
  
  printf(" ----------------- Checking graphics capability ...\n"); 
  printf(" GL Vendor: %s \n", vendor); 
  printf(" GL Renderer: %s \n", renderer); 
  printf(" GL version: %s\n", version); 
  printf(" GL version: %d.%d\n", major, minor); 
  printf(" GLSL version: %s\n", glslVersion); 

  // now check the availability of shaders 

  if (! GLEW_ARB_vertex_program) printf(" ARB vertex program is not supported!!\n");  
  else printf(" ARB vertex program is supported!!\n"); 

  if (! GLEW_ARB_vertex_shader) printf(" ARB vertex shader is not supported!!\n");  
  else printf(" ARB vertex shader is supported!!\n"); 

  if (! GLEW_ARB_fragment_program) printf(" ARB fragment program is not supported!!\n");  
  else printf(" ARB fragment program is supported!!\n"); 

  if (! GLEW_ARB_fragment_shader) printf(" ARB fragment shader is not supported!!\n");  
  else printf(" ARB fragment shader is supported!!\n"); 

  // another way to query the shaders support 

  if (glewGetExtension("GL_ARB_fragment_shader")      != GL_TRUE ||
      glewGetExtension("GL_ARB_vertex_shader")        != GL_TRUE ||
      glewGetExtension("GL_ARB_shader_objects")       != GL_TRUE ||
      glewGetExtension("GL_ARB_shading_language_100") != GL_TRUE)
    {
      fprintf(stderr, "Driver does not support OpenGL Shading Language\n");
      exit(1);
    }
  else fprintf(stderr, "GLSL supported and ready to go\n"); 

  printf(" -----------------  checking graphics capability done. \n"); 

} 

////////////////////////////////////////////////////////////////////////////////////////

int shaderSize(char *fileName, EShaderType shaderType)
{
    //
    // Returns the size in bytes of the shader fileName.
    // If an error occurred, it returns -1.
    //
    // File name convention:
    //
    // <fileName>.vert
    // <fileName>.frag
    //
    int fd; 
    char name[100];
    int count = -1;

    strcpy(name, fileName);

    switch (shaderType)
    {
        case EVertexShader:
            strcat(name, ".vert");
            break;
        case EFragmentShader:
            strcat(name, ".frag");
            break;
        default:
            printf("ERROR: unknown shader file type\n");
            exit(1);
            break;
    }

    //
    // Open the file, seek to the end to find its length
    //
#ifdef WIN32 /*[*/
    fd = _open(name, _O_RDONLY);
    if (fd != -1)
    {
        count = _lseek(fd, 0, SEEK_END) + 1;
        _close(fd);
    }
#else /*][*/
    fd = open(name, O_RDONLY);
    if (fd != -1)
    {
        count = lseek(fd, 0, SEEK_END) + 1;
        close(fd);
    }
#endif /*]*/
    fprintf(stderr, "*** %d\n", count); 
    return count;
}


int readShader(char *fileName, EShaderType shaderType, char *shaderText, int size)
{
    //
    // Reads a shader from the supplied file and returns the shader in the
    // arrays passed in. Returns 1 if successful, 0 if an error occurred.
    // The parameter size is an upper limit of the amount of bytes to read.
    // It is ok for it to be too big.
    //
    FILE *fh;
    char name[100];
    int count;

    strcpy(name, fileName);

    switch (shaderType) 
    {
        case EVertexShader:
            strcat(name, ".vert");
            break;
        case EFragmentShader:
            strcat(name, ".frag");
            break;
        default:
            printf("ERROR: unknown shader file type\n");
            exit(1);
            break;
    }

    //
    // Open the file
    //
    fh = fopen(name, "r");
    if (fh==NULL)
        return -1;

    //
    // Get the shader from a file.
    //
    fseek(fh, 0, SEEK_SET);
    count = fread(shaderText, 1, size, fh);
    shaderText[count] = '\0';

    if (ferror(fh))
        count = 0;

    fclose(fh);
    return count;
}


/*public*/
int readShaderSource(char *fileName, GLchar **vertexShader, GLchar **fragmentShader)
{
    int vSize, fSize;

    //
    // Allocate memory to hold the source of our shaders.
    //
    vSize = shaderSize(fileName, EVertexShader);
    fSize = shaderSize(fileName, EFragmentShader);

    if ((vSize == -1) || (fSize == -1))
    {
        printf("Cannot determine size of the shader %s\n", fileName);
        return 0;
    }

    *vertexShader = (GLchar *) malloc(vSize);
    *fragmentShader = (GLchar *) malloc(fSize);

    //
    // Read the source code
    //
    if (!readShader(fileName, EVertexShader, *vertexShader, vSize))
    {
        printf("Cannot read the file %s.vert\n", fileName);
        return 0;
    }

    if (!readShader(fileName, EFragmentShader, *fragmentShader, fSize))
    {
        printf("Cannot read the file %s.frag\n", fileName);
        return 0;
    }

    return 1;
}


GLint getUniLoc(GLuint program, const GLchar *name)
{
    GLint loc;

    loc = glGetUniformLocation(program, name);

    if (loc == -1)
        printf("No such uniform named \"%s\"\n", name);
    return loc;
}


///////////////////////////////////////////////////////
//
//  This routine creates the shader program, including the vertex shader and fragment shader objects 
//
GLuint SetupGLSL(char *fileName){

        GLuint programObject;
	GLuint vertexShaderObject;
	GLuint fragmentShaderObject;

	check_graphics();  // check the capability of the graphics card in use
	
	programObject = glCreateProgram();  // create an overall shader program 
	if (programObject == 0) {  // error checking 
	  printf(" Error creating shader program object.\n"); 
	  exit(1); 
	} 
	else printf(" Succeeded creating shader program object.\n"); 

	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	if (vertexShaderObject == 0) {  // error checking 
	  printf(" Error creating vertex shader object.\n"); 
	  exit(1); 
	} 
	else printf(" Succeeded creating vertex shader object.\n"); 

	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragmentShaderObject == 0) {  // error checking 
	  printf(" Error creating fragment shader object.\n"); 
	  exit(1); 
	} 
	else printf(" Succeeded creating fragment shader object.\n"); 

	// now input the vertex and fragment programs as ascii 
	readShaderSource(fileName, &vertexShaderSource, &fragmentShaderSource); 
	glShaderSource(vertexShaderObject,1,(const GLchar**)&vertexShaderSource,NULL);
	glShaderSource(fragmentShaderObject,1,(const GLchar**)&fragmentShaderSource,NULL);

	// now compile the shader code; vertex shader first, followed by fragment shader 
	glCompileShader(vertexShaderObject);

	// error checking and printing out log if error 
	GLint result; 
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &result); 
	if (result == GL_FALSE) {
	  printf(" vertex shader compilation failed!\n"); 
	  GLint logLen; 
	  glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &logLen); 
	  if (logLen > 0) {
	    char *log = (char*) malloc(logLen); 
	    GLsizei written; 
	    glGetShaderInfoLog(vertexShaderObject, logLen, &written, log); 
	    printf("Sahder log: \n %s", log); 
	    free(log); 
	  }
	}

	glCompileShader(fragmentShaderObject);
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &result); 
	if (result == GL_FALSE) {
	  printf(" fragment shader compilation failed!\n"); 
	  GLint logLen; 
	  glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &logLen); 
	  if (logLen > 0) {
	    char *log = (char*) malloc(logLen); 
	    GLsizei written; 
	    glGetShaderInfoLog(fragmentShaderObject, logLen, &written, log); 
	    printf("Sahder log: \n %s", log); 
	    free(log); 
	  }
	}
	// insert compiler error checking code later 

	glAttachShader(programObject, vertexShaderObject);
	glAttachShader(programObject, fragmentShaderObject);

	glLinkProgram(programObject);
	// insert linking error checking code later 

	return(programObject); 
}
