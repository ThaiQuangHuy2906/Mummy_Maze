#include "AudioManager.h"
#include <iostream>

// Tải tất cả tài nguyên âm thanh vào bộ nhớ.
// Hàm này thực hiện:
// 1. Tải nhạc nền từ file OGG và cấu hình lặp lại
// 2. Tải tất cả các hiệu ứng âm thanh (SFX) từ các file WAV
// 3. Gán volume và buffer cho từng âm thanh
// Trả về: true nếu mọi file đều tải thành công, false nếu có lỗi
bool AudioManager::load()
{
    // ===== TẢI NHẠC NỀN =====
    // Sử dụng openFromFile() cho Music để stream từ file (tiết kiệm RAM)
    if (!bgMusic.openFromFile("assets/audio/explorerMove.ogg"))
    {
        std::cerr << "Khong the tai nhac nen\n";
        return false;
    }
    bgMusic.setLoop(true); // Lặp lại vô hạn khi hết bài
    bgMusic.setVolume(30); // Đặt âm lượng 30% (0-100)

    // ===== TẢI CÁC HIỆU ỨNG ÂM THANH (SFX) =====
    // Cấu trúc tạm để lưu danh sách file cần tải
    struct SoundFile
    {
        std::string name, path;
    };
    std::vector<SoundFile> list = {
        {"key", "assets/audio/key_pick.wav"},      // Âm thanh nhặt chìa khóa
        {"trap", "assets/audio/trap.wav"},         // Âm thanh rơi vào bẫy
        {"mummy", "assets/audio/mummy_catch.wav"}, // Âm thanh gặp mummy
        {"foostep", "assets/audio/Footsteps.wav"}, // Âm thanh bước chân
        {"click", "assets/audio/click.wav"},       // Âm thanh click button
        {"stairs", "assets/audio/win.wav"},        // Âm thanh lên cầu thang (thắng)
        {"collision", "assets/audio/collide.wav"}  // Âm thanh va chạm giữa các quái
    };

    // Duyệt danh sách và tải từng file
    for (auto &s : list)
    {
        // Tải dữ liệu âm thanh vào buffer
        if (!buffers[s.name].loadFromFile(s.path))
        {
            std::cerr << "Khong the tai " << s.path << "\n";
            return false;
        }
        // Gán buffer cho sound object để có thể phát
        sounds[s.name].setBuffer(buffers[s.name]);
        sounds[s.name].setVolume(100); // Âm lượng tối đa cho SFX
    }

    return true;
}

// Bắt đầu phát nhạc nền.
// Nhạc sẽ tự động lặp lại do đã được cấu hình loop=true.
void AudioManager::playMusic()
{
    bgMusic.play();
}

// Dừng nhạc nền đang phát.
void AudioManager::stopMusic()
{
    bgMusic.stop();
}

// Phát một hiệu ứng âm thanh theo tên.
// name: Tên của âm thanh cần phát (phải khớp với key đã load trong load())
// Nếu tên không tồn tại trong map sounds, hàm sẽ không làm gì.
void AudioManager::playSound(const std::string &name)
{
    if (sounds.count(name))
        sounds[name].play();
}
