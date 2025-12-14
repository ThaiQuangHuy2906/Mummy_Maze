#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "AssetManager.h"
#include "AppTypes.h"
#include "AudioManager.h"

// trạng thái
enum class SelectScreenState {
    Difficulty,
    EasyLevels,
    MediumLevels,
    HardLevels
};

class LevelSelect {
public:
    LevelSelect(AssetManager& assets, AudioManager& audio);

    void handleEvent(sf::Event& event, AppState& currentState,
                     std::string& selectedMap, sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);

private:
    AssetManager& m_assets;
    AudioManager& m_audio;

    // biến trạng thái
    SelectScreenState m_currentScreen;
    sf::Font m_font;

    // biến cuộn
    sf::View m_scrollingView;
    sf::FloatRect m_scrollingAreaRect;
    float m_scrollOffset;
    float m_scrollBoundsBottom;

    // nút
    std::vector<sf::Sprite> m_levelButtonBgs;
    std::vector<sf::Text> m_levelButtons;       // Chứa chữ "Level 1", "Level 2"...
    std::vector<sf::Text> m_starTexts;          // Chứa dấu sao "***" riêng

    std::vector<std::string> m_levelMapFiles;

    // nút giao diện
    sf::Sprite m_background;
    sf::Sprite m_easyButton;
    sf::Sprite m_mediumButton;
    sf::Sprite m_hardButton;
    sf::Sprite m_backToMainButton;
    sf::Sprite m_backToDifficultyButton;


    void setupLevelList(SelectScreenState screen);
};