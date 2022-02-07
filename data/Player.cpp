#include "Player.h"

void Player::PlayerMove(const irr::EKEY_CODE& event)
{
	irr::scene::ISceneNodeAnimator* anim = 0;

	irr::core::vector3df prev_pos = GetPos();

	switch (event)
	{
	case irr::KEY_KEY_A:
		if (1)
		{
			Move(RIGHT);
		}/*
		else
		{
			SetRotation(0);
		}*/
		break;
	case irr::KEY_KEY_W:
		Move(UP);
		break;
	case irr::KEY_KEY_D:
		if (1)
	{
		Move(LEFT);
	}/*
	else
	{
		SetRotation(0);
	}*/
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
}