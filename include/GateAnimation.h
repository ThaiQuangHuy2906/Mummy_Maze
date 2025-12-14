#pragma once
#include <SFML/System.hpp>
#include "GameState.h"

// xử lý logic chuyển đổi frame cho hoạt ảnh đóng/mở cổng.
void updateGateAnimation(GameState& gameState, sf::Time dt);
