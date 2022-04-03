#pragma once
#include "Object.h"
#define BULLET_SPEED 3
#define BULLET_HEALTH 200
#define BULLET_SIZE 2

class Bullet : public Object
{
public:
	virtual void MoveBullet();
	int  GetDeathCounter() const { return m_death_counter; }
	void SetDeathCounter(const int counter) { m_death_counter = counter; }
	int  GetFlag() const { return m_flag; }
	void  SetFlag(const int flag)  { m_flag = flag; }
	void BulletInit(irr::core::vector3df pos, irr::core::vector3df rot);
	Bullet(irr::IrrlichtDevice* device, irr::core::vector3df pos, irr::core::vector3df rot, int flag = 0) : Object(device)
	{
		m_speed = BULLET_SPEED;
		m_flag = flag;
		BulletInit(pos, rot);
	}
private:
	irr::core::vector3df m_starting_pos;
	int m_dir;
	int m_death_counter = BULLET_HEALTH;
	int m_flag;
};