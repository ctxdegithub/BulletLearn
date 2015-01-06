#include "PhysicsDraw3D.h"

PhysicsDraw3D::PhysicsDraw3D()
{

}

PhysicsDraw3D::~PhysicsDraw3D()
{
	
}

PhysicsDraw3D* PhysicsDraw3D::create()
{
	auto draw = new PhysicsDraw3D;
	if (draw && draw->initDraw())
	{
		return draw;
	}

	return nullptr;
}

bool PhysicsDraw3D::initDraw()
{
	_shader = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_U_COLOR);
	if (_shader == nullptr)
	{
		return false;
	}
	_shader->retain();

	_colorLocation = _shader->getUniformLocation("u_color");
	CHECK_GL_ERROR_DEBUG();
	_pointSizeLocation = _shader->getUniformLocation("u_pointSize");
	CHECK_GL_ERROR_DEBUG();
	
	_debugDrawMode = btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE;
	return true;
}

void PhysicsDraw3D::destroy()
{
	CC_SAFE_RELEASE_NULL(_shader);
	delete this;
}

void PhysicsDraw3D::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
	Vec3 vertices[2] = {
		Vec3(from.x(), from.y(), from.z()),
		Vec3(to.x(), to.y(), to.z())
	};

	_color.r = color.x();
	_color.g = color.y();
	_color.b = color.z();

	_shader->use();
	_shader->setUniformsForBuiltins();
	_shader->setUniformLocationWith4fv(_colorLocation, (GLfloat*) &_color.r, 1);

	glEnable(GL_DEPTH_TEST);
	
	GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glDrawArrays(GL_LINES, 0, 2);

	glDisable(GL_DEPTH_TEST);
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,2);	
}

void PhysicsDraw3D::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{

}

void PhysicsDraw3D::reportErrorWarning(const char* warningString)
{

}

void PhysicsDraw3D::draw3dText(const btVector3& location,const char* textString)
{

}

void PhysicsDraw3D::setDebugMode(int debugMode)
{
	_debugDrawMode = debugMode;
}

int PhysicsDraw3D::getDebugMode() const
{
	return _debugDrawMode;
}


