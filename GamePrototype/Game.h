#pragma once
#include "BaseGame.h"
#include <vector>
class Player;
class Hamburger;
class Salad;
class Map;
class Item;
class Texture;

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
	std::vector<Item*> m_pHamburgers;
	std::vector<Item*> m_pSalads;
	std::vector<Item*> m_pSpikes;

	float m_HamburgerSpawnTimer;
	float m_SaladSpawnTimer;
	float m_SpikeSpawnTimer;

	float m_Health;
	float m_PlayTime;
	bool m_GameOver;

	const Rectf m_GameArea;

	Texture* m_pCountdownText;
	int m_CountdownNr;
	Texture* m_pScoreText;
	Texture* m_pGameOverText;

	Point2f m_MousePos;
	bool m_DrawHamburgerTemp;

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	
	void Reset();

	void DrawHealthBar() const;
	void SpawnItems();
	bool CheckConsumeItems(std::vector<Item*>& items);

	void UpdateCountdown();
	void DepleteHealth(float elapsedSec);

	Texture* GetText(const std::string& text, int size = 75);
	void DrawCenterText(const Texture* pText, float vertOffset = 0.f) const;

	float GetDifficulty();
	float ScaleToDifficulty(float valEasy, float valHard);
};