#pragma once

// Các trạng thái của một lượt chơi trong game.
// Game hoạt động theo từng lượt (turn-based), mỗi lượt có 3 trạng thái chính:
enum class TurnState
{
    PlayerTurn, // Đợi người chơi nhập lệnh di chuyển (nhấn phím mũi tên hoặc WASD)
    Animating,  // Đang phát hoạt ảnh di chuyển, mở cổng, va chạm, etc.
    EnemyTurn   // Lượt của các quái vật tự động di chuyển theo AI
};

// Độ khó của game, ảnh hưởng đến hành vi AI của quái vật.
// Độ khó quyết định mức độ thông minh khi quái vật đuổi theo người chơi:
// - Easy: Quái di chuyển ngẫu nhiên, ít đuổi theo
// - Medium: Quái đuổi theo một cách cơ bản
// - Hard: Quái sử dụng thuật toán BFS để tìm đường ngắn nhất đến người chơi
enum class Difficulty
{
    Easy,   // Độ khó dễ - quái di chuyển đơn giản
    Medium, // Độ khó trung bình - quái có chiến thuật cơ bản
    Hard    // Độ khó cao - quái sử dụng AI thông minh (BFS pathfinding)
};

// Các hướng di chuyển trong game.
// Được dùng để xác định:
// - Hướng di chuyển của nhân vật
// - Hướng nhìn/quay mặt của sprite
// - Hàng (row) trong spritesheet để chọn frame animation đúng
enum class Direction
{
    Up,    // Di chuyển/nhìn lên trên
    Down,  // Di chuyển/nhìn xuống dưới
    Left,  // Di chuyển/nhìn sang trái
    Right, // Di chuyển/nhìn sang phải
    None   // Không di chuyển, đứng yên (frame 0 của animation)
};