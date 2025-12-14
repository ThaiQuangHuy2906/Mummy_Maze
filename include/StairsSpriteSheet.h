// StairsSpriteSheet.h
// Quản lý và vẽ cầu thang (stairs) - điểm kết thúc màn chơi.
// Cầu thang là mục tiêu cuối cùng người chơi cần đến để hoàn thành màn.
// Spritesheet chứa 4 hình ảnh cầu thang khác nhau tương ứng 4 hướng đặt.
#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"

// Lớp quản lý texture và vẽ cầu thang theo hướng.
// Spritesheet được chia thành 4 phần:
// - Idx 0: Cầu thang ở phía trên (Up)
// - Idx 1: Cầu thang ở phía phải (Right)
// - Idx 2: Cầu thang ở phía dưới (Down)
// - Idx 3: Cầu thang ở phía trái (Left)
class StairsSpriteSheet
{
private:
    sf::Sprite sprite; // Sprite để vẽ cầu thang lên màn hình
    int cellWidth;     // Chiều rộng của một hình ảnh cầu thang trong spritesheet
    int cellHeight;    // Chiều cao của một hình ảnh cầu thang trong spritesheet

public:
    // Constructor tải texture cầu thang và tính kích thước frame.
    // mazeSize: Kích thước mê cung (6, 8, hoặc 10) - quyết định file texture
    // assetManager: Reference đến AssetManager để load texture
    StairsSpriteSheet(int mazeSize, AssetManager &assetManager);

    // Vẽ cầu thang lên màn hình.
    // window: Cửa sổ render
    // index: Chỉ số xác định hướng cầu thang (0:Up, 1:Right, 2:Down, 3:Left)
    // position: Vị trí pixel để vẽ cầu thang
    void draw(sf::RenderWindow &window, int index, sf::Vector2f position);
};