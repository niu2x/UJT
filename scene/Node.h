#ifndef __node___
#define __node___

#include "macro.h"

#include "../refCount/Ref.h"
#include "../math/Vec3.h"
#include "../math/Quaternion.h"

#include <string>
#include <vector>

class GLProgram;

class Mesh;
class Scene;

class Node:public Ref{
public:
	static Node* create();

	void setPosition(const Math::Vec3&);
	Math::Vec3 getPosition()const{
		return position;
	}

	void setScale(const Math::Vec3 &);
	Math::Vec3 getScale()const{
		return scale;
	}

	void setRotation(const Math::Vec3 &); //欧拉角 x y z
	Math::Vec3 getRotation()const{
		return rotation;
	}

	void setQuaternion(const Math::Vec3 &axis, float angle);
	void setQuaternion(const Math::Quaternion &);

	Math::Quaternion getQuaternion()const;

	void setVisible(bool bVisible);
	bool isVisible()const{
		return visible;
	}

	virtual void visit();
	virtual void visit(const Math::Mat4 &parentModelMatrix);
	virtual void draw();

	virtual void addChild(Node *);
	bool hasChild(Node *);
	Node *getChildByName(const std::string &name);
	void removeChild(Node *);
	void removeAllChildren();
	Node *getParent()const{return parent;}
	Node *getNode(const std::string &path);

	virtual void setName(const std::string &);
	std::string getName()const;

	// void setGLProgram(GLProgram *);
	// GLProgram *getGLProgram(); 这东西应该在material身上

	Scene *getScene();
	void setScene(Scene *);

	void setMesh(Mesh *);
	Mesh *getMesh();

	virtual void onEnterScene();
	virtual void __onEnterScene();
	virtual void onExitScene();
	virtual void __onExitScene();

	bool isRunning(){
		return running;
	} //处在RunningScene中

	Math::Mat4 getNodeToParentTransform()const;
	Math::Mat4 getParentToNodeTransform()const;
	Math::Mat4 getNodeToWorldTransform()const;
	Math::Mat4 getWorldToNodeTransform()const;
	Math::Mat4 getNodeToAncestorTransform(Node *)const;
	Math::Mat4 getAncestorToNodeTransform(Node *)const;

	virtual ~Node();

protected:

	Node();
	Node(const Node &);
	Node& operator=(const Node &);

	virtual bool init();

	void __addChild(Node *);
	void __removeChild(Node *);

	Math::Vec3 position;
	Math::Vec3 rotation;
	Math::Quaternion quat;
	Math::Vec3 scale;

	bool visible;

	std::vector<Node *> children;
	Node *parent;

	Scene *scene;

	Mesh *mesh;

	bool running;

	std::string name;
};


#endif
