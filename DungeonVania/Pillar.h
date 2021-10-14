#pragma once
#include "Texture.h"
#include "structs.h"
#include <vector>
#include "Hero.h"
#include "Enemy.h"
class Pillar
{
public:
	Pillar(const Point2f& pos, Texture* ptexture,float scale);
	~Pillar();
	void Draw()const;
	void HandleCollosion(Hero &hero, std::vector<Enemy*> &enemy);
	Rectf GetShape();
	void SetIsRemoved();
private:
	Rectf m_DestRect;
	Texture* m_pPillarImage;
	std::vector<Point2f>m_Vertices;
	bool m_IsRemoved;
};
