#include "Material.h"
#include "Director.h"
#include "../macro.h"

#include "../GLMgr/GLProgramSource.h"
#include "../GLMgr/Texture2D.h"
#include "../GLMgr/GLProgram.h"

Material::Material():
tex(nullptr),
program(nullptr),
specular(0, 0, 0),
diffuse(0, 0, 0),
ambient(0, 0, 0),
shininess(0)
{

}

Material::~Material(){
	SAFE_RELEASE(tex);
	SAFE_RELEASE(program);
}

void Material::setTexture2D(Texture2D *_tex){
	SAFE_RELEASE(tex);

	if(_tex){
		tex = _tex;
		tex->retain();
	}

}

void Material::setGLProgram(GLProgram *_program){
	SAFE_RELEASE(program);
	if(_program){
		program = _program;
		program->retain();
	}
}

void Material::use(){
	auto director = Director::getInstance();
    auto modelMatrix = director->getMatrix(MatrixType::kModel);
    auto viewMatrix = director->getMatrix(MatrixType::kView);
    auto projectMatrix = director->getMatrix(MatrixType::kProject);

	if(tex){
		tex->use(0);
	}

	program->use();

    program->setUniformMatrix4fv("model", modelMatrix.m);
    program->setUniformMatrix4fv("view", viewMatrix.m);
    program->setUniformMatrix4fv(
        "modelNormal", 
        modelMatrix.getInversed().getTransposed().m
    );
    program->setUniformMatrix4fv(
        "modelViewNormal", 
        (viewMatrix*modelMatrix).getInversed().getTransposed().m
    );
    program->setUniformMatrix4fv("project", projectMatrix.m);
}

void Material::useDefaultShader2(){
    const GLchar* default_vertexSource = GLSL(
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 project;
        uniform mat4 modelNormal;
        uniform mat4 modelViewNormal;
        in vec3 position;
        in vec2 uv;
        in vec4 color;
        in vec3 normal;
        out vec4 mColor;

        void main() {
            vec4 mNormal = modelNormal*vec4(normal, 1);
            vec4 mPosition = model * vec4(position, 1);

            vec3 lightPos = vec3(0, 0, 20000);

            gl_Position = project * view * model * vec4(position, 1.0f);

            vec3 l = vec3(0.8f, 0.8f, 0.8f);
            vec3 eye = vec3(300, 300, 300);
            vec3 diff = vec3(1, 0, 0);
            vec3 spec = vec3(0.6, 0.6, 0.6);

            vec3 _position = vec3(mPosition.x, mPosition.y, mPosition.z);

            vec3 inDir = lightPos - _position;
            inDir = normalize(inDir);

            vec3 _normal = normalize(vec3(mNormal.x, mNormal.y, mNormal.z));
            vec3 outDir = dot(_normal, inDir)*_normal * 2 - inDir;

            vec3 eyeDir = eye - _position;
            eyeDir = normalize(eyeDir);

            vec3 diffRet = dot(_normal, inDir) *l * diff;
            vec3 specRet = dot(_normal, inDir) *l * spec * pow(dot(eyeDir, outDir), 8);

            mColor = vec4(diffRet, 1) + vec4(specRet, 1) + vec4(0.1, 0.1, 0.1, 1)*color;
        }
    );

    const GLchar* default_fragmentSource = GLSL(
        uniform sampler2D Texture0;
        uniform sampler2D Texture1;
        uniform sampler2D Texture2;
        uniform sampler2D Texture3;
        in vec4 mColor;
        out vec4 oColor;
        void main() {
           
            oColor = mColor;
        }
    );

    auto shaderSource = GLProgramSource::createWithStrings(
        default_vertexSource, nullptr, nullptr, nullptr, default_fragmentSource
    );

    auto glProgram = GLProgram::createWithGLProgramSource(shaderSource);
    setGLProgram(glProgram);
}

void Material::useDefaultShader(){
    const GLchar* default_vertexSource = GLSL(
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 project;
        uniform mat4 modelNormal;
        uniform mat4 modelViewNormal;
        in vec3 position;
        in vec2 uv;
        in vec4 color;
        in vec3 normal;

        out vec4 mColor;
        out vec2 mUV;
        out vec4 mNormal;
        out vec4 mPosition;

        out vec3 lightPos;

        void main() {
            mColor = color;
            mUV = uv;
            mNormal = modelNormal*vec4(normal, 1);
            mPosition = model * vec4(position, 1);
            lightPos = vec3(1000, 200, 1000);

            gl_Position = project * view * model * vec4(position, 1.0f);
        }
    );

    const GLchar* default_fragmentSource = GLSL(
        uniform sampler2D Texture0;
        uniform sampler2D Texture1;
        uniform sampler2D Texture2;
        uniform sampler2D Texture3;
        in vec4 mColor;
        in vec2 mUV;
        in vec4 mNormal;
        in vec4 mPosition;
        in vec3 lightPos;

        out vec4 oColor;
        void main() {
            vec3 l = vec3(0.8f, 0.8f, 0.8f);
            vec3 eye = vec3(300, 300, 300);
            vec3 diff = vec3(1, 0.5, 0.7);
            vec3 spec = vec3(0.6, 0.6, 0.6);

            vec3 position = vec3(mPosition.x, mPosition.y, mPosition.z);

            vec3 inDir = lightPos - position;
            inDir = normalize(inDir);

            vec3 normal = normalize(vec3(mNormal.x, mNormal.y, mNormal.z));
            vec3 outDir = dot(normal, inDir)*normal * 2 - inDir;

            vec3 eyeDir = eye - position;
            eyeDir = normalize(eyeDir);

            vec3 diffRet = dot(normal, inDir) *l * diff;
            vec3 specRet = dot(normal, inDir) *l * spec * pow(dot(eyeDir, outDir), 8);

            oColor = vec4(diffRet, 1) + vec4(0.2, 0.2, 0.2, 1) + vec4(specRet, 1);
        }
    );

    auto shaderSource = GLProgramSource::createWithStrings(
        default_vertexSource, nullptr, nullptr, nullptr, default_fragmentSource
    );

    auto glProgram = GLProgram::createWithGLProgramSource(shaderSource);
   	setGLProgram(glProgram);
}


Material *Material::create(){
    Material *material = new (std::nothrow) Material();
    if(material){
        material->autorelease();
        return material;
    }
    SAFE_DELETE(material);
    return nullptr;
}