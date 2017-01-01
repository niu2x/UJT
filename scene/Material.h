#ifndef ___Material___
#define ___Material___

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../refCount/Ref.h"

class Texture2D;
class GLProgram;

class Material:public Ref{
public:
	typedef struct Color{
		GLfloat r;
		GLfloat g;
		GLfloat b;

		Color(GLfloat _r, GLfloat _g, GLfloat _b){
			r = _r;
			g = _g;
			b = _b;
		}
	}Color;

	Color ambient;
	Color diffuse;
	Color specular;
	GLfloat shininess;

	static Material *create();

	Material::Color getAmbient()const{
		return ambient;
	}
	Material::Color getDiffuse()const{
		return diffuse;
	}
	Material::Color getSpecular()const{
		return specular;
	}
	GLfloat getShininess()const{
		return shininess;
	}
	Texture2D *getTexture2D()const{
		return tex;
	}

	GLProgram *getGLProgram()const{
		return program;
	}

	void setAmbient(const Material::Color &c){
		ambient = c;
	}
	void setDiffuse(const Material::Color &c){
		diffuse = c;
	}
	void setSpecular(const Material::Color &c){
		specular = c;
	}
	void setShininess(GLfloat __shininess){
		shininess = __shininess;
	}
	void setTexture2D(Texture2D *);
	void setGLProgram(GLProgram *program);

	void use();

	void useDefaultShader();
	void useDefaultShader2();
	
	virtual ~Material();

protected:
	Texture2D *tex;
	GLProgram *program;

	Material();
	Material(const Material &);
	Material& operator=(const Material &);
};

#endif
