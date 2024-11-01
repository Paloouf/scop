#pragma once
#include "main.hpp"

class Vec3;

class Mat4 {
public:
    float data[16];

    Mat4() {
        // Initialize as identity matrix
        for (int i = 0; i < 16; ++i) data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    // Function to multiply matrices
    Mat4 operator*(const Mat4& other) const;

    // Translation matrix creation
    static Mat4 translate(float x, float y, float z);

    // Rotation matrix creation (for each axis)
    static Mat4 rotateX(float angle);
    static Mat4 rotateY(float angle);
    static Mat4 rotateZ(float angle);

    // Scaling matrix creation
    static Mat4 scale(float x, float y, float z);

    // Perspective projection
    static Mat4 perspective(float fov, float aspect, float near, float far);

	// LookAt function
	static Mat4 lookAt(const Vec3& cameraPosition, const Vec3& target, const Vec3& up);

	const float* value_ptr() const {
        return data;  // Returns a pointer to the first element of the data array
    }
};