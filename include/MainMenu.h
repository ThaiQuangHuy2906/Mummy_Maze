// MainMenu.h
// Màn hình menu chính của game.
// MainMenu là màn hình đầu tiên người chơi thấy khi khởi động game.
// Cung cấp các tùy chọn: Play (chơi mới), Continue (tiếp tục game đã lưu), Quit (thoát).
#pragma once
#include "AudioManager.h"
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "AppTypes.h"

// Lớp quản lý giao diện và logic của màn hình menu chính.
// Chức năng chính:
// - Hiển thị nền và các nút (Play, Continue, Quit)
// - Xử lý click chuột vào các nút
// - Kiểm tra xem có file save tồn tại để hiển thị nút Continue
// - Chuyển state sang LevelSelect hoặc tải game đã lưu
class MainMenu
{
public:
    // Constructor khởi tạo MainMenu.
    // Tải các texture cho background và buttons, kiểm tra file save.
    // assets: Reference đến AssetManager để load hình ảnh
    // audio: Reference đến AudioManager để phát âm thanh click
    MainMenu(AssetManager &assets, AudioManager &audio);

    // Xử lý các sự kiện chuột và bàn phím.
    // Kiểm tra click vào các nút và thay đổi trạng thái game tương ứng:
    // - Play button -> chuyển sang LevelSelect
    // - Continue button -> set flag để main.cpp tải game
    // - Quit button -> đóng cửa sổ
    // event: Sự kiện SFML cần xử lý
    // currentState: Reference đến AppState để thay đổi màn hình
    // window: Reference đến cửa sổ game
    void handleEvent(sf::Event &event, AppState &currentState, sf::RenderWindow &window);

    // Vẽ background và các nút lên màn hình.
    // Nút Continue chỉ được vẽ nếu có file save tồn tại.
    // window: Cửa sổ render
    void draw(sf::RenderWindow &window);

    // Kiểm tra xem người chơi có bấm nút Continue không.
    // Trả về: true nếu Continue được chọn, false nếu không
    bool isContinueSelected() const { return m_isContinueSelected; }

    // Reset cờ Continue sau khi đã xử lý.
    // Cần gọi hàm này sau khi đã tải game để tránh load lại nhiều lần.
    void resetContinueFlag() { m_isContinueSelected = false; }

private:
    AudioManager &m_audio;       // Reference đến AudioManager để phát SFX
    sf::Sprite m_background;     // Sprite hình nền menu
    sf::Sprite m_playButton;     // Nút "Play" - bắt đầu game mới
    sf::Sprite m_quitButton;     // Nút "Quit" - thoát game
    sf::Sprite m_continueButton; // Nút "Continue" - tiếp tục game đã lưu
    bool m_hasSaveFile;          // Có file save tồn tại không (để hiện nút Continue)
    bool m_isContinueSelected;   // Cờ đánh dấu người chơi đã bấm Continue
};