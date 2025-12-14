#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "GameTypes.h"
#include "AudioManager.h"
#include "GameTypes.h"

// Forward declaration
class GameState;
class Explorer;

// Lớp cơ sở cho tất cả các nhân vật trong game
class Character {
protected:
    sf::Vector2i gridPosition;  // Tọa độ của nhân vật trên lưới mê cung (không phải pixel)
    Animation animation;        // Mỗi nhân vật có một animation để xử lý

    // Các biến phục vụ việc di chuyển
    sf::Vector2f pixelPosition;         // Vị trí pixel hiện tại để vẽ trên màn hình
    sf::Vector2f targetPixelPosition;   // Vị trí pixel đích cần di chuyển tới
    bool m_isMoving = false;            // Cờ báo hiệu nhân vật đang di chuyển
    float moveSpeed = 180.0f;           // Tốc độ di chuyển (pixel mỗi giây) 180 / movespeed: toc do di chuyen

    // Các biến cho hoạt ảnh bước đi
    int animationFrame = 0;                     // Frame hiện tại của hoạt ảnh (0-4)
    sf::Time animationTimer;                    // Bộ đếm thời gian để chuyển frame
    Direction m_direction = Direction::Down;    // Hướng di chuyển/nhìn hiện tại

public:
    virtual ~Character() = default;                 // Virtual destructor
    Character(const std::string& textureName,       // Constructor để khởi tạo Animation
        int mazeSize, AssetManager& assetManager);

    void setPosition(int gridX, int gridY, const GameState& gameState);     // Đặt vị trí ban đầu cho nhân vật
    void moveTo(sf::Vector2i targetGridPos, const GameState& gameState, AudioManager& audio);    // Bắt đầu di chuyển tới ô mới
    void update(sf::Time dt);                                               // Cập nhật trạng thái (di chuyển, hoạt ảnh) mỗi frame
    void draw(sf::RenderWindow& window, sf::Vector2f position);             // Hàm vẽ nhân vật

    // Getter
    sf::Vector2i getPosition() const;       // Lấy vị trí trên lưới
    sf::Vector2f getPixelPosition() const;  // Lấy vị trí pixel
    bool isMoving() const;                  // Kiểm tra có đang di chuyển không

    // Hàm kiểm tra một nước đi có hợp lệ không (có vướng tường, cổng,...)
    bool isMoveValid(sf::Vector2i newPos, const GameState& gameState);
};

// Lớp người chơi, kế thừa từ Character
class Explorer : public Character {
public:
    Explorer(int mazeSize, AssetManager& assetManager)
        : Character("explorer" + std::to_string(mazeSize) + ".png", mazeSize, assetManager) {}
};

// Lớp cơ sở cho các kẻ địch, kế thừa từ Character
class Enemy : public Character {
protected:
    int movesRemaining = 0;     // Số nước đi còn lại trong một lượt

    void decideEasyMove(const Explorer& player, const GameState& gameState, AudioManager& audio);
    void decideMediumMove(const Explorer& player, const GameState& gameState, bool horizontalPriority, AudioManager& audio);
    void decideHardMove(const Explorer& player, const GameState& gameState,AudioManager& audio);

public:
    using Character::Character;

    // Tính toán và bắt đầu di chuyển của kẻ địch
    virtual void decideMove(const Explorer& player, const GameState& gameState, Difficulty difficulty, AudioManager& audio) = 0;

    // Các hàm quản lý lượt đi
    virtual void startTurn();               // Bắt đầu lượt, nạp lại số nước đi
    virtual bool isTurnFinished() const;    // Kiểm tra xem lượt đã kết thúc chưa
    // Hàm xác định quái vật gì khi lưu Undo
    virtual std::string getTypeName() const = 0;
};

// Các lớp kẻ địch cụ thể, kế thừa từ Enemy
class MummyWhite : public Enemy {
public:
    using Enemy::Enemy;
    MummyWhite(int mazeSize, AssetManager& assetManager)
        : Enemy("mummy_white" + std::to_string(mazeSize) + ".png", mazeSize, assetManager) {}

    void startTurn() override;
    void decideMove(const Explorer& player, const GameState& gameState, Difficulty difficulty, AudioManager& audio) override;
    std::string getTypeName() const override { return "MW"; } // Trả về mã loại
};

class MummyRed : public Enemy {
public:
    using Enemy::Enemy;
    MummyRed(int mazeSize, AssetManager& assetManager)
        : Enemy("redmummy" + std::to_string(mazeSize) + ".png", mazeSize, assetManager) {}

    void startTurn() override;
    void decideMove(const Explorer& player, const GameState& gameState, Difficulty difficulty, AudioManager& audio) override;
    std::string getTypeName() const override { return "MR"; }
};

class ScorpionWhite : public Enemy {
public:
    using Enemy::Enemy;
    ScorpionWhite(int mazeSize, AssetManager& assetManager)
        : Enemy("white_scorpion" + std::to_string(mazeSize) + ".png", mazeSize, assetManager) {}

    void startTurn() override;
    void decideMove(const Explorer& player, const GameState& gameState, Difficulty difficulty, AudioManager& audio) override;
    std::string getTypeName() const override { return "SW"; } // Trả về mã loại
};

class ScorpionRed : public Enemy {
public:
    using Enemy::Enemy;
    ScorpionRed(int mazeSize, AssetManager& assetManager)
        : Enemy("red_scorpion" + std::to_string(mazeSize) + ".png", mazeSize, assetManager) {}

    void startTurn() override;
    void decideMove(const Explorer& player, const GameState& gameState, Difficulty difficulty, AudioManager& audio) override;
    std::string getTypeName() const override { return "SR"; } // Trả về mã loại
};