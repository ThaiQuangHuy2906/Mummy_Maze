#include "LevelSelect.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include "GameLogic.h"

// constructor
LevelSelect::LevelSelect(AssetManager& assets, AudioManager& audio)
    : m_assets(assets),
    m_audio(audio),
    m_currentScreen(SelectScreenState::Difficulty),
    m_scrollOffset(0.f),
    m_scrollBoundsBottom(0.f) {

    // nạp font
    if (!m_font.loadFromFile("assets/fonts/Super Joyful.ttf"))
        std::cerr << "khong tim thay font 'assets/fonts/Super Joyful.ttf'" << std::endl;


    // 2. nạp và scale background
    sf::Texture& bgTex = assets.getTexture("level_select_background.png");
    m_background.setTexture(bgTex);

    if (bgTex.getSize().x > 0 && bgTex.getSize().y > 0)
        m_background.setScale(494.f / bgTex.getSize().x, 480.f / bgTex.getSize().y);
    else
        std::cerr << "khong tim duoc 'level_select_background.png'" << std::endl;

    // 3. nạp nút
    m_easyButton.setTexture(assets.getTexture("button_easy.png"));
    m_mediumButton.setTexture(assets.getTexture("button_medium.png"));
    m_hardButton.setTexture(assets.getTexture("button_hard.png"));
    m_backToMainButton.setTexture(assets.getTexture("button_back.png"));
    m_backToDifficultyButton.setTexture(assets.getTexture("button_back.png"));

    // 4. scale và định vị nút
    float targetMainBtnWidth = 180.f;
    float targetBackBtnWidth = 100.f;

    auto safeScale = [&](sf::Sprite& s, float targetW, std::string name) {
        float originalW = static_cast<float>(s.getTexture()->getSize().x);
        if (originalW > 0) {
            float scale = targetW / originalW;
            s.setScale(scale, scale);
        } else {
            std::cerr << "nút '" << name << "' bị lỗi" << std::endl;
            s.setScale(1.0f, 1.0f);
        }
    };

    safeScale(m_easyButton, targetMainBtnWidth, "button_easy");
    safeScale(m_mediumButton, targetMainBtnWidth, "button_medium");
    safeScale(m_hardButton, targetMainBtnWidth, "button_hard");
    safeScale(m_backToMainButton, targetBackBtnWidth, "button_back");
    safeScale(m_backToDifficultyButton, targetBackBtnWidth, "button_back");

    float windowCenterX = 494.f / 2.f;
    auto centerSpriteX = [&](sf::Sprite& s, float y) {
        if (s.getGlobalBounds().width > 0)
            s.setPosition(windowCenterX - s.getGlobalBounds().width / 2.f, y);
    };

    centerSpriteX(m_easyButton, 120.f);
    centerSpriteX(m_mediumButton, 200.f);
    centerSpriteX(m_hardButton, 280.f);

    m_backToMainButton.setPosition(20.f, 420.f);
    m_backToDifficultyButton.setPosition(20.f, 420.f);

    // 5. setup view cuộn
    m_scrollingAreaRect = sf::FloatRect(100.f, 100.f, 300.f, 280.f);
    sf::Vector2f winSize(494.f, 480.f);

    sf::FloatRect viewportRect(
        m_scrollingAreaRect.left / winSize.x,
        m_scrollingAreaRect.top / winSize.y,
        m_scrollingAreaRect.width / winSize.x,
        m_scrollingAreaRect.height / winSize.y
    );

    m_scrollingView.setViewport(viewportRect);
    m_scrollingView.setSize(m_scrollingAreaRect.width, m_scrollingAreaRect.height);
    m_scrollingView.setCenter(
        m_scrollingAreaRect.left + m_scrollingAreaRect.width / 2.f,
        m_scrollingAreaRect.top + m_scrollingAreaRect.height / 2.f
    );

}

void LevelSelect::draw(sf::RenderWindow& window) {
    // 1. Luôn reset về View chuẩn (494x480) trước khi vẽ UI tĩnh
    sf::View standardView(sf::FloatRect(0.f, 0.f, 494.f, 480.f));
    window.setView(standardView);

    if (m_background.getTexture() && m_background.getTexture()->getSize().x > 0)
        window.draw(m_background);

    switch (m_currentScreen) {
        case SelectScreenState::Difficulty:
            window.draw(m_easyButton);
            window.draw(m_mediumButton);
            window.draw(m_hardButton);
            window.draw(m_backToMainButton);
            break;

        case SelectScreenState::EasyLevels:
        case SelectScreenState::MediumLevels:
        case SelectScreenState::HardLevels:
            // Khi vẽ danh sách level, ta dùng View cuộn (m_scrollingView)
            window.setView(m_scrollingView);
            for (const auto& sprite : m_levelButtonBgs) window.draw(sprite);
            for (const auto& text : m_levelButtons) window.draw(text);
            for (const auto& star : m_starTexts) window.draw(star);

            // Reset lại View chuẩn để vẽ nút Back
            // Nút Back nằm cố định ở dưới, không trôi theo danh sách
            window.setView(standardView);
            window.draw(m_backToDifficultyButton);
            break;
    }

    // Reset view lần cuối cho chắc chắn
    window.setView(standardView);
}

void LevelSelect::handleEvent(sf::Event& event, AppState& currentState,
                              std::string& selectedMap, sf::RenderWindow& window) {

    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);

    // 1. Tọa độ chuột cho UI tĩnh (Nút Back, Difficulty...) -> Dùng View chuẩn
    sf::View standardView(sf::FloatRect(0.f, 0.f, 494.f, 480.f));
    sf::Vector2f mousePosStandard = window.mapPixelToCoords(mousePixelPos, standardView);

    // 2. Tọa độ chuột cho danh sách Level -> Dùng View cuộn
    sf::Vector2f mousePosScroll = window.mapPixelToCoords(mousePixelPos, m_scrollingView);

    // Xử lý lăn chuột (Giữ nguyên logic nhưng dùng mapCoords để check vùng)
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (m_scrollingAreaRect.contains(mousePosStandard) && // Check vùng cuộn bằng tọa độ chuẩn
            (m_currentScreen != SelectScreenState::Difficulty))
        {
            float delta = event.mouseWheelScroll.delta * -20.f;
            float newOffset = m_scrollOffset + delta;
            if (newOffset < 0.f) newOffset = 0.f;
            if (newOffset > m_scrollBoundsBottom) newOffset = m_scrollBoundsBottom;
            m_scrollOffset = newOffset;
            m_scrollingView.setCenter(
                m_scrollingAreaRect.left + m_scrollingAreaRect.width / 2.f,
                m_scrollingAreaRect.top + m_scrollingAreaRect.height / 2.f + m_scrollOffset
            );
        }
        return;
    }

    if (event.type != sf::Event::MouseButtonPressed) return;
    if (event.mouseButton.button != sf::Mouse::Left) return;

    switch (m_currentScreen) {
        case SelectScreenState::Difficulty:
            // Dùng mousePosStandard cho các nút này
            if (m_easyButton.getGlobalBounds().contains(mousePosStandard)) {
                m_audio.playSound("click");
                m_currentScreen = SelectScreenState::EasyLevels;
                setupLevelList(SelectScreenState::EasyLevels);
            }
            else if (m_mediumButton.getGlobalBounds().contains(mousePosStandard)) {
                m_audio.playSound("click");
                m_currentScreen = SelectScreenState::MediumLevels;
                setupLevelList(SelectScreenState::MediumLevels);
            }
            else if (m_hardButton.getGlobalBounds().contains(mousePosStandard)) {
                m_audio.playSound("click");
                m_currentScreen = SelectScreenState::HardLevels;
                setupLevelList(SelectScreenState::HardLevels);
            }
            else if (m_backToMainButton.getGlobalBounds().contains(mousePosStandard)) {
                m_audio.playSound("click");
                currentState = AppState::MainMenu;
            }
            break;

        case SelectScreenState::EasyLevels:
        case SelectScreenState::MediumLevels:
        case SelectScreenState::HardLevels:
            // Nút Back nằm ở UI tĩnh -> Dùng mousePosStandard
            if (m_backToDifficultyButton.getGlobalBounds().contains(mousePosStandard)) {
                m_audio.playSound("click");
                m_currentScreen = SelectScreenState::Difficulty;
                m_scrollOffset = 0.f;
                m_scrollingView.setCenter(
                    m_scrollingAreaRect.left + m_scrollingAreaRect.width / 2.f,
                    m_scrollingAreaRect.top + m_scrollingAreaRect.height / 2.f
                );
            }
            // Vùng danh sách Level -> Dùng mousePosStandard để check xem có click vào vùng cuộn không
            else if (m_scrollingAreaRect.contains(mousePosStandard)) {

                // Nhưng khi check va chạm với từng nút Level -> Dùng mousePosScroll (đã tính offset cuộn)
                for (int i = 0; i < m_levelButtonBgs.size(); ++i) {
                    if (m_levelButtonBgs[i].getGlobalBounds().contains(mousePosScroll)) {
                        m_audio.playSound("click");
                        selectedMap = m_levelMapFiles[i];
                        currentState = AppState::InGame;
                        m_currentScreen = SelectScreenState::Difficulty; // Reset về Difficulty cho lần sau
                        m_scrollOffset = 0.f;
                        return;
                    }
                }
            }
            break;
    }
}

void LevelSelect::setupLevelList(SelectScreenState screen) {
    m_levelMapFiles.clear();
    m_levelButtonBgs.clear();
    m_levelButtons.clear();
    m_starTexts.clear();

    std::string prefix = "";
    std::string buttonBgTextureName = "button_level_bg.png";

    // 1. xác định prefix file theo độ khó
    switch (screen) {
        case SelectScreenState::EasyLevels:   prefix = "map6_"; break;
        case SelectScreenState::MediumLevels: prefix = "map8_"; break;
        case SelectScreenState::HardLevels:   prefix = "map10_"; break;
        default: return;
    }

    // 2.đếm số lượng map hiện có
    // kiểm tra lần lượt map_1, map_2,..., cho đến khi không tìm thấy nữa.
    int numLevels = 0;
    int i = 1;
    while (true) {
        std::string filename = prefix + std::to_string(i) + ".txt";
        std::string fullPath = "levels/maze/" + filename;

        // Kiểm tra file có tồn tại không
        std::ifstream f(fullPath);
        if (f.good()) {
            // có file, tăng đếm
            numLevels++;
            i++;
            f.close();
        } else {
            // không, dừng
            f.close();
            break;
        }
    }

    // không tìm thấy sẽ báo lỗi
    if (numLevels == 0) {
        std::cerr << "khong tim thay file: " << prefix << "*.txt" << std::endl;
    }

    sf::Texture& buttonTexture = m_assets.getTexture(buttonBgTextureName);

    // 3. Tính toán kích thước NÚT chuẩn
    float originalW = static_cast<float>(buttonTexture.getSize().x);
    if (originalW <= 0) originalW = 1.f;

    float targetBtnWidth = 260.f;
    float scaleBtn = targetBtnWidth / originalW;

    float buttonHeight = buttonTexture.getSize().y * scaleBtn;
    float buttonSpacing = 10.f;

    float currentY = m_scrollingAreaRect.top;

    // 4. tạo danh sách các nút level
    for (int i = 1; i <= numLevels; ++i) {
        // Tên file map (dùng để load và tra cứu save)
        std::string mapFileName = prefix + std::to_string(i) + ".txt";
        m_levelMapFiles.push_back(mapFileName);

        // A. Setup hình nền nút
        sf::Sprite bgSprite;
        bgSprite.setTexture(buttonTexture);
        bgSprite.setScale(scaleBtn, scaleBtn);

        float scrollCenterX = m_scrollingAreaRect.left + m_scrollingAreaRect.width / 2.f;
        bgSprite.setPosition(scrollCenterX - (targetBtnWidth / 2.f), currentY);

        m_levelButtonBgs.push_back(bgSprite);

        // B. Setup chữ "Level X"
        sf::Text levelText;
        if (m_font.getInfo().family != "") {
            levelText.setFont(m_font);
        }

        levelText.setCharacterSize(25);                 // Kích thước chữ Level
        levelText.setFillColor(sf::Color(255, 215, 0)); // Màu chữ Level (Vàng kim)
        levelText.setOutlineColor(sf::Color::Black);
        levelText.setOutlineThickness(2.0f);
        levelText.setString("Level " + std::to_string(i));

        // Căn giữa chữ Level vào nút (nhưng dịch lên trên một chút để nhường chỗ cho sao)
        sf::FloatRect textBounds = levelText.getGlobalBounds();
        sf::FloatRect spriteBounds = bgSprite.getGlobalBounds();

        // Đặt chữ Level nằm ở nửa trên của nút (trừ đi 10px y)
        levelText.setPosition(
            spriteBounds.left + (spriteBounds.width - textBounds.width) / 2.0f,
            spriteBounds.top + (spriteBounds.height - textBounds.height) / 2.0f - 10.f
        );
        m_levelButtons.push_back(levelText);

        // C. Setup sao riêng
        sf::Text starText;
        if (m_font.getInfo().family != "") starText.setFont(m_font);

        int stars = GameLogic::getLevelStars(mapFileName);
        std::string starString = "";
        for (int s = 0; s < stars; ++s) starString += "*"; // Chỉ chứa sao

        starText.setString(starString);

        // 1. Tăng kích thước sao
        starText.setCharacterSize(90);

        // 2. Chỉnh màu Trắng kem + Viền đen
        starText.setFillColor(sf::Color(255, 253, 208));
        starText.setOutlineColor(sf::Color::Black);
        starText.setOutlineThickness(2.0f);

        // 3. Căn giữa sao và đặt nằm DƯỚI chữ Level
        sf::FloatRect starBounds = starText.getGlobalBounds();
        // Tâm X giống nút, Y nằm thấp hơn chữ Level (cộng thêm -5px do size = 90 lớn)
        starText.setPosition(
            spriteBounds.left + (spriteBounds.width - starBounds.width) / 2.0f,
            spriteBounds.top + (spriteBounds.height - starBounds.height) / 2.0f - 5.f
        );

        m_starTexts.push_back(starText);

        currentY += buttonHeight + buttonSpacing;
    }

    // 5. Cập nhật vùng cuộn (Scroll bounds)
    float totalContentHeight = (numLevels * (buttonHeight + buttonSpacing));
    m_scrollBoundsBottom = 0.f;
    if (totalContentHeight > m_scrollingAreaRect.height) {
        m_scrollBoundsBottom = totalContentHeight - m_scrollingAreaRect.height + 20.f;
    }
}