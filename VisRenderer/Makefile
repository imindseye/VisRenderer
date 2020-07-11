
CPP = g++ 

# Uncomment to turn on debugging:
CPPFLAGS = -g -DDEBUG -DGL_SILENCE_DEPRECATION

INCLUDES = -I/Users/shen.94/Graphics/glew-2.1.0/include  -I/Users/shen.94/Graphics/glm  -I/Users/hwshen/Graphics/glew-2.1.0/include  -I/Users/hwshen/Graphics/glm \
#INCLUDES = -I/Users/hwshen/Graphics/glew-1.8.0/include  -I/Users/hwshen/Graphics/glm-0.8.2.2 \
#INCLUDES = -I/Users/hwshen/Graphics/glew-1.8.0/include  -I/Users/hwshen/Graphics/glm 

LIBS = -L/Users/shen.94/Graphics/glew-2.1.0/lib

LIBRARIES =  $(LIBS) -framework GLUT -framework OpenGL -lGLEW

OBJS = containers.o cube.o shaderSetup.o renderer.o 

SRCS = containers.C cube.C shaderSetup.C renderer.C

.C.o:
		$(CPP) -c $(CPPFLAGS) $(INCLUDES) $<
.c.o:
		$(CC) -c $(CPPFLAGS) $(INCLUDES) $<

default: all

all: cube cubeC

cube: cube.o shaderSetup.o 
	$(CPP) -o cube $(CPPFLAGS) cube.o shaderSetup.o \
       $(LDFLAGS) $(LIBRARIES)

cubeC: cube-container.o containers.o shaderSetup.o renderer.o 
	$(CPP) -o cubeC $(CPPFLAGS) cube-container.o renderer.o containers.o shaderSetup.o \
       $(LDFLAGS) $(LIBRARIES)


clean:
	rm *.o cube cubeC

