#pragma once
#include "Texture.h"
class Barrel
{
public:
	Barrel(Texture* texture,const Point2f& pos,float scale);
	~Barrel();

	
	void Draw()const;
	void update(float elapsedSec);
	bool HitBarrelReturnDead(const Rectf& actorShape);
	Rectf GetShape();
private:
	Texture* m_pBarrelSprite;
	Rectf m_DestRect;
	Rectf m_SourceRect;
	bool m_IsHit;
	bool m_IsDead;
	int m_MaxFrames;
	float m_AccuSec;
};

