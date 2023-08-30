#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "math/vec.h"
#include "math/matrix.h"


namespace core {

	class Transform {
	public:
		Transform(const math::Vec3& scale = math::Vec3(1.0f,1.0f,1.0f), const  math::Vec3& rotation = math::Vec3(0.0f), const  math::Vec3& translation = math::Vec3(0.0f));
		Transform(const  math::Mat4& matrix);
		~Transform();

		math::Mat4 getMatrix() const;

		Transform getScaleComponent() const;
		Transform getRotationComponent() const;
		Transform getTranslationComponent() const;

		void setScale(const  math::Vec3& scale);
		void setRotation(const  math::Vec3& rotation);
		void setTranslation(const  math::Vec3& translation);

		void reset();

		Transform operator*(const Transform& rhs) const;

		static Transform buildTranslation(const  math::Vec3& v);
		static Transform buildScale(const  math::Vec3& v);

	private:
		math::Vec3 mScale;
		math::Vec3 mRotation;
		math::Vec3 mTranslation;
		math::Mat4 mMatrix;

		void computeMatrix();
	};

}

#endif