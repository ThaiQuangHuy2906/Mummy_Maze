//
#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"

// Enum để phân loại tường cho dễ gọi
enum class WallType {
    Left,           // Tường dọc thường
    RightShadow,    // Tường dọc có bóng (góc)
    Up,             // Tường ngang có bóng
    UpNoShadow      // Tường ngang không bóng
};

class WallSpriteSheet {
public:
    WallSpriteSheet(int mazeSize, AssetManager& assetManager);

    // Hàm mới: Không vẽ ngay, mà thêm dữ liệu vào mảng VertexArray
    void appendWall(sf::VertexArray& va, sf::Vector2f position, WallType type);

    // Lấy texture để GameRenderer dùng khi vẽ batch
    const sf::Texture* getTexture() const { return m_texture; }

private:
    const sf::Texture* m_texture; // Lưu con trỏ texture
    sf::IntRect leftWallRect, rightWallRect, upWallRect, upWallNoShadowRect;
};