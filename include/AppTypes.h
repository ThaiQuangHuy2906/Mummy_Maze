#pragma once

// Kích thước thiết kế gốc
const int GAME_WIDTH = 494;
const int GAME_HEIGHT = 480;

// Hệ số phóng to
const float WINDOW_SCALE = 1.5f;

// Định nghĩa các trạng thái (màn hình)
enum class AppState {
    MainMenu,    // Đang ở menu chính
    LevelSelect, // Đang ở màn hình chọn màn
    InGame,      // Đang trong màn chơi (chạy GameLoop)
    Exiting      // Đang thoát game
};
