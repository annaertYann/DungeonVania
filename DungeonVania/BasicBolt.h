#pragma once
#include "Spell.h"
#include "Texture.h"
class BasicBolt :public Spell
{
public:
	BasicBolt(const Point2f& pos, float scale, const Vector2f& destination, const Point2f& PosInCamera,Texture* bolt,Texture* boom);
	virtual ~BasicBolt();


	virtual void Draw()const;
	virtual void Update(float elapsedSec) ;
	virtual Rectf GetShape();
	virtual bool ExplosionTime();
	virtual bool GetExplosion();
	
private:

	Texture* m_pBasicBoltSprite;
	Texture* m_pBasicBoltBoomSprite;
	Point2f m_InitialPos;
	bool m_IsDead;
	bool m_IsExplosion;
	bool m_IsGoingLeft;
	Rectf m_SourceRectExplosion;
	int m_DeadTime;
	void MoveBolt(float elapsedSec);
	void UpdateSprite(float elapsedSec);
	void UpdateExplosionSprite(float elapsedSec);
};
