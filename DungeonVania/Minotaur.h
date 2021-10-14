#pragma once
#include "pch.h"
#include "Texture.h"
#include "Enemy.h"
class MinoTaur :public Enemy
{
public:
	MinoTaur(float scale);
	virtual ~MinoTaur();


	void Draw()const;
private:
	Texture m_MinotaurSprite;
	float m_SpellAccuSec;
	//funtions
	void Attack(float elapsedSec, float scale, const Point2f& destination);
};