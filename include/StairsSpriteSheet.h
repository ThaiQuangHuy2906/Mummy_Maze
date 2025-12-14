#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"

// Lớp StairsSpriteSheet: vẽ cầu thang
// Spritesheet của cầu thang chứa 4 hình ảnh tương ứng với 4 vị trí (trên, phải, dưới, trái).
class StairsSpriteSheet {
private:
    sf::Sprite sprite;
    int cellWidth;      // Chiều rộng của một hình ảnh cầu thang
    int cellHeight;     // Chiều cao của một hình ảnh cầu thang

public:
    StairsSpriteSheet(int mazeSize, AssetManager& assetManager);

    // Hàm vẽ, nhận vào chỉ số (index) để biết cần vẽ cầu thang ở vị trí nào (U/R/D/L)
    void draw(sf::RenderWindow& window, int index, sf::Vector2f position);
};