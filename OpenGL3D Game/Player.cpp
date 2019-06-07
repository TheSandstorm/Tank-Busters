#include "Player.h"

CPlayer::CPlayer(glm::vec3 _Pos)
{
	Pos = _Pos;
	Scale = glm::vec3(0.5f, 0.5f, 0.5f);
	Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	Velocity = { 0.0f, 0.0f, 0.0f };
	Target = { 0.0f, 0.0f, 0.0f };

	maxSpeed = 15.0f;
	maxForce = 0.0f;
	HitRadius = 110.0f;
	bShoot = false;
	State = NONE;
	BulletCooldown = 4.0f;
	BulletTimer = 0.0f;

	Type = PLAYER_ENTITY;
	model = CObjectManager::GetModel(Type);
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update(GLfloat deltaTime)
{
	VPMatrix = CCamera::GetMatrix();
	BulletTimer += 10.f * deltaTime;

	if (BulletTimer >= BulletCooldown) 
	{
		if (bShoot) 
		{
			//sm.PlayShootFX();
			BulletVect.push_back(std::make_shared<CBullet>(BulletVelocity, Pos));
			BulletTimer = 0.0f;
		}
	}

	Velocity += AiMove::Seek(Pos, Velocity, Target);
	Pos += Velocity * maxSpeed * deltaTime;
	Render();

	//Checking that bullets dont go out of range
	for (unsigned int i = 0; i < BulletVect.size(); ++i) {
		if (BulletVect[i]->GetPos().y >= 3300.0f ||
			BulletVect[i]->GetPos().y <= -3300.0f ||
			BulletVect[i]->GetPos().x >= 3300.0f ||
			BulletVect[i]->GetPos().x <= -3300.0f) {
			BulletVect.erase(BulletVect.begin(), BulletVect.begin() + i);
			continue;
		}
		BulletVect[i]->Process(deltaTime);
	}
}

void CPlayer::CreateBullet(glm::vec3 Velocity)
{
	BulletVelocity = Velocity;
	bShoot = true;
}

void CPlayer::Render()
{
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(Pos.x, Pos.y, Pos.z) /375.f);

	float PI = 3.14159265359f;
	float angle;
	angle = atan2f(Velocity.x, Velocity.y) * (180.0f / PI);

	//X Rotation
	glm::mat4 RotateX =
		glm::rotate(
			glm::mat4(),
			glm::radians(Rotation.x + 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);

	glm::mat4 RotateY =
		glm::rotate(
			glm::mat4(),
			glm::radians(Rotation.y + (angle * -1.0f)),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(Scale));
	ModelMatrix = TranslationMatrix * (RotateX * RotateY) * ScaleMatrix;
	glUniform3fv(glGetUniformLocation(Shader, "camPos"), 1, glm::value_ptr(CCamera::GetPos()));

	model->Render(ModelMatrix);
}


