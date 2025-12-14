#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>

// Lớp quản lý âm thanh và nhạc nền cho game.
// AudioManager chịu trách nhiệm tải, lưu trữ và phát tất cả các âm thanh
// trong game, bao gồm nhạc nền và các hiệu ứng âm thanh.
class AudioManager
{
private:
    sf::Music bgMusic;                              // Nhạc nền của game (background music)
    std::map<std::string, sf::SoundBuffer> buffers; // Lưu trữ dữ liệu âm thanh (sound buffer)
    std::map<std::string, sf::Sound> sounds;        // Các đối tượng Sound để phát âm thanh

public:
    // Tải tất cả âm thanh và nhạc nền cần thiết cho game.
    // Trả về: true nếu tải thành công, false nếu có lỗi.
    bool load();

    // Phát một âm thanh theo tên.
    // name: Tên của âm thanh cần phát (key, trap, mummy, collision, v.v.)
    void playSound(const std::string &name);

    // Bắt đầu phát nhạc nền.
    void playMusic();

    // Dừng nhạc nền.
    void stopMusic();
};
