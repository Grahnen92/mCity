#include "Plane.h"

Plane::Plane()
{

}

Plane::Plane(float _x, float _y, float _z, float _dX, float _dY)
{
	oType = 'P';

	transform = glm::mat4(1.0f);
	transform[3][0] = _x;
	transform[3][1] = _y;
	transform[3][2] = _z;

	dimensions = glm::vec2(_dX, _dY);

	GLfloat vertex_array_data[] = {
		//Vertex								 Normals						 Texture  
		-_dX / 2.0f, 0.0f, _dY / 2.0f,	0.0f, 1.0f, 0.0f,	//(tX * 256.0f) / 3072.0f, (tY*256.0f) / 3072.0f,
		_dX / 2.0f, 0.0f, _dY / 2.0f,		0.0f, 1.0f, 0.0f,	//(tX * 256.0f + tXLength * 256.0f) / 3072.0f, (tY * 256.0f) / 3072.0f,
		_dX / 2.0f, 0.0f, -_dY / 2.0f,	0.0f, 1.0f, 0.0f,	//(tX * 256.0f + tXLength * 256.0f) / 3072.0f, (tY * 256.0f + tYLength * 256.0f) / 3072.0f,
		-_dX / 2.0f, 0.0f, -_dY / 2.0f,	0.0f, 1.0f, 0.0f,	//(tX * 256.0f) / 3072.0f, (tY * 256.0f + tYLength * 256.0f) / 3072.0f,
	};

	static const GLuint index_array_data[] = {
		0, 1, 2,
		0, 2, 3
	};

	shape = new StaticMesh;

	shape->nrofVerts = 4;
	shape->nrofTris = 2;

	shape->vertexArray = new vertex*[1];
	shape->triangleArray = new triangle*[1];

	shape->vertexArray[0] = new vertex[shape->nrofVerts];
	shape->triangleArray[0] = new triangle[shape->nrofTris];

	for (int i = 0; i < shape->nrofVerts; i++) {
		shape->vertexArray[0][i].xyz[0] = vertex_array_data[i*6];
		shape->vertexArray[0][i].xyz[1] = vertex_array_data[i * 6 + 1];
		shape->vertexArray[0][i].xyz[2] = vertex_array_data[i * 6 + 2];

		shape->vertexArray[0][i].nxyz[0] = vertex_array_data[i * 6 + 3];
		shape->vertexArray[0][i].nxyz[1] = vertex_array_data[i * 6 + 4];
		shape->vertexArray[0][i].nxyz[2] = vertex_array_data[i * 6 + 5];
	}
	for (int i = 0; i < shape->nrofTris; i++) {
		shape->triangleArray[0][i].index[0] = index_array_data[i*3];
		shape->triangleArray[0][i].index[1] = index_array_data[i * 3 +1];
		shape->triangleArray[0][i].index[2] = index_array_data[i * 3 + 2];
	}

	// Generate one vertex array object (VAO) and bind it
	glGenVertexArrays(1, &(shape->vao));
	glBindVertexArray(shape->vao);

	// Generate two buffer IDs
	glGenBuffers(1, &shape->vertexbuffer);
	glGenBuffers(1, &shape->indexbuffer);

	// Activate the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, shape->vertexbuffer);
	// Present our vertex coordinates to OpenGL
	glBufferData(GL_ARRAY_BUFFER,
		shape->nrofVerts * sizeof(vertex), (*shape->vertexArray), GL_STATIC_DRAW);
	// Specify how many attribute arrays we have in our VAO
	glEnableVertexAttribArray(0); // Vertex coordinates
	glEnableVertexAttribArray(1); // Normals
	//glEnableVertexAttribArray(2); // Texture coordinates
	// Specify how OpenGL should interpret the vertex buffer data:
	// Attributes 0, 1, 2 (must match the lines above and the layout in the shader)
	// Number of dimensions (3 means vec3 in the shader, 2 means vec2)
	// Type GL_FLOAT
	// Not normalized (GL_FALSE)
	// Stride 8 (interleaved array with 8 floats per vertex)
	// Array buffer offset 0, 3, 6 (offset into first vertex)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(vertex), (void*)0); // xyz coordinates
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(vertex), (void*)(3 * sizeof(GLfloat))); // normals
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
	//	stride * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // texcoords

	// Activate the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape->indexbuffer);
	// Present our vertex indices to OpenGL
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		3 * shape->nrofTris*sizeof(GLuint), (*shape->triangleArray), GL_STATIC_DRAW);

	// Deactivate (unbind) the VAO and the buffers again.
	// Do NOT unbind the buffers while the VAO is still bound.
	// The index buffer is an essential part of the VAO state.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void Plane::clean()
{


	if (glIsVertexArray(shape->vao)) {
		glDeleteVertexArrays(1, &shape->vao);
	}
	shape->vao = 0;

	if (glIsBuffer(shape->vertexbuffer)) {
		glDeleteBuffers(1, &shape->vertexbuffer);
	}
	shape->vertexbuffer = 0;

	if (glIsBuffer(shape->indexbuffer)) {
		glDeleteBuffers(1, &shape->indexbuffer);
	}
	shape->indexbuffer = 0;
}


Plane::~Plane(void) {
	std::cout << "A Plane has died" << std::endl;
	delete[](*shape->vertexArray);
	delete[](*shape->triangleArray);

	delete[] shape->vertexArray;
	delete[] shape->triangleArray;

	clean();
}



