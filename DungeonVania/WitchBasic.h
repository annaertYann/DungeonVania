#pragma once
#include "Enemy.h"
class WitchBasic:public Enemy
{
public:
	WitchBasic(float scale, int collomsIdle, const Point2f& spawnLocation);
	virtual ~WitchBasic();

	void Draw()const;
private:
	Texture m_WitchSprite;
	float m_SpellAccuSec;
	//funtions
	void Attack(float elapsedSec, float scale,const Point2f& destination);
};

