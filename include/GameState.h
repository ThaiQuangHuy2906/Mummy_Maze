#pragma once
#include <vector>
#include <string>
#include <memory>
#include <stack>
#include <SFML/System/Vector2.hpp>
#include "AssetManager.h"
#include "Character.h"

// Forward declaration: các lớp này có tồn tại
class Explorer;
class Enemy;
class MummyWhite;
class MummyRed;
class ScorpionWhite;
class ScorpionRed;

// Cấu trúc Gate
struct Gate {
    sf::Vector2i position;                  // Vị trí của cửa trên ma trận
    bool isClosed = true;                   // Trạng thái: true = đóng | false = mở
    int animationCellIndex = 0;             // Frame hoạt ảnh hiện tại (0 - 7)
    bool keyWasPressedLastFrame = false;    // Flag kiểm tra xem chìa khóa có được nhấn ở frame trước không
                                            // Tránh việc đóng/mở cổng liên tục khi có người đứng trên chìa khóa

    // Các biến cho hoạt ảnh 'Xoay'
    bool isAnimating = false;               // Flag báo hiệu cửa đang chạy hoạt ảnh
    bool isOpening = false;                 // Flag báo hiệu cửa đang mở (true) hay đóng (false)
    sf::Time animationTimer;                // Bộ đếm thời gian giữa các frame của hoạt ảnh
};

// Cấu trúc lưu trữ dữ liệu của một con quái tại một thời điểm
struct EnemySnapshot {
    std::string type;
    sf::Vector2i position;
};

struct GameSnapshot {
    sf::Vector2i explorerPos;           // Vị trí người chơi
    std::vector<EnemySnapshot> enemies; // Danh sách kẻ địch
    Gate gate;                          // Trạng thái cổng
    sf::Vector2i keyPosition;           // Trạng thái chìa khóa (nếu cần)
    int stepCount;                      // Điểm số
};

// Lớp GameState chứa tất cả dữ liệu về màn chơi hiện tại
class GameState {
private:
    // tải cấu trúc mê cung từ file .txt
    void loadMazeFromFile(const std::string& filePath);

    // tải các đối tượng (nhân vật, quái vật) từ file .txt
    void loadObjectsFromFile(const std::string& filePath, AssetManager& assetManager);

    AssetManager* m_assetManagerRef;    // Lưu tham chiếu để dùng khi tái tạo quái vật

public:
    // Constructor tải màn chơi
    GameState(const std::string& layoutFileName, AssetManager& assetManager);
    ~GameState();   // Destructor

    // 1. Mê cung, cầu thang, bẫy, chìa khóa, cổng
    std::vector<std::string> maze;  // Lưu cấu trúc mê cung
    sf::Vector2i stairPosition;     // Tọa độ cầu thang (điểm chiến thắng)
    sf::Vector2i trapPosition;      // Tọa độ bẫy
    sf::Vector2i keyPosition;       // Tọa độ chìa khóa
    Gate gate;                      // Đối tượng cổng
    int keyAnimationCellIndex = 0;  // Frame hiện tại của hoạt ảnh chìa khóa xoay
    sf::Time keyAnimationTimer;     // Bộ đếm thời gian cho hoạt ảnh chìa khóa

    // 2. Nhân vật (Người chơi, Quái vật)
    std::unique_ptr<Explorer> explorer;
    std::vector<std::unique_ptr<Enemy>> enemies;
    int stepCount = 0; // Biến lưu số bước đi của người chơi

    // Thông số mê cung
    int mazeSize;  // Kích thước mê cung (ví dụ: 6x6, 8x8, 10x10)
    int cellRect;  // Kích thước (pixel) của một ô trong mê cung khi vẽ lên màn hình

    // Vị trí và kích thước để vẽ mê cung
    const int SCREEN_WIDTH = 494;       // Chiều rộng màn hình
    const int SCREEN_HEIGHT = 480;      // Chiều cao màn hình
    const int MAZE_DRAW_OFFSET_X = 67;  // Tọa độ X góc trên bên trái của sàn mê cung
    const int MAZE_DRAW_OFFSET_Y = 80;  // Tọa độ Y góc trên bên trái của sàn mê cung

    // Undo
    std::stack<GameSnapshot> history; // Lịch sử đấu
    void saveSnapshot();              // Lưu
    void restoreSnapshot();           // Khôi phục
};