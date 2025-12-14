// LevelSelect.h
// Màn hình chọn độ khó và level (màn chơi).
// LevelSelect cho phép người chơi:
// 1. Chọn độ khó (Easy, Medium, Hard)
// 2. Chọn kích thước mê cung (6x6, 8x8, 10x10)
// 3. Chọn màn chơi cụ thể từ danh sách
// Màn hình có khả năng cuộn (scroll) để hiển thị nhiều level.
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "AssetManager.h"
#include "AppTypes.h"
#include "AudioManager.h"

// Các trạng thái/màn hình con trong LevelSelect.
// LevelSelect hoạt động như một state machine nhỏ:
// - Difficulty: Màn chọn độ khó
// - EasyLevels/MediumLevels/HardLevels: Danh sách level theo độ khó
enum class SelectScreenState
{
    Difficulty,   // Màn hình chọn độ khó (Easy/Medium/Hard)
    EasyLevels,   // Danh sách các level độ khó Dễ
    MediumLevels, // Danh sách các level độ khó Trung bình
    HardLevels    // Danh sách các level độ khó Khó
};

// Lớp quản lý màn hình chọn level.
// Tính năng:
// - Hiển thị danh sách level với số sao đạt được
// - Cuộn (scroll) danh sách level bằng chuột
// - Xử lý click để chọn level và bắt đầu chơi
// - Nút Back để quay lại menu chính
class LevelSelect
{
public:
    // Constructor khởi tạo LevelSelect.
    // Tải các texture, font, và khởi tạo danh sách level.
    // assets: Reference đến AssetManager
    // audio: Reference đến AudioManager
    LevelSelect(AssetManager &assets, AudioManager &audio);

    // Xử lý sự kiện chuột và bàn phím.
    // Xử lý:
    // - Click vào nút độ khó -> chuyển sang danh sách level tương ứng
    // - Click vào level -> set selectedMap và chuyển sang InGame
    // - Scroll chuột để xem thêm level
    // - Nút Back -> quay về MainMenu
    // event: Sự kiện SFML
    // currentState: Reference đến AppState để chuyển màn hình
    // selectedMap: Output - tên file map được chọn (VD: "map6_1.txt")
    // window: Reference đến cửa sổ game
    void handleEvent(sf::Event &event, AppState &currentState,
                     std::string &selectedMap, sf::RenderWindow &window);

    // Vẽ màn hình LevelSelect.
    // Vẽ background, các nút, danh sách level và số sao đạt được.
    // window: Cửa sổ render
    void draw(sf::RenderWindow &window);

private:
    AssetManager &m_assets; // Reference đến AssetManager
    AudioManager &m_audio;  // Reference đến AudioManager

    // ===== BIẾN TRẠNG THÁI =====
    SelectScreenState m_currentScreen; // Màn hình hiện tại (Difficulty/EasyLevels/...)
    sf::Font m_font;                   // Font để vẽ text

    // ===== SCROLLING (CUỘN DANH SÁCH) =====
    sf::View m_scrollingView;          // View để cuộn danh sách level
    sf::FloatRect m_scrollingAreaRect; // Vùng có thể cuộn
    float m_scrollOffset;              // Độ lệch cuộn hiện tại
    float m_scrollBoundsBottom;        // Giới hạn cuộn dưới cùng

    // ===== CÁC NÚT LEVEL =====
    std::vector<sf::Sprite> m_levelButtonBgs; // Background cho từng nút level
    std::vector<sf::Text> m_levelButtons;     // Text hiển thị "Level 1", "Level 2", etc.
    std::vector<sf::Text> m_starTexts;        // Text hiển thị sao đạt được "★★★"
    std::vector<std::string> m_levelMapFiles; // Tên file map tương ứng với từng level

    // ===== NÚT GIAO DIỆN =====
    sf::Sprite m_background;   // Hình nền
    sf::Sprite m_easyButton;   // Nút chọn độ khó Dễ
    sf::Sprite m_mediumButton; // Nút chọn độ khó Trung bình
    sf::Sprite m_hardButton;
    sf::Sprite m_backToMainButton;
    sf::Sprite m_backToDifficultyButton;

    void setupLevelList(SelectScreenState screen);
};