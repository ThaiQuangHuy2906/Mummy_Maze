// GateAnimation.h
// Quản lý hoạt ảnh mở/đóng cổng (gate animation).
// Khi người chơi nhặt chìa khóa, các cổng sẽ từ từ mở ra (animation).
// Module này xử lý việc chuyển đổi frame để tạo hiệu ứng mượt mà.
#pragma once
#include <SFML/System.hpp>
#include "GameState.h"

// Cập nhật hoạt ảnh mở/đóng cổng theo thời gian.
// Hàm này được gọi mỗi frame để:
// - Tăng frame animation của cổng theo thời gian (dt)
// - Khi đạt frame cuối cùng, đánh dấu animation hoàn thành
// - Cập nhật trạng thái cổng từ "đang mở" sang "đã mở hoàn toàn"
// gameState: Reference đến GameState chứa thông tin cổng và animation
// dt: Delta time - thời gian trôi qua từ frame trước (để animation mượt)
void updateGateAnimation(GameState &gameState, sf::Time dt);
