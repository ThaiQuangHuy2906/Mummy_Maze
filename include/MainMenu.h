#pragma once
#include "AudioManager.h"
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "AppTypes.h"

class MainMenu {
public:
    // Constructor, nhận AssetManager để lấy hình ảnh
    MainMenu(AssetManager& assets, AudioManager& audio);

    // Xử lý sự kiện
    // thay đổi currentState
    void handleEvent(sf::Event& event, AppState& currentState, sf::RenderWindow& window);

    // Hàm vẽ background và nút
    void draw(sf::RenderWindow& window);

    // Kiểm tra xem người chơi có vừa bấm Continue không
    bool isContinueSelected() const { return m_isContinueSelected; }

    // Reset cờ sau khi đã xử lý xong
    void resetContinueFlag() { m_isContinueSelected = false; }

private:
    AudioManager& m_audio;
    sf::Sprite m_background;        // Sprite cho hình nền menu
    sf::Sprite m_playButton;        // Sprite cho nút "Play"
    sf::Sprite m_quitButton;        // Sprite cho nút "Quit"
    sf::Sprite m_continueButton;    // Nút Continue
    bool m_hasSaveFile;             // Kiểm tra có file save không để hiện nút
    bool m_isContinueSelected;      // Cờ đánh dấu người chơi bấm Continue
};