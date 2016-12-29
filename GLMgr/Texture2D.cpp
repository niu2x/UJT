#include "Texture2D.h"

#include "GLMgr.h"

#include <FreeImage.h>
#include "macro.h"

Texture2D::Texture2D():width(-1), height(-1), texID(0), valid(false){

}

Texture2D::~Texture2D(){
	if(valid){
		glDeleteTextures(1, &texID);
		valid = true;
	}
}

Texture2D *Texture2D::createWithRawData(unsigned char *data, int width, int height, GLenum imageFormat){
	Texture2D *tex = new Texture2D();
	if(tex && tex->initWithRawData(data, width, height, imageFormat)){
		tex->autorelease();
		return tex;
	}
	SAFE_DELETE(tex);
	return nullptr;
}

bool Texture2D::initWithRawData(unsigned char *data, int width, int height, GLenum imageFormat){
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID); // 此处暂时无所谓绑定到哪个纹理单元，因为绘制之前还会重新绑定
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    glTexSubImage2D(
        GL_TEXTURE_2D, 
        0, 
        0, 0, 
        width, 
        height, 
        imageFormat,
        GL_UNSIGNED_BYTE,
        data
    );
    
    valid = true;
    return true;
}

void Texture2D::use(GLint index){
	GLMgr::getInstance()->bindTexture2D(this, index);
}

Texture2D *Texture2D::createWithFile(const char *filename){
	Texture2D *tex = new Texture2D();
	if(tex && tex->initWithFile(filename)){
		tex->autorelease();
		return tex;
	}
	SAFE_DELETE(tex);
	return nullptr;
}

bool Texture2D::initWithFile(const char *filename){

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

	int imageType = FreeImage_GetImageType(dib);
	unsigned int bpp = FreeImage_GetBPP(dib);
	GLenum imageFormat;

	switch(bpp){
		case 32:
			imageFormat = GL_BGRA;
			break;
		case 24:
			imageFormat = GL_BGR;
			break;
		default:
			printf("texture2D: error bpp: %d\n", bpp);
			return false;
	}

	printf("colorUsed %d\n", FreeImage_GetColorType(dib));

	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return false;

	bool ret = initWithRawData(bits, width, height, imageFormat);

	FreeImage_Unload(dib);

	return ret;
}
