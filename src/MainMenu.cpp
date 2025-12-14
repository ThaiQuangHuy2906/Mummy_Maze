#include "MainMenu.h"
#include "GameLogic.h"
#include "MainMenu.h"
#include "GameLogic.h"
#include <iostream>

MainMenu::MainMenu(AssetManager& assets, AudioManager& audio)
    : m_audio(audio), m_hasSaveFile(false), m_isContinueSelected(false) {

    // 1. Nạp texture
    m_background.setTexture(assets.getTexture("menu_background.png"));
    m_playButton.setTexture(assets.getTexture("button_play.png"));
    m_quitButton.setTexture(assets.getTexture("button_quit.png"));

    // Nạp texture cho nút Continue
    m_continueButton.setTexture(assets.getTexture("button_continue.png"));

    // 2. Kiểm tra file save ngay khi khởi tạo
    m_hasSaveFile = GameLogic::hasSavedGame();

    // 3. Hàm phụ để scale an toàn
    auto safeScale = [](sf::Sprite& s, float targetW) {
        float originalW = static_cast<float>(s.getTexture()->getSize().x);
        if (originalW > 0) {
            float scale = targetW / originalW;
            s.setScale(scale, scale);
        } else {
            s.setScale(1.0f, 1.0f);
        }
    };

    // 4. Scale background
    sf::Vector2u bgSize = m_background.getTexture()->getSize();
    if (bgSize.x > 0 && bgSize.y > 0) {
        m_background.setScale(494.f / bgSize.x, 480.f / bgSize.y);
    }

    // 5. Scale các nút
    float targetBtnWidth = 220.f;
    safeScale(m_playButton, targetBtnWidth);
    safeScale(m_quitButton, targetBtnWidth);
    safeScale(m_continueButton, targetBtnWidth);

    // 6. Căn giữa và đặt vị trí các nút
    float windowWidth = 494.f;
    float centerX = windowWidth / 2.f;

    // Helper để lấy chiều rộng nút (tránh lặp code)
    auto getBtnHalfWidth = [](sf::Sprite& s) {
        return s.getGlobalBounds().width / 2.f;
    };

    if (m_hasSaveFile) {
        // TH: Có file save ->Hiện 3 nút (Continue -> Play -> Quit)

        // Nút Continue (Trên cùng)
        if (m_continueButton.getGlobalBounds().width > 0)
            m_continueButton.setPosition(centerX - getBtnHalfWidth(m_continueButton), 160.f);

        // Nút Play (Giữa) - Dời xuống xíu
        if (m_playButton.getGlobalBounds().width > 0)
            m_playButton.setPosition(centerX - getBtnHalfWidth(m_playButton), 260.f);

        // Nút Quit (Dưới cùng)
        if (m_quitButton.getGlobalBounds().width > 0)
            m_quitButton.setPosition(centerX - getBtnHalfWidth(m_quitButton), 360.f);
    }
    else {
        // TH: KO có file save -> Hiện 2 nút như cũ (Play -> Quit)

        // Nút Play
        if (m_playButton.getGlobalBounds().width > 0)
            m_playButton.setPosition(centerX - getBtnHalfWidth(m_playButton), 220.f);

        // Nút Quit
        if (m_quitButton.getGlobalBounds().width > 0)
            m_quitButton.setPosition(centerX - getBtnHalfWidth(m_quitButton), 320.f);
    }
}

void MainMenu::draw(sf::RenderWindow& window) {

    if (m_background.getTexture()) window.draw(m_background);
    if (m_hasSaveFile && m_continueButton.getTexture())
        window.draw(m_continueButton);
    if (m_playButton.getTexture()) window.draw(m_playButton);
    if (m_quitButton.getTexture()) window.draw(m_quitButton);
}

void MainMenu::handleEvent(sf::Event& event, AppState& currentState, sf::RenderWindow& window) {
    if (event.type != sf::Event::MouseButtonPressed) return;
    if (event.mouseButton.button != sf::Mouse::Left) return;

    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);

    if (m_hasSaveFile && m_continueButton.getGlobalBounds().contains(mousePos)) {
        m_audio.playSound("click");
        m_isContinueSelected = true;        // Bật cờ báo hiệu
        currentState = AppState::InGame;    // Vào thẳng game
        return;
    }

    if (m_playButton.getGlobalBounds().contains(mousePos)) {
        m_audio.playSound("click"); // chạy âm thanh click tại màn hình menu chính
        currentState = AppState::LevelSelect;
    }

    if (m_quitButton.getGlobalBounds().contains(mousePos)) {
        m_audio.playSound("click");
        currentState = AppState::Exiting;
    }
}