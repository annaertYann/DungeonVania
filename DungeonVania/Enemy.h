#pragma once
#include "Texture.h"
#include "SpellManager.h"
#include "SoundEffect.h"
class Enemy
{
public:
	Enemy(float scale,int collomsIdle);
	virtual ~Enemy();
	virtual void Update(float elapsedsec, const Point2f& destination);
	virtual void Draw()const=0;
	bool IsHit(const Rectf& hitBox);
	void DoDamage(float damage);
	bool GetIsDead();
	std::vector<Spell*>& GetSpells();
	float GetHealth();
	Rectf GetShape();
	void SetPreviousPos();
	void ChangeVolume(int newVolume);
protected:
	SoundEffect m_FireBall;
	SoundEffect m_Laser;
	SoundEffect m_Dying;
	SoundEffect m_Jump;
	Point2f m_PreviousPos;
	SpellManager m_Manager;
	Rectf m_DestRect;
	Rectf m_SourceRect;
	Rectf m_HitBox;
	bool m_HasMovementFreePass;
	bool m_IsHit;
	bool m_IsGoingRight;
	bool m_IsAttacking;
	bool m_IsDead;
	bool m_IsinDeadAnimation;
	bool m_StopMoving;
	int m_CurrentRow;
	int m_Colloms;
	int m_CollomsIdle;
	float m_MovementSpeed;
	float m_Health;
	float m_Scale;
	float m_AccuSec;
	float m_AttackAccuSec;
	float m_AccuSecAnimation;
	float m_Width;
	float m_Height;
	//funtions
	virtual void UpdateSprite(float elapsedSec);
	virtual void Attack(float elapsedSec, float scale, const Point2f& destination)=0;
	void MoveEnemy(float elapsedsec, const Point2f& destination);
	void UpdateHitBox();
};

