#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../macro.h"
#include "GLProgram.h"
#include "GLProgramSource.h"
#include "Vertex.h"

GLProgram::GLProgram():source(nullptr), programID(0)
{

}

GLProgram::~GLProgram(){
	if(source){
		source->release();
		glDeleteProgram(programID);
	}
}

GLProgram *GLProgram::createWithGLProgramSource(GLProgramSource *_source){
	GLProgram *program = new GLProgram();
	if(program && program->initWithGLProgramSource(_source)){
		program->autorelease();
		return program;
	}
	SAFE_DELETE(program);
	return nullptr;
}

bool GLProgram::initWithGLProgramSource(GLProgramSource *_source){
	if(source){
		source->release();
		source = nullptr;
		glDeleteProgram(programID);
	}

	source = _source;
	source->retain();

	return createProgram();
}

bool GLProgram::createProgram(){
	programID = glCreateProgram();
    GLuint shaderID[5];
    bool shaderStatus[5] = {false, false, false, false, false};

    bool success = true;

    if(success && source->getVertexSource()){
    	success = createShader(
    		GL_VERTEX_SHADER, 
    		source->getVertexSource(), 
    		&shaderID[0]
    	);
    	if(success){
        	glAttachShader(programID, shaderID[0]);
        }
        shaderStatus[0] = success;
    }

    if(success && source->getTessCtrlSource()){
    	success = createShader(
    		GL_TESS_CONTROL_SHADER, 
    		source->getTessCtrlSource(), 
    		&shaderID[1]
    	);
    	if(success){
        	glAttachShader(programID, shaderID[1]);
        }
        shaderStatus[1] = success;
    }

    if(success && source->getTessEvalSource()){
        success = createShader(
    		GL_TESS_EVALUATION_SHADER, 
    		source->getTessEvalSource(), 
    		&shaderID[2]
    	);
    	if(success){
        	glAttachShader(programID, shaderID[2]);
        }
        shaderStatus[2] = success;
    }

    if(success && source->getGeometrySource()){
        success = createShader(
    		GL_GEOMETRY_SHADER, 
    		source->getGeometrySource(), 
    		&shaderID[3]
    	);
    	if(success){
        	glAttachShader(programID, shaderID[3]);
        }
        shaderStatus[3] = success;
    }

    if(success && source->getFragmentSource()){
        success = createShader(
    		GL_FRAGMENT_SHADER, 
    		source->getFragmentSource(), 
    		&shaderID[4]
    	);
    	if(success){
        	glAttachShader(programID, shaderID[4]);
        }
        shaderStatus[4] = success;
    }

    if(success){
    	glLinkProgram(programID);
    }


    if(shaderStatus[0] && source->getVertexSource()){
    	glDeleteShader(shaderID[0]);
    }

    if(shaderStatus[1] && source->getTessCtrlSource()){
    	glDeleteShader(shaderID[1]);
    }

    if(shaderStatus[2] && source->getTessEvalSource()){
    	glDeleteShader(shaderID[2]);
    }

    if(shaderStatus[3] && source->getGeometrySource()){
    	glDeleteShader(shaderID[3]);
    }

    if(shaderStatus[4] && source->getFragmentSource()){
    	glDeleteShader(shaderID[4]);
    }

    if(success){
        GLint value;
        glGetProgramiv(programID, GL_LINK_STATUS, &value);
    	success = GL_TRUE == value;
    	if(!success){
    		char logs[1024];
	    	glGetProgramInfoLog(programID, 1023, 0, logs);
	    	puts("GLProgram 链接失败");
	    	puts(logs);

	    	glDeleteProgram(programID);
	    	source->release();
	    	source = nullptr;
    	}
    }

    return success;
}


bool GLProgram::createShader(GLenum type, const GLchar* src, GLuint *shader)
{
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &src, nullptr);
    glCompileShader(*shader);

    GLint value;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &value);
    bool success = GL_TRUE == value;
    if(!success){

    	char logs[1024];
    	glGetShaderInfoLog(*shader, 1023, 0, logs);
    	puts("shader 编译失败");
    	puts(logs);

    	glDeleteShader(*shader);
    }

    return success;
}

void GLProgram::use(){
	glUseProgram(programID);       
    initParams();
}

void GLProgram::initParams(){
	GLint posAttrib = glGetAttribLocation(programID, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    GLint uvAttrib = glGetAttribLocation(programID, "uv");
    glEnableVertexAttribArray(uvAttrib);
    glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);

    GLint colorAttrib = glGetAttribLocation(programID, "color");
    glEnableVertexAttribArray(colorAttrib);
    glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)20);

    GLint tex0Location = glGetUniformLocation(programID, "Texture0");
    GLint tex1Location = glGetUniformLocation(programID, "Texture1");
    GLint tex2Location = glGetUniformLocation(programID, "Texture2");
    GLint tex3Location = glGetUniformLocation(programID, "Texture3");

    GLint viewLocation = glGetUniformLocation(programID, "view");
    GLint projectLocation = glGetUniformLocation(programID, "project");

    localtions[std::string("view")] = viewLocation;
    localtions[std::string("project")] = projectLocation;

    if(tex0Location != -1){
    	glUniform1i(tex0Location, 0);
    }

    if(tex1Location != -1){	
    	glUniform1i(tex1Location, 1);
    }

    if(tex2Location != -1){	
    	glUniform1i(tex2Location, 2);
    }

    if(tex3Location != -1){
    	glUniform1i(tex3Location, 3);
    }
}

void GLProgram::setUniformMatrix4fv(std::string name, void *ptr){
	GLint location = localtions[name];
	glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat *)ptr);
}
