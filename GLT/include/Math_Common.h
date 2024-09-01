#pragma once
#include <GLMath.h>
using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;

using Matrix2 = glm::mat2;
using Matrix3 = glm::mat3;
using Matrix4 = glm::mat4;

using Matrix2x3 = glm::mat2x3;
using Matrix2x4 = glm::mat2x4;
using Matrix3x2 = glm::mat3x2;
using Matrix3x4 = glm::mat3x4;
using Matrix4x2 = glm::mat4x2;
using Matrix4x3 = glm::mat4x3;

using Quaternion = glm::quat;

class Math {
public:
	template<typename T>
	static T zero()
	{
		return glm::zero<T>();
	}
	template<typename T>
	static T identity()
	{
		return glm::identity<T>();
	}
	static Matrix4 trs(Vector4 translate, Quaternion rotate, Vector4 scale)
	{
		auto mat = glm::scale(Math::identity<Matrix4>(), scale);
		mat = glm::mat4_cast(rotate) * mat;
		return glm::translate(mat, translate);
	}

	static void decompose(Matrix4 modelMatrix, Vector4& translate, Quaternion& rotate, Vector4& scale)
	{
		Vector4 perspective;
		Vector3 skew;
		glm::decompose(modelMatrix, scale, rotate, translate, skew, perspective)
	}
};
