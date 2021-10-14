#pragma once
#include "Room.h"
#include "WitchBasic.h"
#include "Hero.h"
#include "Barrel.h"
#include "Pillar.h"
class DungeonRoom:public Room	 
{
public:
	DungeonRoom(float scale,const Rectf &shape,bool isHallway,int volume);
	DungeonRoom(const DungeonRoom& room);
	
	DungeonRoom& operator=(const DungeonRoom& rhs) = delete;
	DungeonRoom(const DungeonRoom&& other) = delete;
	DungeonRoom& operator=(const DungeonRoom&& rhs) = delete;

	~DungeonRoom();
	void Draw()const;
	void Update(float elapsedSec,const Point2f& destinationEnemy);
	void MakeEnemy();
	void MakeInteractibles(Texture* ptexure, Texture* pPillarTexture);
	bool GetIsClear();
	void SetHasBeenEntered(bool hasBeenEntered);
	void NoSpawnEnemy();
	bool CheckEnemy(Spell*& spell);
	bool HitCharacterReturnDead(Hero& hero,float elapsedSec,float damage);
	void ChangeVolume(int newVolume);
private:
	int m_Volume;
	Texture m_RoomImage;
	Rectf m_RoomShape;
	bool m_IsHallway;
	bool m_EnemySpawned;
	bool m_InteraciblesSpawned;
	bool m_IsClear;
	bool m_HasBeenEntered;
	std::vector<Enemy*>m_Enemys;
	std::vector<Barrel*>m_Boxes;
	std::vector <Pillar*>m_Pillars;
};

