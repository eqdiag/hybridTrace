
#include "core/transform.h"




core::Transform::Transform(const  math::Vec3& scale, const  math::Vec3& rotation, const  math::Vec3& translation):
	mScale{scale},mRotation{rotation},mTranslation{translation}
{
	computeMatrix();
}

core::Transform::Transform(const  math::Mat4& matrix):
	mScale{ math::Vec3(1.0f,1.0f,1.0f) }, mRotation{ math::Vec3{} }, mTranslation{ math::Vec3{} }, mMatrix{ matrix }
{
	
}


core::Transform::~Transform()
{
}

math::Mat4 core::Transform::getMatrix() const
{
	return mMatrix;
}

core::Transform core::Transform::getScaleComponent() const
{
	return Transform{ mScale };
}

core::Transform core::Transform::getRotationComponent() const
{
	return Transform{ math::Vec3(1.0f,1.0f,1.0f),mRotation};
}

core::Transform core::Transform::getTranslationComponent() const
{
	return Transform{ math::Vec3(1.0f,1.0f,1.0f),math::Vec3{},mTranslation };
}

void core::Transform::setScale(const math::Vec3& scale)
{
	this->mScale = scale;
	computeMatrix();
}

void core::Transform::setRotation(const math::Vec3& rotation)
{
	this->mRotation = rotation;
	computeMatrix();
}

void core::Transform::setTranslation(const math::Vec3& translation)
{
	this->mTranslation = translation;
	computeMatrix();
}

void core::Transform::reset()
{
	mScale = math::Vec3(1.0f,1.0,1.0f);
	mRotation = math::Vec3{};
	mTranslation = math::Vec3{};
	mMatrix = math::Mat4{};
}

core::Transform core::Transform::operator*(const Transform& rhs) const
{
	return Transform{mMatrix * rhs.getMatrix()};
}

core::Transform core::Transform::buildTranslation(const math::Vec3& v)
{
	return Transform{ math::Vec3{1.0f,1.0f,1.0f},math::Vec3{},v };
}

core::Transform core::Transform::buildScale(const math::Vec3& v)
{
	return Transform{ v,math::Vec3{},math::Vec3{} };
}

void core::Transform::computeMatrix()
{
	mMatrix =
		math::Mat4::fromTranslation(mTranslation) *
		//glm::eulerAngleX(mRotation.x) *
		//glm::eulerAngleY(mRotation.y) *
		//glm::eulerAngleZ(mRotation.z) *
		math::Mat4::fromScale(mScale );
}
