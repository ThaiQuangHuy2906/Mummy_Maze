#include "Animation.h"
#include "GameTypes.h"

Animation::Animation(const std::string& textureName, int mazeSize, AssetManager& assetManager)
    // Lấy texture từ AssetManager
    : texture(assetManager.getTexture(textureName)) {

    sprite.setTexture(texture); // Gán texture cho sprite

    int cols = 5; // 5 frame mỗi hàng (0: đứng yên, 1-4: bước đi)
    int rows = 4; // 4 hàng tương ứng 4 hướng (0: UP, 1: RIGHT, 2: DOWN, 3: LEFT)

    // Tính toán kích thước của một frame
    cellWidth   = texture.getSize().x / cols;
    cellHeight  = texture.getSize().y / rows;
}

// Đặt hướng cho hoạt ảnh
void Animation::setDirection(Direction dir) {
    m_direction = dir;
}

// Cập nhật frame sẽ được hiển thị
void Animation::update(int frame) {
    // Xác định hàng (row) trong spritesheet dựa vào hướng
    int row = 0;                                // Mặc định là UP
    switch (m_direction) {
        case Direction::Right:  row = 1; break;
        case Direction::Down:   row = 2; break;
        case Direction::Left:   row = 3; break;
        case Direction::Up:     row = 0; break;
        default:                row = 0; break;
    }

    // Xác định cột (col) trong spritesheet dựa vào chỉ số frame (0-4)
    int col = frame % 5;

    // Cắt (crop) đúng frame cần vẽ từ spritesheet
    sprite.setTextureRect(sf::IntRect(col * cellWidth, row * cellHeight, cellWidth, cellHeight));
}

// Vẽ sprite (frame đã được cắt) lên cửa sổ tại vị trí cho trước
void Animation::draw(sf::RenderWindow& window, sf::Vector2f pos) {
    sprite.setPosition(pos);
    window.draw(sprite);
}