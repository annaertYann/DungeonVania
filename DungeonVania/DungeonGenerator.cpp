#include "pch.h"
#include "DungeonRoom.h"
#include "DungeonGenerator.h"

DungeonGenerator::DungeonGenerator(const Window& window,DungeonGenerator::Boss currentBoss,int volume)
	:m_DungeonSize{ (rand()%3)+2}
	,m_DoorImage{"Resources/Door.png"}
	,m_LockedDoorImage{ "Resources/LockedDoor.png" }
	,m_Scale{window.width/1280}
	, m_DoorHeight{170}
	,m_DoorWidth{150}
	,m_pInteractiblesSprite{ new Texture{"Resources/Interactibles.png"} }
	,m_BossDoorShape{}
	,m_CurrentBoss{currentBoss}
	,m_HallwayImage{"Resources/Hallway.png"}
	,m_BossDoorImage{"Resources/BossDoor.png"}
	,m_pFakeSpell{nullptr}
	,m_IsFighting{false}
	,m_pPillarImage{ new Texture{"Resources/Pillar.png"} }
	,m_pChestImage{}
	,m_Volume{volume}
	,m_Buy{"Resources/Sound/Buy.mp3"}
{
	MakeRooms(window);
	MakeHallWays();
	m_Buy.SetVolume(volume);
}
void DungeonGenerator::MakeRooms(const Window &window)
{

	//dungeonLayout
	int k{};
	for (int i{}; i < m_DungeonSize * m_DungeonSize; i++)
	{
		int j{ i / m_DungeonSize };
		m_LayOutShapes.push_back(Rectf{ k * window.width * 2,j * window.height * 2,window.width * 2,window.height * 2 });
		k++;
		if (k >= m_DungeonSize)
		{
			k = 0;
		}
	}
	//smaller roomshapes inside the bigger rectangles
	for (int i{}; i < m_DungeonSize * m_DungeonSize; i++)
	{
		float tempSizeDiffx{ (rand() % int(window.width) / 4) + window.width / 4 };
		float tempSizeDiffy{ (rand() % int(window.height) / 4) + window.height / 4 };
		m_RoomShapes.push_back(Rectf{ m_LayOutShapes[i].left + tempSizeDiffx,m_LayOutShapes[i].bottom + tempSizeDiffy,m_LayOutShapes[i].width - tempSizeDiffx * 2,m_LayOutShapes[i].height - tempSizeDiffy * 2 });

	}
	//actual DungeonRooms
	for (int i{}; i < m_DungeonSize * m_DungeonSize; i++)
	{
		DungeonRoom* temp{ new DungeonRoom {float(window.width / 1280), m_RoomShapes[i],false,m_Volume} };
		m_Rooms.push_back(temp);

	}
	//make bossdoor
	int temp{ rand() % int(m_Rooms.size()) };
	m_BossDoorShape = Rectf{ m_RoomShapes[temp].left + m_RoomShapes[temp].width / 2, m_RoomShapes[temp].bottom + m_RoomShapes[temp].height / 2, m_DoorWidth, m_DoorHeight };
	for (size_t i{}; i < m_Rooms.size(); i++)
	{
		if (utils::IsOverlapping(m_RoomShapes[i], m_BossDoorShape))
		{
			m_Rooms[i]->NoSpawnEnemy();
		}
		//make no enemy in first room
		m_Rooms[0]->NoSpawnEnemy();
	}
	//make chest room
	int x{ rand() % int(m_Rooms.size()) };
	while (x == temp)
	{
		x = rand() % int(m_Rooms.size());
	}
	m_Rooms[x]->NoSpawnEnemy();
	m_pChestImage = new Chest{ m_pInteractiblesSprite,Point2f{ m_RoomShapes[x].left + m_RoomShapes[x].width / 2, m_RoomShapes[x].bottom + m_RoomShapes[x].height / 2},m_Scale };

}
void DungeonGenerator::CheckChest(Hero& hero)
{
	if (utils::IsOverlapping(hero.GetShape(),m_pChestImage->GetShape()))
	{
		m_pChestImage->ChestHit();
		m_Buy.Play(0);
		hero.SetCoins(hero.GetCoins()+1);
	}
	
}
void DungeonGenerator::MakeHallWays()
{
	//makeRects
	int counterx{-1};
	int countery{-1};
	for (int i{}; i < m_DungeonSize * m_DungeonSize; i++)
	{
		//horizontal hallways
		if (i < (m_DungeonSize * m_DungeonSize)-1)
		{
			if (utils::IsOverlapping(m_RoomShapes[size_t (i) + 1], Rectf{ m_RoomShapes[i].left + m_LayOutShapes[i].width,m_RoomShapes[i].bottom,m_RoomShapes[i].width,m_RoomShapes[i].height }))
			{
				counterx++;
				m_HallwayShapesx.push_back(Rectf{ m_RoomShapes[i].left + m_RoomShapes[i].width    ,m_RoomShapes[i].bottom + m_RoomShapes[i].height/2    ,m_RoomShapes[size_t(i)+1].left-(m_RoomShapes[i].left+(m_RoomShapes[i].width))    ,m_RoomShapes[i].height / 4 });
				//make door in room
				m_DoorsTopRight.push_back(Rectf{ (m_RoomShapes[i].left + m_RoomShapes[i].width)-m_DoorWidth/2,(m_RoomShapes[i].bottom + m_RoomShapes[i].height / 2 )+ m_HallwayShapesx[counterx].height/2-m_DoorHeight/2,m_DoorWidth,m_DoorHeight});
				//make door in connected room
				m_DoorsDownLeft.push_back(Rectf{ (m_RoomShapes[i].left + m_RoomShapes[i].width) +m_HallwayShapesx[counterx].width -m_DoorWidth/2,(m_RoomShapes[i].bottom + m_RoomShapes[i].height / 2)+ m_HallwayShapesx[counterx].height/2-m_DoorHeight/2 ,m_DoorWidth,m_DoorHeight });
			}
		}
		//vertical hallways
		if (i < (m_DungeonSize * m_DungeonSize) - m_DungeonSize)
		{
			if (utils::IsOverlapping(m_RoomShapes[size_t(i) + m_DungeonSize], Rectf{ m_RoomShapes[i].left ,m_RoomShapes[i].bottom+m_RoomShapes[i].height,m_RoomShapes[i].width,m_RoomShapes[i].height }))
			{
				countery++;
				m_HallwayShapesy.push_back(Rectf{ m_RoomShapes[i].left + m_RoomShapes[i].width/2    ,m_RoomShapes[i].bottom + m_RoomShapes[i].height    ,m_RoomShapes[i].width/4    ,m_RoomShapes[size_t(i) + m_DungeonSize].bottom - (m_RoomShapes[i].bottom + (m_RoomShapes[i].height)) });
				//make door in room
				m_DoorsTopRight.push_back(Rectf{(m_RoomShapes[i].left +m_RoomShapes[i].width/2)+ m_HallwayShapesy[countery].width/2 -m_DoorWidth/2,(m_RoomShapes[i].bottom + m_RoomShapes[i].height)-m_DoorHeight/2 ,m_DoorWidth,m_DoorHeight });
				//make door in connected room
				m_DoorsDownLeft.push_back(Rectf{ (m_RoomShapes[i].left + m_RoomShapes[i].width / 2 )+ m_HallwayShapesy[countery].width/2-m_DoorWidth/2,(m_RoomShapes[i].bottom + m_RoomShapes[i].height) +m_HallwayShapesy[countery].height-m_DoorHeight/2 ,m_DoorWidth,m_DoorHeight });
			}
		}
	}

	//make doors in hallways
	for (Rectf element:m_HallwayShapesx)
	{
		m_HallwaysHorizontal.push_back(new DungeonRoom{ m_Scale,element,true,m_Volume });
	}
	for (Rectf element : m_HallwayShapesy)
	{
		m_HallwaysVertical.push_back(new DungeonRoom{ m_Scale,element,true,m_Volume });
	}
	


}
DungeonGenerator::~DungeonGenerator()
{
	for (int i{}; i < m_DungeonSize * m_DungeonSize; i++)
	{
		delete m_Rooms[i];
		m_Rooms[i] = nullptr;
	}
	for (DungeonRoom* element : m_HallwaysHorizontal)
	{
		delete element;
		element = nullptr;
	}
	for (DungeonRoom* element : m_HallwaysVertical)
	{
		delete element;
		element = nullptr;
	}
	delete m_pInteractiblesSprite;
	m_pInteractiblesSprite = nullptr;
	delete m_pPillarImage;
	m_pPillarImage = nullptr;
	delete m_pChestImage;
	m_pChestImage = nullptr;
}
void DungeonGenerator::Draw()const
{
		/*for (int i{}; i < m_DungeonSize * m_DungeonSize; i++)
		{
			utils::SetColor(Color4f{1,1,1,1});
			utils::FillRect(m_LayOut[i]);
			utils::SetColor(Color4f{ 0,0,0,1 });
			utils::DrawRect(m_LayOut[i]);

		}*/

		for (int i{}; i < m_DungeonSize * m_DungeonSize; i++)
		{
			utils::SetColor(Color4f{ 1,0,1,1 });
			utils::FillRect(m_RoomShapes[i]);
			utils::SetColor(Color4f{ 0,0,0,1 });
			utils::DrawRect(m_RoomShapes[i]);

		}
		for (int i{}; i < m_DungeonSize * m_DungeonSize; i++)
		{
			m_Rooms[i]->Draw();

		}

		for (Rectf element : m_HallwayShapesx)
		{
			utils::SetColor(Color4f{ 1,0,1,1 });
			utils::FillRect(element);
			utils::SetColor(Color4f{ 0,0,0,1 });
			utils::DrawRect(element);
			m_HallwayImage.Draw(element, Rectf{ 0,0,m_HallwayImage.GetWidth(),m_HallwayImage.GetHeight() });
		}
		for (Rectf element : m_HallwayShapesy)
		{
			utils::SetColor(Color4f{ 1,0,1,1 });
			utils::FillRect(element);
			utils::SetColor(Color4f{ 0,0,0,1 });
			utils::DrawRect(element);
			m_HallwayImage.Draw(element, Rectf{ 0,0,m_HallwayImage.GetWidth(),m_HallwayImage.GetHeight() });
		}
		for (Rectf element : m_DoorsTopRight)
		{
			if (!m_IsFighting)
			{
				m_DoorImage.Draw(element, Rectf{ 0,0,100,100 });
			}
			else
			{
				m_LockedDoorImage.Draw(element, Rectf{ 0,0,100,100 });
			}

		}
		for (Rectf element : m_DoorsDownLeft)
		{
			if (!m_IsFighting)
			{
				m_DoorImage.Draw(element, Rectf{ 0,0,100,100 });
			}
			else
			{
				m_LockedDoorImage.Draw(element, Rectf{ 0,0,100,100 });
			}
		}

		m_BossDoorImage.Draw(m_BossDoorShape, Rectf{ 0,0,100,100 });
		m_pChestImage->Draw();
}
void DungeonGenerator::Update(float elapsedSec,Rectf &actorshape)
{
	int counter{ 0 };
	for (int i{}; i < m_DungeonSize * m_DungeonSize; i++)
	{
		m_Rooms[i]->HandleCollision(actorshape);
		m_Rooms[i]->Update(elapsedSec, Point2f{actorshape.left,actorshape.bottom});
		
		if (m_Rooms[i]->GetIsClear()==false)
		{
			counter++;
			
		}
		if (counter>0)
		{
			m_IsFighting = true;
		}
		else
		{
			m_IsFighting = false;
		}
	}
	for (DungeonRoom* element : m_HallwaysHorizontal)
	{
		element->HandleCollision(actorshape);
	}
	for (DungeonRoom* element : m_HallwaysVertical)
	{
		element->HandleCollision(actorshape);
	}
	m_pChestImage->update(elapsedSec);
}
void DungeonGenerator::HitEnemys(Spell*& spell)
{
	for (int i{}; i < m_DungeonSize * m_DungeonSize; i++)
	{
		Spell* temp{m_pFakeSpell};
		m_Rooms[i]->CheckEnemy(temp);
		if (spell != nullptr)
		{
			if (m_Rooms[i]->CheckEnemy(spell)== true)
			{
				if (spell->ExplosionTime() == true)
				{
					delete spell;
					spell = nullptr;
				}	
			}
		}
	}
	
}
DungeonRoom* DungeonGenerator::GetCurrentRoom(const Rectf& actorShape)
{
	for (int i{}; i < m_DungeonSize * m_DungeonSize; i++)
	{
		if (utils::IsOverlapping(actorShape, m_RoomShapes[i]))
		{
			if (utils::IsPointInRect(Point2f{actorShape.left,actorShape.bottom},m_RoomShapes[i])
				&& utils::IsPointInRect(Point2f{ actorShape.left+actorShape.width,actorShape.bottom }, m_RoomShapes[i])
					&& utils::IsPointInRect(Point2f{ actorShape.left + actorShape.width,actorShape.bottom+actorShape.height }, m_RoomShapes[i])
						&&utils::IsPointInRect(Point2f{ actorShape.left + actorShape.width,actorShape.bottom+actorShape.height }, m_RoomShapes[i])
				)
			{
				m_Rooms[i]->SetHasBeenEntered(true);
				m_Rooms[i]->MakeEnemy();
				m_Rooms[i]->MakeInteractibles(m_pInteractiblesSprite,m_pPillarImage);
			}
		

			return m_Rooms[i];
		}
		
	}
	for (size_t i{};i<m_HallwayShapesx.size();i++)
	{
		if (utils::IsOverlapping(actorShape, m_HallwayShapesx[i]))
		{
			return m_HallwaysHorizontal[i];
		}
	}
	for (size_t i{}; i < m_HallwayShapesx.size(); i++)
	{
		if (utils::IsOverlapping(actorShape, m_HallwayShapesy[i]))
		{
			return m_HallwaysVertical[i];
		}
	}
	return m_HallwaysVertical[0];
	
}
bool DungeonGenerator::IsInDoor(const Rectf &actorShape)
{
	if (m_IsFighting == false)
	{

		for (Rectf element : m_DoorsTopRight)
		{
			if (utils::IsOverlapping(actorShape, Rectf{ element.left+50*m_Scale,element.bottom+50*m_Scale,element.width-50*m_Scale,element.height-50*m_Scale }))
			{
				return true;
			}
		}
	}
	return false;
}
bool DungeonGenerator::IsInDoorBack(const Rectf& actorShape)
{
	if (m_IsFighting == false)
	{
		for (Rectf element : m_DoorsDownLeft)
		{
			if (utils::IsOverlapping(actorShape, element))
			{
				return true;
			}
		}
	}
	return false;
}
void DungeonGenerator::DoCollision(bool x)
{
	for (int i{}; i < m_DungeonSize * m_DungeonSize; i++)
	{
		m_Rooms[i]->SetDoCollision(x);
	}
	for (size_t i{}; i < m_HallwayShapesx.size(); i++)
	{
		
		m_HallwaysHorizontal[i]->SetDoCollision(x);
		
	}
	for (size_t i{}; i < m_HallwayShapesx.size(); i++)
	{
		
		m_HallwaysVertical[i]->SetDoCollision(x);
	}
}
bool DungeonGenerator::IsInBossDoor(const Rectf& actorShape)
{
	if (utils::IsOverlapping(actorShape,m_BossDoorShape))
	{
		return true;
	}
	else
	{
		return false;
	}
}
DungeonGenerator::Boss DungeonGenerator::GetCurrentBoss()
{
	return m_CurrentBoss;
}
bool DungeonGenerator::HitCharacterReturnDead(Hero& hero, float elapsedSec, float damage)
{
	for (DungeonRoom* pelement : m_Rooms)
	{
		if (pelement->HitCharacterReturnDead(hero, elapsedSec, damage))
		{
			return true;
		}
	}
	return false;
}
void DungeonGenerator::ChangeVolume(int newVolume)
{
	m_Buy.SetVolume(newVolume);
	for (DungeonRoom*& pElement : m_Rooms)
	{
		if (pElement != nullptr)
		{
			pElement->ChangeVolume(newVolume);
		}
		
	}
}