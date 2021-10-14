#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "BossRoom.h"
#include <iostream>
#include"Lobby.h"
#include "SpellManager.h"
#include "Spell.h"
#include "Hud.h"
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
#include <algorithm>
Game::Game( const Window& window ) 
	:m_Window{ window }
	, m_Character{ Point2f{window.width/2,window.height/2} }
	, m_pRoom{ nullptr}
	,m_CurrentRoomNumber{0}
	, m_ActorShape{}
	, m_pLobby{ nullptr }
	,m_IsMinotaurDefeated{}
	,m_IsCyclopsDefeated{}
	,m_IsSantaDefeated{}
	,m_OriginalWindowWidth{1280}
	,m_pDungeon{nullptr}
	,m_Background{"Resources/Background.png"}
	, m_pStartScreen{ new StartScreen{window} }
	,m_IsInOptionMenu{false}
	,m_Hud{window,50}
	,m_Quit{}
	,m_CoinsOn{true}
	, m_PreviousSoundVolume{50}
	,m_AmbientSound{"Resources/Sound/Ambient.mp3"}
	,m_PreviousCoins{1}
	,m_SoundVolume{50}
	,m_Click{"Resources/Sound/Click.mp3"}
	,m_Door{"Resources/Sound/Door.mp3"}
	,m_SoundOn{true}
{
	Initialize( );
}
Game::~Game( )
{
	Cleanup( );
}
void Game::Initialize( )
{
	m_ActorShape = m_Character.GetShape();
	m_AmbientSound.Play(true);
	m_AmbientSound.SetVolume(m_SoundVolume);
	m_Click.SetVolume(m_SoundVolume);
	m_Door.SetVolume(m_SoundVolume);
	if (m_pDungeon != nullptr)
	{
		m_pDungeon->ChangeVolume(m_SoundVolume);
	}
	
	
}
void Game::UpdateDungeon(float elapsedSec)
{
	if (m_pRoom == nullptr && m_pLobby == nullptr && m_pDungeon != nullptr)
	{
		m_Character.Update(elapsedSec, m_pDungeon->GetCurrentRoom(m_Character.GetShape()));
	}
	if (m_pDungeon != nullptr)
	{
		m_pDungeon->Update(elapsedSec, m_Character.GetShape());
		DoEnemyHits();
		GoToNextRoom();
		HitCharacter(elapsedSec);
	}
}
void Game::DeleteCharacterSpells()
{
	for (Spell*& element : m_Character.GetManager().GetSpells())
	{
		if (element != nullptr)
		{
			if (element->GetDead())
			{
				delete element;
				element = nullptr;
			}
		}
	}
}
void Game::Cleanup( )
{
	delete m_pRoom;
	m_pRoom = nullptr;
	delete m_pLobby;
	m_pLobby = nullptr;
	delete m_pDungeon;
	m_pDungeon=nullptr;
	if (m_pStartScreen != nullptr)
	{
		delete m_pStartScreen;
		m_pStartScreen = nullptr;
	}
}
void Game::Update(float elapsedSec)
{
	
	if (!m_IsInOptionMenu)
	{
		UpdateBossRoom(elapsedSec);
		UpdateLobby(elapsedSec);
		DoWallHits();
		UpdateDungeon(elapsedSec);
		DeleteCharacterSpells();
		m_Hud.SetCurrentColor(m_Character.GetCurrentType());
	}
	if (m_pStartScreen == nullptr)
	{
		m_Hud.Update(elapsedSec, m_Character.GetCameraPos(), m_Character.GetCoins(), m_Character.GetHealth(), m_IsInOptionMenu);
	}
	
	
}
void Game::UpdateLobby(float elapsedSec)
{
	if (m_pLobby != nullptr)
	{
		m_pLobby->HandleCollision(m_Character.GetShape());
		m_pLobby->Update(elapsedSec);
		m_Character.Update(elapsedSec, m_pLobby);
	}
	if (m_pRoom != nullptr)
	{
		m_Character.LockBorderCamera(true);
	}
	else
	{
		m_Character.LockBorderCamera(false);
	}
}
void Game::UpdateBossRoom(float elapsedSec)
{
	if (m_pRoom != nullptr)
	{
		m_Character.Update(elapsedSec, m_pRoom);
		m_pRoom->HandleCollision(m_Character.GetShape());
		m_pRoom->Update(elapsedSec, m_Character.GetShape());
		DoEnemyHits();
		HitCharacter(elapsedSec);
	}
}
void Game::Draw( ) const
{
	ClearBackground( );
	float backGroundNumber{-1};
	float backGroundSize{ 10000 };
	//drawing background
	{


		for (float vertical{ -1 }; vertical < 9; vertical++)
		{
			for (float horizontal{ -1 }; horizontal < 9; horizontal++)
			{
				m_Background.Draw(Rectf{ horizontal * backGroundSize,backGroundNumber * backGroundSize, backGroundSize, backGroundSize }, Rectf{ 0,0, backGroundSize, backGroundSize });
			}
			backGroundNumber++;
		}
	}
	if (m_pRoom!=nullptr)
	{
		m_pRoom->Draw();
	}
	if (m_pLobby != nullptr)
	{
		m_pLobby->Draw();
	}
	if (m_pDungeon!=nullptr)
	{
		m_pDungeon->Draw();
	}
	m_Character.Draw();
	if (m_pStartScreen != nullptr)
	{
		m_pStartScreen->Draw();
	}
	if (m_pStartScreen == nullptr)
	{
		m_Hud.Draw();
	}
}
void Game::ChangeVolume()
{
	m_AmbientSound.SetVolume(m_SoundVolume);
	m_Click.SetVolume(m_SoundVolume);
	m_Door.SetVolume(m_SoundVolume);
	m_Character.ChangeVolume(m_SoundVolume);
	if (m_pDungeon != nullptr)
	{
		m_pDungeon->ChangeVolume(m_SoundVolume);
	}
	if (m_pRoom != nullptr)
	{
		m_pRoom->ChangeVolume(m_SoundVolume);
	}
	m_Hud.ChangeVolume(m_SoundVolume);
}
void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	if (m_SoundOn)
	{
		switch (e.keysym.sym)
		{


		case SDLK_UP:
			if (m_SoundVolume < 100)
			{
				m_SoundVolume += 10;
			}

			break;
		case SDLK_DOWN:
			if (m_SoundVolume > 0)
			{
				m_SoundVolume -= 10;
			}
			break;

		}
		ChangeVolume();
	}
}
void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch ( e.keysym.sym )
	{
	case SDLK_p:
		if (m_SoundVolume <= 0)
		{
			m_SoundVolume = m_PreviousSoundVolume;
		}
		else
		{
			m_PreviousSoundVolume = m_SoundVolume;
			m_SoundVolume = 0;
		}
		
		

		break;

	case SDLK_e:
		InteractWithLobby();
		InteractWithBossRoom();
		InteractWithDungeon();
		break;
	case SDLK_i:
		std::cout << "Press WASD keys to Move the hero " << std::endl;
		std::cout << "Press E Key to interact with doors,chests,shop" << std::endl;
		std::cout << "Left MouseButton To shoot bullets to mousepos" << std::endl;
		std::cout << "press E on a door to start a level" << std::endl;
		std::cout << "Up and Down to adjust volume" << std::endl;
		std::cout << "P to mute" << std::endl;
		std::cout << "esc to open menu" << std::endl;
		break;
	case SDLK_ESCAPE:
		if (m_IsInOptionMenu)
		{
			m_IsInOptionMenu = false;
		}
		else
		{
			m_IsInOptionMenu = true;
		}
	}
}
void Game::InteractWithDungeon()
{
	if (m_pDungeon != nullptr)
	{
		m_pDungeon->CheckChest(m_Character);
		if (m_pDungeon->IsInBossDoor(m_Character.GetShape()))
		{
			m_Door.Play(0);
			m_Character.Transition(Point2f{ m_Window.width,m_Window.height / 2 }, 0, 640);
			if (m_pDungeon->GetCurrentBoss() == DungeonGenerator::Boss::Minotaur)
			{
				
				m_pRoom = new BossRoom{ m_Window.width / 640,m_SoundVolume,BossRoom::CurrentEnemy::Minotaur };
				ChangeVolume();
			}
			if (m_pDungeon->GetCurrentBoss() == DungeonGenerator::Boss::Cyclops)
			{

				m_pRoom = new BossRoom{ m_Window.width / 640,m_SoundVolume,BossRoom::CurrentEnemy::Cyclops};
				ChangeVolume();
			}
			if (m_pDungeon->GetCurrentBoss() == DungeonGenerator::Boss::Santa)
			{
				m_pRoom = new BossRoom{ m_Window.width / 640,m_SoundVolume,BossRoom::CurrentEnemy::Santa};
				ChangeVolume();
			}

			delete m_pDungeon;
			m_pDungeon = nullptr;
		}
	}
}
void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	Vector2f mousePos{ float(e.x),float(m_Window.height - e.y) };
	if (m_pStartScreen != nullptr)
	{
		m_pStartScreen->OverlappingButtons(mousePos.ToPoint2f());
	}
	if (m_IsInOptionMenu)
	{
		m_Hud.InteractWithOptions(mousePos.ToPoint2f(),false,m_CoinsOn,m_Character.GetCameraPos());

	}
}
void Game::LoadGame()
{

	bool tempCoinsOn;
	bool tempSoundOn;
	std::ifstream file{ "Resources/Saves/load.txt" };
	if (file)
	{
		std::string line{};
		//CoinsOn
		std::getline(file, line, ':');
		std::getline(file, line, '\n');
		m_CoinsOn = std::stoi(line);
		tempCoinsOn = std::stoi(line);
		//Coin
		std::getline(file, line, ':');
		std::getline(file, line, '\n');
		if (m_CoinsOn)
		{
			m_Character.SetCoins(std::stoi(line));
			m_PreviousCoins = std::stoi(line);
		}
		else
		{
			m_PreviousCoins = std::stoi(line);
			m_Character.SetCoins(9999);
		}
		//Lobby
		std::getline(file, line, ':');
		std::getline(file, line, '\n');
		m_pLobby = new Lobby{ m_Window.width / m_OriginalWindowWidth, Point2f{},std::stoi(line) };
		switch (std::stoi(line))
		{
		case 1:
			
			break;
		case 2:
			m_IsMinotaurDefeated = true;
			break;
		case 3:
			m_IsMinotaurDefeated = true;
			m_IsCyclopsDefeated = true;
			break;

		}
		//SoundVolume
		std::getline(file, line, ':');
		std::getline(file, line, '\n');
		m_SoundVolume = std::stoi(line);
		ChangeVolume();
		//SoundOn
		std::getline(file, line, ':');
		std::getline(file, line, '\n');
		m_SoundOn = std::stoi(line);
		tempSoundOn = std::stoi(line);
		//Health
		std::getline(file, line, ':');
		std::getline(file, line, '\n');
		m_Character.SetOptions(std::stoi(line),1);
		//currenweapon
		std::getline(file, line, ':');
		std::getline(file, line, '\n');
		switch (std::stoi(line))
		{
		case 0:
			m_Character.SetCurrentType(Hero::BoltType{ Hero::BoltType::blue });
			break;
		case 1:
			m_Character.SetCurrentType(Hero::BoltType{ Hero::BoltType::green });
			break;
		case 2:
			m_Character.SetCurrentType(Hero::BoltType{ Hero::BoltType::red });
			break;
		}
		//won?
		std::getline(file, line, ':');
		std::getline(file, line, '\n');
		if (std::stoi(line) == 0)
		{

		}
		else
		{
			m_pLobby->SetWon();
		}
		//gender
		std::getline(file, line, ':');
		std::getline(file, line, '\n');
		m_Character.SetOptions(int (m_Character.GetHealth()), std::stoi(line));
		m_Hud.SetSettings(tempSoundOn, tempCoinsOn);

	}
	else
	{
		m_Click.Play(0);
		m_Character.SkipShootingOnce();
		m_pLobby = new Lobby{ m_Window.width / m_OriginalWindowWidth, Point2f{},1 };
		delete m_pStartScreen;
		m_pStartScreen = nullptr;
	}

}
void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{

	Vector2f mousePos{ float(e.x),float(m_Window.height - e.y) };
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		
		//click on startscreen
		if (m_pStartScreen != nullptr)
		{
			if (m_pStartScreen->IsNewGameClicked(mousePos.ToPoint2f()))
			{
				m_Click.Play(0);
				m_Character.SkipShootingOnce();
				m_pLobby = new Lobby{ m_Window.width / m_OriginalWindowWidth, Point2f{},1 };
				m_pStartScreen->SetNewClicked();
			}
			else

			if (m_pStartScreen->IsLoadGameClicked(mousePos.ToPoint2f()))
			{
				m_Click.Play(0);
				m_Character.SkipShootingOnce();
				LoadGame();
				delete m_pStartScreen;
				m_pStartScreen = nullptr;
			}
			else 
			{
				m_Character.SkipShootingOnce();
				switch (m_pStartScreen->IsFemaleClicked(mousePos.ToPoint2f()))
				{
				case genders::female:
					m_Character.SetOptions(int (m_Character.GetHealth()), true);
					delete m_pStartScreen;
					m_pStartScreen = nullptr;
				break;
				case genders::male:
					m_Character.SetOptions(int (m_Character.GetHealth()), false);
					delete m_pStartScreen;
					m_pStartScreen = nullptr;
					break;
				case genders::nothing:
					break;
				default:
					break;
				}
			
			}
		}
		//click on shop
		if (m_pLobby != nullptr)
		{
			
			m_Hud.InteractWithShop(m_Character, mousePos.ToPoint2f());
			
		}
		//click on options
		
		if (m_IsInOptionMenu)
		{
			m_Click.Play(0);
			ButtonClicked tempButton = m_Hud.InteractWithOptions(mousePos.ToPoint2f(), true, m_CoinsOn,m_Character.GetCameraPos());

			switch (tempButton)
			{
			case ButtonClicked::play:
				m_IsInOptionMenu = false;
				m_Character.SkipShootingOnce();
				break;
			case ButtonClicked::quit:
				m_Quit = true;
				break;
			case ButtonClicked::nothing:
				
				m_Character.SkipShootingOnce();
				break;
			case ButtonClicked::money:
				if (m_CoinsOn)
				{
					m_PreviousCoins = m_Character.GetCoins();
					m_Character.SetCoins(9999);
					m_CoinsOn = false;
					m_Hud.InteractWithOptions(mousePos.ToPoint2f(), true, m_CoinsOn, m_Character.GetCameraPos());
					m_Character.SkipShootingOnce();
				}
				else
				{
					m_CoinsOn = true;
					m_Hud.InteractWithOptions(mousePos.ToPoint2f(), true, m_CoinsOn, m_Character.GetCameraPos());
					m_Character.SkipShootingOnce();
					m_Character.SetCoins(m_PreviousCoins);
				}
				break;
			case ButtonClicked::sound:
				if (m_SoundVolume <= 0)
				{
					m_SoundOn = true;
					m_SoundVolume = m_PreviousSoundVolume;
					ChangeVolume();
					m_AmbientSound.Resume();
					m_Character.SkipShootingOnce();
				}
				else
				{
					m_SoundOn = false;
					m_PreviousSoundVolume = m_SoundVolume;
					m_SoundVolume = 0;
					ChangeVolume();
					m_AmbientSound.Pause();
					m_Character.SkipShootingOnce();
				}
				break;
			default:
				break;
			}

			
		}
		if (!m_IsInOptionMenu)
		{
			if (m_Hud.InteractWithoptionicon(mousePos.ToPoint2f(), m_Character.GetCameraPos()))
			{
				m_Character.SkipShootingOnce();
				m_IsInOptionMenu = true;
			}
			else
			{
				
				m_IsInOptionMenu = false;
			}
		}

		//attack with character
		m_Character.SetAttack(Point2f{ m_Character.GetShape().left,m_Character.GetShape().bottom }, m_Window.width / m_OriginalWindowWidth, mousePos);
	
		break;
	}
}
void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}
void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
bool Game::GetQuit()
{
	return m_Quit;
}
void Game::InteractWithBossRoom()
{
	if (m_pRoom != nullptr)
	{
		if (m_pRoom->IsActorInDoor(m_Character.GetShape()) == true)
		{
			m_Door.Play(0);
			m_Character.Transition(Point2f{ m_Window.width / 2 ,m_Window.height / 2 }, 0, 0);
			if (m_pRoom->getMinotaurDead() == true)
			{
				
				switch (m_pRoom->GetCurrentEnemy())
				{
				case BossRoom::CurrentEnemy::Minotaur:
					m_IsMinotaurDefeated = true;
						break;
				case BossRoom::CurrentEnemy::Cyclops:
					m_IsCyclopsDefeated = true;
					break;
				case BossRoom::CurrentEnemy::Santa:
					m_IsSantaDefeated = true;
					break;
				default:
					break;
				}
			}
			delete m_pRoom;
			m_pRoom = nullptr;
		
			if (m_IsMinotaurDefeated == false)
			{
				m_pLobby = new Lobby{ m_Window.width / m_OriginalWindowWidth, Point2f{} ,1 };
				m_Character.LockCamera();
			}
			else 
				if (m_IsCyclopsDefeated == false)
			{
				m_pLobby = new Lobby{ m_Window.width / m_OriginalWindowWidth, Point2f{} ,2 };
				m_Character.LockCamera();
			}
				else
					{
						m_pLobby = new Lobby{ m_Window.width / m_OriginalWindowWidth, Point2f{} ,3 };
						m_Character.LockCamera();
						if (m_IsSantaDefeated)
						{
							m_pLobby->SetWon();
						}
					}
		}
	}
}
void Game::InteractWithLobby()
{
		if (m_pLobby != nullptr)
		{
			if (m_pLobby->IsInSavePoint(m_Character.GetShape()))
			{
				m_Hud.SetSaved();
				std::ofstream newFile{ "Resources/Saves/load.txt" };
				newFile << "CoinsON:" << m_CoinsOn << std::endl;
				newFile << "Coins:" << m_Character.GetCoins() << std::endl;

				if (m_IsMinotaurDefeated == false)
				{
					newFile << "LobbyNumber:" << 1 << std::endl;
				}
				else
					if (m_IsCyclopsDefeated == false)
					{
						newFile << "LobbyNumber:" << 2 << std::endl;
					}
					else
					{
						newFile << "LobbyNumber:" << 3 << std::endl;
					}
				newFile << "SoundVolume:" << m_SoundVolume << std::endl;
				newFile << "SoundOn:" << m_SoundOn << std::endl;

				newFile << "Health:" << m_Character.GetHealth() << std::endl;
				newFile << "CurrentWeapon:" << std::to_string(int(m_Character.GetCurrentType())) << std::endl;
				if (m_IsSantaDefeated)
				{
					newFile << "Won:" << 1 << std::endl;
				}
				else
				{
					newFile << "Won:" << 0 << std::endl;
				}
				newFile << "gender:" << m_Character.GetIsFemale() << std::endl;
			}



			if (m_pLobby->IsInShop(m_Character.GetShape()))
			{
				m_Hud.SetIsShopOpen(true);
			}
			else
			{
				m_Hud.SetIsShopOpen(false);
			}
			if (m_pLobby->IsOnDoor(m_Character.GetShape()) == 1)
			{

				m_Door.Play(0);
				m_Character.Transition(Point2f{ m_pLobby->GetBorder(Lobby::Border::right),m_Window.height / 2 }, 0, m_pLobby->GetBorder(Lobby::Border::right) / 2);
				delete m_pLobby;
				m_pLobby = nullptr;
				m_pDungeon = new DungeonGenerator{ m_Window,DungeonGenerator::Boss::Minotaur,m_SoundVolume };
				m_Character.LockCamera();
				ChangeVolume();
			}
			else
				if (m_pLobby->IsOnDoor(m_Character.GetShape()) == 2)
				{

					m_Door.Play(0);
					m_Character.Transition(Point2f{ m_pLobby->GetBorder(Lobby::Border::right),m_Window.height / 2 }, 0, m_pLobby->GetBorder(Lobby::Border::right) / 2);
					delete m_pLobby;
					m_pLobby = nullptr;
					m_pDungeon = new DungeonGenerator{ m_Window,DungeonGenerator::Boss::Cyclops ,m_SoundVolume };
					m_Character.LockCamera();
					ChangeVolume();
				}
				else
					if (m_pLobby->IsOnDoor(m_Character.GetShape()) == 3)
					{

						m_Door.Play(0);
						m_Character.Transition(Point2f{ m_pLobby->GetBorder(Lobby::Border::right),m_Window.height / 2 }, 0, m_pLobby->GetBorder(Lobby::Border::right) / 2);
						delete m_pLobby;
						m_pLobby = nullptr;
						m_pDungeon = new DungeonGenerator{ m_Window,DungeonGenerator::Boss::Santa,m_SoundVolume };
						m_Character.LockCamera();
						ChangeVolume();
					}
		}


	}
void Game::DoWallHits()
{
	std::vector <Spell*>& ptemp{ m_Character.GetManager().GetSpells() };
	for (Spell*& pelement : ptemp)
	{
		if (pelement != nullptr)
		{
			Rectf tempRect{ pelement->GetShape() };

			//check BossRoom
			if (m_pRoom != nullptr)
			{
				if (m_pRoom->CollisionDetection(tempRect) == true)
				{
					if (pelement->ExplosionTime() == true)
					{
						delete pelement;
						pelement = nullptr;

					}
				}
			}
			//check Lobby
			if (m_pLobby != nullptr)
			{
				if (m_pLobby->CollisionDetection(tempRect) == true)
				{
					if (pelement->ExplosionTime() == true)
					{
						delete pelement;
						pelement = nullptr;
					}
				}
			}
		}
	}
}
void Game::HitCharacter(float elapsedSec)
{
	//Hit in BossRoom
	if (m_pRoom != nullptr)
	{
		std::vector <Spell*>& ptemp1{ m_pRoom->GetSpells() };
		for (Spell*& pelement : ptemp1)
		{
			if (pelement != nullptr && m_pRoom != nullptr)
			{
				if (m_Character.CheckHitReturnDead(pelement->GetShape(), elapsedSec, 50) == true)
				{
					//when the character dies
					{
						m_Character.LockCamera();
						m_Character.Transition(Point2f{ m_Window.width / 2 ,m_Window.height / 2 }, 0, 0);

						if (m_pRoom != nullptr)
						{
							delete m_pRoom;
							m_pRoom = nullptr;
						}
						if (m_pDungeon != nullptr)
						{
							delete m_pDungeon;
							m_pDungeon = nullptr;
						}
						if (m_IsMinotaurDefeated == false)
						{
							m_pLobby = new Lobby{ m_Window.width / 1280, Point2f{} ,1 };
						}
						else
							if (m_IsCyclopsDefeated == false)
							{
								m_pLobby = new Lobby{ m_Window.width / 1280, Point2f{} ,2 };
							}
							else
							{
								m_pLobby = new Lobby{ m_Window.width / 1280, Point2f{} ,3 };
							}
					}
				}
			}
		}

		//hit in dungeon
		if (m_pRoom != nullptr)
		{
			if (m_pRoom->getMinotaurDead() == false)
			{
				if (utils::IsOverlapping(m_pRoom->GetShapeEnemy(), m_Character.GetShape()))
				{

					if (m_Character.CheckHitReturnDead(m_pRoom->GetShapeEnemy(), elapsedSec, 50) == true)
					{
						//when the character dies
						{
							m_Character.LockCamera();
							m_Character.Transition(Point2f{ m_Window.width / 2 ,m_Window.height / 2 }, 0, 0);

							if (m_pRoom != nullptr)
							{
								delete m_pRoom;
								m_pRoom = nullptr;
							}
							if (m_pDungeon != nullptr)
							{
								delete m_pDungeon;
								m_pDungeon = nullptr;
							}
							if (m_IsMinotaurDefeated == false)
							{
								m_pLobby = new Lobby{ m_Window.width / 1280, Point2f{} ,1 };
							}
							else
								if (m_IsCyclopsDefeated == false)
								{
									m_pLobby = new Lobby{ m_Window.width / 1280, Point2f{} ,2 };
								}
								else
								{
									m_pLobby = new Lobby{ m_Window.width / 1280, Point2f{} ,3 };
								}
						}
					}
				}
			}
		}
	}
	//hit on overlapping
	if (m_pDungeon != nullptr)
	{
		if (m_pDungeon->HitCharacterReturnDead(m_Character, elapsedSec, 20))
		{
			//when the character dies
			{
				m_Character.LockCamera();
				m_Character.Transition(Point2f{ m_Window.width / 2 ,m_Window.height / 2 }, 0, 0);

				if (m_pRoom != nullptr)
				{
					delete m_pRoom;
					m_pRoom = nullptr;
				}
				if (m_pDungeon != nullptr)
				{
					delete m_pDungeon;
					m_pDungeon = nullptr;
				}
				if (m_IsMinotaurDefeated == false)
				{
					m_pLobby = new Lobby{ m_Window.width / 1280, Point2f{} ,1 };
				}
				else
					if (m_IsCyclopsDefeated == false)
					{
						m_pLobby = new Lobby{ m_Window.width / 1280, Point2f{} ,2 };
					}
					else
					{
						m_pLobby = new Lobby{ m_Window.width / 1280, Point2f{} ,3 };
					}
			}
		}
	}
}
void Game::DoEnemyHits()
{
	//check if dead with non existing hitbox
	if (m_pRoom!=nullptr)
	{
		m_pRoom->CheckEnemy(Rectf{ -10,-10,-10,-10 });
	}
	//Spell Hit Enemy
	std::vector <Spell*>& temp{ m_Character.GetManager().GetSpells() };
	for (Spell*& element : temp)
	{
		if (element != nullptr)
		{
			if (m_pRoom != nullptr)
			{
				if (m_pRoom->CheckEnemy(element->GetShape()) == true)
				{
					if (element->ExplosionTime() == true)
					{
						
						delete element;
						element = nullptr;
						m_pRoom->DoDamage(0.05f);
					}
				}
			}
			if (m_pDungeon!=nullptr)
			{
				m_pDungeon->HitEnemys(element);
			}
		}
	}
}
void Game:: GoToNextRoom()
{
	if (m_pDungeon->IsInDoor(m_Character.GetShape()))
	{
		m_pDungeon->DoCollision(false);
		m_Character.SetIsInDoor(true);
	}
	else
	if (m_pDungeon->IsInDoorBack(m_Character.GetShape()))
	{
		m_pDungeon->DoCollision(false);
		m_Character.SetIsInDoor(true);
	}
	else
	{
		m_pDungeon->DoCollision(true);
		m_Character.SetIsInDoor(false);
	}
	
}