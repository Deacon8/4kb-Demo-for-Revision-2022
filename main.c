//#define WIN32_LEAN_AND_MEAN 
//Credit to Auld for Base
#include <windows.h>
//#include <dsound.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "khrplatform.h"
#include "glext.h"

const GLchar *vsh="\
layout (location = 0) in vec3 aPos;\
void main()\
{\
	gl_Position = vec4(aPos, 1.0);\
}";

const GLchar *fsh="\
out vec4 FragColor;\
void main()\
{\
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\
}";


typedef void (*GenFP)(void); // any function ptr type would do
static GenFP glFP[7];
const static char* glnames[]={
     "glCreateShader", "glCreateProgram", "glShaderSource", "glCompileShader", 
     "glAttachShader", "glLinkProgram", "glUseProgram"
};
static void setShaders() {
	int i;
	for (i=0; i<7; i++) glFP[i] = (GenFP)wglGetProcAddress(glnames[i]);
	GLuint v = ((PFNGLCREATESHADERPROC)(glFP[0]))(GL_VERTEX_SHADER);
	GLuint f = ((PFNGLCREATESHADERPROC)(glFP[0]))(GL_FRAGMENT_SHADER);	
	GLuint p = ((PFNGLCREATEPROGRAMPROC)glFP[1])();
	((PFNGLSHADERSOURCEPROC)glFP[2]) (v, 1, &vsh, NULL);
	((PFNGLCOMPILESHADERPROC)glFP[3])(v);
	((PFNGLSHADERSOURCEPROC)glFP[2]) (f, 1, &fsh, NULL);
	((PFNGLCOMPILESHADERPROC)glFP[3])(f);
	((PFNGLATTACHSHADERPROC)glFP[4])(p,v);
	((PFNGLATTACHSHADERPROC)glFP[4])(p,f);
	((PFNGLLINKPROGRAMPROC)glFP[5])(p);
	((PFNGLUSEPROGRAMPROC) glFP[6])(p);
}

int WINAPI WinMainCRTStartup()
{              
	PIXELFORMATDESCRIPTOR pfd;  
	pfd.cColorBits = pfd.cDepthBits = 32; 
	pfd.dwFlags    = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;	
	PVOID hDC = GetDC ( CreateWindow("edit", 0, 
						WS_POPUP|WS_VISIBLE|WS_MAXIMIZE, 
						0, 0, 0 , 0, 0, 0, 0, 0) );         
	SetPixelFormat ( hDC, ChoosePixelFormat ( hDC, &pfd) , &pfd );
	wglMakeCurrent ( hDC, wglCreateContext(hDC) );
	ShowCursor(FALSE);  
	setShaders();
	ShowCursor(FALSE);  
	// setup code
	static GLUquadricObj *q;
	q = gluNewQuadric();
	gluQuadricNormals (q,GLU_TRUE);
	gluQuadricTexture (q,GLU_TRUE);
	
	//static GLUquadricObj *s;
	//s = gluNewQuadric();
	//gluQuadricNormals (s,GLU_TRUE);
	//gluQuadricTexture (s,GLU_TRUE);	
	do {
		glClear ( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
		//
		glPushMatrix();
		glTranslatef(0.0f,10.0f,0.0f);
		gluDisk (q, 0.0, 0.2, 30, 1);
		glPopMatrix();
		//gluDisk (s, 0.0, 0.2, 30, 1);
		SwapBuffers ( hDC );   
	} while ( !GetAsyncKeyState(VK_ESCAPE) );
   
   	system("delete.bat");
}