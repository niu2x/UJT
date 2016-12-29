#ifndef __texture2D__manager__
#define __texture2D__manager__

#include <map>
#include <string>

class Texture2D;

class Texture2DManager{
public:
	static void createInstance();
    static void deleteInstance();
    static Texture2DManager* getInstance();

    Texture2D* getTexture2D(const char *filename);
    void releaseUnusedTexture2D();

    virtual ~Texture2DManager();

private:

	Texture2DManager();
	Texture2DManager(const Texture2DManager &);
	Texture2DManager& operator=(const Texture2DManager &);

	std::map<std::string, Texture2D *> textures;

	static Texture2DManager *instance;
};

#endif