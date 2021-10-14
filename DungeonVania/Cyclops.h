#pragma once
#include "Enemy.h"
#include "Texture.h"
class Cyclops:public Enemy
{
public:
	Cyclops(float scale);
	virtual ~Cyclops();

	

	virtual void Draw()const override;
	virtual void UpdateSprite(float elapsedSec)override;
	virtual void Update(float elapsedsec, const Point2f& destination)override;

private:
	Texture m_CyclopsSprite;
	void Attack(float elapsedSec, float scale, const Point2f& destination)override;
	float m_SpellTimer;
	float m_SecondAttackAccuSec;
	float m_Scale;
	bool m_IsFirstAttack;
	float m_FirstAttackAccuSec;
};

