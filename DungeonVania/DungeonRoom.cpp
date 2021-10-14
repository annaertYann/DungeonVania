#include "pch.h"
#include "DungeonRoom.h"
#include "WitchBasic.h"
#include "utils.h"
#include "Dwarf.h"
#include "Imp.h"
DungeonRoom::DungeonRoom(float scale,const Rectf &shape,bool isHallway,int volume)
	:Room::Room(scale, Point2f{shape.left,shape.bottom})
	, m_RoomImage{ "Resources/RoomNotClear.png" }
	,m_RoomShape{shape}
	,m_IsHallway{isHallway}
	,m_EnemySpawned{false}
	,m_HasBeenEntered{false}
	,m_IsClear{true}
	,m_InteraciblesSpawned{false}
	,m_Volume{volume}
{
	if (isHallway)
	{
		m_IsClear = true;
		m_HasBeenEntered = true;
		Point2f point{ shape.left,shape.bottom };
		m_Vertices.push_back(Point2f{ point.x+20,point.y + 20 });
		m_Vertices.push_back(Point2f{ (point.x + m_RoomShape.width * scale)-20,point.y + 20 });
		m_Vertices.push_back(Point2f{ (point.x + m_RoomShape.width * scale)-20,(point.y + m_RoomShape.height * scale)-20 });
		m_Vertices.push_back(Point2f{ point.x+20,(point.y + m_RoomShape.height * scale)-20 });
		m_Vertices.push_back(Point2f{ point.x+20,point.y + 20 });
	}
	else
	{
		Point2f point{ shape.left,shape.bottom };
		m_Vertices.push_back(Point2f{ point.x,point.y  });
		m_Vertices.push_back(Point2f{ point.x + m_RoomShape.width * scale,point.y  });
		m_Vertices.push_back(Point2f{ point.x + m_RoomShape.width * scale,point.y + m_RoomShape.height * scale });
		m_Vertices.push_back(Point2f{ point.x,point.y + m_RoomShape.height * scale });
		m_Vertices.push_back(Point2f{ point.x,point.y });
	}
}
DungeonRoom::DungeonRoom(const DungeonRoom& room)
	:Room::Room(room.m_Scale, Point2f{ m_RoomShape.left,m_RoomShape.bottom })
	, m_RoomImage{ "Resources/RoomNotClear.png" }
	, m_RoomShape{ room.m_RoomShape }
	,m_IsHallway{room.m_IsHallway}
	,m_IsClear{room.m_IsClear}
	,m_HasBeenEntered{room.m_HasBeenEntered}
	,m_EnemySpawned{room.m_EnemySpawned}
	,m_InteraciblesSpawned{room.m_InteraciblesSpawned}
	,m_Volume{room.m_Volume}
{
	if(m_IsHallway)
	{
		Point2f point{ m_RoomShape.left,m_RoomShape.bottom };
		m_Vertices.push_back(Point2f{ point.x+20,point.y + 20 });
		m_Vertices.push_back(Point2f{ point.x-20 + m_RoomShape.width * m_Scale,point.y + 20 });
		m_Vertices.push_back(Point2f{ point.x-20 + m_RoomShape.width * m_Scale,point.y-20 + m_RoomShape.height * m_Scale });
		m_Vertices.push_back(Point2f{ point.x+20,point.y-20 + m_RoomShape.height * m_Scale });
		m_Vertices.push_back(Point2f{ point.x+20,point.y + 20 });
	}
	else
	{
		Point2f point{ m_RoomShape.left,m_RoomShape.bottom };
		m_Vertices.push_back(Point2f{ point.x,point.y  });
		m_Vertices.push_back(Point2f{ point.x + m_RoomShape.width * m_Scale,point.y });
		m_Vertices.push_back(Point2f{ point.x + m_RoomShape.width * m_Scale,point.y + m_RoomShape.height * m_Scale });
		m_Vertices.push_back(Point2f{ point.x,point.y + m_RoomShape.height * m_Scale });
		m_Vertices.push_back(Point2f{ point.x,point.y });
	}
}
DungeonRoom::~DungeonRoom()
{
	for (Enemy* element : m_Enemys)
	{
		if (element != nullptr)
		{
			delete element;
			element = nullptr;
		}
	}
	for (Barrel* element : m_Boxes)
	{
		if (element != nullptr)
		{
			delete element;
			element = nullptr;
		}
	}
	for (Pillar* element : m_Pillars)
	{
		if (element != nullptr)
		{
			delete element;
			element = nullptr;
		}
	}
}
void DungeonRoom::Draw()const
{
	m_RoomImage.Draw(m_RoomShape, Rectf{ 0,0,m_RoomImage.GetWidth(),m_RoomImage.GetHeight() });

	for (Barrel* element:m_Boxes)
	{
		if (element != nullptr)
		{
			element->Draw();
		}
	}
	for (Enemy* element : m_Enemys)
	{
		if (element != nullptr)
		{
			element->Draw();
		}

	}
	for (Pillar* element : m_Pillars)
	{
			element->Draw();
	}
}
void DungeonRoom::Update(float elepsedSec,const Point2f& destinationEnemy)
{
int counter{};
for(Enemy*& element:m_Enemys)
{
	if (element!=nullptr)
	{
		element->Update(elepsedSec, destinationEnemy);
		counter++;
		if (element->GetIsDead())
		{
			delete element;
			element = nullptr;
		}
	}
}
for (Barrel*& element : m_Boxes)
{
	if (element != nullptr)
	{
		element->update(elepsedSec);
	}
}
if (counter>0)
{
	m_IsClear = false;
}
else
{
	m_IsClear = true;
}
}
void DungeonRoom::MakeEnemy()
{
	if (m_HasBeenEntered&&!m_EnemySpawned)
	{
		m_EnemySpawned= true;
		for (int i{};i<((rand()%4)+1); i++)
		{
			if (rand() % 2 == 0)
			{
				m_Enemys.push_back(new Dwarf{ m_Scale,8 ,Point2f{m_RoomShape.left + (rand() % int(m_RoomShape.width-150*m_Scale)),m_RoomShape.bottom + rand() % int(m_RoomShape.height-100*m_Scale)} });
			}
			else if (rand() % 2 == 0)
			{
				m_Enemys.push_back(new WitchBasic{ m_Scale,8 ,Point2f{m_RoomShape.left + (rand() % int(m_RoomShape.width-150*m_Scale)),m_RoomShape.bottom + rand() % int(m_RoomShape.height-m_Scale)} });
			}
			else
			{
				m_Enemys.push_back(new Imp{ m_Scale,6 ,Point2f{m_RoomShape.left + (rand() % int(m_RoomShape.width-150*m_Scale)),m_RoomShape.bottom + rand() % int(m_RoomShape.height-100*m_Scale)} });
			}
			
		}
		ChangeVolume(m_Volume);
		m_IsClear = false;
	}
	
}
void DungeonRoom::MakeInteractibles(Texture* ptexure,Texture* pPillarTexture)
{
	if (m_HasBeenEntered && !m_InteraciblesSpawned)
	{
		m_InteraciblesSpawned = true;
		for (int i{}; i < ((rand() % 20) + 1); i++)
		{
			m_Boxes.push_back(new Barrel(ptexure, Point2f{ m_RoomShape.left + (rand() % int(m_RoomShape.width-200*m_Scale)),m_RoomShape.bottom + (rand() % int(m_RoomShape.height-200*m_Scale)) }, m_Scale));
		}
		for (int i{}; i < ((rand() % 5) + 1); i++)
		{
			m_Pillars.push_back(new Pillar{ Point2f{ m_RoomShape.left + (rand() % int(m_RoomShape.width - 88 * m_Scale)),m_RoomShape.bottom + (rand() % int(m_RoomShape.height - 160 * m_Scale)) }, pPillarTexture, m_Scale });
		}
	}


	//no spawn pillars into eachother
	std::vector<Rectf>shapes;
	for (Pillar* element : m_Pillars)
	{
		shapes.push_back(element->GetShape());
	}
	for (size_t i{}; i < shapes.size(); i++)
	{
		for (size_t k{}; k < shapes.size(); k++)
		{
			if (shapes[i].left != shapes[k].left)
			{
				if (utils::IsOverlapping(shapes[i], shapes[k]))
				{
					m_Pillars[i]->SetIsRemoved();
				}
			}
		}
	}
	//no spawn pillars into doors

}
void DungeonRoom::NoSpawnEnemy()
{
	m_EnemySpawned = true;
	m_InteraciblesSpawned = true;
}
bool DungeonRoom::GetIsClear()
{
	return m_IsClear;
}
void DungeonRoom::SetHasBeenEntered(bool hasBeenEntered)
{
	m_HasBeenEntered = hasBeenEntered;
}
bool DungeonRoom::CheckEnemy(Spell*& spell)
{
		for (Enemy*& pelement : m_Enemys)
			if (pelement != nullptr)
			{
				if (spell != nullptr)
				{
					if (pelement->IsHit(spell->GetShape()) == true)
					{
						if (pelement->GetHealth() > 0)
						{
							if (spell->ExplosionTime() == true)
							{
								pelement->DoDamage(0.4f);
								if (pelement->GetIsDead() == true)
								{
									delete pelement;
									pelement = nullptr;
									return true;
								}
								return true;
							}
						}
					}
				}
				if (pelement->GetIsDead() == true)
				{
					delete pelement;
					pelement = nullptr;
					return true;
				}


			}
		for (Barrel*& pelement : m_Boxes)
			if (spell != nullptr)
			{
				if (pelement != nullptr)
				{
					if (utils::IsOverlapping(spell->GetShape(), pelement->GetShape()))
					{
						if (spell->ExplosionTime())
						{
							if (spell != nullptr)
							{
								delete spell;
								spell = nullptr;
							}	
						}
					}
					if (spell != nullptr)
					{
						if (pelement->HitBarrelReturnDead(spell->GetShape()))
						{
							delete pelement;
							pelement = nullptr;

						}
					}
					else
					{
						if (pelement->HitBarrelReturnDead(Rectf{0,0,0,0}))
						{
							delete pelement;
							pelement = nullptr;

						}
					}
				}
			}
	
	return false;
}
bool DungeonRoom::HitCharacterReturnDead(Hero& hero, float elapsedSec, float damage)
{
	for (Enemy* pelement : m_Enemys)
	{
		if (pelement != nullptr)
		{
			for (Spell* spels : pelement->GetSpells())
			{
				if (spels != nullptr)
				{

					//hit when overlapping with spells
					if (hero.CheckHitReturnDead(spels->GetShape(), elapsedSec, damage))
					{
						//when hero dies
						return true;
					}
				}
			}
			//hit when overlapping with enemy
			if (utils::IsOverlapping(pelement->GetShape(), hero.GetShape()))
			{
				if (hero.CheckHitReturnDead(pelement->GetShape(), elapsedSec, damage))
				{
					//when hero dies
					return true;
				}
			}
		}
	}
	
	for (Barrel*& pelement : m_Boxes)
		
			if (pelement != nullptr)
			{
				if (pelement->HitBarrelReturnDead(hero.GetShape()))
				{
					delete pelement;
					pelement = nullptr;
				}
			}
	for (Pillar*& element : m_Pillars)
	{
		element->HandleCollosion(hero,m_Enemys);
	}
	return false;
}
void DungeonRoom::ChangeVolume(int newVolume)
{
	for (Enemy*& pElement : m_Enemys)
	{
		if (pElement != nullptr)
		{
			pElement->ChangeVolume(newVolume);
		}
	}
}