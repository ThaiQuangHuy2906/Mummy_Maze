#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>

class AudioManager {
private:
    sf::Music bgMusic;
    std::map<std::string, sf::SoundBuffer> buffers;
    std::map<std::string, sf::Sound> sounds;

public:
    bool load();
    void playSound(const std::string &name);
    void playMusic();
    void stopMusic();
};
