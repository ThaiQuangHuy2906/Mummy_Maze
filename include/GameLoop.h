// GameLoop.h
// Lớp GameLoop chịu trách nhiệm chạy vòng lặp chính của game:
//  - Quản lý SFML window (reference).
//  - Xử lý event (chuột, bàn phím, đóng cửa sổ).
//  - Quản lý trạng thái lượt: PlayerTurn / Animating / EnemyTurn.
//  - Gọi GameLogic để cập nhật logic và GameRenderer để vẽ.

#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "AssetManager.h"
#include "GameTypes.h"
#include "WallSpriteSheet.h"
#include "StairsSpriteSheet.h"
#include "GameRenderer.h"
#include "AudioManager.h"
#include <string>

// Lớp GameLoop chạy vòng lặp chính của game
// quản lý cửa sổ game, xử lý event, cập nhật trạng thái và vẽ
class GameLoop {
public:
    // mapName: tên file map hiện tại (ví dụ: "map6_1.txt").
    // state:   tham chiếu GameState chứa toàn bộ dữ liệu màn chơi.
    // assets:  AssetManager dùng chung cho toàn game.
    // audio:   AudioManager dùng chung cho toàn game.
    // windowRef: tham chiếu tới cửa sổ SFML do main() tạo.
    GameLoop(std::string mapName, GameState& state, AssetManager& assets, AudioManager& audio, sf::RenderWindow& windowRef);

    // Hàm chính để bắt đầu và chạy vòng lặp game
    void run();
    // người chơi muốn làm gì: Restart / Next Level / Quay về LevelSelect.
    bool isRestartRequested() const { return m_restartRequested; }
    bool isNextLevelRequested() const { return m_nextLevelRequested; }
    bool isReturnToLevelSelectRequested() const { return m_returnToLevelSelect; }

private:
    // Tham chiếu tới trạng thái game hiện tại.
    GameState& gameState;
    AssetManager& assetManager;

    // Các thành phần của SFML
    AudioManager& audio;
    sf::RenderWindow& m_window; // Cửa sổ game chính
    sf::Clock clock;            // Đồng hồ để đo thời gian giữa các frame (delta time)

    // Biến trạng thái của game
    TurnState currentTurn;      // Lưu trữ lượt chơi hiện tại
    bool isGameOver;            // Flag báo hiệu game đã kết thúc hay chưa
    Difficulty m_difficulty;    // Độ khó (ảnh hưởng tới AI quái)
    sf::Time gameOverTimer;
    bool m_gameFinished;        // Khi true thì thoát khỏi vòng while trong run().

    // Trạng thái điều hướng màn hình khác.
    bool m_returnToLevelSelect = false; // Có quay lại màn chọn level không?
    sf::Sprite m_menuButtonSprite;      // Nút Menu (trong InGame)

    // Biến trạng thái của menu tạm dừng / game over.
    bool m_isPaused;            // Đang tạm dừng?
    bool m_restartRequested;    // Người chơi bấm Restart?
    bool m_showGameOverMenu;    // Đang hiển thị menu Game Over?
    bool m_playerWon;           // Người chơi thắng hay thua.

    // // Biến cho Next Level.
    bool m_nextLevelRequested;      // Người chơi có bấm Next không?
    bool m_hasNextLevel;            // Map tiếp theo có tồn tại không?
    std::string m_currentMapName;   // Lưu tên map hiện tại

    // UI: Font + Text hiển thị trong Pause/Game Over.
    sf::Font m_font;
    sf::Text m_pauseTitleText;
    sf::Text m_winText;
    sf::Text m_loseText;

    int m_starRating = 0;   // Lưu số sao đạt được (0-3)
    sf::Text m_starText;    // Text để hiển thị sao "***"

    // Sprite cho nền mờ và các nút trong Pause/GameOver.
    sf::Sprite m_pauseBgSprite;
    sf::Sprite m_resumeButtonSprite;
    sf::Sprite m_restartButtonSprite;
    sf::Sprite m_quitButtonSprite;
    sf::Sprite m_nextButtonSprite;
    sf::Sprite m_saveButtonSprite;

    // Khởi tạo font, text, button, background cho menu Pause/GameOver.
    void initMenus();

    // Xử lý event trong trạng thái Pause.
    void handlePauseEvents(const sf::Event& event);

    // Xử lý event trong trạng thái Game Over (Win/Lose).
    void handleGameOverEvents(const sf::Event& event);

    // Vẽ menu Pause.
    void drawPauseMenu();

    // Vẽ menu Game Over (Win/Lose + star rating + các nút).
    void drawGameOverMenu();

    // Các đối tượng đồ họa
    WallSpriteSheet m_wallSprites;      // Quản lý sprite tường (spritesheet).
    StairsSpriteSheet m_stairSprites;   // Quản lý sprite cầu thang.
    GameRenderer m_renderer;            // Lớp chuyên vẽ toàn bộ game.

    // Nhóm hàm xử lý event/logic/vẽ cho vòng lặp.
    void handleEvents();                // Poll & phân loại mọi event.
    void handleWindowEvents(const sf::Event& event);    // quản lý đóng cửa sổ
    void handleMouseEvents(const sf::Event& event);     // xử lý click chuột trái
    void handleKeyboardEvents(const sf::Event& event);  // xử lý bàn phím

    void update(sf::Time dt);           // Cập nhật toàn bộ logic game mỗi frame
    void draw();                        // Vẽ frame mới
};