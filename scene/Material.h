#ifndef ___Material___
#define ___Material___

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../refCount/Ref.h"

class Texture2D;

class Material:public Ref{
public:
	typedef {
		GLfloat r;
		GLfloat g;
		GLfloat b;
	}Color;

	Color ambient;
	Color diffuse;
	Color specular;
	GLfloat shininess;

	Texture2D *tex;

	Material *create();

	Material::Color getAmbient()const;
	Material::Color getDiffuse()const;
	Material::Color getSpecular()const;
	GLfloat getShininess()const;
	Texture2D *getTexture2D()const;

	void setAmbient(const Material::Color &);
	void setDiffuse(const Material::Color &);
	void setSpecular(const Material::Color &);
	void setShininess(GLfloat);
	void setTexture2D(Texture2D *);
	
private:

	Material();
	Material(const Material &);
	Material& operator=(const Material &);
	virtual ~Material();
};

#endif