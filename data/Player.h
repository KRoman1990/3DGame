#pragma once
#include "Object.h"
#include "Bullet.h"
#define PLAYER_SPEED 25

class Player : public Object
{
public:
	void PlayerInit();
	void PlayerMove(const irr::EKEY_CODE& event);
	Bullet* PlayerShoot();
	Player(irr::IrrlichtDevice* device) : Object(device)
	{
		m_speed = PLAYER_SPEED;
		PlayerInit();
	 }
	void SetInvincibleCounter(int counter) { m_inv_count = counter; }
	int GetInvincibleCounter() { return m_inv_count; }
	void player_set_shot_timer(int time);
	int  player_get_shot_timer() { return m_shot_timer; }
	void player_set_move_anim_timer(int time);
	int  player_get_move_anim_timer() { return m_move_anim_timer; }
	int  GetFlag() const { return m_flag; }
	void  SetFlag(const int flag) { m_flag = flag; }

private:
	irr::core::vector3df m_starting_pos;
	int m_inv_count = 0;
	int m_dir;
	int m_shot_timer = 0;
	int m_move_anim_timer = 0;
	int m_flag = 0;
};