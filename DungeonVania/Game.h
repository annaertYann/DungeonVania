#pragma once
#include "Hero.h"
#include "Texture.h"
#include "Room.h"
#include "BossRoom.h"
#include "Lobby.h"
#include "DungeonGenerator.h"
#include "StartScreen.h"
#include "Hud.h"
#include "SoundStream.h"
#include "SoundEffect.h"

class Game
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;
	bool GetQuit();
	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );
private:
	
	int m_SoundVolume;
	int m_PreviousSoundVolume;
	SoundStream m_AmbientSound;
	SoundEffect m_Click;
	SoundEffect m_Door;
	bool m_SoundOn;
	bool m_Quit;
	// DATA MEMBERS
	const Window m_Window;
	Hero m_Character;
	int m_CurrentRoomNumber;
	BossRoom* m_pRoom;
	DungeonGenerator* m_pDungeon;
	Rectf m_ActorShape;
	Lobby* m_pLobby;
	Texture m_Background;
	bool m_IsInOptionMenu;
	bool m_IsMinotaurDefeated;
	bool m_IsCyclopsDefeated;
	bool m_IsSantaDefeated;
	float m_OriginalWindowWidth;
	int m_PreviousCoins;
	Hud m_Hud;
	bool m_CoinsOn;
	StartScreen* m_pStartScreen;

	// FUNCTIONS
	void LoadGame();
	void Initialize( );
	void ChangeVolume();
	void Cleanup( );
	void ClearBackground( ) const;
	void InteractWithBossRoom();
	void InteractWithLobby();
	void InteractWithDungeon();
	void DoWallHits();
	void DoEnemyHits();
	void HitCharacter(float elapsedSec);
	void UpdateLobby(float elapsedSec);
	void UpdateBossRoom(float elapsedSec);
	void GoToNextRoom();
	void UpdateDungeon(float elapsedSec);
	void DeleteCharacterSpells();
};