#include "Character.h"
#include "GameState.h"
#include "GameLogic.h"
#include <AudioManager.h>
#include <cmath>
#include <random>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include <iostream>

// Constructor
Character::Character(const std::string& textureName, int mazeSize, AssetManager& assetManager)
    : animation(textureName, mazeSize, assetManager) {}

// hàm vẽ nhân vật
void Character::draw(sf::RenderWindow& window, sf::Vector2f position) {
    animation.draw(window, position);
}

// Hàm đặt vị trí cho nhân vật
// Cập nhật cả tọa độ lưới (grid) và tọa độ pixel
void Character::setPosition(int gridX, int gridY, const GameState& gameState) {
    gridPosition.x = gridX;
    gridPosition.y = gridY;

    // Tính toán vị trí pixel tương ứng trên màn hình dựa vào vị trí lưới
    pixelPosition.x = gameState.MAZE_DRAW_OFFSET_X + (gridPosition.y / 2) * gameState.cellRect;
    pixelPosition.y = gameState.MAZE_DRAW_OFFSET_Y + (gridPosition.x / 2) * gameState.cellRect;

    // Ban đầu, vị trí đích cũng chính là vị trí hiện tại
    targetPixelPosition = pixelPosition;
}

// Lấy vị trí trên lưới (grid)
sf::Vector2i Character::getPosition() const {
    return gridPosition;
}

// Kiểm tra xem một nước đi có hợp lệ hay không
bool Character::isMoveValid(sf::Vector2i newPos, const GameState& gameState) {
    // 1. Kiểm tra xem có đi ra ngoài biên của mê cung không
    if (newPos.x < 1 || newPos.x >= gameState.maze.size() - 1 ||
        newPos.y < 1 || newPos.y >= gameState.maze[0].size() - 1)
        return false;

    // 2. Nhân vật KHÔNG ĐƯỢC đi vào ô của CỔNG hoặc CẦU THANG
    if (gameState.gate.position.x != -1 && newPos == gameState.gate.position)
        return false;

    if (gameState.stairPosition.x != -1 && newPos == gameState.stairPosition)
        return false;


    // 3. Tính toán tọa độ của vật cản (tường hoặc cổng) nằm giữa ô hiện tại và ô đích
    int wallX = (gridPosition.x + newPos.x) / 2;
    int wallY = (gridPosition.y + newPos.y) / 2;

    // 4. Kiểm tra xem đường đi có cắt qua một CỔNG ĐANG ĐÓNG hay không
    if (gameState.gate.isClosed &&
        gameState.gate.position.x == wallX &&
        gameState.gate.position.y == wallY)
        return false;


    // 5. Kiểm tra xem đường đi có cắt qua một BỨC TƯỜNG thông thường hay không
    if (gameState.maze[wallX][wallY] == '%')
        return false;

    return true; // Nước đi hợp lệ
}

// Bắt đầu một di chuyển mới đến ô đích
void Character::moveTo(sf::Vector2i targetGridPos, const GameState& gameState, AudioManager& audio) {
    if (m_isMoving) return; // Nếu đang di chuyển thì không cho phép di chuyển tiếp

    // Xác định hướng di chuyển dựa trên vị trí đích và vị trí hiện tại
    if      (targetGridPos.x > gridPosition.x)  m_direction = Direction::Down;
    else if (targetGridPos.x < gridPosition.x)  m_direction = Direction::Up;
    else if (targetGridPos.y > gridPosition.y)  m_direction = Direction::Right;
    else if (targetGridPos.y < gridPosition.y)  m_direction = Direction::Left;

    gridPosition = targetGridPos;   // Cập nhật vị trí lưới mới
    m_isMoving = true;              // Bật flag báo hiệu đang di chuyển

    // Tính toán và lưu lại vị trí pixel đích cần tiến tới
    targetPixelPosition.x = gameState.MAZE_DRAW_OFFSET_X + (targetGridPos.y / 2) * gameState.cellRect;
    targetPixelPosition.y = gameState.MAZE_DRAW_OFFSET_Y + (targetGridPos.x / 2) * gameState.cellRect;
    audio.playSound("foostep"); // Phát âm thanh bước đi
}

// Cập nhật logic di chuyển và hoạt ảnh mỗi frame
void Character::update(sf::Time dt) {
    // 1. Logic khi nhân vật đang di chuyển
    if (m_isMoving) {
        // Tính vector hướng và khoảng cách đến đích
        sf::Vector2f moveDirection = targetPixelPosition - pixelPosition;
        float distance = std::sqrt(moveDirection.x * moveDirection.x + moveDirection.y * moveDirection.y);

        // Nếu khoảng cách còn lại nhỏ hơn quãng đường di chuyển trong 1 frame,
        // thì coi như đã đến nơi.
        if (distance < moveSpeed * dt.asSeconds()) {
            pixelPosition = targetPixelPosition;    // Dịch chuyển thẳng đến đích
            m_isMoving = false;                     // Tắt cờ di chuyển
        }
        else {
            // Nếu chưa đến nơi, di chuyển nhân vật một đoạn nhỏ theo hướng đích
            sf::Vector2f normalizedDir = moveDirection / distance;          // Vector hướng đơn vị
            pixelPosition += normalizedDir * moveSpeed * dt.asSeconds();    // Cập nhật vị trí pixel

            // Cập nhật frame hoạt ảnh bước đi
            animationTimer += dt;
            if (animationTimer.asSeconds() > 0.1f) {        // Cứ 0.1 giây thì chuyển frame
                animationFrame = (animationFrame + 1) % 5;  // Spritesheet có 5 frame/hàng
                animationTimer = sf::Time::Zero;
            }
        }
    }
    // 2. Logic khi nhân vật đứng yên
    else
        // Nếu không di chuyển, luôn quay về frame đầu tiên (dáng đứng yên)
        animationFrame = 0;

    // 3. Cập nhật đối tượng Animation
    // Cho nó biết hướng và frame hiện tại để vẽ cho đúng
    animation.setDirection(m_direction);
    animation.update(animationFrame);
}

// Lấy tọa độ Pixel của nhân vật
sf::Vector2f Character::getPixelPosition() const {
    return pixelPosition;
}

// Nhân vật đang di chuyển hay không
bool Character::isMoving() const {
    return m_isMoving;
}

// Hàm bắt đầu lượt đi mặc định của Enemy
void Enemy::startTurn() {
    // Mặc định kẻ địch không di chuyển
    movesRemaining = 0;
}

// Kiểm tra xem lượt đi của Enemy đã kết thúc chưa
bool Enemy::isTurnFinished() const {
    // Lượt kết thúc khi hết nước đi VÀ không còn đang di chuyển
    return movesRemaining <= 0 && !m_isMoving;
}

// Nạp lại số lượt đi cho Mummy
void MummyWhite::startTurn() {
    // Mummy có 2 nước đi mỗi lượt
    movesRemaining = 2;
}

void MummyRed::startTurn() {
    // Mummy có 2 nước đi mỗi lượt
    movesRemaining = 2;
}

// Nạp lại số lượt đi cho Scorpion
void ScorpionWhite::startTurn() {
    // Scorpion chỉ có 1 nước đi mỗi lượt
    movesRemaining = 1;
}

void ScorpionRed::startTurn() {
    // Scorpion chỉ có 1 nước đi mỗi lượt
    movesRemaining = 1;
}

void Enemy::decideEasyMove(const Explorer& player, const GameState& gameState, AudioManager& audio) {
    if (movesRemaining <= 0 || isMoving()) return;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(-1, 1);

    if (movesRemaining <= 0 || isMoving()) return;      // Điều kiện canh gác

    sf::Vector2i playerPos  = player.getPosition();
    sf::Vector2i currentPos = getPosition();

    // Thử di chuyển ngang
    sf::Vector2i nextHorizontalPos = currentPos;
    int direction = distrib(gen);
    nextHorizontalPos.y += 2 * direction;    // Tính ô đích theo chiều ngang
    if (currentPos.y != playerPos.y && isMoveValid(nextHorizontalPos, gameState)) {
        moveTo(nextHorizontalPos, gameState, audio);
        movesRemaining--;
        return; // Đã di chuyển, xong
    }

    // Thử di chuyển dọc
    sf::Vector2i nextVerticalPos = currentPos;
    direction = distrib(gen);
    nextVerticalPos.x += 2 * direction;      // Tính ô đích theo chiều dọc
    if (currentPos.x != playerPos.x && isMoveValid(nextVerticalPos, gameState)) {
        moveTo(nextVerticalPos, gameState, audio);
        movesRemaining--;
        return; // Đã di chuyển, xong
    }

    // Nếu không thể di chuyển cả ngang và dọc, bỏ lượt
    movesRemaining--;
}

void Enemy::decideMediumMove(const Explorer& player, const GameState& gameState, bool horizontalPriority, AudioManager& audio) {
    // 1. Điều kiện canh gác: Chỉ di chuyển nếu còn lượt và không đang trong hoạt ảnh.
    if (movesRemaining <= 0 || isMoving()) return;

    // Lấy vị trí người chơi và vị trí của mummy
    sf::Vector2i playerPos = player.getPosition();
    sf::Vector2i currentPos = getPosition();

    // 2. Logic chính: Dựa vào flag 'horizontalPriority' để quyết định hướng đi ưu tiên.
    if (horizontalPriority) {
        // Ưu tiên ngang (Dành cho MummyWhite & ScorpionWhite)
        // a. Thử đi ngang trước:
        sf::Vector2i nextHorizontalPos = currentPos;
        nextHorizontalPos.y += 2 * GameLogic::sign(playerPos.y - currentPos.y);
        if (currentPos.y != playerPos.y && isMoveValid(nextHorizontalPos, gameState)) {
            moveTo(nextHorizontalPos, gameState, audio);
            movesRemaining--;
            return; // Đã di chuyển thành công, kết thúc.
        }

        // b. Nếu đi ngang không được, thử đi dọc
        sf::Vector2i nextVerticalPos = currentPos;
        nextVerticalPos.x += 2 * GameLogic::sign(playerPos.x - currentPos.x);
        if (currentPos.x != playerPos.x && isMoveValid(nextVerticalPos, gameState)) {
            moveTo(nextVerticalPos, gameState, audio);
            movesRemaining--;
            return; // Đã di chuyển thành công, kết thúc.
        }
    }
    else {
        // Ưu tiên dọc (Dành cho MummyRed & ScorpionRed)
        // a. Thử đi dọc trước
        sf::Vector2i nextVerticalPos = currentPos;
        nextVerticalPos.x += 2 * GameLogic::sign(playerPos.x - currentPos.x);
        if (currentPos.x != playerPos.x && isMoveValid(nextVerticalPos, gameState)) {
            moveTo(nextVerticalPos, gameState, audio);
            movesRemaining--;
            return; // Đã di chuyển thành công, kết thúc.
        }

        // b. Nếu đi dọc không được, thử đi ngang
        sf::Vector2i nextHorizontalPos = currentPos;
        nextHorizontalPos.y += 2 * GameLogic::sign(playerPos.y - currentPos.y);
        if (currentPos.y != playerPos.y && isMoveValid(nextHorizontalPos, gameState)) {
            moveTo(nextHorizontalPos, gameState, audio);
            movesRemaining--;
            return; // Đã di chuyển thành công, kết thúc.
        }
    }

    // 3. Bỏ lượt nếu bị kẹt: Nếu cả hai hướng đều không đi được, vẫn phải tiêu tốn một lượt đi.
    movesRemaining--;
}

void Enemy::decideHardMove(const Explorer& player, const GameState& gameState, AudioManager& audio) {
    if (movesRemaining <= 0 || isMoving()) return;  // điều kiện canh gác

    sf::Vector2i playerPos  = player.getPosition(); // vị trí của người chơi
    sf::Vector2i currentPos   = getPosition();      // vị trí hiện tại của Mummy

    // Nếu đã đứng cạnh người chơi, nước đi tiếp theo là tấn công trực tiếp
    if (abs(currentPos.x - playerPos.x) + abs(currentPos.y - playerPos.y) == 2) {
        if (isMoveValid(playerPos, gameState)) {
            moveTo(playerPos, gameState, audio);
            movesRemaining--;
            return;
        }
    }

    // 1. Dùng static để biến chỉ khởi tạo 1 lần duy nhất
    static std::queue<sf::Vector2i> q;
    static std::unordered_map<int, sf::Vector2i> trace;    // dùng để truy hồi lại đường đi
    static std::unordered_map<int, bool> visited;          // kiểm tra xem đã đi một ô hay chưa

    // 2. Vì là biến tĩnh (lưu giá trị cũ), phải dọn dẹp sạch sẽ trước khi dùng lại
    // Cách clear queue nhanh nhất là swap với một queue rỗng
    std::queue<sf::Vector2i> emptyQueue;
    std::swap(q, emptyQueue);
    // Clear map
    trace.clear();
    visited.clear();

    // Mã hóa tọa độ (Hash key)
    auto key = [&](sf::Vector2i p) { return p.y * 100 + p.x; };

    std::vector<sf::Vector2i> directions = {
        {0, -2}, {0, 2}, {-2, 0}, {2, 0} // 4 hướng có thể đi
    };

    q.push(currentPos);
    visited[key(currentPos)] = true;
    sf::Vector2i targetCell = {-1, -1}; // Ô đích cần đến

    // Dùng thuật toán bfs để tìm đường đi tối ưu nhất
    while (!q.empty()) {
        sf::Vector2i cur = q.front();
        q.pop();

        // Kiểm tra xem có đang ở cạnh người chơi không
        if (abs(cur.x - playerPos.x) + abs(cur.y - playerPos.y) == 2) {
            targetCell = cur;
            break; // Đã tìm thấy ô đích, thoát BFS
        }

        for (auto dir : directions) {
            sf::Vector2i next = cur + dir;
            if (!GameLogic::isPathClear(cur, next, gameState)) continue;
            if (visited[key(next)]) continue;
            visited[key(next)] = true;
            trace[key(next)] = cur;
            q.push(next);
        }
    }

    if (targetCell.x == -1) {   // Không tìm thấy đường
        movesRemaining--;       // Bỏ lượt
        return;
    }

    // Truy hồi lại đường đi để tìm bước đi đầu tiên
    sf::Vector2i step = targetCell;
    if (trace.find(key(step)) == trace.end() && step != currentPos) {
        // Trường hợp đặc biệt: mục tiêu ngay cạnh vị trí bắt đầu
        // Không làm gì hết
    }
    else
        while (trace.count(key(step)) && trace[key(step)] != currentPos)
            step = trace[key(step)];


    if (isMoveValid(step, gameState)) {
        moveTo(step, gameState, audio);
        movesRemaining--;
        return;
    }

    // Nếu bước đi tính toán ra bị lỗi, vẫn bỏ lượt
    movesRemaining--;
}

void MummyWhite::decideMove(const Explorer& player, const GameState& gameState, Difficulty difficulty, AudioManager& audio) {
    switch (difficulty) {
        case Difficulty::Hard:
            decideHardMove(player, gameState, audio);
            break;
        case Difficulty::Medium:
            decideMediumMove(player, gameState, true, audio);  // Ưu tiên ngang
            break;
        case Difficulty::Easy:
        default:
            decideEasyMove(player, gameState, audio);
            break;
    }
}

void MummyRed::decideMove(const Explorer& player, const GameState& gameState, Difficulty difficulty, AudioManager& audio) {
    switch (difficulty) {
        case Difficulty::Hard:
            decideHardMove(player, gameState, audio);
            break;
        case Difficulty::Medium:
            decideMediumMove(player, gameState, false, audio); // Ưu tiên dọc
            break;
        case Difficulty::Easy:
        default:
            decideEasyMove(player, gameState, audio);
            break;
    }
}

void ScorpionWhite::decideMove(const Explorer& player, const GameState& gameState, Difficulty difficulty, AudioManager& audio) {
    switch (difficulty) {
        case Difficulty::Hard:
            decideHardMove(player, gameState, audio);
            break;
        case Difficulty::Medium:
            decideMediumMove(player, gameState, true, audio);  // Ưu tiên ngang
            break;
        case Difficulty::Easy:
        default:
            decideEasyMove(player, gameState, audio);
            break;
    }
}

void ScorpionRed::decideMove(const Explorer& player, const GameState& gameState, Difficulty difficulty, AudioManager& audio) {
    switch (difficulty) {
        case Difficulty::Hard:
            decideHardMove(player, gameState, audio);
            break;
        case Difficulty::Medium:
            decideMediumMove(player, gameState, false, audio); // Ưu tiên dọc
            break;
        case Difficulty::Easy:
        default:
            decideEasyMove(player, gameState, audio);
            break;
    }
}
