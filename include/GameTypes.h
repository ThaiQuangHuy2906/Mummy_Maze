#pragma once

// Các trạng thái của một lượt chơi
enum class TurnState {
    PlayerTurn, // Đợi người chơi nhập input
    Animating,  // Trạng thái đang diễn ra hoạt ảnh (di chuyển, mở cổng,...)
    EnemyTurn   // Lượt của quái vật di chuyển
};

// xác định độ khó của game, ảnh hưởng đến AI của quái vật
enum class Difficulty {
    Easy,       // Dễ
    Medium,     // Bình thường
    Hard        // Khó
};

// Xác định hướng di chuyển
enum class Direction {
    Up,
    Down,
    Left,
    Right,
    None // dùng cho lúc đứng yên
};