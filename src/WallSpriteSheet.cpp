#include "WallSpriteSheet.h"

WallSpriteSheet::WallSpriteSheet(int mazeSize, AssetManager& assetManager) {
    // Lưu con trỏ texture thay vì sprite
    std::string textureName = "walls" + std::to_string(mazeSize) + ".png";
    m_texture = &assetManager.getTexture(textureName);

    // (Giữ nguyên logic tọa độ rect như cũ)
    if (mazeSize == 6) {
        leftWallRect = {0, 0, 12, 78};
        rightWallRect = {84, 0, 12, 78};
        upWallRect = {12, 0, 72, 18};
        upWallNoShadowRect = {12, 0, 66, 18};
    } else if (mazeSize == 8) {
        leftWallRect = {0, 0, 12, 63};
        rightWallRect = {69, 0, 12, 63};
        upWallRect = {12, 0, 57, 18};
        upWallNoShadowRect = {12, 0, 51, 18};
    } else if (mazeSize == 10) {
        leftWallRect = {0, 0, 8, 48};
        rightWallRect = {52, 0, 8, 48};
        upWallRect = {8, 0, 44, 12};
        upWallNoShadowRect = {8, 0, 38, 12};
    }
}

void WallSpriteSheet::appendWall(sf::VertexArray& va, sf::Vector2f position, WallType type) {
    // 1. Xác định Rect cần dùng
    sf::IntRect rect;
    switch (type) {
        case WallType::Left:        rect = leftWallRect; break;
        case WallType::RightShadow: rect = rightWallRect; break;
        case WallType::Up:          rect = upWallRect; break;
        case WallType::UpNoShadow:  rect = upWallNoShadowRect; break;
    }

    // 2. Tạo 4 đỉnh cho 1 hình chữ nhật (Quad)
    // Thứ tự: Top-Left -> Top-Right -> Bottom-Right -> Bottom-Left
    float w = static_cast<float>(rect.width);
    float h = static_cast<float>(rect.height);
    float u = static_cast<float>(rect.left);
    float v = static_cast<float>(rect.top);

    // Đỉnh 1: Top-Left
    va.append(sf::Vertex(position, sf::Vector2f(u, v)));

    // Đỉnh 2: Top-Right
    va.append(sf::Vertex(sf::Vector2f(position.x + w, position.y), sf::Vector2f(u + w, v)));

    // Đỉnh 3: Bottom-Right
    va.append(sf::Vertex(sf::Vector2f(position.x + w, position.y + h), sf::Vector2f(u + w, v + h)));

    // Đỉnh 4: Bottom-Left
    va.append(sf::Vertex(sf::Vector2f(position.x, position.y + h), sf::Vector2f(u, v + h)));
}