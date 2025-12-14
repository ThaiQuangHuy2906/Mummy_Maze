// main.cpp
// Điểm vào chính của game Mummy Maze.
// Nhiệm vụ:
//  - Tạo cửa sổ SFML với kích thước scale lên từ kích thước thiết kế gốc.
//  - Tạo AssetManager, AudioManager, MainMenu, LevelSelect.
//  - Chạy vòng lặp state machine: MainMenu -> LevelSelect -> InGame -> Exiting.
//  - Khi vào InGame thì tạo GameState + GameLoop và cho GameLoop tự chạy.

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "AudioManager.h"
#include "GameState.h"
#include "GameLoop.h"
#include "MainMenu.h"
#include "LevelSelect.h"
#include "AppTypes.h"
#include "GameLogic.h"
#include <string>
#include <iostream>
#include <memory>

int main() {
    // 1. Tạo cửa sổ với kích thước LỚN HƠN (nhân với hệ số scale).
    //    GAME_WIDTH/GAME_HEIGHT là kích thước thiết kế gốc (494x480),
    //    WINDOW_SCALE = 1.5f giúp scale to lên cho dễ nhìn hơn.
    sf::RenderWindow window(sf::VideoMode(GAME_WIDTH * WINDOW_SCALE, GAME_HEIGHT * WINDOW_SCALE), "Mummy Maze");
    window.setFramerateLimit(60);   // Giới hạn FPS cho ổn định.

    // 2. Tạo một View (Camera) với kích thước GỐC.
    //    Nội dung game luôn được vẽ trong hệ tọa độ 494x480,
    //    sau đó SFML tự stretch để fit với cửa sổ thật.
    sf::View gameView(sf::FloatRect(0.f, 0.f, (float)GAME_WIDTH, (float)GAME_HEIGHT));
    window.setView(gameView);

    // 3. Đặt icon cho cửa sổ (nếu load được)
    sf::Image icon;
    if (icon.loadFromFile("assets/images/icon.png"))
        // Hàm setIcon yêu cầu: chiều rộng, chiều cao, và mảng pixel
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // 4. Khởi tạo các hệ thống tài nguyên & âm thanh.
    AssetManager assetManager;
    AudioManager audio;
    if (!audio.load())
        std::cerr << "Loi: Khong the load am thanh!" << std::endl;

    // 5. Khởi tạo các màn hình (scene) chính.
    MainMenu mainMenu(assetManager, audio);
    LevelSelect levelSelect(assetManager, audio);

    // 6. Biến trạng thái toàn cục của app:
    //    - currentState: đang ở màn hình nào.
    //    - selectedMap: tên file map được chọn khi vào InGame.
    AppState currentState = AppState::MainMenu;
    std::string selectedMap = "";

    // Cờ cho biết có đang load từ file savegame.txt hay không.
    bool isLoadingFromSave = false;

    // 7. Vòng lặp tổng của chương trình (application loop).
    while (window.isOpen()) {

        // Biến event dùng chung cho từng state.
        sf::Event event;
        // Lưu ý: ta chỉ pollEvent trong MainMenu và LevelSelect.
        // Khi vào InGame thì GameLoop sẽ tự xử lý event riêng.

        // 8. State machine chính điều hướng các màn hình.
        switch (currentState) {
            case AppState::MainMenu:
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();

                    // MainMenu tự thay đổi currentState khi click Play/Quit/Continue.
                    mainMenu.handleEvent(event, currentState, window);

                    // Nếu người chơi bấm nút Continue trong MainMenu:
                    if (mainMenu.isContinueSelected()) {
                        isLoadingFromSave = true;       // Đánh dấu là muốn Load Game từ file savegame.txt
                        mainMenu.resetContinueFlag();   // Reset cờ, tránh bị giữ trạng thái cho lần sau.
                    }
                }
                break;

            case AppState::LevelSelect:
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();

                    // LevelSelect sẽ:
                    //  - cập nhật currentState sang InGame khi chọn map.
                    //  - gán selectedMap = "tên file map" tương ứng.
                    levelSelect.handleEvent(event, currentState, selectedMap, window);
                }
                break;

            case AppState::InGame:
                // Không xử lý event ở đây
                // Vì GameLoop (vòng lặp trong game) sẽ tự đảm nhận.
                break;

            case AppState::Exiting:
                window.close();
                break;
        }

        // Nếu chuyển sang trạng thái Exiting thì đóng cửa sổ và bỏ qua phần vẽ.
        if (currentState == AppState::Exiting) {
            window.close();
            continue;
        }

        // 9. Vẽ màn hình (chỉ vẽ cho MainMenu và LevelSelect).
        //    Khi ở InGame, GameLoop sẽ tự clear/draw/display.
        if (currentState != AppState::InGame) {
            window.clear();

            if (currentState == AppState::MainMenu)
                mainMenu.draw(window);
            else if (currentState == AppState::LevelSelect)
                levelSelect.draw(window);

            window.display();
        }

        // 10. Logic chuyển từ Menu/LevelSelect vào InGame.
        if (currentState == AppState::InGame) {
            std::unique_ptr<GameState> gameStatePtr;

            if (isLoadingFromSave) {
                // TH1: Nếu bấm Continue -> load từ file save.
                // hàm loadSavedGame sẽ trả về GameState* hoặc nullptr.
                std::cout << "Dang load game tu file save..." << std::endl;
                std::string loadedMapName;

                // Hàm GameLogic::loadGameProgress sẽ:
                // 1. Đọc file
                // 2. Tạo mới GameState và trả về qua con trỏ gameStatePtr
                // 3. Cập nhật vị trí nhân vật, quái vật...
                bool success = GameLogic::loadGameProgress(gameStatePtr, loadedMapName, assetManager);

                if (success) {
                    selectedMap = loadedMapName; // Cập nhật tên map để dùng cho logic restart nếu cần
                    isLoadingFromSave = false;   // Đã load xong, reset cờ
                } else {
                    std::cerr << "Loi load game! Quay ve menu." << std::endl;
                    currentState = AppState::MainMenu;
                    isLoadingFromSave = false;
                    continue; // Bỏ qua lượt này, quay lại vòng lặp
                }
            }
            else
                // TH2: New Game (Chơi từ Level Select hoặc Next Level)
                // Tự tạo mới GameState như bình thường
                gameStatePtr = std::make_unique<GameState>(selectedMap, assetManager);

            // Khởi tạo GameLoop với con trỏ gameStatePtr
            GameLoop gameLoop(selectedMap, *gameStatePtr, assetManager, audio, window);

            // Chạy vòng lặp game. Hàm này sẽ:
            //  - tự xử lý input
            //  - cập nhật logic
            //  - vẽ khung hình
            //  - và thoát khi game kết thúc hoặc chuyển màn.
            gameLoop.run();
            audio.stopMusic();

            // Xử lý sau khi game kết thúc (Thắng/Thua/Pause/Exit)
            // 1. Chơi lại (Restart)
            if (gameLoop.isRestartRequested())
                // Restart cùng một map: không đổi selectedMap.
                currentState = AppState::InGame;

            // 2. Màn tiếp theo (Next Level)
            else if (gameLoop.isNextLevelRequested()) {
                size_t lastUnderscore = selectedMap.find_last_of('_');
                size_t dotTxt = selectedMap.find(".txt");

                bool conversionSuccess = false;

                if (lastUnderscore != std::string::npos && dotTxt != std::string::npos) {
                    try {
                        // Tách phần đầu: "map6_"
                        std::string prefix = selectedMap.substr(0, lastUnderscore + 1);
                        // Tách phần số: "1"
                        std::string numStr = selectedMap.substr(lastUnderscore + 1, dotTxt - lastUnderscore - 1);

                        // Chuyển chuỗi sang số và cộng 1
                        int currentNum = std::stoi(numStr);
                        int nextNum = currentNum + 1;

                        // Tạo tên map mới
                        std::string nextMapName = prefix + std::to_string(nextNum) + ".txt";

                        // Cập nhật và giữ trạng thái InGame
                        selectedMap = nextMapName;
                        currentState = AppState::InGame; // Đảm bảo vẫn ở trong game
                        conversionSuccess = true;

                        printf("Chuyen sang map: %s\n", nextMapName.c_str());

                    } catch (...) {
                        // Nếu lỗi stoi
                        conversionSuccess = false;
                    }
                }

                // Nếu có lỗi bất kỳ (không parse được tên file), về Menu
                if (!conversionSuccess) {
                    printf("Loi: Khong the xac dinh map tiep theo tu: %s\n", selectedMap.c_str());
                    currentState = AppState::MainMenu;
                }

            }

            // 3. Level Select (Về chọn màn)
            else if (gameLoop.isReturnToLevelSelectRequested()) {
                currentState = AppState::LevelSelect;
                selectedMap = "";
            }

            // 4. Mặc định: Về Main Menu (Quit/Win/Lose mà không chọn gì khác)
            else
                currentState = AppState::MainMenu;
        }
    }

    return 0;
}