#include "GameState.h"
#include "Character.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Hàm constructor sẽ gọi các hàm load
GameState::GameState(const std::string& layoutFileName, AssetManager& assetManager) {
    m_assetManagerRef = &assetManager;
    // Xây dựng đường dẫn đầy đủ đến các file cấu hình
    std::string mazePath = "levels/maze/" + layoutFileName;
    std::string agentsPath = "levels/agents/" + layoutFileName;

    // Tải cấu trúc mê cung và các đối tượng
    loadMazeFromFile(mazePath);
    loadObjectsFromFile(agentsPath, assetManager);
}

// Hàm hủy
GameState::~GameState() = default;

// Định nghĩa hàm loadMazeFromFile
void GameState::loadMazeFromFile(const std::string& filePath) {
    std::ifstream file(filePath);   // Mở file
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open maze file: " << filePath << std::endl;
        return;
    }

    // Đọc toàn bộ file theo line vào vector 'maze'
    while (std::getline(file, line)) {
        maze.push_back(line);
    }
    file.close();   // đóng file và tiếp tục xử lý maze

    // Khởi tạo vị trí mặc định để biết map có vật phẩm đó không
    keyPosition     = {-1, -1};
    trapPosition    = {-1, -1};
    gate.position   = {-1, -1};

    // Lặp qua ma trận để tìm các đối tượng đặc biệt
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            char cell = maze[i][j];
            if (cell == 'K') {
                keyPosition = {i, j};
                maze[i][j] = ' ';       // Biến ô chìa khóa thành đường đi bình thường
            } else if (cell == 'T') {
                trapPosition = {i, j};
                maze[i][j] = ' ';       // Biến ô bẫy thành đường đi bình thường
            } else if (cell == 'G') {
                gate.position = {i, j};
                // Ô cổng không biến thành đường đi, nó vẫn là một vật cản
            } else if (cell == 'S') {
                stairPosition = {i, j};
                // Ô cầu thang cũng là một vật cản
            }
        }
    }

    // Tính toán các thông số của maze
    if (!maze.empty()) {
        mazeSize = maze.size() / 2; // Kích thước logic của mê cung (6, 8, 10)
        cellRect = 360 / mazeSize;  // Kích thước pixel của mỗi ô,
                                    // 360 là kích thước cố định của khu vực sàn mê cung
    }
}

// Định nghĩa hàm loadObjectsFromFile
void GameState::loadObjectsFromFile(const std::string& filePath, AssetManager& assetManager) {
    std::ifstream file(filePath);   // Mở file
    std::string line;

    // Đọc file theo từng line để tạo các nhân vật
    while (std::getline(file, line)) {
        std::stringstream ss(line);     // tách chuỗi
        std::string type;
        int x, y;
        ss >> type >> x >> y;   // Tách loại nhân vật và tọa độ x, y

        // Dựa vào 'type', tạo đối tượng tương ứng và thêm vào vector
        if (type == "E") {  // Explorer
            explorer = std::make_unique<Explorer>(mazeSize, assetManager);
            explorer->setPosition(x, y, *this);
        }
        else if (type == "MW") {
            std::unique_ptr<Enemy> mummy = std::make_unique<MummyWhite>(mazeSize, assetManager);
            mummy->setPosition(x, y, *this);
            enemies.push_back(std::move(mummy));
        }
        else if (type == "MR") {
            std::unique_ptr<Enemy> mummy = std::make_unique<MummyRed>(mazeSize, assetManager);
            mummy->setPosition(x, y, *this);
            enemies.push_back(std::move(mummy));
        }
        else if (type == "SW") {    // Scorpion White
            std::unique_ptr<Enemy> scorpion = std::make_unique<ScorpionWhite>(mazeSize, assetManager);
            scorpion->setPosition(x, y, *this);
            enemies.push_back(std::move(scorpion));
        }
        else if (type == "SR") {    // Scorpion Red
            std::unique_ptr<Enemy> scorpion = std::make_unique<ScorpionRed>(mazeSize, assetManager);
            scorpion->setPosition(x, y, *this);
            enemies.push_back(std::move(scorpion));
        }
    }

    file.close();   // đóng file
}

void GameState::saveSnapshot() {
    GameSnapshot snap;

    // 1. Lưu người chơi
    if (explorer) snap.explorerPos = explorer->getPosition();

    // 2. Lưu quái vật
    for (const auto& e : enemies) {
        snap.enemies.push_back({e->getTypeName(), e->getPosition()});
    }

    // 3. Lưu môi trường
    snap.gate = gate;
    snap.keyPosition = keyPosition;
    snap.stepCount = stepCount;

    history.push(snap); // Đẩy vào ngăn xếp
}

void GameState::restoreSnapshot() {
    if (history.empty()) return;

    GameSnapshot snap = history.top();
    history.pop();

    // 1. Khôi phục người chơi
    if (explorer) explorer->setPosition(snap.explorerPos.x, snap.explorerPos.y, *this);

    // 2. Khôi phục môi trường
    gate = snap.gate;
    keyPosition = snap.keyPosition;
    stepCount = snap.stepCount;

    // 3. Khôi phục quái vật (Xóa cũ -> Tạo mới)
    enemies.clear();
    for (const auto& data : snap.enemies) {
        std::unique_ptr<Enemy> e;

        if (data.type == "MW") e = std::make_unique<MummyWhite>(mazeSize, *m_assetManagerRef);
        else if (data.type == "MR") e = std::make_unique<MummyRed>(mazeSize, *m_assetManagerRef);
        else if (data.type == "SW") e = std::make_unique<ScorpionWhite>(mazeSize, *m_assetManagerRef);
        else if (data.type == "SR") e = std::make_unique<ScorpionRed>(mazeSize, *m_assetManagerRef);

        if (e) {
            e->setPosition(data.position.x, data.position.y, *this);
            enemies.push_back(std::move(e));
        }
    }
}