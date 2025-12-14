#include "AssetManager.h"
#include <iostream>

// Lấy texture theo tên file, tự động tải nếu chưa có trong bộ nhớ.
// Hàm này thực hiện lazy loading:
// - Nếu texture đã được tải trước đó, trả về reference đến texture đã có
// - Nếu chưa tải, nạp texture từ file và lưu vào map
// Điều này đảm bảo mỗi texture chỉ được nạp vào RAM một lần duy nhất,
// tiết kiệm bộ nhớ và tăng hiệu năng.
// filename: Tên file texture (VD: "player.png", "wall.png")
//           File sẽ được tìm trong thư mục "assets/images/"
// Trả về: Reference đến texture đã được tải
sf::Texture &AssetManager::getTexture(const std::string &filename)
{
    // Kiểm tra xem texture đã được tải chưa
    // find() trả về iterator đến phần tử nếu tìm thấy, end() nếu không tìm thấy
    if (textures.find(filename) == textures.end())
    {
        // Texture chưa có trong map -> cần tải từ file

        // Sử dụng operator[] để tự động tạo phần tử mới trong map
        // Sau đó gọi loadFromFile() để nạp dữ liệu từ đĩa vào texture
        if (!textures[filename].loadFromFile("assets/images/" + filename))
        {
            // Thông báo lỗi nếu không tải được (file không tồn tại, sai định dạng, etc.)
            std::cerr << "Error: Could not load texture: " << filename << std::endl;
        }
    }

    // Trả về reference đến texture (đã có sẵn hoặc vừa được tải)
    return textures[filename];
}