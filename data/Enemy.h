#pragma once
#include "Object.h"
#include "Bullet.h"

class Enemy : public Object
{
public:
	Enemy(irr::IrrlichtDevice* device, irr::core::vector3df pos, int id) : Object(device)
	{
		m_id = id;
		m_mesh = m_sm->getMesh("../media/spaceship_02.obj");
		m_model = m_sm->addMeshSceneNode(m_mesh);
		if (m_model)
		{
			m_model->setMaterialTexture(0, m_driver->getTexture("../media/texture/T_Ships_red.png"));
			m_model->setPosition(pos);
			m_model->setRotation(irr::core::vector3df(0, 180, 0));
		}
		m_speed = BASE_SPEED;
		m_objectSelector = m_sm->createTriangleSelector(m_model->getMesh(), m_model);
		m_model->setTriangleSelector(m_objectSelector);
		m_death_flag = 0;
		m_starting_pos = m_model->getPosition();
		m_pos = m_starting_pos;
		m_rot = m_model->getRotation();
		m_dir = LEFT;
		m_move_anim_timer = 0;
		m_shot_timer = 0;
	}
	Enemy(){}
	void MoveSequence();
	void enemy_move();
	Bullet* enemy_shoot();
	void enemy_set_move_anim_timer(int time);
	int  enemy_get_move_anim_timer() { return m_move_anim_timer; }
	void enemy_set_shot_timer(int time);
	int  enemy_get_shot_timer() { return m_shot_timer; }
	int  enemy_get_id() { return m_id; }

private:
	irr::core::vector3df m_starting_pos;
	int m_id;
	int m_dir;
	int m_move_anim_timer;
	int m_shot_timer;
};