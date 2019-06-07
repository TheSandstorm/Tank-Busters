#include "Floor.h"

CFloor::CFloor(glm::vec3 _Pos)
{
	Scale = glm::vec3(0.3f, 0.1f, 0.3f);
	Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	Pos = _Pos;
	VAO = CObjectManager::GetMesh(FLOOR_ENTITY)->VAO;
	IndicesCount = CObjectManager::GetMesh(FLOOR_ENTITY)->IndicesCount;
	Texture = CObjectManager::GetMesh(FLOOR_ENTITY)->Texture;
	Shader = CObjectManager::GetMesh(FLOOR_ENTITY)->Shader;
	Type = FLOOR_ENTITY;
}

void CFloor::Update(float _DeltaTime)
{
	VPMatrix = CCamera::GetMatrix();
	Render();
}
