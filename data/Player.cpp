#include "Player.h"

void Player::PlayerMove(const irr::EKEY_CODE& event)
{
	if (m_device->getTimer()->getTime() - player_get_move_anim_timer() >= TIME_FOR_ANIM)
	{
		irr::scene::ISceneNodeAnimator* anim = 0;

		irr::core::vector3df prev_pos = GetPos();

		switch (event)
		{
		case irr::KEY_KEY_A:
			if (1)
			{
				Move(RIGHT);
			}
			break;
		case irr::KEY_KEY_W:
			Move(UP);
			break;
		case irr::KEY_KEY_D:
			if (1)
			{
				Move(LEFT);
			}
			break;
		case irr::KEY_KEY_S:
			Move(DOWN);
			break;
		case irr::KEY_KEY_0:
			SetRotation(0);
		case irr::KEY_SPACE:
			break;
		default:
			return;
		}
		anim = m_device->getSceneManager()->createFlyStraightAnimator(
			prev_pos, GetPos(), TIME_FOR_ANIM, false);
		GetModel()->setRotation(GetRot());
		GetModel()->addAnimator(anim);
		anim->drop();
		player_set_move_anim_timer(m_device->getTimer()->getTime());
	}
}

Bullet* Player::PlayerShoot()
{
	irr::core::vector3df bullet_pos = m_pos;
	bullet_pos.Z += 12;
	return new Bullet(m_device, bullet_pos, m_rot);
}

void Player::player_set_shot_timer(int time)
{
	m_shot_timer = time;
}
void Player::player_set_move_anim_timer(int time)
{
	m_move_anim_timer = time;
}