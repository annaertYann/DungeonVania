#pragma once
#include "Spell.h"
#include "Texture.h"
class Laser:public Spell
{
public:
	Laser(const Point2f& pos, float scale, const Vector2f& velocity, const Point2f& PosInCamera);
	virtual ~Laser();
	
	virtual void Draw()const ;
	virtual void Update(float elapsedSec) ;
	virtual Rectf GetShape() ;
	virtual bool ExplosionTime();
	virtual bool GetExplosion();
private:
	float m_LaserGrowthSpeed;
	float m_LaserMoveSpeed;
	Texture m_LaserSprite;
	Texture m_LaserParticle;
	void UpdateSprite(float elapsedSec);
	void MoveLaser(float elapsedSec);
	float m_Angle;
	Point2f m_InitialPos;
	bool m_IsGoingLeft;
	Rectf m_DestRectParticle;
	Rectf m_SourceRectParticle;
	float m_HitBoxDiff;
};

