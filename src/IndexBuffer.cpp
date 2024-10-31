#include "../include/IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_count(count) {
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	// std::vector<uint> check(count);
	// glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(unsigned int), check.data());
	// for (size_t i = 0; i < check.size(); i++) {
    // 	std::cout << "Value at " << i << ": " << check[i] << std::endl;
	// }
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &ibo);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Update(const void* data, unsigned int count){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

unsigned int IndexBuffer::GetCount() const {
	return m_count;
}