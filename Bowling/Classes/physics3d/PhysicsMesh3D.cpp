#include "PhysicsMesh3D.h"
#include "PhysicsHelper3D.h"

PhysicsMesh3D* PhysicsMesh3D::constuct(const char* fileName)
{
	auto mesh = new PhysicsMesh3D;
	if (mesh && mesh->initWithFile(fileName))
	{
		return mesh;
	}

	delete mesh;
	return nullptr;
}

void PhysicsMesh3D::destroy()
{
	_verts.clear();
	_verIndices.clear();
	delete _indexVertexArrays;
	delete this;
}

bool PhysicsMesh3D::initWithFile(const char* fileName)
{
	_indexVertexArrays = nullptr;
	_verts.clear();
	_verIndices.clear();
	if (PhysicsHelper3D::loadRaw(fileName, _verts))
	{
		_verIndices.resize(_verts.size());
		for (int i=0; i < _verts.size(); ++i)
		{
			_verIndices[i] = i;
		}
		_indexVertexArrays = new btTriangleIndexVertexArray(_verts.size() / 9, &_verIndices[0], 3 * sizeof(int),
			_verts.size() / 3, (btScalar*)&_verts[0], 3 * sizeof(float));
		_meshShape = new btBvhTriangleMeshShape(_indexVertexArrays, true);

		return true;
	}

	return false;
}