#pragma once
#include "Enemy.h"
#include "utils.h"
class Dwarf:public Enemy
{
public:
	Dwarf(float scale, int collomsIdle, const Point2f& spawnLocation);
	virtual~Dwarf();


	virtual void Draw()const override;
private:
	Texture m_DwarfSprite;
	void Attack(float elapsedSec, float scale,const Point2f& destination)override;
};

