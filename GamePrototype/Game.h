#pragma once
#include "BaseGame.h"
#include <vector>
class Player;
class Hamburger;
class Salad;
class Map;
class Item;

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	Player* m_pPlayer;
	Player* m_pPlayer2;
	std::vector<Item*> m_pHamburgers;
	std::vector<Item*> m_pSalads;

	float m_HamburgerSpawnTimer;
	float m_SaladSpawnTimer;

	int m_Lives;
	float m_PlayTime;
	bool m_Playing;

	static const int m_MAX_LIVES;

	const Rectf m_GameArea;

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void DrawUI() const;
	void SpawnItems();
	bool CheckConsumeItems(std::vector<Item*>& items);

	float GetDifficulty();
	float ScaleToDifficulty(float valEasy, float valHard);
};