
CPP = g++ 

# Uncomment to turn on debugging:
CPPFLAGS = -g -DDEBUG

INCLUDES = -I/Users/shen.94/Graphics/glew-2.1.0/include  -I/Users/shen.94/Graphics/glm  -I/Users/hwshen/Graphics/glew-2.1.0/include  -I/Users/hwshen/Graphics/glm \
#INCLUDES = -I/Users/hwshen/Graphics/glew-1.8.0/include  -I/Users/hwshen/Graphics/glm-0.8.2.2 \
#INCLUDES = -I/Users/hwshen/Graphics/glew-1.8.0/include  -I/Users/hwshen/Graphics/glm 

LIBS = -L/Users/shen.94/Graphics/glew-2.1.0/lib

LIBRARIES =  $(LIBS) -framework GLUT -framework OpenGL -lGLEW


default: all

all: cube 

.C.o:
	$(CPP) -c $(CPPFLAGS) $(INCLUDES) $<


cube: cube.o shaderSetup.o 
	$(CPP) -o cube $(CPPFLAGS) cube.o shaderSetup.o \
        $(LDFLAGS) $(LIBRARIES)


clean:
	rm *.o cube

