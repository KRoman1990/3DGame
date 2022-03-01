#pragma once
#include "Object.h"
#define BULLET_SPEED 2
#define BULLET_HEALTH 200

class Bullet : public Object
{
public:
	void MoveBullet();
	int GetDeathCounter() { return m_death_counter; }
	void SetDeathCounter(int counter) { m_death_counter = counter; }
	Bullet(irr::IrrlichtDevice* device, irr::core::vector3df pos, irr::core::vector3df rot) : Object(device)
	{
		m_mesh = m_sm->getMesh("../media/bullet.obj");
		m_model = m_sm->addMeshSceneNode(m_mesh);
		if (m_model)
		{
			m_model->setMaterialTexture(0, m_driver->getTexture("../media/texture/space_props_red.png"));
			m_model->setPosition(pos);
			m_model->setRotation(rot);
			m_model->setScale(irr::core::vector3df(20, 20, 20));
		}
		m_speed = BULLET_SPEED;
		m_pos.Z += m_speed * sin((m_rot.Y + 90) * (3.14 / 180)) * 5;
		m_objectSelector = m_sm->createTriangleSelector(m_model->getMesh(), m_model);
		m_model->setTriangleSelector(m_objectSelector);
		m_death_flag = 0;
		m_starting_pos = m_model->getPosition();
		m_pos = m_starting_pos;
		m_rot = m_model->getRotation();
		m_death_counter = BULLET_HEALTH;
	}
private:
	irr::core::vector3df m_starting_pos;
	int m_dir;
	int m_death_counter;
};