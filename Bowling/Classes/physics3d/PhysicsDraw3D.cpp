#include "PhysicsDraw3D.h"

static bool s_initialized = false;
static GLProgram* s_shader = nullptr;
static int s_colorLocation = -1;
static Color4F s_color(1.0f,1.0f,1.0f,1.0f);
static int s_pointSizeLocation = -1;
static GLfloat s_pointSize = 1.0f;

static void lazy_init()
{
	if( ! s_initialized ) {

		//
		// Position and 1 color passed as a uniform (to simulate glColor4ub )
		//
		s_shader = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_U_COLOR);
		s_shader->retain();

		s_colorLocation = s_shader->getUniformLocation("u_color");
		CHECK_GL_ERROR_DEBUG();
		s_pointSizeLocation = s_shader->getUniformLocation("u_pointSize");
		CHECK_GL_ERROR_DEBUG();
		s_color.g = s_color.b = 0;
		s_initialized = true;
	}
}

void free()
{
	CC_SAFE_RELEASE_NULL(s_shader);
	s_initialized = false;
}

void PhysicsDraw3D::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
	lazy_init();

	Vec3 vertices[2] = {
		Vec3(from.x(), from.y(), from.z()),
		Vec3(to.x(), to.y(), to.z())
	};

	s_shader->use();
	s_shader->setUniformsForBuiltins();
	s_shader->setUniformLocationWith4fv(s_colorLocation, (GLfloat*) &s_color.r, 1);
	glEnable(GL_DEPTH_TEST);
	GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );
#ifdef EMSCRIPTEN
	setGLBufferData(vertices, 16);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);
#else
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, vertices);
#endif // EMSCRIPTEN
	glDrawArrays(GL_LINES, 0, 2);

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