#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

// Lớp AssetManager giúp quản lý tài nguyên game một cách hiệu quả.
// Nó đảm bảo mỗi tệp hình ảnh (texture) chỉ được tải vào bộ nhớ một lần duy nhất.
class AssetManager {
private:
    std::map<std::string, sf::Texture> textures;            // Dùng map để lưu trữ các texture đã nạp

public:
    sf::Texture& getTexture(const std::string& filename);   // Lấy texture, nếu chưa có sẽ nạp từ file
};