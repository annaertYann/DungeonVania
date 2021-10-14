#pragma once
#include "structs.h"
#include "Camera.h"
#include "Texture.h"
#include "Room.h"
#include "SpellManager.h"
#include "Vector2f.h"
#include "SoundEffect.h"
class Hero
{
public:
	enum class State
	{
	idle
	,movingLeft
	,movingRight
	,attacking
	,transition
	,hit
	,dead
	};
	enum class BoltType
	{
	blue
	,green
	,red
	};
	Hero(Point2f screenCenter);
	~Hero();
	
	void Update(float elapsedsec, const Room* currentRoom);
	void Draw()const;
	bool SetAttack(const Point2f& pos, float scale, const Vector2f& velocity);
	Rectf& GetShape();
	SpellManager& GetManager();
	void Transition(const Point2f& pos, float distanceVertical,float distanceHorizontal);
	bool CheckHitReturnDead(const Rectf& hitBox, float elapsedSec, float damage);
	void Teleport(float distanceHorizontal,float distanceVertical);
	void SetIsInDoor(bool isInDoor);
	void LockCamera();
	void LockBorderCamera(bool IsLocked);
	void FixPosInCamera();
	void SkipShootingOnce();
	Point2f GetCameraPos();
	float GetHealth();
	int GetCoins();
	void SetCoins(int x);
	bool GetIsFemale();
	BoltType GetCurrentType();
	 void SetCurrentType(Hero::BoltType boltType);
	 void ChangeVolume(int newVolume);
	 void SetOptions(int newHealth,bool female);
private:
	bool m_IsCameraLocked;
	bool m_IsSkipShootingOnce;
	bool m_IsDead;
	bool m_IsDoneAttacking;
	bool m_IsDoneTransition;
	bool m_IsDoneHit;
	bool m_IsDoneDead;
	bool m_IsInDoor;
	float m_Health;
	int m_Coins;
	Rectf m_DestRect;
	Rectf m_SourceRect;
	Rectf m_HitBox;
	Point2f m_PositionInCamera;
	Point2f m_CameraPos;
	const Point2f m_WindowMiddle;
	float m_Movementspeed;
	Camera m_Camera;
	Texture m_WitchSprite;
	
	bool m_IsFemale;
	BoltType m_CurrentBoltType;
	int m_CurrentFrame;
	float m_AccumulatedSec;
	float m_InvinicbleTimer;
	Point2f m_PreviousPosInCamera;
	State m_CurrentState;
	State m_PreviousState;
	Point2f m_PreviousPos;
	bool m_IsInteracting;
	int m_SpriteRow;
	int m_SpriteColloms;
	float m_TranistionScale;
	float m_InvincibleScale;
	SoundEffect m_BasicBoltSound;

	SoundEffect m_Dying;
	SpellManager m_SpellManager;
	
	void UpdateMovement(float elapsedsec, const  Room*& currentroom);
	void UpdateSprite(float elapsedsec);
	void UpdateTransition(float elapsedsec);
};