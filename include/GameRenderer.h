// GameRenderer.h
// Lớp chuyên trách vẽ mọi thứ lên màn hình InGame:
//  - Nền, sàn.
//  - Tường (đã build trước thành VertexArray).
//  - Chìa khóa, cổng, cầu thang, bẫy.
//  - Nhân vật (explorer + enemies).
//  - Nút Undo + Text số bước đi.

#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "AssetManager.h"
#include "WallSpriteSheet.h"
#include "StairsSpriteSheet.h"

class GameRenderer {
public:
    GameRenderer(AssetManager& assets, WallSpriteSheet& walls, StairsSpriteSheet& stairs);

    // Hàm gọi 1 lần lúc bắt đầu game để tạo lưới mê cung (m_wallMesh).
    // Duyệt map, phát hiện vị trí tường và convert sang vertex trong m_wallMesh.
    void initMazeMesh(const GameState& gameState);

    // Hàm vẽ toàn bộ game: nền, sàn, tường, chìa, cổng, cầu thang, bẫy, nhân vật, nút Undo.
    void drawGame(sf::RenderWindow& window, GameState& gameState);

    // Hàm lấy vùng bấm của nút Undo (dùng cho xử lý click chuột).
    sf::FloatRect getUndoButtonBounds() const;

private:
    AssetManager& assetManager;         // Truy cập texture
    WallSpriteSheet& wallSprites;       // Sprite tường
    StairsSpriteSheet& stairSprites;    // Sprite cầu thang

    // Biến tối ưu hóa: chứa toàn bộ tường dưới dạng VertexArray (quads).
    sf::VertexArray m_wallMesh;

    // UI: font + text hiển thị số bước đi (step count).
    sf::Font m_font;      // Font chữ
    sf::Text m_stepText;  // Text để hiển thị số bước

    // Undo
    sf::Sprite m_undoButtonSprite;

    // Hàm vẽ
    void drawBackdropAndFloor(sf::RenderWindow& window, const GameState& gameState);
    void drawWalls(sf::RenderWindow& window);

    // Vẽ chìa khóa, cổng, cầu thang, bẫy, nhân vật
    void drawKey(sf::RenderWindow& window, const GameState& gameState);
    void drawGate(sf::RenderWindow& window, const GameState& gameState);
    void drawStairs(sf::RenderWindow& window, const GameState& gameState);
    void drawTrap(sf::RenderWindow& window, const GameState& gameState);
    void drawCharacters(sf::RenderWindow& window, const GameState& gameState);
};