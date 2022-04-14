//#define WIN32_LEAN_AND_MEAN 
//Credit to Auld for Base
#include <windows.h>
//#include <dsound.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
//#include <GL/glu.h>
#include "khrplatform.h"
#include "glext.h"
//#include "Test1.h"
#include "shader_code.h" 

typedef void (*GenFP)(void); // any function ptr type would do
static GenFP glFP[22];
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

const GLuint SH_WIDTH = 1280;
const GLuint SH_HEIGHT = 720; 
const GLuint SH_DEPTH = 3;//Extra Layer for Rendering Scene

const static char* glnames[]={
     "glGenTextures", "glActiveTexture", "glBindTexture", "glTexParameteri", 
     "glTexImage3D", "glBindImageTexture", "glCreateShader", "glShaderSource", 
	 "glCompileShader", "glCreateProgram", "glAttachShader", "glLinkProgram",
	 "glGenFramebuffers", "glUseProgram", 
	 "glBindFramebuffer", "glFramebufferTexture3D", 
	 "glDispatchCompute", "glMemoryBarrier", "glBlitFramebuffer",
	 "glUniform1f", "glUniform1i", "glGetUniformLocation"
};

static 	PIXELFORMATDESCRIPTOR pfd; 
static DEVMODE dmScreenSettings;

int WINAPI WinMainCRTStartup()
{   

	dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
	dmScreenSettings.dmPelsWidth	= 1280;
	dmScreenSettings.dmPelsHeight= 720;
	dmScreenSettings.dmFields=DM_PELSWIDTH|DM_PELSHEIGHT;
	ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN); 
 
	pfd.cColorBits = pfd.cDepthBits = 32; 
	pfd.dwFlags    = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;	
	PVOID hDC = GetDC ( CreateWindow("edit", 0, WS_POPUP|WS_VISIBLE|WS_MAXIMIZE, 0, 0, 0 , 0, 0, 0, 0, 0) );         
	SetPixelFormat ( hDC, ChoosePixelFormat ( hDC, &pfd) , &pfd );
	wglMakeCurrent ( hDC, wglCreateContext(hDC) );
	ShowCursor(FALSE);
	
	GLuint FBO;
	GLuint tex_output;
	int i;
	for (i=0; i<22; i++) glFP[i] = (GenFP)wglGetProcAddress(glnames[i]);
	glGenTextures(1, &tex_output);
	((PFNGLACTIVETEXTUREPROC)glFP[1])(GL_TEXTURE0);
	//((PFNGLBINDTEXTURESPROC)glFP[2])(GL_TEXTURE_3D, tex_output);
	glBindTexture(GL_TEXTURE_3D, tex_output);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	((PFNGLTEXIMAGE3DPROC)glFP[4])(GL_TEXTURE_3D, 0, GL_RGBA32F, SH_WIDTH, SH_HEIGHT, SH_DEPTH, 0, GL_RGBA, GL_FLOAT, NULL);
	((PFNGLBINDIMAGETEXTUREPROC)glFP[5])(0, tex_output, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F);
	GLuint ray_shader = ((PFNGLCREATESHADERPROC)(glFP[6])) (GL_COMPUTE_SHADER);
	((PFNGLSHADERSOURCEPROC)glFP[7]) (ray_shader, 1, &shader1_cs, NULL);
	((PFNGLCOMPILESHADERPROC)glFP[8]) (ray_shader);
	GLuint ray_program = ((PFNGLCREATEPROGRAMPROC)(glFP[9])) ();
	((PFNGLATTACHSHADERPROC)glFP[10]) (ray_program, ray_shader);
	((PFNGLLINKPROGRAMPROC)glFP[11]) (ray_program);
	((PFNGLGENFRAMEBUFFERSPROC)glFP[12]) (1, &FBO);
	((PFNGLUSEPROGRAMPROC)glFP[13]) (ray_program);
	
	
	PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)glFP[14];
	PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)glFP[15];
	PFNGLDISPATCHCOMPUTEPROC glDispatchCompute = (PFNGLDISPATCHCOMPUTEPROC)glFP[16];
	PFNGLMEMORYBARRIERPROC glMemoryBarrier = (PFNGLMEMORYBARRIERPROC)glFP[17];
	PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)glFP[18];
	PFNGLUNIFORM1FPROC glUniform1f = (PFNGLUNIFORM1FPROC)glFP[19];
	PFNGLUNIFORM1IPROC glUniform1i = (PFNGLUNIFORM1IPROC)glFP[20];
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glFP[21];
	float time = 0;
	int stage = 0;
	do {	
		if(time < 10)
		{
			glClearColor(1, 1, 1, 1);
			glClear ( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
		}
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, tex_output, 0, 1);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glDispatchCompute(SH_WIDTH, SH_HEIGHT, SH_DEPTH);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		glBlitFramebuffer(0, 0, SH_WIDTH, SH_HEIGHT, 0, 0, SH_WIDTH, SH_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		time+=1.;
		glUniform1f(glGetUniformLocation(ray_program, VAR_TIME), time);
		stage++; if(stage > 1){glUniform1i(glGetUniformLocation(ray_program, VAR_STAGE), 1); stage=0;}
		SwapBuffers ( hDC );   
	} while ( !GetAsyncKeyState(VK_ESCAPE) );
   
   	system("delete.bat");
}