#include "Texture2D.h"

#include "GLMgr.h"

#include "../external/FreeImage/FreeImage.h"

Texture2D::Texture2D():width(-1), height(-1), texID(0), valid(false){

}

Texture2D::~Texture2D(){
	if(valid){
		glDeleteTextures(1, &textureId);
		valid = true;
	}
}

Texture2D *Texture2D::createWithRawData(unsigned char *data, int width, int height, GLenum image_format){
	Texture2D *tex = new Texture2D();
	if(tex && tex->initWithRawData(data, width, height, image_format)){
		valid = true;
		tex->autorelease();
		return tex;
	}
	SAFE_DELETE(tex);
	return nullptr;
}

bool Texture2D::initWithRawData(unsigned char *data, int width, int height, GLenum image_format){
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID); // 此处暂时无所谓绑定到哪个纹理单元，因为绘制之前还会重新绑定
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    glTexSubImage2D(
        GL_TEXTURE_2D, 
        0, 
        0, 0, 
        width, 
        height, 
        image_format,
        GL_UNSIGNED_BYTE,
        data
    );
    return true;
}

void Texture2D::use(GLint index){
	GLMgr::getInstance()->bindTexture2D(this, index);
}

Texture2D *Texture2D::createWithPNG(const char *filename, GLenum image_format){
	Texture2D *tex = new Texture2D();
	if(tex && tex->initWithPNG(filename, image_format)){
		valid = true;
		tex->autorelease();
		return tex;
	}
	SAFE_DELETE(tex);
	return nullptr;
}

bool Texture2D::initWithPNG(const char *filename, GLenum image_format){

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *dib(0);
	BYTE* bits(0);
	unsigned int width(0), height(0);
	fif = FreeImage_GetFileType(filename, 0);
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(filename);
	if(fif == FIF_UNKNOWN)
		return false;
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	if(!dib)
		return false;

	bits = FreeImage_GetBits(dib);
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);

	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return false;

	bool ret = initWithRawData(bits, width, height, image_format);

	FreeImage_Unload(dib);

	return ret;
}