#pragma once
#include "Texture.h"
class Chest
{
public:
	Chest(Texture* texture, const Point2f& pos, float scale);
	 ~Chest();
	
	void Draw()const;
	void update(float elapsedSec);
	void ChestHit();
	Rectf GetShape();
private:
	Texture* m_pChestSprite;
	Rectf m_CoinDestRect;
	Rectf m_CoinSourceRect;
	Rectf m_DestRect;
	Rectf m_SourceRect;
	bool m_IsHit;
	float m_Scale;
	bool m_IsCoinDone;
	bool m_IsOpen;
	int m_MaxFrames;
	float m_AccuSec;
};

