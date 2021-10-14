#pragma once
#include "Spell.h"
#include "Texture.h"
class SlamGround :public Spell
{
public:
	SlamGround(const Point2f& pos, float scale, const Vector2f& velocity, const Point2f& PosInCamera,Texture* texture);
	virtual ~SlamGround();

	virtual void Draw()const ;
	virtual void Update(float elapsedSec) ;
	virtual Rectf GetShape();
	virtual bool ExplosionTime() ;
	virtual bool GetExplosion() ;
private:
	Texture* m_FireBallSprite;
	float m_MoveCounter;
	float m_FrameRateAnimation;
	int m_Rows;
	int m_Colloms;
	int m_CurrentRow;
	float m_HitBoxDiff;
	void UpdateSpell(float elapsedSec);
	void UpdateSprite(float elapsedSec);
};

