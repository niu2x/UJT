#ifndef __mesh___
#define __mesh___

#include "../refCount/Ref.h"
#include "../GLMgr/Vertex.h"

class Node;
class Material;

class Mesh:public Ref{
public:
	Mesh *create();
	Mesh *createWithData(const char *data, unsigned int size);
	Mesh *createCube();

	void setMaterial(Material *);
	Material *getMaterial();
	void attachTo(Node *);
	
private:
	typedef struct{
		Node *node;
		union {
			struct{std::string nodeName;};
			struct{int nodeIndex;};
		};
		float weight;
	}NodeWeight;

	Mesh();
	Mesh(const Mesh &);
	Mesh& operator=(const Mesh &);
	virtual ~Mesh();

	std::vector<Vertex> vertexs;
	std::map<Vertex *, std::vector<NodeWeight> > weights;

	Material *material;
};

#endif