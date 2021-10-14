#pragma once
#include "Room.h"
class Lobby:public Room
{
public:
	Lobby(float scale,const Point2f& pos, int unlockedDoors);
	~Lobby();
	
	virtual void Draw()const ;
	int IsOnDoor(const Rectf& actorshape);
	void Update(float elapsedSec);
	bool IsInShop(const Rectf& actorShape);
	bool IsInSavePoint(const Rectf& actorShape);
	void SetWon();
private:
	bool m_Won;
	int m_ShopCurrentFrame;
	float m_AccuSec;
	float m_AccuSecTimingShop;
	Texture m_Save;
	Texture m_Throphy;
	Texture m_Lobby0Image;
	Texture m_Lobby1Image;
	Texture m_Lobby2Image;
	Texture m_Lobby3Image;
	Texture m_ShopSprite;
	Rectf m_SavePoint;
	Rectf m_Door1Shape;
	Rectf m_ShopShape;
	Rectf m_Door2Shape;
	Rectf m_Door3Shape;
	float m_Scale;
	int m_UnlockedDoors;
	
};

