#pragma once
#include "GameState.h"
#include <AudioManager.h>
#include <string>

// Nhóm các hàm logic liên quan đến game
namespace GameLogic {
    // Cập nhật logic liên quan đến việc nhấn chìa khóa và trạng thái của cổng
    void updateKeyAndGateLogic(GameState& gameState, AudioManager& audio);

    // Kiểm tra các điều kiện thắng/thua của game
    void checkGameConditions(GameState& gameState, bool& isGameOver, AudioManager& audio);

    // Hàm check
    bool isPathClear(sf::Vector2i from, sf::Vector2i to, const GameState& gameState);

    // Kiểm tra xem tất cả kẻ địch đã hoàn thành lượt đi của mình chưa
    bool allEnemiesFinished(GameState& gameState);

    // Cập nhật hoạt ảnh xoay của chìa khóa
    void updateKeyAnimation(GameState& gameState, sf::Time dt);

    // Xử lý va chạm giữa các loại kẻ địch (kẻ mạnh hơn tiêu diệt kẻ yếu hơn)
    // Mummy White > Mummy Red > Scorpion White > Scorpion Red
    void resolveEnemyCollisions(GameState& gameState, AudioManager& audio);

    // Chuyển đổi tọa độ pixel sang tọa độ lưới
    sf::Vector2i pixelToGrid(sf::Vector2i pixelPos, const GameState& gameState);

    // Hàm sign xác định hướng (1, -1, 0)
    int sign(int val);

    // Lưu kết quả màn chơi (chỉ ghi đè nếu kết quả mới tốt hơn)
    void saveLevelResult(const std::string& levelName, int steps, int stars);

    // Lấy số sao của một màn chơi (trả về 0 nếu chưa chơi)
    int getLevelStars(const std::string& levelName);

    // Lưu trạng thái game hiện tại vào file save_progress.txt
    void saveGameProgress(const GameState& gameState, const std::string& mapName);

    // Tải game từ file. Trả về true nếu tải thành công.
    // Hàm này sẽ khởi tạo lại GameState dựa trên dữ liệu file.
    bool loadGameProgress(std::unique_ptr<GameState>& outState, std::string& outMapName, AssetManager& assets);

    // Kiểm tra xem có file save tồn tại không (để hiện nút Continue)
    bool hasSavedGame();
}
