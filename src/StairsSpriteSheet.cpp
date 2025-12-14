#include "StairsSpriteSheet.h"

StairsSpriteSheet::StairsSpriteSheet(int mazeSize, AssetManager& assetManager) {\
    // Tải texture cầu thang tương ứng với kích thước mê cung
    std::string textureName = "stairs" + std::to_string(mazeSize) + ".png";
    sprite.setTexture(assetManager.getTexture(textureName));

    // Spritesheet cầu thang có 4 frame nằm ngang (UP, RIGHT, DOWN, LEFT)
    // Tính toán kích thước của một frame
    cellWidth   = sprite.getTexture()->getSize().x / 4;
    cellHeight  = sprite.getTexture()->getSize().y;
}

// Hàm vẽ cầu thang
void StairsSpriteSheet::draw(sf::RenderWindow& window, int index, sf::Vector2f position) {
    // Cắt đúng frame cầu thang cần vẽ dựa trên chỉ số (index)
    // index = 0: UP, 1: RIGHT, 2: DOWN, 3: LEFT
    sprite.setTextureRect(sf::IntRect(index * cellWidth, 0, cellWidth, cellHeight));
    sprite.setPosition(position);
    window.draw(sprite);
}