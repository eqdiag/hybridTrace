#include "math/vec.h"
#include <cstdlib>
#include <cassert>

math::Vec3::Vec3(float x, float y, float z):
	mX{x},mY{y},mZ{z}
{

}

float math::Vec3::x() const
{
	return mX;
}

float math::Vec3::y() const
{
	return mY;
}

float math::Vec3::z() const
{
	return mZ;
}

float math::Vec3::r() const
{
	return mX;
}

float math::Vec3::g() const
{
	return mY;
}

float math::Vec3::b() const
{
	return mZ;
}

float math::Vec3::operator[](size_t index) const
{
	assert(index <= 3);
	return data[index];
}

float& math::Vec3::operator[](size_t index)
{
	assert(index <= 3);
	return data[index];
}

/*Vector space operations*/
math::Vec3 math::Vec3::operator+(const Vec3& rhs) const {
	return Vec3{ mX + rhs.x(),mY + rhs.y() ,mZ + rhs.z() };
}

math::Vec3 math::Vec3::operator+=(const Vec3& rhs)
{
	mX += rhs.x();
	mY += rhs.y();
	mZ += rhs.z();
	return *this;
}
math::Vec3 math::Vec3::operator-(const Vec3& rhs) const {
	return Vec3{ mX - rhs.x(),mY - rhs.y() ,mZ - rhs.z() };
}

math::Vec3 math::Vec3::operator-=(const Vec3& rhs)
{
	mX -= rhs.x();
	mY -= rhs.y();
	mZ -= rhs.z();
	return *this;
}
math::Vec3 math::Vec3::operator*(float rhs) const {
	return Vec3{ mX*rhs,mY*rhs ,mZ*rhs};

}
math::Vec3 math::Vec3::operator*=(float rhs)
{
	mX *= rhs;
	mY *= rhs;
	mZ *= rhs;
	return *this;
}
math::Vec3 math::Vec3::operator/(float rhs) const {
	return Vec3{ mX / rhs,mY/ rhs ,mZ/ rhs };
}
math::Vec3 math::Vec3::operator/=(float rhs)
{
	mX /= rhs;
	mY /= rhs;
	mZ /= rhs;
	return *this;
}
bool math::Vec3::operator==(const Vec3& rhs) const
{
	return (mX == rhs.x()) && (mY == rhs.y()) && (mZ == rhs.z());
}
math::Vec3 math::Vec3::operator-() const {
	return Vec3{ -mX,-mY,-mZ };
}

float* math::Vec3::getRawData()
{
	return (float*)data;
}

/*Inner product and cross product*/
float math::Vec3::dot(const Vec3& rhs) const {
	return mX * rhs.x() + mY * rhs.y() + mZ * rhs.z();
}
math::Vec3 math::Vec3::cross(const Vec3& rhs) const {
	return Vec3{
		mY*rhs.z() - mZ*rhs.y(),
		mZ*rhs.x() - mX*rhs.z(),
		mX*rhs.y() - mY*rhs.x()
	};
}
float math::Vec3::norm2() const {
	return this->dot(*this);
}
float math::Vec3::norm() const {
	return sqrt(norm2());
}
math::Vec3 math::Vec3::normalize() const {
	return *this / norm();
}

math::Vec3 math::Vec3::rand()
{
	return math::Vec3{randFloat() * 2.0f - 1.0f, randFloat() * 2.0f - 1.0f, randFloat() * 2.0f - 1.0f};
}

std::ostream& math::operator<<(std::ostream& out, const Vec3& v)
{
	return out << "[" << v.x() << "," << v.y() << "," << v.z() << "]\n";
}

math::Vec4::Vec4(float x, float y, float z,float w) :
	mX{ x }, mY{ y }, mZ{ z },mW{ w }
{

}

float math::Vec4::x() const
{
	return mX;
}

float math::Vec4::y() const
{
	return mY;
}

float math::Vec4::z() const
{
	return mZ;
}

float math::Vec4::w() const
{
	return mW;
}

float math::Vec4::r() const
{
	return mX;
}

float math::Vec4::g() const
{
	return mY;
}

float math::Vec4::b() const
{
	return mZ;
}

float math::Vec4::a() const
{
	return mW;
}

math::Vec3 math::Vec4::xyz() const
{
	return Vec3(mX,mY,mZ);
}

float math::Vec4::operator[](size_t index) const{
	assert(index <= 3);
	return data[index];
}

float& math::Vec4::operator[](size_t index) {
	assert(index <= 3);
	return data[index];
}


/*Vector space operations*/
math::Vec4 math::Vec4::operator+(const Vec4& rhs) const {
	return Vec4{ mX + rhs.x(),mY + rhs.y() ,mZ + rhs.z(), mW + rhs.w() };
}

void math::Vec4::operator+=(const Vec4& rhs)
{
	mX += rhs.x();
	mY += rhs.y();
	mZ += rhs.z();
	mW += rhs.w();
}
math::Vec4 math::Vec4::operator-(const Vec4& rhs) const {
	return Vec4{ mX - rhs.x(),mY - rhs.y() ,mZ - rhs.z(), mW - rhs.w() };
}

void math::Vec4::operator-=(const Vec4& rhs)
{
	mX -= rhs.x();
	mY -= rhs.y();
	mZ -= rhs.z();
	mW -= rhs.w();
}
math::Vec4 math::Vec4::operator*(float rhs) const {
	return Vec4{ mX * rhs,mY * rhs ,mZ * rhs, mW*rhs};

}
void math::Vec4::operator*=(float rhs)
{
	mX *= rhs;
	mY *= rhs;
	mZ *= rhs;
	mW *= rhs;
}
math::Vec4 math::Vec4::operator/(float rhs) const {
	return Vec4{ mX / rhs,mY / rhs ,mZ / rhs,mW / rhs };
}
void math::Vec4::operator/=(float rhs)
{
	mX /= rhs;
	mY /= rhs;
	mZ /= rhs;
	mW /= rhs;
}
bool math::Vec4::operator==(const Vec4& rhs) const
{
	return (mX == rhs.x()) && (mY == rhs.y()) && (mZ == rhs.z()) && (mW == rhs.w());
}
math::Vec4 math::Vec4::operator-() const {
	return Vec4{ -mX,-mY,-mZ, -mW};
}

float* math::Vec4::getRawData()
{
	return (float*)data;
}

/*Inner product and cross product*/
float math::Vec4::dot(const Vec4& rhs) const {
	return mX * rhs.x() + mY * rhs.y() + mZ * rhs.z() + mW*rhs.w();
}

float math::Vec4::norm2() const {
	return this->dot(*this);
}
float math::Vec4::norm() const {
	return sqrt(norm2());
}
math::Vec4 math::Vec4::normalize() const {
	return *this / norm();
}

math::Vec4 math::Vec4::rand()
{
	return math::Vec4{randFloat() * 2.0f - 1.0f, randFloat() * 2.0f - 1.0f, randFloat() * 2.0f - 1.0f, randFloat() * 2.0f - 1.0f};
}

std::ostream& math::operator<<(std::ostream& out, const Vec4& v)
{
	return out << "[" << v.x() << "," << v.y() << "," << v.z() << "," << v.w() <<  "]\n";
}
