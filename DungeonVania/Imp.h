#pragma once
#include "Enemy.h"
#include "structs.h"

class Imp:public Enemy
{
public:
	Imp(float scale, int collomsIdle, const Point2f& spawnLocation);
	virtual ~Imp();
	
	virtual void Draw()const override;
private:
	Texture m_ImpSprite;
	float m_SpellAccuSec;
	void Attack(float elapsedSec, float scale,const Point2f& destination);
};
