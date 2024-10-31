#include "../include/VertexBuffer.hpp"

VertexBuffer::VertexBuffer(const float* data, unsigned int size) {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	// std::vector<float> check(size/sizeof(float));
	// glGetBufferSubData(GL_ARRAY_BUFFER, 0, size, check.data());
	// for (size_t i = 0; i < check.size(); i++) {
    // 	std::cout << "Value at " << i << ": " << check[i] << std::endl;
	// }
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &vbo);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Update(const void* data, unsigned int size) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}