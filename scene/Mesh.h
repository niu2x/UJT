#ifndef __mesh___
#define __mesh___

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../refCount/Ref.h"
#include "../GLMgr/Vertex.h"

#include <map>
#include <string>
#include <vector>

class Node;
class Material;

class Mesh:public Ref{
public:
	static Mesh *create();
	static Mesh *createWithData(const char *data, unsigned int size);
	static Mesh *createCube(int size);

	void setMaterial(Material *);
	Material *getMaterial(){
		return material;
	}

	void attachTo(Node *);

	void calculateBlendVertexs();

	Vertex *getVertexData(){
		return blendVertexs;
	}

	int getVertexNum(){
		return numOfVertex;
	}

	GLushort *getIndicesData(){
		return indices;
	}

	int getIndicesNum(){
		return numOfIndices;
	}
	
	virtual ~Mesh();

	typedef struct NodeWeight{
		std::string nodeName;
		Node *node;
		float x, y, z;
		float nx, ny, nz;
		float weight;
	}NodeWeight;

protected:
	

	Mesh();
	Mesh(const Mesh &);
	Mesh& operator=(const Mesh &);

	bool init();
	bool initWithData(const char *, unsigned int);
	bool initWithCube(int);

	bool allocMemery(int, int);
	void freeMemory();

	bool calculateVertexPosInBones();

	Vertex *vertexs;
	Vertex *blendVertexs;

	GLushort *indices;

	std::map<Vertex *, std::vector<NodeWeight> > weights;

	Material *material;

	Node *node;

	int numOfIndices;
	int numOfVertex;
};

#endif
