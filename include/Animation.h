#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "GameTypes.h"

// Lớp Animation quản lý việc hiển thị hoạt ảnh từ một tệp ảnh spritesheet.
// Một spritesheet là một tệp ảnh chứa nhiều frame hoạt ảnh khác nhau.
class Animation {
private:
    sf::Sprite sprite;                          // Sprite để vẽ lên màn hình
    sf::Texture& texture;                       // Tham chiếu đến texture (được quản lý bởi AssetManager)
    int cellWidth, cellHeight;                  // Kích thước (pixel) của một frame đơn trong spritesheet
    Direction m_direction = Direction::Down;    // Hướng mặc định

public:
    // tải ảnh và tính toán kích thước frame
    Animation(const std::string& textureName, int mazeSize, AssetManager& assetManager);

    void update(int frame);                 // Cập nhật frame hoạt ảnh sẽ được vẽ
    void setDirection(Direction dir);       // Đặt hướng của hoạt ảnh (UP, DOWN, LEFT, RIGHT)
    void draw(sf::RenderWindow& window, sf::Vector2f position); // Vẽ frame hiện tại lên màn hình
};