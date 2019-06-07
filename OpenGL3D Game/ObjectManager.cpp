#include "ObjectManager.h"


std::shared_ptr<MESH> CObjectManager::CubeMesh = nullptr;
std::shared_ptr<MESH> CObjectManager::SphereMesh = nullptr;

std::shared_ptr<Model> CObjectManager::Player = nullptr;
std::shared_ptr<Model> CObjectManager::Enemy1 = nullptr;
std::shared_ptr<Model> CObjectManager::Enemy2 = nullptr;

std::shared_ptr<CObjectManager> CObjectManager::ObjectManagerptr = nullptr;

GLuint CObjectManager::ObjectShader;
GLuint CObjectManager::ModelShader;

CObjectManager::CObjectManager()
{
	GLuint VAO, VBO, EBO, Texture;
	int width, height;

	ObjectShader = ShaderLoader::CreateProgram(Utility::ObjectShaderVert.data(), Utility::ObjectShaderFrag.data());
	ModelShader = ShaderLoader::CreateProgram(Utility::ModelShaderVert.data(), Utility::ModelShaderFrag.data());
	//Defines Cube Vertices
	GLfloat CubeVerts[] = {
		// Positions             // Normal Coords        // TexCoords
		// Front Face
		-1.0f, 1.0f, 1.0f,       0.0f, 0.0f, 1.0f,      0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,        0.0f, 0.0f, 1.0f,      1.0f, 0.0f,
		1.0f, -1.0f, 1.0f,       0.0f, 0.0f, 1.0f,      1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,      0.0f, 0.0f, 1.0f,      0.0f, 1.0f,

		// Right Face
		1.0f, 1.0f, 1.0f,        1.0f, 0.0f, 0.0f,      0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,       1.0f, 0.0f, 0.0f,      1.0f, 0.0f,
		1.0f, -1.0f, -1.0f,      1.0f, 0.0f, 0.0f,      1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,       1.0f, 0.0f, 0.0f,      0.0f, 1.0f,

		// Back Face
		1.0f, 1.0f, -1.0f,       0.0f, 0.0f, -1.0f,     0.0f, 0.0f,
		-1.0f, 1.0f, -1.0f,      0.0f, 0.0f, -1.0f,     1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,     0.0f, 0.0f, -1.0f,     1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,      0.0f, 0.0f, -1.0f,     0.0f, 1.0f,

		// Left Face
		-1.0f, 1.0f, -1.0f,      -1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,       -1.0f, 0.0f, 0.0f,     1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,      -1.0f, 0.0f, 0.0f,     1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,     -1.0f, 0.0f, 0.0f,     0.0f, 1.0f,

		// Top Face
		-1.0f, 1.0f, -1.0f,      0.0f, 1.0f, 0.0f,      0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,       0.0f, 1.0f, 0.0f,      1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,        0.0f, 1.0f, 0.0f,      1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,       0.0f, 1.0f, 0.0f,      0.0f, 1.0f,

		// Bottom Face
		-1.0f, -1.0f, 1.0f,      0.0f, -1.0f, 0.0f,     0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,       0.0f, -1.0f, 0.0f,     1.0f, 0.0f,
		1.0f, -1.0f, -1.0f,      0.0f, -1.0f, 0.0f,     1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,     0.0f, -1.0f, 0.0f,     0.0f, 1.0f,
	};

	//Defines Cube Indices
	GLuint CubeIndices[] = {
		0, 1, 2,		0, 2, 3,		// Front Face
		4, 5, 6,		4, 6, 7,		// Right Face
		8, 9, 10,		8, 10, 11,		// Back Face
		12, 13, 14,		12, 14, 15,		// Left Face
		16, 17, 18,		16, 18, 19,		// Top Face
		20, 21, 22,		20, 22, 23,		// Bottom Face
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//Binding and setting buffer data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVerts), CubeVerts, GL_STATIC_DRAW);

	//Enabling the positional floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Enabling Normal Floats
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Enabling the Texture floats
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Generating EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices, GL_STATIC_DRAW);

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	//Getting the image from filepath
	unsigned char* image = SOIL_load_image(
		Utility::TestTexture.data(),
		&width,
		&height,
		0,
		SOIL_LOAD_RGBA
	);

	//Generating the texture from image data
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width, height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image
	);

	//Generating mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Setting Texture wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Setting texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Freeing up data
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Setting the Mesh for the objects to use
	CubeMesh = make_shared<MESH>();
	CubeMesh->VAO = VAO;
	CubeMesh->IndicesCount = sizeof(CubeIndices);
	CubeMesh->Texture = Texture;
	CubeMesh->Shader = ObjectShader;


	Player = make_shared<Model>(Utility::PlayerModel.data(), ModelShader);
	Enemy1 = make_shared<Model>(Utility::EnemyModel1.data(), ModelShader);
	Enemy2 = make_shared<Model>(Utility::EnemyModel2.data(), ModelShader);
}

CObjectManager::~CObjectManager() 
{
	CubeMesh = nullptr;
	SphereMesh = nullptr;
	Player = nullptr;
	Enemy1 = nullptr;
	Enemy2 = nullptr;
}

std::shared_ptr<CObjectManager> CObjectManager::GetInstance()
{
	if (ObjectManagerptr == nullptr) 
	{
		ObjectManagerptr = std::shared_ptr<CObjectManager>(new CObjectManager());
	}
	return ObjectManagerptr;
}

void CObjectManager::DestroyInstance()
{
	ObjectManagerptr = nullptr;
}

std::shared_ptr<MESH> CObjectManager::GetMesh(Object_Attributes _ObjectType)
{
	return CubeMesh;
}

std::shared_ptr<Model> CObjectManager::GetModel(Object_Attributes _ObjectType)
{
	if (_ObjectType == PLAYER_ENTITY) return Player;

	if (_ObjectType == SEEK_ENEMY) return Enemy1;

	if (_ObjectType == WANDER_ENEMY) return Enemy2;

	return nullptr;
}
