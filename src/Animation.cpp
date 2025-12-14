#include "Animation.h"
#include "GameTypes.h"

// Constructor khởi tạo đối tượng Animation.
// Hàm này tải texture từ AssetManager và tính toán kích thước của mỗi frame
// trong spritesheet. Spritesheet được tổ chức theo dạng lưới:
// - 5 cột: 1 frame đứng yên + 4 frame bước đi
// - 4 hàng: tương ứng 4 hướng (UP, RIGHT, DOWN, LEFT)
// textureName: Tên file texture (spritesheet) cần tải
// mazeSize: Kích thước mê cung (6x6, 8x8, 10x10) - tham số này có thể dùng cho scaling
// assetManager: Reference đến AssetManager để lấy texture
Animation::Animation(const std::string &textureName, int mazeSize, AssetManager &assetManager)
    : texture(assetManager.getTexture(textureName))
{ // Lấy texture từ AssetManager

    sprite.setTexture(texture); // Gán texture cho sprite

    // Cấu trúc của spritesheet
    int cols = 5; // 5 frame mỗi hàng: frame 0 (đứng yên) + frame 1-4 (bước đi)
    int rows = 4; // 4 hàng tương ứng 4 hướng: 0:UP, 1:RIGHT, 2:DOWN, 3:LEFT

    // Tính toán kích thước (pixel) của một frame dựa trên kích thước texture
    cellWidth = texture.getSize().x / cols;
    cellHeight = texture.getSize().y / rows;
}

// Đặt hướng cho hoạt ảnh.
// Hướng này quyết định hàng (row) nào trong spritesheet sẽ được sử dụng
// khi cập nhật frame tiếp theo.
// dir: Hướng mới (UP, DOWN, LEFT, RIGHT)
void Animation::setDirection(Direction dir)
{
    m_direction = dir;
}

// Cập nhật frame hoạt ảnh sẽ được hiển thị.
// Hàm này chọn đúng frame từ spritesheet dựa trên:
// - Hướng hiện tại (m_direction) -> chọn hàng (row)
// - Chỉ số frame (0-4) -> chọn cột (col)
// Sau đó cắt (crop) vùng tương ứng từ texture bằng setTextureRect.
// frame: Chỉ số frame cần hiển thị (0: đứng yên, 1-4: các bước đi)
void Animation::update(int frame)
{
    // Xác định hàng (row) trong spritesheet dựa vào hướng hiện tại
    int row = 0; // Mặc định là UP (hàng 0)
    switch (m_direction)
    {
    case Direction::Right:
        row = 1;
        break; // Hàng 1: nhìn phải
    case Direction::Down:
        row = 2;
        break; // Hàng 2: nhìn xuống
    case Direction::Left:
        row = 3;
        break; // Hàng 3: nhìn trái
    case Direction::Up:
        row = 0;
        break; // Hàng 0: nhìn lên
    default:
        row = 0;
        break;
    }

    // Xác định cột (col) trong spritesheet dựa vào chỉ số frame
    // Sử dụng modulo 5 để đảm bảo frame luôn trong khoảng 0-4
    int col = frame % 5;

    // Cắt (crop) đúng frame cần vẽ từ spritesheet
    // IntRect(left, top, width, height)
    sprite.setTextureRect(sf::IntRect(
        col * cellWidth,  // Tọa độ X của frame
        row * cellHeight, // Tọa độ Y của frame
        cellWidth,        // Chiều rộng frame
        cellHeight        // Chiều cao frame
        ));
}

// Vẽ sprite (frame đã được cắt) lên cửa sổ.
// window: Cửa sổ render mà sprite sẽ được vẽ lên
// pos: Vị trí (x, y) trên màn hình để vẽ sprite
void Animation::draw(sf::RenderWindow &window, sf::Vector2f pos)
{
    sprite.setPosition(pos); // Đặt vị trí sprite
    window.draw(sprite);     // Vẽ sprite lên window
}