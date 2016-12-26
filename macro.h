#ifndef __macro
#define __macro

#define SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0)
#define SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)


#define CHECK_GL_ERROR_DEBUG() \
	do { \
	    GLenum __error = glGetError(); \
	    if(__error) { \
	        printf("OpenGL error 0x%04X in %s %s %d ％s\n", __error, __FILE__, __FUNCTION__, __LINE__, gluErrorString(__error)); \
	    } \
	} while (false)

#endif