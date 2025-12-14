// WallSpriteSheet.h
// Quản lý và render các loại tường trong mê cung.
// Lớp này sử dụng kỹ thuật batch rendering (vẽ hàng loạt) để tối ưu hiệu năng.
// Thay vì vẽ từng tường một (gây ra nhiều draw call), ta thêm tất cả tường
// vào một VertexArray rồi vẽ một lần duy nhất.
#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"

// Các loại tường khác nhau trong spritesheet.
// Mỗi loại tường có vị trí và hiệu ứng bóng đổ khác nhau:
enum class WallType
{
    Left,        // Tường dọc thông thường (không có bóng)
    RightShadow, // Tường dọc có bóng đổ (dùng ở góc giao với tường ngang)
    Up,          // Tường ngang có bóng đổ
    UpNoShadow   // Tường ngang không bóng (dùng khi vẽ đè lên tường có góc)
};

// Lớp quản lý texture và render tường theo batch.
// Thay vì vẽ từng sprite tường riêng lẻ, lớp này cung cấp phương thức
// appendWall() để thêm các quad (hình chữ nhật) vào VertexArray.
// Sau đó GameRenderer chỉ cần vẽ toàn bộ VertexArray một lần.
class WallSpriteSheet
{
public:
    // Constructor tải texture tường và tính toán vị trí các frame.
    // mazeSize: Kích thước mê cung (6, 8, hoặc 10) - quyết định file texture nào được dùng
    // assetManager: Reference đến AssetManager để load texture
    WallSpriteSheet(int mazeSize, AssetManager &assetManager);

    // Thêm một tường vào VertexArray (batch rendering).
    // Hàm này không vẽ trực tiếp mà thêm 4 đỉnh (quad) vào VertexArray
    // với texture coordinates tương ứng loại tường.
    // va: VertexArray chứa tất cả các tường (được vẽ cùng lúc sau này)
    // position: Vị trí pixel để vẽ tường
    // type: Loại tường (Left, RightShadow, Up, UpNoShadow)
    void appendWall(sf::VertexArray &va, sf::Vector2f position, WallType type);

    // Lấy texture để GameRenderer sử dụng khi vẽ batch.
    const sf::Texture *getTexture() const { return m_texture; }

private:
    const sf::Texture *m_texture; // Con trỏ đến texture tường (quản lý bởi AssetManager)

    // Các IntRect lưu vị trí từng loại tường trong spritesheet
    sf::IntRect leftWallRect;       // Vùng texture cho tường dọc thường
    sf::IntRect rightWallRect;      // Vùng texture cho tường dọc có bóng
    sf::IntRect upWallRect;         // Vùng texture cho tường ngang có bóng
    sf::IntRect upWallNoShadowRect; // Vùng texture cho tường ngang không bóng
};