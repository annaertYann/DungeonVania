#pragma once
#include "Vector2f.h"
class Spell
{
public:
	enum class SpellType
	{
		basicBolt
		,groundSlam
		,laser
	};
	Spell(const Point2f& pos,float scale,const Vector2f& velocity, const Point2f &PosInCamera);
	virtual ~Spell();
	virtual void Draw()const=0;
	virtual void Update(float elapsedSec)=0;
	virtual Rectf GetShape()=0;
	virtual bool ExplosionTime()=0;
	virtual bool GetExplosion()=0;
	SpellType GetType();
	bool GetDead();
protected:
	Point2f m_PosInCamera;
	Rectf m_DestRect;
	Rectf m_SourceRect;
	Rectf m_HitBox;
	int m_CurrentFrame;
	float m_Scale;
	float m_Velocity;
	float m_MovementSpeed;
	Vector2f MousePos;
	float m_Width;
	float m_Height;
	float m_AccuSec;
	SpellType m_CurrentType;
	bool m_Dead;
};

