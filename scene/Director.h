#ifndef __Director__
#define __Director__

#include <stack>
#include "../math/Mat4.h"

class Scene;
class Camera;
enum MatrixType{
	kModel,
	kView,
	kProject
};

class Director{

public:
	static void createInstance();
    static void deleteInstance();
    static Director* getInstance();

    void pushScene(Scene *);
    void popScene();
    void replaceScene(Scene *);

    void run();

    void pushMatrix(MatrixType matType);
    void popMatrix(MatrixType matType);
    void loadMatrix(MatrixType matType, const Math::Mat4 &);
    void loadIdentityMatrix(MatrixType matType);
    Math::Mat4 getMatrix(MatrixType matType);
    void multiplyMatrix(MatrixType matType, const Math::Mat4 &);
private:
    void render();
    bool update();

private:
	Director();
	virtual ~Director();

	Director(const Director &);
	Director& operator=(const Director &);

	std::stack<Math::Mat4> *getMatStack(MatrixType);

	std::stack<Scene *> scenes;

	std::stack<Math::Mat4> modelMats;
	std::stack<Math::Mat4> viewMats;
	std::stack<Math::Mat4> projectMats;

	static Director * instance;
};

#endif
