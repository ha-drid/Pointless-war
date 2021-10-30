#include <glad\glad.h>

#include "VertexArrayObject.h"

static void vaoInit(struct VertexArrayObject* const vao) {
	glGenVertexArrays(1, &(vao->vao));
	vectorInit(vao->buffers, 0);

	vao->indicesBuffer = 0;
	vao->indicesCount = 0;
}

static void vaoBind(struct VertexArrayObject* const vao)
{
    glBindVertexArray(vao->vao);
}

static void vaoRebind()
{
    glBindVertexArray(0);
}

static void vaoAddVertexBufferObject(struct VertexArrayObject* const vao, uint32_t vertexInPoint, size_t arraySize, float* array) {
    vaoBind(vao);
    // create vbo
    GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, arraySize * sizeof(float), array, GL_STATIC_DRAW);
	glVertexAttribPointer(vao->buffers.size, vertexInPoint, GL_FLOAT, GL_FALSE, 0, NULL);

	// push
	vectorPushBack(vao->buffers, vbo);
}

static void vaoAddIndices(struct VertexArrayObject* const vao, size_t array_size, unsigned* array) {
    assert(vao->indicesBuffer == 0); // мы не можем создать ещё один буфер
    vao->indicesCount = array_size;

	vaoBind(vao);

	glGenBuffers(1, &(vao->indicesBuffer));

	// обратите внимание, что мы используем не GL_ARRAY_BUFFER, а GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, array_size * sizeof(unsigned), array, GL_STATIC_DRAW);
}

static void vaoDrawArrays(struct VertexArrayObject* const vao, unsigned type, unsigned count)
{
    vaoBind(vao);
	for (uint32_t i = 0; i < vao->buffers.size; ++i)
		glEnableVertexAttribArray(i);

	glDrawArrays(type, 0, count);

	for (uint32_t i = 0; i < vao->buffers.size; ++i)
		glDisableVertexAttribArray(i);
}

static void vaoDrawElements(struct VertexArrayObject* const vao,unsigned type)
{
	assert(vao->indicesBuffer != 0);

	vaoBind(vao);
	for (uint32_t i = 0; i < vao->buffers.size; ++i)
		glEnableVertexAttribArray(i);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->indicesBuffer);
	glDrawElements(type, vao->indicesCount, GL_UNSIGNED_INT, NULL);

	for (uint32_t i = 0; i < vao->buffers.size; ++i)
		glDisableVertexAttribArray(i);
}

void vaoDrawElementsInstanced(struct VertexArrayObject* const vao, unsigned type, uint32_t instanceCount) {
    assert(vao->indicesBuffer != 0);

	vaoBind(vao);
	for (uint32_t i = 0; i < vao->buffers.size; ++i)
	{
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->indicesBuffer);
	glDrawElementsInstanced(type, vao->indicesCount, GL_UNSIGNED_INT, NULL, instanceCount);

	for (uint32_t i = 0; i < vao->buffers.size; ++i)
	{
		glDisableVertexAttribArray(i);
	}
}

static void vaoDelete(struct VertexArrayObject* const vao) {
    vao->indicesCount = 0;
    // delete vbo
    glDeleteBuffers(vao->buffers.size, vao->buffers.data);
    glDeleteBuffers(1, &(vao->indicesBuffer));

	glDeleteVertexArrays(1, &(vao->vao));
}


struct VertexArrayObjectManager vertexArrayObjectManagerInit()
{
    struct VertexArrayObjectManager manager;
    manager.addVertexBufferObject = &vaoAddVertexBufferObject;
    manager.addIndices = &vaoAddIndices;
    manager.bind = &vaoBind;
    manager.rebind = &vaoRebind;
    manager.delete = &vaoDelete;
    manager.drawElements = &vaoDrawElements;
    manager.drawArrays = &vaoDrawArrays;
    manager.drawElementsInstanced = &vaoDrawElementsInstanced;
    manager.init = &vaoInit;
    return manager;
}
