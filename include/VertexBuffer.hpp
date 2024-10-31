#pragma once
#include "main.hpp"

class VertexBuffer {
	private:
		GLuint vbo;
	public:
		VertexBuffer(const float* data, unsigned int size);
		~VertexBuffer();
		GLuint getVbo(){return vbo;};
		void Bind() const;
		void Unbind() const;
		void Update(const void* data, unsigned int size);
};