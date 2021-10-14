#pragma once
#include "DungeonRoom.h"
#include "utils.h"
#include "Chest.h"
#include "SoundEffect.h"
class DungeonGenerator
{
public:
	enum class Boss
	{
		Minotaur
		, Cyclops
		, Santa
	};
	DungeonGenerator(const Window& window,Boss currentBoss,int volume);
	~DungeonGenerator();
	DungeonGenerator(const DungeonGenerator& other) = delete;
	DungeonGenerator& operator=(const DungeonGenerator& rhs) = delete;
	DungeonGenerator(const DungeonGenerator&& other) = delete;
	DungeonGenerator& operator=(const DungeonGenerator&& rhs) = delete;
	void Draw()const;
	void Update(float elapsedSec,Rectf &actorShape);
	DungeonRoom* GetCurrentRoom(const Rectf& actorShape);
	bool IsInDoor(const Rectf& actorShape);
	bool IsInDoorBack(const Rectf& actorShape);
	bool IsInBossDoor(const Rectf& actorShape);
	void CheckChest(Hero& hero);
	void DoCollision(bool doCollosion);
	Boss GetCurrentBoss();
	void HitEnemys(Spell*& spell);
	bool HitCharacterReturnDead(Hero& hero, float elapsedSec, float damage);
	void ChangeVolume(int newVolume);
private:
	SoundEffect m_Buy;
	int m_Volume;
	std::vector<Rectf>m_LayOutShapes;
	std::vector<Rectf>m_RoomShapes;
	std::vector<Rectf>m_HallwayShapesx;
	std::vector<Rectf>m_HallwayShapesy;
	std::vector<Rectf>m_DoorsTopRight;
	std::vector<Rectf>m_DoorsDownLeft;
	std::vector<DungeonRoom*>m_Rooms;
	std::vector<DungeonRoom*>m_HallwaysHorizontal;
	std::vector<DungeonRoom*>m_HallwaysVertical;
	std::vector<Barrel*>m_Boxes;
	BasicBolt* m_pFakeSpell;
	Texture m_HallwayImage;
	Texture m_BossDoorImage;
	Boss m_CurrentBoss;
	Rectf m_BossDoorShape;
	Texture m_DoorImage;
	Texture m_LockedDoorImage;
	Texture* m_pPillarImage;
	Texture* m_pInteractiblesSprite;
	Chest* m_pChestImage;
	int m_DungeonSize;
	float m_Scale;
	float m_DoorWidth;
	float m_DoorHeight;
	bool m_IsFighting;
	void MakeRooms(const Window& window);
	void MakeHallWays();
	
};
