#ifndef __DRAW_NODE_3D_H__
#define __DRAW_NODE_3D_H__

#include "cocos2d.h"
USING_NS_CC;

class DrawNode3D : public Node
{
public:

	static DrawNode3D* create();

	void drawPoint(const Vec3& point, const float pointSize, const Color4F &color);

	void drawPoints(const Vec3 *position, unsigned int numberOfPoints, const Color4F &color);

	void drawLine(const Vec3 &origin, const Vec3 &destination, const Color4F &color);
	// Overrides
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

    void clear();

    const BlendFunc& getBlendFunc() const;

    void setBlendFunc(const BlendFunc &blendFunc);

    void onDraw(const Mat4 &transform, uint32_t flags);
    void onDrawGLLine(const Mat4 &transform, uint32_t flags);
    void onDrawGLPoint(const Mat4 &transform, uint32_t flags);

CC_CONSTRUCTOR_ACCESS:
	DrawNode3D();
	virtual ~DrawNode3D();
	virtual bool init();

protected:
	void ensureCapacity(int count);
	void ensureCapacityGLPoint(int count);
	void ensureCapacityGLLine(int count);

	GLuint      _vao;
	GLuint      _vbo;
	GLuint      _vaoGLPoint;
	GLuint      _vboGLPoint;
	GLuint      _vaoGLLine;
	GLuint      _vboGLLine;

	int         _bufferCapacity;
	GLsizei     _bufferCount;
	V3F_C4B_T2F *_buffer;

	int         _bufferCapacityGLPoint;
	GLsizei     _bufferCountGLPoint;
	V3F_C4B_T2F *_bufferGLPoint;
	Color4F     _pointColor;
	int         _pointSize;

	int         _bufferCapacityGLLine;
	GLsizei     _bufferCountGLLine;
	V3F_C4B_T2F *_bufferGLLine;

	BlendFunc   _blendFunc;
	CustomCommand _customCommand;
	CustomCommand _customCommandGLPoint;
	CustomCommand _customCommandGLLine;

	bool        _dirty;
	bool        _dirtyGLPoint;
	bool        _dirtyGLLine;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(DrawNode3D);
};

#endif