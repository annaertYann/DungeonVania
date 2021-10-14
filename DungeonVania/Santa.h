#pragma once
#include "Enemy.h"
#include "Texture.h"
class Santa :public Enemy
{
public:
	Santa(float scale);
	virtual ~Santa();
	

	virtual void Draw()const override;
private:
	Texture m_CyclopsSprite;
	void Attack(float elapsedSec, float scale, const Point2f& destination);
};

