#pragma once
#include "Room.h"
#include "Minotaur.h"
#include "Enemy.h"
#include "Barrel.h"
class BossRoom:public Room
{
public:
	enum class CurrentEnemy
	{
		Minotaur
		,Cyclops
		,Santa
	};
	BossRoom(float scale,int volume, BossRoom::CurrentEnemy currentEnemy,const Point2f& pos = Point2f{});
	~BossRoom();
	BossRoom(const BossRoom& other) = delete;
	BossRoom& operator=(const BossRoom& rhs) = delete;
	BossRoom(const BossRoom&& other) = delete;
	BossRoom& operator=(const BossRoom&& rhs) = delete;
	bool IsActorInDoor(const Rectf& actorShape);
	void Draw()const;
	bool CheckEnemy(Rectf hitBox);
	void Update(float elapsedsec,const Rectf& actorShape);
	void DoDamage(float damage);
	std::vector<Spell*> &GetSpells();
	bool getMinotaurDead();
	CurrentEnemy GetCurrentEnemy();
	Rectf GetShapeEnemy();
	void ChangeVolume(int newVolume);
private:
	int m_Volume;
	CurrentEnemy m_currentEnemy;
	float m_IntroAccuSec;
	Rectf m_Door;
	Texture m_Room;
	Texture m_RoomFinished;
	Texture m_RoomIntro;
	Texture m_RoomIntro1;
	Texture m_RoomIntro2;
	Enemy* m_pEnemy;
	std::vector<Spell*> m_EmptyManager;
	bool m_IsInDoor;
	bool m_IsFinished;
	bool m_IsEnemySpawned;
	bool m_IsMinotaurDead;
	void UpdateIsInDoor(const Rectf& actorShape);
	void UpdateEnemy(float elapsedSec,const Rectf& actorShape);
	void UpdateIntro(float elapsedSec);
	
};
