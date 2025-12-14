#include "GameLogic.h"
#include "Character.h"
#include <iostream>
#include <algorithm>
#include <AudioManager.h>
#include <fstream>
#include <map>

// Hàm lấy thứ hạng sức mạnh của một kẻ địch
// Càng cao càng mạnh
int getEnemyRank(Enemy* enemy) {
    if (dynamic_cast<MummyWhite*>(enemy)) return 4;
    if (dynamic_cast<MummyRed*>(enemy)) return 3;
    if (dynamic_cast<ScorpionWhite*>(enemy)) return 2;
    if (dynamic_cast<ScorpionRed*>(enemy)) return 1;
    return 0; // Không xác định
}

namespace GameLogic {

// Kiểm tra xem tất cả kẻ địch đã kết thúc lượt chưa
bool allEnemiesFinished(GameState& gameState) {
    // Nếu có bất kỳ kẻ địch nào chưa kết thúc lượt thì trả về false
    for (auto& e: gameState.enemies)
        if (!e->isTurnFinished())
            return false;

    // Nếu tất cả đều đã kết thúc, trả về true.
    return true;
}

// Cập nhật trạng thái của chìa khóa và cổng
void updateKeyAndGateLogic(GameState& gameState, AudioManager& audio) {
    // Nếu tọa độ x của chìa khóa là -1 thì map này không có chìa khóa
    if (gameState.keyPosition.x == -1) return;

    bool keyIsPressed = false;  // flag lưu trạng thái chìa được nhấn hay ko
    // False: Chìa không được nhấn | True: Chìa được nhấn

    // Nếu explorer tồn tại và tọa độ của explorer trùng với tọa độ chìa khóa
    if (gameState.explorer && gameState.explorer->getPosition() == gameState.keyPosition)
        keyIsPressed = true;    // Chìa được nhấn

    // Nếu explorer không đứng thì kiểm tra quái có đứng lên không
    if (!keyIsPressed)
        for (const auto& e: gameState.enemies)
            if (e->getPosition() == gameState.keyPosition) {
                keyIsPressed = true;
                break;
            }


    // Nếu chìa khóa được nhấn trong frame này
    if (keyIsPressed) {
        // Và ở frame trước đó nó không được nhấn (đây là lần đầu tiên nhấn)
        if (!gameState.gate.keyWasPressedLastFrame) {
            audio.playSound("key");                     // Phát âm thanh đóng/mở cổng
            gameState.gate.isClosed = !gameState.gate.isClosed;     // Đảo trạng thái cổng (Đóng -> Mở hoặc Mở -> Đóng)

            // Kích hoạt hoạt ảnh xoay của cổng
            gameState.gate.isAnimating = true;                      // Hoạt ảnh xoay cổng
            gameState.gate.isOpening = !gameState.gate.isClosed;    // Đặt hướng hoạt ảnh (đang mở hay đang đóng)
            gameState.gate.animationTimer = sf::Time::Zero;         // Reset bộ đếm thời gian
        }
        gameState.gate.keyWasPressedLastFrame = true;   // Đánh dấu ở frame này chìa đang được nhấn
                                                        // để frame kế tiếp người chơi vẫn ở trên chìa
                                                        // thì không kích hoạt lại nữa
    }
    else
        gameState.gate.keyWasPressedLastFrame = false;  // Khi không ai đứng trên chìa thì reset flag về false
                                                        // để khi có E/M/S bước lên chìa thì lại xoay cổng
}

// Kiểm tra điều kiện thắng/thua
void checkGameConditions(GameState& gameState, bool& isGameOver, AudioManager& audio) {
    // Nếu game không có explorer thì thoát
    if (!gameState.explorer) return;

    // Nếu nhân vật vẫn đang trượt tới ô đích, chưa kiểm tra gì cả
    if (gameState.explorer->isMoving()) return;

    // Lấy vị trí người chơi
    sf::Vector2i playerPos = gameState.explorer->getPosition();

    // 1. Kiểm tra thua
    // a. Thua do chạm bẫy
    if (gameState.trapPosition.x != -1 && playerPos == gameState.trapPosition) {
    // Nếu game có bẫy và người chơi dậm bẫy (tọa độ trùng)
        audio.playSound("trap");
        isGameOver = true;  // Thua và kết thúc game
        return;
    }
    // b. Thua do chạm kẻ địch (check tất cả kẻ địch)
    for (const auto& e: gameState.enemies)
        if (e->getPosition() == playerPos && !e->isMoving()) {
            audio.playSound("mummy");
            isGameOver = true;
            return;
        }

    // 2. Kiểm tra thắng (Cạnh cầu thang)
    sf::Vector2i stairPos = gameState.stairPosition;    // Lấy tọa độ cầu thang để kiểm tra
    // Kiểm tra 4 ô xung quanh người chơi
    if ((playerPos.x == stairPos.x && abs(playerPos.y - stairPos.y) == 1) ||
        (playerPos.y == stairPos.y && abs(playerPos.x - stairPos.x) == 1)) {
        audio.playSound("stairs");
        isGameOver = true;  // Thắng và kết thúc game
        return;
    }
}

bool isPathClear(sf::Vector2i from, sf::Vector2i to, const GameState& gameState) {
    // 1. Kiểm tra biên của mê cung
    if (to.x < 1 || to.x >= gameState.maze.size() - 1 ||
        to.y < 1 || to.y >= gameState.maze[0].size() - 1)
        return false;

    // 2. Ô đích không được là cổng hoặc cầu thang
    if (gameState.gate.position.x != -1 && to == gameState.gate.position)
        return false;

    if (gameState.stairPosition.x != -1 && to == gameState.stairPosition)
        return false;

    // 3. Tính toán vật cản nằm giữa
    int wallX = (from.x + to.x) / 2;
    int wallY = (from.y + to.y) / 2;

    // 4. Kiểm tra cổng đóng chắn giữa đường
    if (gameState.gate.isClosed &&
        gameState.gate.position.x == wallX &&
        gameState.gate.position.y == wallY)
        return false;


    // 5. Kiểm tra tường chắn giữa đường (lấy từ cả hai hàm)
    if (gameState.maze[wallX][wallY] == '%')
        return false;

    return true; // Đường đi hoàn toàn thông thoáng
}

// Xử lý va chạm giữa các kẻ địch
void resolveEnemyCollisions(GameState& gameState, AudioManager& audio) {
    // Vector để đánh dấu các kẻ địch sẽ bị xóa
    std::vector<bool> toDelete(gameState.enemies.size(), false);
    bool collisionOccurred = false;

    // Dùng hai vòng lặp để so sánh mỗi cặp kẻ địch với nhau
    for (size_t i = 0; i < gameState.enemies.size(); ++i) {
        for (size_t j = i + 1; j < gameState.enemies.size(); ++j) {
            // Nếu hai kẻ địch ở cùng một vị trí và chưa bị đánh dấu xóa
            if (!toDelete[i] && !toDelete[j] && gameState.enemies[i]->getPosition() == gameState.enemies[j]->getPosition()) {
                collisionOccurred = true;
                audio.playSound("collision"); // Phát âm thanh va chạm

                // Lấy "thứ hạng" của chúng
                int rank_i = getEnemyRank(gameState.enemies[i].get());
                int rank_j = getEnemyRank(gameState.enemies[j].get());

                // So sánh và đánh dấu kẻ yếu hơn để xóa
                if (rank_i > rank_j)
                    toDelete[j] = true;
                else if (rank_j > rank_i)
                    toDelete[i] = true;
                else
                    // Nếu chúng bằng hạng (cùng loại), xóa một con
                    toDelete[j] = true;
            }
        }
    }

    // Nếu không có va chạm nào thì không cần làm gì thêm
    if (!collisionOccurred) return;

    // Dùng erase-remove idiom để xóa tất cả các phần tử đã được đánh dấu
    gameState.enemies.erase(
        std::remove_if(gameState.enemies.begin(), gameState.enemies.end(),
            [&](const auto& enemy) {
                // Tìm chỉ số của kẻ địch hiện tại
                auto index = &enemy - &gameState.enemies[0];
                return toDelete[index];
            }),
        gameState.enemies.end()
    );
}

// Cập nhật hoạt ảnh xoay của chìa khóa
void updateKeyAnimation(GameState& gameState, sf::Time dt) {
    if (gameState.keyPosition.x == -1) return;  // Không có chìa khóa thì thôi

    const int KEY_ANIMATION_FRAMES = 36;    // Ảnh spritesheet của "key.png"" có 36 frame
    const int FRAME_DURATION_MS = 50;       // Mỗi frame kéo dài 50ms

    // Cộng dồn thời gian từ frame trước
    gameState.keyAnimationTimer += dt;

    // Có thể đã qua nhiều frame, nên dùng vòng lặp while
    while (gameState.keyAnimationTimer.asMilliseconds() >= FRAME_DURATION_MS) {
        // Chuyển sang frame tiếp theo, quay vòng lại nếu hết
        gameState.keyAnimationCellIndex = (gameState.keyAnimationCellIndex + 1) % KEY_ANIMATION_FRAMES;

        // Trừ đi thời gian của một frame đã xử lý
        gameState.keyAnimationTimer -= sf::milliseconds(FRAME_DURATION_MS);
    }
}

sf::Vector2i pixelToGrid(sf::Vector2i pixelPos, const GameState& gameState) {
    // Kiểm tra xem cú click có nằm trong khu vực mê cung không
    if (pixelPos.x < gameState.MAZE_DRAW_OFFSET_X ||
        pixelPos.y < gameState.MAZE_DRAW_OFFSET_Y ||
        pixelPos.x > gameState.MAZE_DRAW_OFFSET_X + gameState.mazeSize * gameState.cellRect ||
        pixelPos.y > gameState.MAZE_DRAW_OFFSET_Y + gameState.mazeSize * gameState.cellRect)
        return {-1, -1}; // Trả về tọa độ không hợp lệ


    // Tính toán vị trí ô logic (0, 1, 2, ...)
    int cellX = (pixelPos.x - gameState.MAZE_DRAW_OFFSET_X) / gameState.cellRect;
    int cellY = (pixelPos.y - gameState.MAZE_DRAW_OFFSET_Y) / gameState.cellRect;

    // Chuyển đổi sang tọa độ lưới của nhân vật (1, 3, 5, ...)
    int gridX = cellY * 2 + 1;
    int gridY = cellX * 2 + 1;

    return {gridX, gridY};
}

// Hàm sign xác định hướng (1, -1, 0)
int sign(int val) {
    if (val == 0) return 0;
    return (val > 0) ? 1 : -1;
}

const std::string SAVE_FILE = "savegame.txt";

// Hàm hỗ trợ đọc file save vào Map
std::map<std::string, std::pair<int, int>> loadSaveData() {
    std::map<std::string, std::pair<int, int>> data;
    std::ifstream file(SAVE_FILE);
    if (!file.is_open()) return data; // Nếu chưa có file thì trả về map rỗng

    std::string name;
    int steps, stars;
    // Định dạng file: LevelName Steps Stars
    while (file >> name >> steps >> stars)
        data[name] = {steps, stars};
    file.close();
    return data;
}

void saveLevelResult(const std::string& levelName, int steps, int stars) {
    // 1. Đọc dữ liệu cũ lên
    auto data = loadSaveData();

    // 2. Kiểm tra xem có cần cập nhật không
    // Nếu chưa từng chơi map này hoặc số bước mới ít hơn kỷ lục cũ
    if (data.find(levelName) == data.end() || steps < data[levelName].first) {
        // Cập nhật kỷ lục mới
        data[levelName] = {steps, stars};

        // 3. Ghi lại toàn bộ xuống file
        std::ofstream file(SAVE_FILE);
        for (auto const& [name, record] : data) {
            file << name << " " << record.first << " " << record.second << "\n";
        }
        file.close();
        std::cout << "Saved: " << levelName << " Steps: " << steps << " Stars: " << stars << std::endl;
    }
}

int getLevelStars(const std::string& levelName) {
    auto data = loadSaveData();
    if (data.find(levelName) != data.end())
        return data[levelName].second; // Trả về số sao
    return 0; // Chưa chơi
}

const std::string PROGRESS_FILE = "save_progress.txt";

void saveGameProgress(const GameState& gameState, const std::string& mapName) {
    std::ofstream file(PROGRESS_FILE);
    if (!file.is_open()) return;

    // 1. Lưu tên Map (để biết load map gốc nào trước)
    file << mapName << "\n";

    // 2. Lưu thông tin chung
    file << gameState.stepCount << "\n";

    // 3. Lưu vị trí Explorer
    if (gameState.explorer) {
        sf::Vector2i pos = gameState.explorer->getPosition();
        file << pos.x << " " << pos.y << "\n";
    } else {
        file << "-1 -1\n"; // Không có explorer (lỗi?)
    }

    // 4. Lưu trạng thái Cổng & Chìa khóa
    file << gameState.gate.isClosed << "\n"; // 1: Đóng, 0: Mở
    // (Vị trí cổng và chìa khóa là cố định theo map, không cần lưu lại, chỉ cần lưu trạng thái)

    // 5. Lưu danh sách Quái vật
    file << gameState.enemies.size() << "\n";
    for (const auto& enemy : gameState.enemies) {
        sf::Vector2i pos = enemy->getPosition();
        // Lưu: Loại_Quái  X  Y
        file << enemy->getTypeName() << " " << pos.x << " " << pos.y << "\n";
    }

    file.close();
    std::cout << "Game Saved to " << PROGRESS_FILE << std::endl;
}

bool loadGameProgress(std::unique_ptr<GameState>& outState, std::string& outMapName, AssetManager& assets) {
    std::ifstream file(PROGRESS_FILE);
    if (!file.is_open()) return false;

    std::string mapName;
    if (!(file >> mapName)) return false; // Đọc tên map thất bại

    outMapName = mapName;

    // 1. Khởi tạo GameState gốc từ tên map (để load tường, kích thước, v.v.)
    outState = std::make_unique<GameState>(mapName, assets);

    // 2. Đọc và ghi đè các chỉ số
    int steps;
    file >> steps;
    outState->stepCount = steps;

    // 3. Đọc và đặt lại vị trí Explorer
    int exX, exY;
    file >> exX >> exY;
    if (exX != -1 && outState->explorer)
        outState->explorer->setPosition(exX, exY, *outState);

    // 4. Đọc trạng thái Cổng
    bool isGateClosed;
    file >> isGateClosed;
    outState->gate.isClosed = isGateClosed;
    // Cập nhật lại animation frame cho khớp trạng thái
    outState->gate.animationCellIndex = isGateClosed ? 0 : 7;
    outState->gate.isOpening = !isGateClosed;

    // 5. Đọc Quái vật (Xóa quái cũ của map gốc, tạo quái mới từ file save)
    int enemyCount;
    file >> enemyCount;

    outState->enemies.clear(); // Xóa quái mặc định của map

    for (int i = 0; i < enemyCount; ++i) {
        std::string type;
        int eX, eY;
        file >> type >> eX >> eY;

        std::unique_ptr<Enemy> enemy;
        if (type == "MW")       enemy = std::make_unique<MummyWhite>(outState->mazeSize, assets);
        else if (type == "MR")  enemy = std::make_unique<MummyRed>(outState->mazeSize, assets);
        else if (type == "SW")  enemy = std::make_unique<ScorpionWhite>(outState->mazeSize, assets);
        else if (type == "SR")  enemy = std::make_unique<ScorpionRed>(outState->mazeSize, assets);

        if (enemy) {
            enemy->setPosition(eX, eY, *outState);
            outState->enemies.push_back(std::move(enemy));
        }
    }

    file.close();
    return true;
}

bool hasSavedGame() {
    std::ifstream file(PROGRESS_FILE);
    return file.good();
}

}
