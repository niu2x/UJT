#include "Texture2D.h"
#include "Texture2DManager.h"

Texture2DManager::Texture2DManager(){

}

Texture2DManager::~Texture2DManager(){
	for(auto iter = textures.begin(); iter != textures.end(); iter++){
		Texture2D *tex = (*iter).second;
		tex->release();
	}
}

Texture2DManager *Texture2DManager::instance = nullptr;

void Texture2DManager::createInstance(){
	if(instance == nullptr){
		instance = new Texture2DManager();
	}
}

void Texture2DManager::deleteInstance(){
	if(instance != nullptr){
		delete instance;
		instance = 0;
	}
}

Texture2DManager *Texture2DManager::getInstance(){
	return instance;
}

Texture2D* Texture2DManager::getTexture2D(const char *filename){
	std::string key(filename);
	if(textures.find(key) == textures.end()){
		Texture2D *tex = Texture2D::createWithFile(filename);
		if(tex){
			textures[key] = tex;
			tex->retain();
			return tex;
		}
		else{
			return nullptr;
		}
	}
	return textures[key];
}

void Texture2DManager::releaseUnusedTexture2D(){
	for(auto iter = textures.begin(); iter != textures.end();){
		Texture2D *tex = (*iter).second;
		if(tex->getReferenceCount() == 1){
			tex->release();
			textures.erase(iter ++);
		}
		else{
			iter ++;
		}
	}
}
