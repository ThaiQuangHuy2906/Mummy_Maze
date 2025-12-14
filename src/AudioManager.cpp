#include "AudioManager.h"
#include <iostream>

bool AudioManager::load() {
    // Tải nhạc nền

    if (!bgMusic.openFromFile("assets/audio/explorerMove.ogg")) {
        std::cerr << "Khong the tai nhac nen\n";
        return false;
    }
    bgMusic.setLoop(true); // lặp vô hạn
    bgMusic.setVolume(30); // âm lượng 30

    // âm thanh
    struct SoundFile { std::string name, path; };
    std::vector<SoundFile> list = {
        {"key",  "assets/audio/key_pick.wav"},
        {"trap", "assets/audio/trap.wav"},
        {"mummy","assets/audio/mummy_catch.wav"},
        {"foostep","assets/audio/Footsteps.wav"},
        {"click","assets/audio/click.wav"},
        {"stairs","assets/audio/win.wav"},
        {"collision","assets/audio/collide.wav"}
    };

    for (auto &s : list) {
        if (!buffers[s.name].loadFromFile(s.path)) {
            std::cerr << "Khong the tai " << s.path << "\n";
            return false;
        }
        sounds[s.name].setBuffer(buffers[s.name]);
        sounds[s.name].setVolume(100);
    }

    return true;
}

void AudioManager::playMusic() {
    bgMusic.play();
}

void AudioManager::stopMusic() {
    bgMusic.stop();
}

void AudioManager::playSound(const std::string &name) {
    if (sounds.count(name))
        sounds[name].play();
}
