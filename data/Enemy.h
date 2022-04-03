#pragma once
#include "Object.h"
#include "Bullet.h"

#define ENEMY_SHOT_DELAY 2500
class Enemy : public Object
{
public:
	void EnemyInit(irr::core::vector3df pos);
	Enemy(irr::IrrlichtDevice* device, irr::core::vector3df pos, int id, int flag) : Object(device)
	{
		m_flag = flag;
		m_id = id;
		m_speed = BASE_SPEED;
		EnemyInit(pos);
	}
	int  GetFlag() const { return m_flag; }
	void  SetFlag(const int flag) { m_flag = flag; }
	void MoveSequence();
	void enemy_move();
	virtual Bullet* enemy_shoot();
	void enemy_set_move_anim_timer(int time);
	int  enemy_get_move_anim_timer() { return m_move_anim_timer; }
	void enemy_set_shot_timer(int time);
	int  enemy_get_shot_timer() { return m_shot_timer; }
	int  enemy_get_id() { return m_id; }

private:
	irr::core::vector3df m_starting_pos;
	int m_id;
	int m_flag = 0;
	int m_move_anim_timer = 0;
	int m_shot_timer = 0;
	int m_dir = LEFT;
};