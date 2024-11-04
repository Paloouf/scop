#pragma once
#include "main.hpp"

class Vec3{
	public:
		Vec3(float x,float y, float z) :x(x),y(y),z(z){}
		Vec3() : x(0), y(0), z(0) {}
		float x,y,z;
		Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z);}
		Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
		Vec3 operator*(float scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }

		Vec3 cross(const Vec3& other) const {
			return Vec3(
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x
			);
		}


		static Vec3 findCentroid(const std::vector<float>& vertices) {
			Vec3 centroid = {0.0f, 0.0f, 0.0f};
			size_t vertexCount = vertices.size() / 8; // Each vertex has 6 components (x, y, z, r, g, b)

			// Sum the coordinates
			for (size_t i = 0; i < vertexCount; ++i) {
				centroid.x += vertices[i * 8 + 0]; // x
				centroid.y += vertices[i * 8 + 1]; // y
				centroid.z += vertices[i * 8 + 2]; // z
			}

			// Calculate average
			if (vertexCount > 0) {
				centroid.x /= vertexCount;
				centroid.y /= vertexCount;
				centroid.z /= vertexCount;
			}

			return centroid;
		}

		Vec3 normalize() const {
			float length = std::sqrt(x * x + y * y + z * z);
			return Vec3(x / length, y / length, z / length);
		}

		float dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }
};