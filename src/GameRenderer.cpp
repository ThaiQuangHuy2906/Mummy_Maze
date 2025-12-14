#include "GameRenderer.h"
#include "Character.h"
#include <string>
#include <iostream>

GameRenderer::GameRenderer(AssetManager& assets, WallSpriteSheet& walls, StairsSpriteSheet& stairs)
    : assetManager(assets), wallSprites(walls), stairSprites(stairs) {
    // Khởi tạo VertexArray chế độ Quads (Hình chữ nhật)
    m_wallMesh.setPrimitiveType(sf::Quads);

    if (!m_font.loadFromFile("assets/fonts/Super Joyful.ttf"))
        std::cerr << "Khong the tai font UI trong GameRenderer\n";

    m_stepText.setFont(m_font);
    m_stepText.setCharacterSize(30);                // Kích thước chữ
    m_stepText.setFillColor(sf::Color::White);      // Màu chữ trắng
    m_stepText.setOutlineColor(sf::Color::Black);   // Viền đen cho dễ đọc
    m_stepText.setOutlineThickness(2);
    m_stepText.setPosition(20.f, 15.f);             // Vị trí góc trên bên trái

    // Setup nút Undo
    m_undoButtonSprite.setTexture(assetManager.getTexture("button_undo.png"));
    // Scale nhỏ lại chút cho gọn
    float targetSize = 50.f;
    float scale = targetSize / m_undoButtonSprite.getTexture()->getSize().x;
    m_undoButtonSprite.setScale(scale, scale);
    // Đặt ở góc phải trên cùng (X=420, Y=15)
    m_undoButtonSprite.setPosition(420.f, 15.f);
}
// Xây dựng toàn bộ tường mê cung thành 1 VertexArray duy nhất.// Hàm xây dựng toàn bộ mê cung thành 1 đối tượng duy nhất
void GameRenderer::initMazeMesh(const GameState& gameState) {
    m_wallMesh.clear(); // Xóa dữ liệu cũ

    // 1. Tường ngang
    //    Ở cấu trúc maze, những ô có tường ngang thường nằm ở hàng chẵn, cột lẻ.
    for (int i = 2; i < gameState.maze.size(); i += 2) {
        for (int j = 1; j < gameState.maze[i].size(); j += 2) {
            if (gameState.maze[i][j] == '%') {
                float pixelX = gameState.MAZE_DRAW_OFFSET_X + (j / 2) * gameState.cellRect;
                float pixelY = gameState.MAZE_DRAW_OFFSET_Y + (i / 2) * gameState.cellRect;

                // Thêm 1 đoạn tường ngang vào m_wallMesh.
                wallSprites.appendWall(m_wallMesh, {pixelX - 3, pixelY - 9}, WallType::Up);
            }
        }
    }

    // 2. Tường dọc (bao gồm logic xử lý bóng đổ).
    //    Tường dọc thường ở hàng lẻ, cột chẵn.
    for (int i = 1; i < gameState.maze.size(); i += 2) {
        for (int j = 2; j < gameState.maze[i].size(); j += 2) {
            if (gameState.maze[i][j] == '%') {
                float pixelX = gameState.MAZE_DRAW_OFFSET_X + (j / 2) * gameState.cellRect;
                float pixelY = gameState.MAZE_DRAW_OFFSET_Y + (i / 2) * gameState.cellRect;

                // Kiểm tra góc giao giữa tường ngang và dọc để vẽ bóng cho đẹp hơn.
                if (i + 1 < gameState.maze.size() && j + 1 < gameState.maze[i].size() &&
                    gameState.maze[i+1][j+1] == '%') {

                    // Vẽ tường dọc có bóng (RightShadow).
                    wallSprites.appendWall(m_wallMesh, {pixelX - 3, pixelY - 9}, WallType::RightShadow);

                    // Vẽ đè lại tường ngang liền kề bằng loại không bóng,
                    // tránh bị bóng trùm lên sai chỗ.
                    float redrawX = gameState.MAZE_DRAW_OFFSET_X + ((j + 1) / 2) * gameState.cellRect;
                    float redrawY = gameState.MAZE_DRAW_OFFSET_Y + ((i + 1) / 2) * gameState.cellRect;
                    wallSprites.appendWall(m_wallMesh, {redrawX - 3, redrawY - 9}, WallType::UpNoShadow);
                } else
                    // Tường dọc bình thường (không xử lý góc).
                    wallSprites.appendWall(m_wallMesh, {pixelX - 3, pixelY - 9}, WallType::Left);
            }
        }
    }
}

// 1. Vẽ nền và sàn
void GameRenderer::drawBackdropAndFloor(sf::RenderWindow& window, const GameState& gameState) {
    sf::Sprite backdrop(assetManager.getTexture("backdrop.png"));
    window.draw(backdrop);

    std::string floorTextureName = "floor" + std::to_string(gameState.mazeSize) + ".jpg";
    sf::Sprite floor(assetManager.getTexture(floorTextureName));
    floor.setPosition(gameState.MAZE_DRAW_OFFSET_X, gameState.MAZE_DRAW_OFFSET_Y);
    window.draw(floor);
}

// 2. Vẽ tường
void GameRenderer::drawWalls(sf::RenderWindow& window) {
    if (wallSprites.getTexture())
        window.draw(m_wallMesh, wallSprites.getTexture());
}

// 3. Vẽ chìa khóa
void GameRenderer::drawKey(sf::RenderWindow& window, const GameState& gameState) {
    // Không có chìa khóa trên map
    if (gameState.keyPosition.x == -1) return;

    sf::Sprite keySprite(assetManager.getTexture("key.png"));
    const int FRAMES = 36;  // 36 frames
    int cellW = keySprite.getTexture()->getSize().x / FRAMES;
    int cellH = keySprite.getTexture()->getSize().y;

    // Lấy frame hiện tại từ GameState và cắt sprite
    int frameIndex = gameState.keyAnimationCellIndex;
    keySprite.setTextureRect(sf::IntRect(frameIndex * cellW, 0, cellW, cellH));

    // Co giãn sprite cho vừa với kích thước một ô
    float scaleX = (float)gameState.cellRect / cellW;
    float scaleY = (float)gameState.cellRect / cellH;
    keySprite.setScale(scaleX, scaleY);

    // Đặt vị trí và vẽ
    float pixelX = gameState.MAZE_DRAW_OFFSET_X + (gameState.keyPosition.y / 2) * gameState.cellRect;
    float pixelY = gameState.MAZE_DRAW_OFFSET_Y + (gameState.keyPosition.x / 2) * gameState.cellRect;
    keySprite.setPosition(pixelX, pixelY);
    window.draw(keySprite);
}

// 4. Vẽ cổng
void GameRenderer::drawGate(sf::RenderWindow& window, const GameState& gameState) {
    if (gameState.gate.position.x == -1) return;

    std::string gateTextureName = "gate" + std::to_string(gameState.mazeSize) + ".png";
    sf::Sprite gateSprite(assetManager.getTexture(gateTextureName));

    const int FRAMES = 8;   // 8 frames
    int cellW = gateSprite.getTexture()->getSize().x / FRAMES;
    int cellH = gateSprite.getTexture()->getSize().y;

    // Cắt sprite dựa trên frame hoạt ảnh hiện tại của cổng
    int frameIndex = gameState.gate.animationCellIndex;
    gateSprite.setTextureRect(sf::IntRect(frameIndex * cellW, 0, cellW, cellH));

    // Tính toán vị trí vẽ (cổng nằm trên đường kẻ giữa các ô)
    float pixelX = gameState.MAZE_DRAW_OFFSET_X + (gameState.gate.position.y * 0.5f) * gameState.cellRect;
    float pixelY = gameState.MAZE_DRAW_OFFSET_Y + (gameState.gate.position.x * 0.5f) * gameState.cellRect;

    // Đặt gốc tọa độ vào tâm để xoay và căn chỉnh
    gateSprite.setOrigin(cellW / 2.f, cellH / 2.f);
    gateSprite.setPosition(pixelX, pixelY);
    window.draw(gateSprite);
}

// 5. Vẽ cầu thang
void GameRenderer::drawStairs(sf::RenderWindow& window, const GameState& gameState) {
    if (gameState.stairPosition.x == -1) return;

    sf::Vector2i stairPos = gameState.stairPosition;

    // Tính toán vị trí pixel cơ bản
    float pixelX = gameState.MAZE_DRAW_OFFSET_X + (stairPos.y / 2) * gameState.cellRect;
    float pixelY = gameState.MAZE_DRAW_OFFSET_Y + (stairPos.x / 2) * gameState.cellRect;

    // Xác định hướng của cầu thang (0: UP, 1: RIGHT, 2: DOWN, 3: LEFT)
    int stairIndex = 0;     // Mặc định
    int maxCoord = gameState.mazeSize * 2;

    if      (stairPos.y == maxCoord) stairIndex = 1;    // RIGHT
    else if (stairPos.x == maxCoord) stairIndex = 2;    // DOWN
    else if (stairPos.y == 0)        stairIndex = 3;    // LEFT
    else if (stairPos.x == 0)        stairIndex = 0;    // UP

    // Điều chỉnh vị trí
    if (stairIndex == 0) pixelY -= 70;      // UP
    else if (stairIndex == 3) pixelX -= 60; // LEFT

    stairSprites.draw(window, stairIndex, {pixelX, pixelY});
}

// 6. Vẽ bẫy
void GameRenderer::drawTrap(sf::RenderWindow& window, const GameState& gameState) {
    if (gameState.trapPosition.x == -1) return;

    std::string trapTextureName = "trap" + std::to_string(gameState.mazeSize) + ".png";
    sf::Sprite trapSprite(assetManager.getTexture(trapTextureName));

    // 1. Lấy kích thước gốc của ảnh bẫy
    float w = trapSprite.getTexture()->getSize().x;
    float h = trapSprite.getTexture()->getSize().y;

    // 2. Tính toán tỉ lệ co giãn cần thiết
    // Tỉ lệ = (kích thước mong muốn) / (kích thước gốc)
    float scaleX = (float)gameState.cellRect / w;
    float scaleY = (float)gameState.cellRect / h;

    // 3. Áp dụng tỉ lệ vào sprite
    trapSprite.setScale(scaleX, scaleY);

    // Tính toán vị trí vẽ
    float pixelX = gameState.MAZE_DRAW_OFFSET_X + (gameState.trapPosition.y / 2) * gameState.cellRect;
    float pixelY = gameState.MAZE_DRAW_OFFSET_Y + (gameState.trapPosition.x / 2) * gameState.cellRect;
    trapSprite.setPosition(pixelX, pixelY);
    window.draw(trapSprite);
}

// 7. Vẽ nhân vật
void GameRenderer::drawCharacters(sf::RenderWindow& window, const GameState& gameState) {
    if (gameState.explorer)
        gameState.explorer->draw(window, gameState.explorer->getPixelPosition());

    for (const auto& enemy : gameState.enemies)
        enemy->draw(window, enemy->getPixelPosition());
}

// Vẽ toàn bộ frame InGame.
void GameRenderer::drawGame(sf::RenderWindow& window, GameState& gameState) {
    drawBackdropAndFloor(window, gameState);
    drawWalls(window);
    drawKey(window, gameState);
    drawGate(window, gameState);
    drawStairs(window, gameState);
    drawTrap(window, gameState);
    drawCharacters(window, gameState);

    // 4. Vẽ nút Undo + Text số bước.
    window.draw(m_undoButtonSprite);

    // Cập nhật nội dung text theo số bước hiện tại
    m_stepText.setString("Steps: " + std::to_string(gameState.stepCount));
    // Vẽ lên màn hình (vẽ sau cùng để chữ nằm đè lên mọi thứ)
    window.draw(m_stepText);
}

// Vùng va chạm/nút bấm của Undo.
sf::FloatRect GameRenderer::getUndoButtonBounds() const {
    return m_undoButtonSprite.getGlobalBounds();
}
