#include "../include/Mat4.hpp"

Mat4 Mat4::operator*(const Mat4& other) const {
    Mat4 result;

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result.data[row * 4 + col] = 
                data[row * 4 + 0] * other.data[0 * 4 + col] +
                data[row * 4 + 1] * other.data[1 * 4 + col] +
                data[row * 4 + 2] * other.data[2 * 4 + col] +
                data[row * 4 + 3] * other.data[3 * 4 + col];
        }
    }

    return result;
}

Mat4 Mat4::translate(float x, float y, float z){
	Mat4 result;
    result.data[12] = x;
    result.data[13] = y;
    result.data[14] = z;
    return result;
}

Mat4 Mat4::scale(float sx, float sy, float sz) {
    Mat4 result;
    result.data[0] = sx;
    result.data[5] = sy;
    result.data[10] = sz;
    return result;
}

Mat4 Mat4::rotateX(float angle) {
    Mat4 result;
    float c = cos(angle);
    float s = sin(angle);

    result.data[5] = c;
    result.data[6] = -s;
    result.data[9] = s;
    result.data[10] = c;
    return result;
}

Mat4 Mat4::rotateY(float angle) {
    Mat4 result;
    float c = cos(angle);
    float s = sin(angle);

    result.data[0] = c;
    result.data[2] = s;
    result.data[8] = -s;
    result.data[10] = c;
    return result;
}

Mat4 Mat4::rotateZ(float angle) {
    Mat4 result;
    float c = cos(angle);
    float s = sin(angle);

    result.data[0] = c;
    result.data[1] = -s;
    result.data[4] = s;
    result.data[5] = c;
    return result;
}

Mat4 Mat4::perspective(float fov, float aspect, float near, float far) {
	Mat4 result;
	
	// Zero out the matrix
	for (int i = 0; i < 16; ++i) result.data[i] = 0.0f;

	// Calculate scale based on FOV and aspect ratio
	float tanHalfFOV = tan(fov / 2.0f);

	result.data[0] = 1.0f / (aspect * tanHalfFOV); // scale x
	result.data[5] = 1.0f / (tanHalfFOV);          // scale y
	result.data[10] = -(far + near) / (far - near); // scale z
	result.data[11] = -1.0f;                        // perspective z (row-major)
	result.data[14] = -(2.0f * far * near) / (far - near); // perspective w

	return result;
}

Mat4 Mat4::lookAt(const Vec3& cameraPosition, const Vec3& target, const Vec3& up) {
	Vec3 forward = (target - cameraPosition).normalize();
	Vec3 right = up.cross(forward).normalize();
	Vec3 trueUp = forward.cross(right);

	Mat4 result;

	result.data[0] = right.x;
	result.data[1] = trueUp.x;
	result.data[2] = -forward.x;
	result.data[3] = 0.0f;

	result.data[4] = right.y;
	result.data[5] = trueUp.y;
	result.data[6] = -forward.y;
	result.data[7] = 0.0f;

	result.data[8] = right.z;
	result.data[9] = trueUp.z;
	result.data[10] = -forward.z;
	result.data[11] = 0.0f;

	result.data[12] = -right.dot(cameraPosition);
	result.data[13] = -trueUp.dot(cameraPosition);
	result.data[14] = forward.dot(cameraPosition);
	result.data[15] = 1.0f;

	return result;
}