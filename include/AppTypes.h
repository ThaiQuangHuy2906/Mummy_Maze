// AppTypes.h
// Định nghĩa các hằng số và enum chung cho toàn bộ ứng dụng.
// File này chứa các thông số cấu hình cơ bản và trạng thái của game.
#pragma once

// ===== KÍCH THƯỚC CỬA SỔ =====

// Chiều rộng thiết kế gốc của game (internal resolution).
// Đây là kích thước mà tất cả các tính toán vị trí, collision được thực hiện.
// Cửa sổ thực tế sẽ được scale lên dựa trên WINDOW_SCALE.
const int GAME_WIDTH = 494;

// Chiều cao thiết kế gốc của game (internal resolution).
const int GAME_HEIGHT = 480;

// Hệ số phóng to cửa sổ game.
// Cửa sổ thực tế = GAME_WIDTH * WINDOW_SCALE x GAME_HEIGHT * WINDOW_SCALE
// VD: 494 * 1.5 = 741, 480 * 1.5 = 720
// Tăng giá trị này để game hiển thị to hơn trên màn hình.
const float WINDOW_SCALE = 1.5f;

// ===== TRẠNG THÁI ỨNG DỤNG =====

// Các màn hình/trạng thái chính của game.
// Game sử dụng state machine để chuyển đổi giữa các màn hình.
// Mỗi state có logic render và input handling riêng.
enum class AppState
{
    MainMenu,    // Màn hình menu chính (Play, Settings, Exit)
    LevelSelect, // Màn hình chọn màn chơi (6x6, 8x8, 10x10 + các level)
    InGame,      // Đang trong gameplay (GameLoop đang chạy)
    Exiting      // Game đang đóng (dọn dẹp tài nguyên và thoát)
};
