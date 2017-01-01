#include "Mesh.h"
#include "Material.h"
#include "../math/Vec3.h"
#include "../math/Mat4.h"
#include "../math/Quaternion.h"
#include "Node.h"

Mesh::Mesh():
material(nullptr),
vertexs(nullptr),
indices(nullptr),
blendVertexs(nullptr),
numOfIndices(0),
numOfVertex(0)
{

}

Mesh::~Mesh(){
	if(material){
		material->release();
		material = nullptr;
	}

	freeMemory();
}

void Mesh::setMaterial(Material *m){
	if(material){
		material->release();
		material = nullptr;
	}

	if(m){
		material = m;
		material->retain();
	}

}
void Mesh::attachTo(Node *n){
	node = n;

	calculateVertexPosInBones();
}

void Mesh::calculateBlendVertexs(){
	for(int i = 0; i < numOfVertex; i ++){
		Vertex * vertexPtr = &vertexs[i];
		auto nodeWeightGrp = weights[vertexPtr];

		Math::Vec3 pos(0, 0, 0);
		Math::Vec3 normal(0, 0, 0);

		std::for_each(nodeWeightGrp.begin(), nodeWeightGrp.end(), [&pos, &normal, this](NodeWeight &nodeWeight){
			Math::Vec3 localPos(nodeWeight.x, nodeWeight.y, nodeWeight.z);
			Math::Vec3 localNormal(nodeWeight.nx, nodeWeight.ny, nodeWeight.nz);
			auto mat4 = nodeWeight.node->getNodeToAncestorTransform(node);
			mat4.transformPoint(&localPos);
			auto normalMat4 = mat4.getInversed().getTransposed();
			normalMat4.transformVector(&localNormal);
			pos += localPos*nodeWeight.weight;
			normal += localNormal*nodeWeight.weight;

		});

		normal.normalize();

		memcpy(&blendVertexs[i], &vertexs[i], sizeof(Vertex));
		blendVertexs[i].x = pos.x;
		blendVertexs[i].y = pos.y;
		blendVertexs[i].z = pos.z;

		blendVertexs[i].nx = normal.x;
		blendVertexs[i].ny = normal.y;
		blendVertexs[i].nz = normal.z;
	}
}

bool Mesh::calculateVertexPosInBones(){
	std::for_each(weights.begin(), weights.end(), [this](std::map<Vertex *, std::vector<NodeWeight> >::reference item){
		auto vertexPtr = item.first;
		auto nodeWeightGrpPtr = &(item.second);

		std::for_each(nodeWeightGrpPtr->begin(), nodeWeightGrpPtr->end(), [this, &vertexPtr](NodeWeight &nodeWeight){
			if(!nodeWeight.node){
				nodeWeight.node = node->getNode(nodeWeight.nodeName);
			}

			Math::Mat4 transform = nodeWeight.node->getAncestorToNodeTransform(node);
            Math::Vec3 pos(vertexPtr->x, vertexPtr->y, vertexPtr->z);
			transform.transformPoint(&pos);
			nodeWeight.x = pos.x;
			nodeWeight.y = pos.y;
			nodeWeight.z = pos.z;

            Math::Mat4 normalTransform;
            normalTransform = transform.getInversed().getTransposed();
            Math::Vec3 normal(vertexPtr->nx, vertexPtr->ny, vertexPtr->nz);
			normalTransform.transformVector(&normal);
			normal.normalize();
			nodeWeight.nx = normal.x;
			nodeWeight.ny = normal.y;
			nodeWeight.nz = normal.z;

		});
	});
    return true;
}


Mesh *Mesh::create(){
	Mesh *mesh = new (std::nothrow) Mesh();
	if(mesh && mesh->init()){
		mesh->autorelease();
		return mesh;
	}
	SAFE_DELETE(mesh);
	return mesh;
}

Mesh *Mesh::createWithData(const char *data, unsigned int size){
	Mesh *mesh = new (std::nothrow) Mesh();
	if(mesh && mesh->initWithData(data, size)){
		mesh->autorelease();
		return mesh;
	}
	SAFE_DELETE(mesh);
	return mesh;
}

Mesh *Mesh::createCube(int size){
	Mesh *mesh = new (std::nothrow) Mesh();
	if(mesh && mesh->initWithCube(size)){
		mesh->autorelease();
		return mesh;
	}
	SAFE_DELETE(mesh);
	return mesh;	
}

bool Mesh::init(){
	return true;
}

bool Mesh::initWithData(const char *data, unsigned int len){
	return false;
}

bool Mesh::initWithCube(int size){
	if(!allocMemery(24, 36)){
		return false;
	}

	int index = 0;

	vertexs[index].x = 0;
	vertexs[index].y = 0;
	vertexs[index].z = 0;
	vertexs[index].r = 1;
	vertexs[index].g = 0;
	vertexs[index].b = 0;
	vertexs[index].a = 1;
	vertexs[index].nx = -1;
	vertexs[index].ny = -1;
	vertexs[index].nz = -1;
	vertexs[index].u = 0;
	vertexs[index].v = 0;

	index ++;

	vertexs[index].x = size;
	vertexs[index].y = 0;
	vertexs[index].z = 0;
	vertexs[index].r = 1;
	vertexs[index].g = 0;
	vertexs[index].b = 0;
	vertexs[index].a = 1;
	vertexs[index].nx = 1;
	vertexs[index].ny = -1;
	vertexs[index].nz = -1;
	vertexs[index].u = 0;
	vertexs[index].v = 0;

	index ++;

	vertexs[index].x = size;
	vertexs[index].y = size;
	vertexs[index].z = 0;
	vertexs[index].r = 1;
	vertexs[index].g = 0;
	vertexs[index].b = 0;
	vertexs[index].a = 1;
	vertexs[index].nx = 1;
	vertexs[index].ny = 1;
	vertexs[index].nz = -1;
	vertexs[index].u = 0;
	vertexs[index].v = 0;

	index ++;

	vertexs[index].x = 0;
	vertexs[index].y = size;
	vertexs[index].z = 0;
	vertexs[index].r = 1;
	vertexs[index].g = 0;
	vertexs[index].b = 0;
	vertexs[index].a = 1;
	vertexs[index].nx = -1;
	vertexs[index].ny = 1;
	vertexs[index].nz = -1;
	vertexs[index].u = 0;
	vertexs[index].v = 0;

	index ++;

	vertexs[index].x = 0;
	vertexs[index].y = 0;
	vertexs[index].z = size;
	vertexs[index].r = 1;
	vertexs[index].g = 0;
	vertexs[index].b = 0;
	vertexs[index].a = 1;
	vertexs[index].nx = -1;
	vertexs[index].ny = -1;
	vertexs[index].nz = 1;
	vertexs[index].u = 0;
	vertexs[index].v = 0;

	index ++;

	vertexs[index].x = size;
	vertexs[index].y = 0;
	vertexs[index].z = size;
	vertexs[index].r = 1;
	vertexs[index].g = 0;
	vertexs[index].b = 0;
	vertexs[index].a = 1;
	vertexs[index].nx = 1;
	vertexs[index].ny = -1;
	vertexs[index].nz = 1;
	vertexs[index].u = 0;
	vertexs[index].v = 0;

	index ++;

	vertexs[index].x = size;
	vertexs[index].y = size;
	vertexs[index].z = size;
	vertexs[index].r = 1;
	vertexs[index].g = 0;
	vertexs[index].b = 0;
	vertexs[index].a = 1;
	vertexs[index].nx = 1;
	vertexs[index].ny = 1;
	vertexs[index].nz = 1;
	vertexs[index].u = 0;
	vertexs[index].v = 0;

	index ++;

	vertexs[index].x = 0;
	vertexs[index].y = size;
	vertexs[index].z = size;
	vertexs[index].r = 1;
	vertexs[index].g = 0;
	vertexs[index].b = 0;
	vertexs[index].a = 1;
	vertexs[index].nx = -1;
	vertexs[index].ny = 1;
	vertexs[index].nz = 1;
	vertexs[index].u = 0;
	vertexs[index].v = 0;

	index ++;

	for(int i = 0; i < 8; i ++){
		vertexs[i].x -= size/2;
		vertexs[i].y -= size/2;
		vertexs[i].z -= size/2;
		memcpy(&vertexs[i+8], &vertexs[i], sizeof(Vertex));
		memcpy(&vertexs[i+16], &vertexs[i], sizeof(Vertex));

		vertexs[i].ny = 0;
		vertexs[i].nz = 0;

		vertexs[i+8].nx = 0;
		vertexs[i+8].nz = 0;

		vertexs[i+16].nx = 0;
		vertexs[i+16].ny = 0;
	}

	index = 0;

	indices[index ++] = 0 + 8;
	indices[index ++] = 1 + 8;
	indices[index ++] = 5 + 8;

	indices[index ++] = 5 + 8;
	indices[index ++] = 4 + 8;
	indices[index ++] = 0 + 8;

	indices[index ++] = 3 + 8;
	indices[index ++] = 7 + 8;
	indices[index ++] = 6 + 8;

	indices[index ++] = 3 + 8;
	indices[index ++] = 6 + 8;
	indices[index ++] = 2 + 8;

	indices[index ++] = 2;
	indices[index ++] = 6;
	indices[index ++] = 5;

	indices[index ++] = 2;
	indices[index ++] = 5;
	indices[index ++] = 1;

	indices[index ++] = 3;
	indices[index ++] = 0;
	indices[index ++] = 4;

	indices[index ++] = 3;
	indices[index ++] = 4;
	indices[index ++] = 7;

	indices[index ++] = 6 + 16;
	indices[index ++] = 7 + 16;
	indices[index ++] = 4 + 16;

	indices[index ++] = 6 + 16;
	indices[index ++] = 4 + 16;
	indices[index ++] = 5 + 16;

	indices[index ++] = 2 + 16;
	indices[index ++] = 0 + 16;
	indices[index ++] = 3 + 16;

	indices[index ++] = 1 + 16;
	indices[index ++] = 0 + 16;
	indices[index ++] = 2 + 16;

	NodeWeight nodeWeight;
	nodeWeight.weight = 1;
	nodeWeight.node = nullptr;
	nodeWeight.nodeName = std::string(".");

	std::vector<NodeWeight> nodeWeightGrp;
	nodeWeightGrp.push_back(nodeWeight);

	for(int i = 0; i < 24; i ++){
		weights[&vertexs[i]] = nodeWeightGrp;
	}

	return true;
}

bool Mesh::allocMemery(int nv, int ni){

	numOfVertex = nv;
	numOfIndices = ni;

	vertexs = new (std::nothrow) Vertex[nv];
	blendVertexs = new (std::nothrow) Vertex[nv];
	indices = new (std::nothrow) GLushort[ni];
	return vertexs != nullptr && indices != nullptr;
}

void Mesh::freeMemory(){
	if(blendVertexs){
		delete []blendVertexs;
	}
	if(vertexs){
		delete []vertexs;
	}
	if(indices){
		delete []indices;
	}
}
