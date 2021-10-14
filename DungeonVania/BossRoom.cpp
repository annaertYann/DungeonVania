#include "pch.h"
#include "BossRoom.h"
#include "Cyclops.h"
#include "Room.h"
#include "utils.h"
#include "SpellManager.h"
#include "Santa.h"
BossRoom::BossRoom(float scale,int volume, BossRoom::CurrentEnemy currentEnemy, const Point2f& pos)
	:Room::Room(scale,pos)
	, m_Door{  }
	, m_IsFinished{ false }
	, m_IsInDoor{ false }
	, m_pEnemy{nullptr }
	, m_Room{ "Resources/Room1.png" }
	, m_RoomFinished{ "Resources/Room0.png" }
	,m_RoomIntro{ "Resources/MinotaurIntro.png" }
	,m_RoomIntro1{"Resources/CyclopsIntro.png"}
	, m_RoomIntro2{ "Resources/SantaIntro.png" }
	,m_IntroAccuSec{}
	,m_IsEnemySpawned{false}
	,m_IsMinotaurDead{false}
	,m_currentEnemy{currentEnemy}
	,m_Volume{volume}
{
	m_Vertices.push_back(Point2f{ 0,0 });
	m_Vertices.push_back(Point2f{ m_Room.GetWidth() * m_Scale,0 });
	m_Vertices.push_back(Point2f{ m_Room.GetWidth() * m_Scale,m_Room.GetHeight() * m_Scale });
	m_Vertices.push_back(Point2f{ 0,m_Room.GetHeight() * m_Scale });
	m_Vertices.push_back(Point2f{ 0,0 });
	for (Point2f element : m_Vertices)
	{
		element.x += pos.x;
		element.y += pos.y;
	}
	m_Door = Rectf{ ((m_Room.GetWidth() * m_Scale) / 2) - 40 * m_Scale,((m_Room.GetHeight() * m_Scale)) - 80 * m_Scale     ,80 * m_Scale,80 * m_Scale };
}
BossRoom::~BossRoom()
{
	if (m_pEnemy !=nullptr)
	{
		delete m_pEnemy;
		m_pEnemy = nullptr;
	}
	
}
bool BossRoom::IsActorInDoor(const Rectf& actorshape)
{
	if (utils::IsOverlapping(actorshape, m_Door) == true && m_IsFinished == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void BossRoom::Draw()const
{
	//draw Room /RoomFinished
	if (m_IsFinished == false)
	{
		m_Room.Draw(Rectf{ m_Position.x,m_Position.y,m_Room.GetWidth() * m_Scale,m_Room.GetHeight() * m_Scale }, Rectf{ 0,0,m_Room.GetWidth(),m_Room.GetHeight() });
	}
	else
	{
		m_RoomFinished.Draw(Rectf{ m_Position.x,m_Position.y,m_Room.GetWidth() * m_Scale,m_Room.GetHeight() * m_Scale }, Rectf{ 0,0,m_Room.GetWidth(),m_Room.GetHeight() });
		if (m_IsInDoor == true)
		{
			utils::SetColor(Color4f(1, 0, 0, 1));
			utils::DrawRect(m_Door);
		}
	}
	//draw enemy
	if (m_pEnemy != nullptr)
	{
		m_pEnemy->Draw();
	}
	//draw intro
	if (m_IntroAccuSec<3)
	{
		switch (m_currentEnemy)
		{
		case BossRoom::CurrentEnemy::Minotaur:
			m_RoomIntro.Draw(Rectf{ (m_Room.GetWidth() * m_Scale) / 4,0,640 * m_Scale	,400 * m_Scale }, Rectf{ 0,0,1920,1080 });
			break;
		case BossRoom::CurrentEnemy::Cyclops:
			m_RoomIntro1.Draw(Rectf{ (m_Room.GetWidth() * m_Scale) / 4,0,640 * m_Scale	,400 * m_Scale }, Rectf{ 0,0,1920,1080 });
			break;
		case BossRoom::CurrentEnemy::Santa:
			m_RoomIntro2.Draw(Rectf{ (m_Room.GetWidth() * m_Scale) / 4,0,640 * m_Scale	,400 * m_Scale }, Rectf{ 0,0,1920,1080 });
			break;
		default:
			break;
		}
		
	}
}
void BossRoom::ChangeVolume(int newVolume)
{

	if (m_pEnemy != nullptr)
	{
		m_pEnemy->ChangeVolume(newVolume);
	}
}
bool BossRoom::CheckEnemy(Rectf hitbox)
{
	if (m_pEnemy != nullptr)
	{
	
		if (m_pEnemy->IsHit(hitbox) == true||m_pEnemy==nullptr)
		{
			return true;
		}
		if (m_pEnemy->GetIsDead() == true)
		{
			m_IsMinotaurDead = true;
			delete m_pEnemy;
			m_pEnemy = nullptr;
			return true;
		}

		
		
	}
	return false;
}
void BossRoom::Update(float elapsedsec,const Rectf &actorShape)
{
	UpdateIntro(elapsedsec);
	UpdateEnemy(elapsedsec, actorShape);
	UpdateIsInDoor(actorShape);
	if (m_pEnemy != nullptr)
	{


		if (m_pEnemy->GetIsDead() == true)
		{
			m_IsMinotaurDead = true;
			delete m_pEnemy;
			m_pEnemy = nullptr;
		}

	}
}
void BossRoom::UpdateIsInDoor(const Rectf& actorShape)
{
	if (utils::IsOverlapping(actorShape, m_Door) == true)
	{
		m_IsInDoor = true;
	}
	else
	{
		m_IsInDoor = false;
	}
}
void BossRoom::UpdateEnemy(float elapsedSec,const Rectf& actorShape)
{
	//updateEnemy
	if (m_pEnemy != nullptr)
	{
		m_pEnemy->Update(elapsedSec, Point2f{ actorShape.left,actorShape.bottom });
	}

	//deleteEnemypells
	if (m_pEnemy != nullptr)
	{
		std::vector <Spell*>& temp{ BossRoom::GetSpells() };
		for (size_t i{}; i < temp.size(); i++)
		{
			if (temp[i] != nullptr && temp[i]->GetType() != Spell::SpellType::laser)
			{	
				if (temp[i]!=nullptr)
				{
					Rectf tempRect{ temp[i]->GetShape() };
					if (CollisionDetection(tempRect) == true)
					{
						delete temp[i];
						temp[i] = nullptr;
					}
				}
			
				

			}
		}
	}
}
void BossRoom::UpdateIntro(float elapsedSec)
{
	if (m_IntroAccuSec < 3)
	{
		m_IntroAccuSec += elapsedSec;
		if (m_IntroAccuSec >= 3)
		{
			switch (m_currentEnemy)
			{
			case BossRoom::CurrentEnemy::Minotaur:
				m_pEnemy = new MinoTaur{ m_Scale };
				ChangeVolume(m_Volume);
				break;
			case BossRoom::CurrentEnemy::Cyclops:
				m_pEnemy = new Cyclops{ m_Scale };
				ChangeVolume(m_Volume);
				break;
			case BossRoom::CurrentEnemy::Santa:
				m_pEnemy = new Santa{ m_Scale };
				ChangeVolume(m_Volume);
				break;
			default:
				break;
			}
			
		}
	}
}
void BossRoom::DoDamage(float damage)
{
	if (m_pEnemy!=nullptr)
	{
		m_pEnemy->DoDamage(damage);
		if (m_pEnemy->GetHealth()<=0)
		{
			m_IsFinished = true;
		}
	}
}
std::vector<Spell*>& BossRoom::GetSpells()
{
	if (m_pEnemy != nullptr)
	{
		return m_pEnemy->GetSpells();
	}
	else
	{
		return m_EmptyManager;
	}
}
bool BossRoom ::getMinotaurDead()
{
	return m_IsMinotaurDead;
}
BossRoom::CurrentEnemy BossRoom::GetCurrentEnemy()
{
	return m_currentEnemy;
}
Rectf BossRoom::GetShapeEnemy()
{
	if (m_pEnemy != nullptr) 
	{
		return m_pEnemy->GetShape();
	}
	else
	{
		//non existing hitbox
		return Rectf{ -10,-10,-10,-10 };
	}
	
}