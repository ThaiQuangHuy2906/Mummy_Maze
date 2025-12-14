#include "AssetManager.h"
#include <iostream>

sf::Texture& AssetManager::getTexture(const std::string& filename) {
    // Nếu texture chưa được nạp (không tìm thấy trong map)
    if (textures.find(filename) == textures.end())
        // Nạp texture từ file và đưa vào map
        // textures[name] sẽ tự động tạo một phần tử mới trong map với key là 'filename'.
        if (!textures[filename].loadFromFile("assets/images/" + filename))
            std::cerr << "Error: Could not load texture: " << filename << std::endl;

    return textures[filename];
}