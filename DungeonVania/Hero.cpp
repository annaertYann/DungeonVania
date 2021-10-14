#include "pch.h"
#include "Hero.h"
#include "utils.h"
#include "Room.h"

Hero::Hero(Point2f screenCenter)
	:m_Camera{screenCenter}
	,m_Movementspeed{( ((screenCenter.x * 2) / 640 )*200)}
	,m_DestRect{screenCenter.x,screenCenter.y,((screenCenter.x*2)/640 )*64,((screenCenter.x * 2) / 640) * 64 }
	,m_SourceRect{0,-32,32,32}
	,m_PositionInCamera{screenCenter}
	, m_WitchSprite{ "Resources/Hero Knight/Temp.png" }
	,m_CurrentFrame{}
	,m_AccumulatedSec{}
	,m_Health{100}
	,m_CurrentState{Hero::State::idle}
	,m_IsDoneAttacking{true}
	,m_PreviousState{Hero::State::movingRight}
	,m_IsDoneTransition{true}
	,m_SpriteRow{}
	,m_SpriteColloms{4}
	,m_WindowMiddle{screenCenter}
	,m_SpellManager{}
	,m_InvinicbleTimer{}
	,m_IsDead{}
	,m_IsDoneDead{true}
	, m_IsDoneHit{ true }
	,m_HitBox{}
	,m_TranistionScale{1}
	,m_InvincibleScale{0}
	,m_IsInDoor{false}
	,m_IsCameraLocked{true}
	,m_PreviousPosInCamera{}
	,m_IsInteracting{}
	,m_PreviousPos{}
	,m_IsSkipShootingOnce{false}
	,m_CameraPos{}
	,m_Coins{1}
	,m_CurrentBoltType{Hero::BoltType::blue}
	,m_BasicBoltSound{"Resources/Sound/BasicBolt.mp3"}
	,m_Dying{"Resources/Sound/Dying.mp3"}
	, m_IsFemale{ false }
{
	m_HitBox = Rectf{ m_DestRect.left,m_DestRect.bottom,m_DestRect.width,m_DestRect.height - 10 * ((m_WindowMiddle.x * 2) / 640) };
	m_BasicBoltSound.SetVolume(50);
	m_Dying.SetVolume(50);

	
}
Hero::~Hero()
{

}
Rectf& Hero::GetShape()
{
	return m_DestRect;
}
void Hero::ChangeVolume(int newvolume)
{
	m_BasicBoltSound.SetVolume(newvolume);
	m_Dying.SetVolume(newvolume);
}
void Hero::Update(float elapsedsec, const Room* currentroom)
{
	m_SpellManager.Update(elapsedsec);
	m_SourceRect.bottom = m_SpriteRow * m_SourceRect.height;
	if (m_CurrentState != Hero::State::attacking&&m_CurrentState!=Hero::State::transition&&m_CurrentState!=Hero::State::hit&&m_CurrentState!=Hero::State::dead)
	{
		
		UpdateMovement(elapsedsec, currentroom);
		
	}
	
		switch (m_CurrentState)
		{
		case Hero::State::idle:
			m_SpriteColloms = 4;
			m_SpriteRow = 1;
			UpdateSprite(elapsedsec);
			break;
		case Hero::State::movingLeft:
			m_SpriteColloms = 8;
			m_SpriteRow = 2;
			UpdateSprite(elapsedsec);
			break;
		case Hero::State::movingRight:
			m_SpriteColloms = 8;
			m_SpriteRow = 2;
			UpdateSprite(elapsedsec);
			break;
		case Hero::State::attacking:
			m_SpriteColloms = 5;
			m_SpriteRow = 3;
			m_IsDoneAttacking = false;
			UpdateSprite(elapsedsec);
			break;
		case Hero::State::transition:
			UpdateTransition(elapsedsec);
			break;
		case Hero::State::hit:
			UpdateSprite(elapsedsec);
		case Hero::State::dead:
			UpdateSprite(elapsedsec);
		default:
			break;
		}
	
	m_AccumulatedSec += elapsedsec;
	m_InvinicbleTimer += elapsedsec;
	if (m_InvincibleScale>0&&m_IsDead!=true)
	{
		m_InvincibleScale -= 0.01f;
	}
}
void Hero::UpdateMovement(float elapsedsec, const Room*& currentroom)
{
	m_PreviousPosInCamera = m_PositionInCamera;
	m_PreviousPos = Point2f{ m_DestRect.left,m_DestRect.bottom };
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	m_CurrentState = Hero::State::idle;

	if (pStates[SDL_SCANCODE_W])
	{
	
		m_DestRect.bottom += elapsedsec * m_Movementspeed;
		if (m_PreviousState == Hero::State::movingLeft)
		{
			m_CurrentState = Hero::State::movingLeft;
		}
		else
		{
			m_CurrentState = Hero::State::movingRight;
		}
		if (!m_IsCameraLocked)
		{
			if (m_DestRect.bottom <= currentroom->GetBorder(Room::Border::top) - m_DestRect.height || m_IsInDoor)
			{
				if (!m_IsInteracting)
				{
					m_PositionInCamera.y += elapsedsec * m_Movementspeed;
					m_Camera.MoveUp(m_PositionInCamera, elapsedsec * m_Movementspeed, m_DestRect, currentroom->GetBorder(Room::Border::top));
				}
			}
		}
		else
		{
			m_PositionInCamera.y += elapsedsec * m_Movementspeed;
		}
	}
	if
		(pStates[SDL_SCANCODE_S])
	{
		
	
		if (m_PreviousState == Hero::State::movingLeft)
		{
			m_CurrentState = Hero::State::movingLeft;
		}
		else
		{
			m_CurrentState = Hero::State::movingRight;
		}
		m_DestRect.bottom -= elapsedsec * m_Movementspeed;
		if (!m_IsCameraLocked)
		{
			if (m_DestRect.bottom >= currentroom->GetBorder(Room::Border::down) || m_IsInDoor)
			{
				if (!m_IsInteracting)
				{
					m_PositionInCamera.y -= elapsedsec * m_Movementspeed;
					m_Camera.MoveDown(m_PositionInCamera, elapsedsec * m_Movementspeed, m_DestRect, currentroom->GetBorder(Room::Border::down));
				}
			}
		}
		else
		{
			m_PositionInCamera.y -= elapsedsec * m_Movementspeed;
		}
	}
	
	if (pStates[SDL_SCANCODE_A])
	{
		
		m_CurrentState = Hero::State::movingLeft;
		m_PreviousState = Hero::State::movingLeft;
		m_DestRect.left -= elapsedsec * m_Movementspeed;
		if (!m_IsCameraLocked)
		{
			if (m_DestRect.left >= currentroom->GetBorder(Room::Border::left) || m_IsInDoor)
			{
				if (!m_IsInteracting)
				{
					m_PositionInCamera.x -= elapsedsec * m_Movementspeed;
					m_Camera.MoveLeft(m_PositionInCamera, elapsedsec * m_Movementspeed, m_DestRect, currentroom->GetBorder(Room::Border::left));
				}
			}
		}
		else
		{
			m_PositionInCamera.x -= elapsedsec * m_Movementspeed;
		}
	}
	
	if
		(pStates[SDL_SCANCODE_D])
	{
	
		m_CurrentState = Hero::State::movingRight;
		m_PreviousState = Hero::State::movingRight;
		m_DestRect.left += elapsedsec * m_Movementspeed;
		if (!m_IsCameraLocked)
		{
			if (m_DestRect.left <= currentroom->GetBorder(Room::Border::right) - m_DestRect.width || m_IsInDoor)
			{
				if (!m_IsInteracting)
				{
					m_PositionInCamera.x += elapsedsec * m_Movementspeed;
					m_Camera.MoveRight(m_PositionInCamera, elapsedsec * m_Movementspeed, m_DestRect, currentroom->GetBorder(Room::Border::right));
				}
			}
		}
		else
		{
			m_PositionInCamera.x += elapsedsec * m_Movementspeed;
		}
		
	}

	
	m_HitBox = Rectf{ m_DestRect.left,m_DestRect.bottom,m_DestRect.width,m_DestRect.height - 20 * ((m_WindowMiddle.x * 2) / 640) };
	m_IsInteracting = false;
}
void Hero::SetIsInDoor(bool x)
{
	m_IsInDoor = x;
}
void Hero::UpdateSprite(float elapsedsec)
{
	float idleFramerate{0.1f};
	if (m_AccumulatedSec > idleFramerate)
	{
		m_CurrentFrame++;
		
		if (m_CurrentFrame * m_SourceRect.width >=(m_SpriteColloms-1)*m_SourceRect.width)
		{
			m_CurrentFrame = 0;
			m_IsDoneAttacking = true;
			m_IsDoneHit = true;
			m_IsDoneDead = true;
			if (m_CurrentState==Hero::State::dead)
			{
				m_IsDead = true;
			}
			m_CurrentState = Hero::State::idle;
			
		}
		m_SourceRect.left = m_CurrentFrame * m_SourceRect.width;
		m_AccumulatedSec = 0;
	}
}
void Hero::UpdateTransition(float elapsedsec)
{
	
	if (m_AccumulatedSec<6.f)
	{
		m_TranistionScale -= 0.01f;
		m_AccumulatedSec += elapsedsec;
	}
	else
	{
		m_TranistionScale = 1;
		m_CurrentState = Hero::State::idle;
		m_AccumulatedSec = 0;
	}


}
void Hero::Draw()const
{
	
	m_SpellManager.Draw();
	switch (m_CurrentState)
	{
	case Hero::State::idle:
		if (m_PreviousState == Hero::State::movingLeft)
		{
			glPushMatrix();
			glTranslatef(2 * m_DestRect.left +  m_DestRect.width, 0, 0);
			glScalef(-1, 1, 1);
			m_WitchSprite.Draw(m_DestRect, m_SourceRect);
			glPopMatrix();
		}
		else
		{
			m_WitchSprite.Draw(m_DestRect, m_SourceRect);
		}
		break;
	case Hero::State::movingLeft:
		glPushMatrix();
		glTranslatef(2 * m_DestRect.left + m_DestRect.width, 0, 0);
		glScalef(-1, 1, 1);
		m_WitchSprite.Draw(m_DestRect, m_SourceRect);
		glPopMatrix();
		break;
	case Hero::State::movingRight:
		m_WitchSprite.Draw(m_DestRect, m_SourceRect);
		break;
	case Hero::State::attacking:
		if (m_PreviousState == Hero::State::movingLeft)
		{
			glPushMatrix();
			glTranslatef(2 * m_DestRect.left +m_DestRect.width, 0, 0);
			glScalef(-1, 1, 1);
			m_WitchSprite.Draw(m_DestRect, m_SourceRect);
			glPopMatrix();
		}else
		{
			m_WitchSprite.Draw(m_DestRect, m_SourceRect);
		}
		break;
	case Hero::State::transition:
		utils::SetColor(Color4f(0, 0, 0, m_TranistionScale));
		utils::FillRect(Rectf{0,0,2560,1600});
		break;
	case Hero::State::hit:
		if (m_PreviousState == Hero::State::movingLeft)
		{
			glPushMatrix();
			glTranslatef(2 * m_DestRect.left + m_DestRect.width, 0, 0);
			glScalef(-1, 1, 1);
			m_WitchSprite.Draw(m_DestRect, m_SourceRect);
			glPopMatrix();
		}
		else
		{
			m_WitchSprite.Draw(m_DestRect, m_SourceRect);
		}
		break;
	case Hero::State::dead:
		if (m_PreviousState == Hero::State::movingLeft)
		{
			glPushMatrix();
			glTranslatef(2 * m_DestRect.left + m_DestRect.width, 0, 0);
			glScalef(-1, 1, 1);
			m_WitchSprite.Draw(m_DestRect, m_SourceRect);
			glPopMatrix();
		}
		else
		{
			m_WitchSprite.Draw(m_DestRect, m_SourceRect);
		}
		break;
	default:
		break;
	}
}
bool Hero::SetAttack(const Point2f& pos, float scale, const Vector2f& velocity)
{
	
	if (!m_IsSkipShootingOnce)
	{
		if (m_IsDoneAttacking == true && m_IsDoneDead == true && m_IsDoneHit == true && m_IsDoneTransition == true && m_CurrentState != Hero::State::transition)
		{
			m_BasicBoltSound.Play(0);
			m_CurrentFrame = 0;
			m_CurrentState = Hero::State::attacking;
			if (m_CurrentBoltType == Hero::BoltType::blue)
			{
				m_SpellManager.AddSpell(SpellManager::SpellType::basicBolt, pos, scale, velocity, m_PositionInCamera);
			}
			if (m_CurrentBoltType == Hero::BoltType::green)
			{
				m_SpellManager.AddSpell(SpellManager::SpellType::green , pos, scale, velocity, m_PositionInCamera);
			}
			if (m_CurrentBoltType == Hero::BoltType::red)
			{
				m_SpellManager.AddSpell(SpellManager::SpellType::red, pos, scale, velocity, m_PositionInCamera);
			}
			if (velocity.x < m_PositionInCamera.x)
			{
				m_PreviousState = Hero::State::movingLeft;
			}
			else
			{
				m_PreviousState = Hero::State::movingRight;
			}
		}
		m_IsDoneAttacking = false;
	}
	else
	{
		m_IsSkipShootingOnce = false;
	}
	return m_IsDoneAttacking;
}
void Hero::Transition(const Point2f& pos,float distancey,float distancex)
{
	m_CurrentState = Hero::State::transition;
	glLoadIdentity();
	m_Camera.SetCameraPos(Point2f{ 0,0 });
	m_PositionInCamera.x =m_WindowMiddle.x;
	m_PositionInCamera.y =m_WindowMiddle.y;
	m_DestRect.bottom = pos.y;
	m_DestRect.left = pos.x;
	glTranslatef(-distancex, -distancey, 0);
	m_Camera.SetCameraPos(Point2f{ m_Camera.GetCameraPos().x+distancex,m_Camera.GetCameraPos().y+distancey });
	m_AccumulatedSec = 0;
}
SpellManager& Hero::GetManager()
{
	return m_SpellManager;
	
}
bool Hero::CheckHitReturnDead(const Rectf& hitbox,float elapsedSec,float damage)
{
	
	if (m_InvinicbleTimer >= 3.f)
	{
		if (utils::IsOverlapping(m_HitBox, hitbox))
		{
			m_InvincibleScale = 1;
			m_Health -= damage;
			m_InvinicbleTimer = 0;
			m_SpriteRow = 4;
			m_SpriteColloms = 3;
			if (m_IsDoneHit==true)
			{
				m_IsDoneHit = false;
				m_CurrentFrame = 0;
			}
			m_CurrentState = Hero::State::hit;
		}
	}

	if (m_Health<=0)
	{
		m_Dying.Play(0);
		
		m_CurrentState = Hero::State::dead;
		m_SpriteRow = 5;
		m_SpriteColloms = 7;
		if (m_IsDoneDead==true)
		{
			m_IsDoneDead = false;
			m_CurrentFrame = 0;
		}
	}
	if (m_IsDead==true)
	{
		m_IsDead = false;
		m_Health = 100;
		m_InvincibleScale = 0;
		return true;
	}
	else
	{
		return false;
	}
	
}
void Hero::Teleport(float distanceX, float distanceY)
{
	m_DestRect.left += distanceX;
	m_DestRect.bottom += distanceY;
	glTranslatef(-distanceX,-distanceY,0);
	m_Camera.SetCameraPos(Point2f{ m_Camera.GetCameraPos().x + distanceX,m_Camera.GetCameraPos().y + distanceY });
	
}
void Hero::LockCamera()
{
	if(m_IsCameraLocked)
	{
		m_IsCameraLocked = false;
	}
	else
	{
		m_IsCameraLocked = true;
	}
}
void Hero::LockBorderCamera(bool isLocked)
{
	m_Camera.SetCameraBorderLock(isLocked);
}
void Hero::FixPosInCamera()
{
	m_IsInteracting = true;
	m_PositionInCamera = m_PreviousPosInCamera;
	m_DestRect.left = m_PreviousPos.x;
	m_DestRect.bottom = m_PreviousPos.y;
}
void Hero::SkipShootingOnce()
{
	m_IsSkipShootingOnce = true;
}
Point2f Hero::GetCameraPos()
{
	return m_Camera.GetCameraPos();
}
float Hero::GetHealth()
{
	return m_Health;
}
int Hero::GetCoins()
{
	return m_Coins;
}
void Hero::SetCoins(int x)
{
	m_Coins = x;
}
Hero::BoltType Hero::GetCurrentType()
{
	return m_CurrentBoltType;
}
void Hero::SetCurrentType(Hero::BoltType currentType)
{
	m_CurrentBoltType = currentType;
}
void Hero::SetOptions(int newHealth,bool female)
{
	m_Health = float (newHealth);
	if (female)
	{
		m_WitchSprite = Texture{ "Resources/Hero Knight/Tempf.png" };
	}
}
bool Hero::GetIsFemale()
{
	return m_IsFemale;
}