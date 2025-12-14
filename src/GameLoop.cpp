#include "GameLoop.h"
#include "GameRenderer.h"
#include "GameLogic.h"
#include "GateAnimation.h"
#include "Character.h"
#include <AudioManager.h>
#include <iostream>
#include <fstream>

// Constructor
GameLoop::GameLoop(std::string mapName, GameState& state, AssetManager& assets, AudioManager& audio, sf::RenderWindow& windowRef)
    : m_currentMapName(mapName), // Lưu tên map
    gameState(state),
    assetManager(assets),     // Khởi tạo các tham chiếu
    audio(audio),
    m_window(windowRef),
    currentTurn(TurnState::PlayerTurn),
    isGameOver(false),
    m_gameFinished(false),
    m_isPaused(false),
    m_restartRequested(false),
    m_showGameOverMenu(false),
    m_playerWon(false),
    m_nextLevelRequested(false),
    m_hasNextLevel(false),
    m_wallSprites(state.mazeSize, assets),
    m_stairSprites(state.mazeSize, assets),
    m_renderer(assets, m_wallSprites, m_stairSprites) {

    // Xác định độ khó dựa trên kích thước mê cung
    if (gameState.mazeSize >= 10)       m_difficulty = Difficulty::Hard;
    else if (gameState.mazeSize >= 8)   m_difficulty = Difficulty::Medium;
    else                                m_difficulty = Difficulty::Easy;

    // Kiểm tra xem có map tiếp theo không
    try {
        size_t lastUnderscore = m_currentMapName.find_last_of('_');
        size_t dotTxt = m_currentMapName.find(".txt");

        if (lastUnderscore != std::string::npos && dotTxt != std::string::npos) {
            std::string prefix = m_currentMapName.substr(0, lastUnderscore + 1);
            std::string numStr = m_currentMapName.substr(lastUnderscore + 1, dotTxt - lastUnderscore - 1);

            int nextNum = std::stoi(numStr) + 1;
            std::string nextMapName = prefix + std::to_string(nextNum) + ".txt";

            // kiểm tra file có tồn tại không
            std::ifstream f("levels/maze/" + nextMapName);
            if (f.good())
                m_hasNextLevel = true;

            f.close();
        }
    } catch (...) {
        // Nếu parse tên file lỗi thì coi như không có level tiếp theo.
        m_hasNextLevel = false;
    }

    // Xây dựng lưới mê cung một lần duy nhất lúc bắt đầu
    m_renderer.initMazeMesh(gameState);
    audio.playMusic();  // Bật nhạc nền.
    initMenus();        // Khởi tạo UI (font, text, background, nút menu trong Pause/GameOver).
}

// Hàm khởi tạo Menu (Setup Font, Text, Nút bấm)
void GameLoop::initMenus() {
    sf::Vector2u winSize = m_window.getSize();
    sf::Vector2f winCenter(winSize.x / 2.f, winSize.y / 2.f);

    // Load Font dùng chung cho toàn bộ UI InGame.
    if (!m_font.loadFromFile("assets/fonts/Super Joyful.ttf"))
       std::cerr << "Error loading font\n";

    // Setup text cơ bản cho các tiêu đề (PAUSE, YOU WIN, YOU LOSE).
    auto setupText = [&](sf::Text& text, std::string str, int size, sf::Color color) {
        text.setFont(m_font);
        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(color);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(3);

        // Đặt origin vào giữa chữ để dễ căn giữa bằng setPosition(center).
        sf::FloatRect r = text.getLocalBounds();
        text.setOrigin(r.left + r.width/2.f, r.top + r.height/2.f);
        text.setPosition(winCenter.x, winCenter.y - 100.f);
    };

    // Setup 3 dòng chữ chính cho menu.
    setupText(m_pauseTitleText, "PAUSE", 60, sf::Color::White);
    setupText(m_winText, "YOU WIN!", 50, sf::Color::Yellow);
    setupText(m_loseText, "YOU LOSE!", 50, sf::Color::Red);

    // Setup Text hiển thị Sao (Star Rating)
    m_starText.setFont(m_font);
    m_starText.setCharacterSize(80);
    m_starText.setFillColor(sf::Color::Yellow);
    m_starText.setOutlineColor(sf::Color::Black);
    m_starText.setOutlineThickness(3);
    m_starText.setPosition(winCenter.x, winCenter.y - 20.f); // Nằm dưới chữ You Win

    // Nền mờ phía sau menu (Pause/Game Over).
    m_pauseBgSprite.setTexture(assetManager.getTexture("cartoon-cliff-background.jpg"));
    sf::Vector2u bgSize = m_pauseBgSprite.getTexture()->getSize();
    m_pauseBgSprite.setScale((float)winSize.x / bgSize.x, (float)winSize.y / bgSize.y);
    // Giảm sáng + tăng alpha cho cảm giác mờ.
    m_pauseBgSprite.setColor(sf::Color(100, 100, 100, 200));

    // Nạp texture cho các nút.
    m_resumeButtonSprite.setTexture(assetManager.getTexture("ingame_resume-button.png"));
    m_restartButtonSprite.setTexture(assetManager.getTexture("ingame_restart_button.png"));
    m_quitButtonSprite.setTexture(assetManager.getTexture("ingame_quit_button.png"));
    m_nextButtonSprite.setTexture(assetManager.getTexture("ingame_next_button.png"));
    m_menuButtonSprite.setTexture(assetManager.getTexture("ingame_menu_button.png"));
    m_saveButtonSprite.setTexture(assetManager.getTexture("button_save_game.png"));

    // Hàm helper để scale nút & đặt origin cho các nút.
    auto setupButton = [&](sf::Sprite& s) {
        float targetSize = 80.f;    // chiều rộng mong muốn
        float scale = targetSize / s.getTexture()->getSize().x;
        s.setScale(scale, scale);
        sf::FloatRect b = s.getLocalBounds();
        s.setOrigin(b.width / 2.f, b.height / 2.f);
    };

    setupButton(m_resumeButtonSprite);
    setupButton(m_restartButtonSprite);
    setupButton(m_quitButtonSprite);
    setupButton(m_nextButtonSprite);
    setupButton(m_menuButtonSprite);
    setupButton(m_saveButtonSprite);

    // Một vị trí mặc định, chủ yếu dành cho Resume,
    // còn GameOverMenu sẽ set lại vị trí chính xác sau.
    float buttonY = winSize.y - 80.f;
    float spacing = 40.f;
    float btnW = m_resumeButtonSprite.getGlobalBounds().width;

    m_restartButtonSprite.setPosition(winCenter.x, buttonY);
    m_resumeButtonSprite.setPosition(winCenter.x - btnW - spacing, buttonY);
    m_quitButtonSprite.setPosition(winCenter.x + btnW + spacing, buttonY);
}

// Hàm vòng lặp chính
void GameLoop::run() {
    // Tính toán vị trí pixel của người chơi
    if (gameState.explorer)
        gameState.explorer->setPosition(
            gameState.explorer->getPosition().x,
            gameState.explorer->getPosition().y,
            gameState
        );

    // Vòng lặp chính: chạy cho tới khi:
    //  - cửa sổ bị đóng, hoặc
    //  - m_gameFinished = true (người chơi thoát về menu / level select / next level).
    while (m_window.isOpen() && !m_gameFinished) {
        sf::Time dt = clock.restart();  // Đo thời gian đã trôi qua kể từ frame trước (delta time)

        if (m_showGameOverMenu) {
            // Nếu đang ở màn hình Game Over, chỉ xử lý event liên quan đến menu này.
            sf::Event event;
            while (m_window.pollEvent(event)) {
                handleWindowEvents(event);      // Cho phép đóng cửa sổ.
                handleGameOverEvents(event);    // Bấm Next/Restart/Menu...
            }
        }
        else if (m_isPaused) {
            // Nếu đang Pause, xử lý event của menu Pause.
            sf::Event event;
            while (m_window.pollEvent(event)) {
                handleWindowEvents(event);
                handlePauseEvents(event);
            }
        }
        else {
            // Trạng thái chơi bình thường:
            //  - Xử lý tất cả event input (chuột + bàn phím).
            //  - Cập nhật logic dựa trên dt
            handleEvents(); // Xử lý sự kiện (input)
            update(dt);     // Cập nhật logic game
        }

        draw();             // Vẽ lại màn hình
    }
}

// Xử lý tất cả event (khi đang InGame và không ở Pause/GameOver).
void GameLoop::handleEvents() {
    sf::Event event;

    // Lặp qua tất cả các sự kiện đang chờ xử lý
    while (m_window.pollEvent(event)) {
        handleWindowEvents(event);      // Xử lý sự kiện cửa sổ
        handleMouseEvents(event);       // xử lý click chuột trái
        handleKeyboardEvents(event);    // xử lý bàn phím
    }
}

// Xử lý sự kiện cửa sổ
void GameLoop::handleWindowEvents(const sf::Event& event) {
    if (event.type == sf::Event::Closed) m_window.close();
}

// Xử lý chuột
void GameLoop::handleMouseEvents(const sf::Event& event) {
    if (event.type != sf::Event::MouseButtonPressed) return;
    if (event.mouseButton.button != sf::Mouse::Left) return;

    // 1. Lấy tọa độ chuột và chuyển đổi sang hệ tọa độ Game
    sf::Vector2i mPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f mousePosF = m_window.mapPixelToCoords(mPos);

    // Kiểm tra click nút undo
    if (m_renderer.getUndoButtonBounds().contains(mousePosF))
        if (!gameState.history.empty() && currentTurn == TurnState::PlayerTurn) {
            audio.playSound("click");
            gameState.restoreSnapshot();
            return;
        }

    if (currentTurn != TurnState::PlayerTurn) return;
    if (!gameState.explorer) return;

    // 2. Chuẩn bị tọa độ cho hàm xử lý Grid
    // Vì logic game (pixelToGrid) tính toán dựa trên độ phân giải gốc 494x480,
    // nên ta dùng tọa độ mousePosF (đã được map về không gian gốc) để tính toán.
    sf::Vector2i mouseInputForGrid;
    mouseInputForGrid.x = static_cast<int>(mousePosF.x);
    mouseInputForGrid.y = static_cast<int>(mousePosF.y);

    sf::Vector2i targetGridPos = GameLogic::pixelToGrid(mouseInputForGrid, gameState);

    // Nếu click ra ngoài vùng chơi thì bỏ qua
    if (targetGridPos.x == -1) return;

    sf::Vector2i currentPos = gameState.explorer->getPosition();

    // Tính khoảng cách
    int distance = abs(targetGridPos.x - currentPos.x) + abs(targetGridPos.y - currentPos.y);

    // TH1: Click vào ô bên cạnh (Di chuyển)
    if (distance == 2) {
        if (gameState.explorer->isMoveValid(targetGridPos, gameState)) {

            // Lưu trạng thái trước khi đi
            gameState.saveSnapshot();

            audio.playSound("click");
            ++gameState.stepCount;
            gameState.explorer->moveTo(targetGridPos, gameState, audio);
            currentTurn = TurnState::Animating;
        }
    }
    // TH2: Click tại chỗ (Đứng yên / Bỏ lượt)
    else if (distance == 0) {

        // Lưu trạng thái trước khi đứng yên
        gameState.saveSnapshot();

        audio.playSound("click");
        currentTurn = TurnState::Animating;
    }
}

// Xử lý bàn phím
void GameLoop::handleKeyboardEvents(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return;

    // 1. Xử lý Pause
    if (event.key.code == sf::Keyboard::Escape) {
        m_isPaused = !m_isPaused;
        audio.playSound("click");
        return;
    }

    // 2. Kiểm tra điều kiện lượt chơi
    if (currentTurn != TurnState::PlayerTurn) return;
    if (!gameState.explorer) return;

    sf::Vector2i currentPos = gameState.explorer->getPosition();
    sf::Vector2i nextPos = currentPos;

    // 3. Xác định phím bấm
    switch (event.key.code) {
        case sf::Keyboard::Up:    nextPos.x -= 2; break;
        case sf::Keyboard::Down:  nextPos.x += 2; break;
        case sf::Keyboard::Left:  nextPos.y -= 2; break;
        case sf::Keyboard::Right: nextPos.y += 2; break;
        case sf::Keyboard::Space:
            // Lưu ý: Đứng yên cũng là một hành động chiến thuật,
            // nên phải lưu lại để người chơi có thể Undo nếu lỡ tay.
            gameState.saveSnapshot();

            audio.playSound("click");

            // Thay vì tự gọi startTurn, ta chuyển sang Animating giống logic chuột
            // Hàm update() sẽ tự phát hiện nhân vật không di chuyển và chuyển lượt cho quái.
            currentTurn = TurnState::Animating;
            return;

        default: return;
    }

    // 4. Logic di chuyển (Mũi tên)
    if (nextPos != currentPos && gameState.explorer->isMoveValid(nextPos, gameState)) {

        gameState.saveSnapshot();

        ++gameState.stepCount;
        gameState.explorer->moveTo(nextPos, gameState, audio);
        currentTurn = TurnState::Animating;
    }
}

// pause events
void GameLoop::handlePauseEvents(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        m_isPaused = false;
        audio.playSound("click");
        return;
    }
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mPos = sf::Mouse::getPosition(m_window);
        // Dùng mapPixelToCoords để lấy đúng tọa độ nút bấm khi phóng to
        sf::Vector2f mousePos = m_window.mapPixelToCoords(mPos);

        // Xử lý nút Save
        if (m_saveButtonSprite.getGlobalBounds().contains(mousePos)) {
            audio.playSound("click");
            GameLogic::saveGameProgress(gameState, m_currentMapName);
            return; // Thoát hàm để không click nhầm nút khác
        }
        else if (m_resumeButtonSprite.getGlobalBounds().contains(mousePos)) {
            audio.playSound("click");
            m_isPaused = false;
        }
        else if (m_restartButtonSprite.getGlobalBounds().contains(mousePos)) {
            audio.playSound("click");
            m_restartRequested = true;
            m_gameFinished = true;
        }
        else if (m_quitButtonSprite.getGlobalBounds().contains(mousePos)) {
            audio.playSound("click");
            m_gameFinished = true;
        }
    }
}

// Hàm xử lý sự kiện khi Game Over (Click nút)
void GameLoop::handleGameOverEvents(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mPos = sf::Mouse::getPosition(m_window);
        sf::Vector2f mousePos = m_window.mapPixelToCoords(mPos);

        // 1. Click Restart
        if (m_restartButtonSprite.getGlobalBounds().contains(mousePos)) {
            audio.playSound("click");
            m_restartRequested = true;
            m_gameFinished = true;
        }
        // 2. Click Menu (Về chọn level)
        else if (m_menuButtonSprite.getGlobalBounds().contains(mousePos)) {
            audio.playSound("click");
            m_returnToLevelSelect = true;
            m_gameFinished = true;
        }
        // 3. Click Next (Chỉ khi thắng và có màn sau)
        else if (m_playerWon && m_hasNextLevel && m_nextButtonSprite.getGlobalBounds().contains(mousePos)) {
            audio.playSound("click");
            m_nextLevelRequested = true;
            m_gameFinished = true;
        }
    }
}

void GameLoop::update(sf::Time dt) {
    if (m_showGameOverMenu) return;

    if (isGameOver) {
        gameOverTimer += dt;
        if (gameOverTimer.asSeconds() > 1.5f) { // 5 giây thì hiện bảng kết quả
            m_showGameOverMenu = true;

            sf::Vector2i pPos = gameState.explorer->getPosition();
            sf::Vector2i sPos = gameState.stairPosition;
            if ((pPos.x == sPos.x && abs(pPos.y - sPos.y) == 1) ||
                (pPos.y == sPos.y && abs(pPos.x - sPos.x) == 1)) {

                m_playerWon = true;

                // Mê cung 6x6 (Easy): Chuẩn khoảng 20 bước
                // Mê cung 8x8 (Medium): Chuẩn khoảng 30 bước
                // Mê cung 10x10 (Hard): Chuẩn khoảng 40 bước

                int benchmark = gameState.mazeSize * 4; // Ví dụ: size 6 -> 24 bước là mốc

                if (gameState.stepCount <= benchmark) {
                    m_starRating = 3;
                    m_starText.setString("* * *"); // 3 Sao
                }
                else if (gameState.stepCount <= benchmark + 10) {
                    m_starRating = 2;
                    m_starText.setString("* *");  // 2 Sao
                }
                else {
                    m_starRating = 1;
                    m_starText.setString("*");   // 1 Sao
                }

                // gameState.stepCount: số bước đi
                // m_starRating: số sao vừa tính được
                GameLogic::saveLevelResult(m_currentMapName, gameState.stepCount, m_starRating);

                // Căn giữa dòng text sao
                sf::FloatRect r = m_starText.getLocalBounds();
                m_starText.setOrigin(r.left + r.width/2.f, r.top + r.height/2.f);
            }
            else
                m_playerWon = false;
        }
        return;
    }

    using namespace GameLogic;
    // Cập nhật tất cả hoạt ảnh trước tiên
    updateGateAnimation(gameState, dt);
    if (gameState.explorer) gameState.explorer->update(dt);
    for (auto& e: gameState.enemies) e->update(dt);

    // Điều này sẽ bắt được ngay khoảnh khắc quái vật vừa di chuyển xong
    checkGameConditions(gameState, isGameOver, audio);
    // Cập nhật logic game và lượt đi
    updateKeyAndGateLogic(gameState, audio);

    if (currentTurn == TurnState::Animating) {
        // Kiểm tra xem còn hoạt ảnh nào đang chạy không
        bool stillAnimating = false;
        if (gameState.explorer && gameState.explorer->isMoving())
            stillAnimating = true;  // explorer đang di chuyển
        if (gameState.gate.isAnimating)
            stillAnimating = true;  // Cổng đang mở/đóng

        // Nếu tất cả hoạt ảnh đã kết thúc
        if (!stillAnimating) {
            // Điều chỉnh số bước đi mặc định của quái vật (2 cho Mummy và 1 cho Scorpion)
            for (auto& e: gameState.enemies) e->startTurn();
            // Chuyển sang lượt của quái vật
            currentTurn = TurnState::EnemyTurn;
        }
    }
    else if (currentTurn == TurnState::EnemyTurn) {
        if (gameState.explorer)
            // Yêu cầu mỗi quái vật quyết định nước đi của mình
            for (auto& enemy : gameState.enemies)
                enemy->decideMove(*gameState.explorer, gameState, m_difficulty, audio);

        bool finishedTurn = allEnemiesFinished(gameState);
        if (finishedTurn) {
            GameLogic::resolveEnemyCollisions(gameState, audio);    // Xử lý va chạm địch-địch
            checkGameConditions(gameState, isGameOver, audio);      // Kiểm tra điều kiện thắng/thua
            currentTurn = TurnState::PlayerTurn;                    // Chuyển về lượt người chơi
        }
    }
}

// // Vẽ toàn bộ frame hiện tại.
void GameLoop::draw() {
    m_window.clear();     // Xóa màn hình của frame trước
    m_renderer.drawGame(m_window, gameState);  // vẽ toàn bộ trạng thái game

    // Nếu đang ở Game Over -> vẽ menu Game Over đè lên.
    if (m_showGameOverMenu)
        drawGameOverMenu();
        // Nếu đang Pause -> vẽ menu Pause.
    else if (m_isPaused)
        drawPauseMenu();

    m_window.display();   // hiển thị frame mới lên màn hình
}

// Vẽ menu pause
void GameLoop::drawPauseMenu() {
    // 1. Reset về View chuẩn 494x480 (tránh bị view của GameRenderer ảnh hưởng).
    sf::View standardView(sf::FloatRect(0.f, 0.f, 494.f, 480.f));
    m_window.setView(standardView);

    // 2. Vẽ nền mờ phía sau.
    m_window.draw(m_pauseBgSprite);

    // 3. Tính toán tọa độ logic cho UI.
    float logicalWidth = 494.f;
    float logicalHeight = 480.f;
    float centerX = logicalWidth / 2.f;
    float centerY = logicalHeight / 2.f;

    // 4. Đặt và vẽ chữ "PAUSE" ở phía trên giữa.
    m_pauseTitleText.setPosition(centerX, centerY - 50.f);
    m_window.draw(m_pauseTitleText);

    // 5. Tính vị trí ngang để xếp các nút dưới cùng:
    //    Save - Resume - Restart - Quit.
    float buttonY = logicalHeight - 150.f;  // Cách đáy 150px
    float spacing = 20.f;                   // Khoảng cách giữa các nút
    float btnW = m_resumeButtonSprite.getGlobalBounds().width;

    // Tính toán vị trí X để căn giữa 4 nút
    // Thứ tự từ trái qua phải: Save -> Resume -> Restart -> Quit
    // Nút Save (Ngoài cùng bên trái)
    m_saveButtonSprite.setPosition(centerX - 1.5f * (btnW + spacing), buttonY);
    // Nút Resume (Giữa trái)
    m_resumeButtonSprite.setPosition(centerX - 0.5f * (btnW + spacing), buttonY);
    // Nút Restart (Giữa phải)
    m_restartButtonSprite.setPosition(centerX + 0.5f * (btnW + spacing), buttonY);
    // Nút Quit (Ngoài cùng bên phải)
    m_quitButtonSprite.setPosition(centerX + 1.5f * (btnW + spacing), buttonY);

    // Vẽ lần lượt các nút.
    m_window.draw(m_saveButtonSprite);
    m_window.draw(m_resumeButtonSprite);
    m_window.draw(m_restartButtonSprite);
    m_window.draw(m_quitButtonSprite);
}

// Hàm vẽ giao diện Game Over (Sắp xếp nút + Vẽ Sao)
void GameLoop::drawGameOverMenu() {
    // 1. Reset về View chuẩn 494x480
    sf::View standardView(sf::FloatRect(0.f, 0.f, 494.f, 480.f));
    m_window.setView(standardView);

    m_window.draw(m_pauseBgSprite);

    // 2. Tính toán tọa độ trung tâm chuẩn
    float logicalWidth = 494.f;
    float logicalHeight = 480.f;
    float centerX = logicalWidth / 2.f;
    float centerY = logicalHeight / 2.f;

    // 3. Cập nhật lại vị trí Text cho đúng giữa màn hình chuẩn
    // Đặt chữ YOU WIN cao hơn tâm một chút
    m_winText.setPosition(centerX, centerY - 60.f);
    m_loseText.setPosition(centerX, centerY - 60.f);

    // Đặt Sao ngay dưới chữ YOU WIN
    m_starText.setPosition(centerX, centerY + 10.f);

    // Vẽ Text
    if (m_playerWon) {
        m_window.draw(m_winText);

        // Căn giữa text sao (Origin)
        sf::FloatRect r = m_starText.getLocalBounds();
        m_starText.setOrigin(r.left + r.width/2.f, r.top + r.height/2.f);
        m_window.draw(m_starText);
    }
    else {
        m_window.draw(m_loseText);
    }

    // 4. Tính toán vị trí Nút bấm (Dựa trên tọa độ chuẩn)
    float buttonY = logicalHeight - 80.f; // Cách đáy 80px
    float spacing = 30.f;
    float btnW = m_restartButtonSprite.getGlobalBounds().width;

    // Sắp xếp nút
    if (m_playerWon && m_hasNextLevel) {
        m_menuButtonSprite.setPosition(centerX - btnW - spacing, buttonY);
        m_restartButtonSprite.setPosition(centerX, buttonY);
        m_nextButtonSprite.setPosition(centerX + btnW + spacing, buttonY);

        m_window.draw(m_menuButtonSprite);
        m_window.draw(m_restartButtonSprite);
        m_window.draw(m_nextButtonSprite);
    }
    else {
        m_menuButtonSprite.setPosition(centerX - btnW/2.f - spacing/2.f, buttonY);
        m_restartButtonSprite.setPosition(centerX + btnW/2.f + spacing/2.f, buttonY);

        m_window.draw(m_menuButtonSprite);
        m_window.draw(m_restartButtonSprite);
    }
}