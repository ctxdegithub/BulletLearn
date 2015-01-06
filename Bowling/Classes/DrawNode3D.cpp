#include "DrawNode3D.h"

DrawNode3D::DrawNode3D()
	: _vao(0)
	, _vbo(0)
	, _bufferCapacity(0)
	, _bufferCount(0)
	, _buffer(nullptr)
	, _dirty(false)
	, _vaoGLPoint(0)
	, _vboGLPoint(0)
	, _bufferCapacityGLPoint(0)
	, _bufferCountGLPoint(0)
	, _bufferGLPoint(nullptr)
	, _pointColor(1,1,1,1)
	, _pointSize(1)
	, _dirtyGLPoint(false)
	, _vaoGLLine(0)
	, _vboGLLine(0)
	, _bufferCapacityGLLine(0)
	, _bufferCountGLLine(0)
	, _bufferGLLine(nullptr)
	, _dirtyGLLine(false)
{
	_blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
}

DrawNode3D::~DrawNode3D()
{
	free(_buffer);
	_buffer = nullptr;
	free(_bufferGLPoint);
	_bufferGLPoint = nullptr;
	free(_bufferGLLine);
	_bufferGLLine = nullptr;

	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_vboGLLine);
	glDeleteBuffers(1, &_vboGLPoint);
	_vbo = 0;
	_vboGLPoint = 0;
	_vboGLLine = 0;

	if (Configuration::getInstance()->supportsShareableVAO())
	{
		glDeleteVertexArrays(1, &_vao);
		glDeleteVertexArrays(1, &_vaoGLLine);
		glDeleteVertexArrays(1, &_vaoGLPoint);
		GL::bindVAO(0);
		_vao = 0;
	}
}

DrawNode3D* DrawNode3D::create()
{
	DrawNode3D* ret = new (std::nothrow) DrawNode3D();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

void DrawNode3D::ensureCapacity(int count)
{
	CCASSERT(count>=0, "capacity must be >= 0");

	if(_bufferCount + count > _bufferCapacity)
	{
		_bufferCapacity += MAX(_bufferCapacity, count);
		_buffer = (V3F_C4B_T2F*)realloc(_buffer, _bufferCapacity*sizeof(V3F_C4B_T2F));
	}
}

void DrawNode3D::ensureCapacityGLPoint(int count)
{
	CCASSERT(count>=0, "capacity must be >= 0");

	if(_bufferCountGLPoint + count > _bufferCapacityGLPoint)
	{
		_bufferCapacityGLPoint += MAX(_bufferCapacityGLPoint, count);
		_bufferGLPoint = (V3F_C4B_T2F*)realloc(_bufferGLPoint, _bufferCapacityGLPoint*sizeof(V3F_C4B_T2F));
	}
}

void DrawNode3D::ensureCapacityGLLine(int count)
{
	CCASSERT(count>=0, "capacity must be >= 0");

	if(_bufferCountGLLine + count > _bufferCapacityGLLine)
	{
		_bufferCapacityGLLine += MAX(_bufferCapacityGLLine, count);
		_bufferGLLine = (V3F_C4B_T2F*)realloc(_bufferGLLine, _bufferCapacityGLLine*sizeof(V3F_C4B_T2F));
	}
}

bool DrawNode3D::init()
{
	_blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;

	setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR));

	ensureCapacity(512);
	ensureCapacityGLPoint(64);
	ensureCapacityGLLine(256);

	if (Configuration::getInstance()->supportsShareableVAO())
	{
		glGenVertexArrays(1, &_vao);
		GL::bindVAO(_vao);
	}
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(V3F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);
	// vertex
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, vertices));
	// color
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, colors));
	// texcood
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, texCoords));

	if (Configuration::getInstance()->supportsShareableVAO())
	{
		glGenVertexArrays(1, &_vaoGLLine);
		GL::bindVAO(_vaoGLLine);
	}
	glGenBuffers(1, &_vboGLLine);
	glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
	glBufferData(GL_ARRAY_BUFFER, sizeof(V3F_C4B_T2F)*_bufferCapacityGLLine, _bufferGLLine, GL_STREAM_DRAW);
	// vertex
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, vertices));
	// color
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, colors));
	// texcood
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, texCoords));

	if (Configuration::getInstance()->supportsShareableVAO())
	{
		glGenVertexArrays(1, &_vaoGLPoint);
		GL::bindVAO(_vaoGLPoint);
	}
	glGenBuffers(1, &_vboGLPoint);
	glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
	glBufferData(GL_ARRAY_BUFFER, sizeof(V3F_C4B_T2F)*_bufferCapacityGLPoint, _bufferGLPoint, GL_STREAM_DRAW);
	// vertex
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, vertices));
	// color
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, colors));
	// texcood
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, texCoords));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (Configuration::getInstance()->supportsShareableVAO())
	{
		GL::bindVAO(0);
	}

	CHECK_GL_ERROR_DEBUG();

	_dirty = true;
	_dirtyGLLine = true;
	_dirtyGLPoint = true;

#if CC_ENABLE_CACHE_TEXTURE_DATA
	// Need to listen the event only when not use batchnode, because it will use VBO
	auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
		/** listen the event that renderer was recreated on Android/WP8 */
		this->init();
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif

	return true;
}

void DrawNode3D::clear()
{
	_bufferCount = 0;
	_dirty = true;
	_bufferCountGLLine = 0;
	_dirtyGLLine = true;
	_bufferCountGLPoint = 0;
	_dirtyGLPoint = true;
}

void DrawNode3D::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	if(_bufferCount)
	{
		_customCommand.init(_globalZOrder);
		_customCommand.func = CC_CALLBACK_0(DrawNode3D::onDraw, this, transform, flags);
		renderer->addCommand(&_customCommand);
	}

	if(_bufferCountGLPoint)
	{
		_customCommandGLPoint.init(_globalZOrder);
		_customCommandGLPoint.func = CC_CALLBACK_0(DrawNode3D::onDrawGLPoint, this, transform, flags);
		renderer->addCommand(&_customCommandGLPoint);
	}

	if(_bufferCountGLLine)
	{
		_customCommandGLLine.init(_globalZOrder);
		_customCommandGLLine.func = CC_CALLBACK_0(DrawNode3D::onDrawGLLine, this, transform, flags);
		renderer->addCommand(&_customCommandGLLine);
	}
}

void DrawNode3D::onDraw(const Mat4 &transform, uint32_t flags)
{
	auto glProgram = getGLProgram();
	glProgram->use();
	glProgram->setUniformsForBuiltins(transform);

	GL::blendFunc(_blendFunc.src, _blendFunc.dst);

	if (_dirty)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V3F_C4B_T2F)*_bufferCapacity, _buffer, GL_STREAM_DRAW);

		_dirty = false;
	}
	if (Configuration::getInstance()->supportsShareableVAO())
	{
		GL::bindVAO(_vao);
	}
	else
	{
		GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		// vertex
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, vertices));
		// color
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, colors));
		// texcood
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, texCoords));
	}
	glEnable(GL_DEPTH_TEST);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glDrawArrays(GL_TRIANGLES, 0, _bufferCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _bufferCount);
	CHECK_GL_ERROR_DEBUG();
}

void DrawNode3D::onDrawGLLine(const Mat4 &transform, uint32_t flags)
{
	auto glProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR);
	glProgram->use();
	glProgram->setUniformsForBuiltins(transform);

	if (_dirtyGLLine)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V3F_C4B_T2F)*_bufferCapacityGLLine, _bufferGLLine, GL_STREAM_DRAW);
		_dirtyGLLine = false;
	}
	if (Configuration::getInstance()->supportsShareableVAO())
	{
		GL::bindVAO(_vaoGLLine);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
		GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
		// vertex
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, vertices));
		// color
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, colors));
		// texcood
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, texCoords));
	}

	glEnable(GL_DEPTH_TEST);

	glLineWidth(1);
	glBindBuffer(GL_ARRAY_BUFFER, _vboGLLine);
	glDrawArrays(GL_LINES, 0, _bufferCountGLLine);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glDisable(GL_DEPTH_TEST);
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,_bufferCountGLLine);
	CHECK_GL_ERROR_DEBUG();
}

void DrawNode3D::onDrawGLPoint(const Mat4 &transform, uint32_t flags)
{
	auto glProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_U_COLOR);
	glProgram->use();
	glProgram->setUniformsForBuiltins(transform);

	glProgram->setUniformLocationWith4fv(glProgram->getUniformLocation("u_color"), (GLfloat*) &_pointColor.r, 1);
	glProgram->setUniformLocationWith1f(glProgram->getUniformLocation("u_pointSize"), _pointSize);

	glEnable(GL_DEPTH_TEST);

	glBindBuffer(GL_ARRAY_BUFFER, _vboGLPoint);
	glBufferData(GL_ARRAY_BUFFER, sizeof(V3F_C4B_T2F)*_bufferCapacityGLPoint, _bufferGLPoint, GL_STREAM_DRAW);

	GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, vertices));
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V3F_C4B_T2F), (GLvoid *)offsetof(V3F_C4B_T2F, colors));

	glDrawArrays(GL_POINTS, 0, _bufferCountGLPoint);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisable(GL_DEPTH_TEST);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,_bufferCountGLPoint);
	CHECK_GL_ERROR_DEBUG();
}

void DrawNode3D::drawPoint(const Vec3& position, const float pointSize, const Color4F &color)
{
	V3F_C4B_T2F *point = (V3F_C4B_T2F*)(_bufferGLPoint + _bufferCountGLPoint);
	V3F_C4B_T2F a = {position, Color4B(color), Tex2F(0.0, 0.0) };
	*point = a;

	_pointSize = pointSize;
	_pointColor = color;

	_bufferCountGLPoint += 1;
}

void DrawNode3D::drawPoints(const Vec3 *position, unsigned int numberOfPoints, const Color4F &color)
{
	ensureCapacityGLPoint(numberOfPoints);

	V3F_C4B_T2F *point = (V3F_C4B_T2F*)(_bufferGLPoint + _bufferCountGLPoint);

	for(unsigned int i=0; i < numberOfPoints; i++,point++)
	{
		V3F_C4B_T2F a = {position[i], Color4B(color), Tex2F(0.0, 0.0) };
		*point = a;
	}

	_pointColor = color;

	_bufferCountGLPoint += numberOfPoints;
}

void DrawNode3D::drawLine(const Vec3 &origin, const Vec3 &destination, const Color4F &color)
{
	ensureCapacityGLLine(2);

	V3F_C4B_T2F *point = (V3F_C4B_T2F*)(_bufferGLLine + _bufferCountGLLine);

	V3F_C4B_T2F a = {origin, Color4B(color), Tex2F(0.0, 0.0)};
	V3F_C4B_T2F b = {destination, Color4B(color), Tex2F(0.0, 0.0)};

	*point = a;
	*(point+1) = b;

	_bufferCountGLLine += 2;
	_dirtyGLLine = true;
}
