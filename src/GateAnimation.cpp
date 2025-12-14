#include "GateAnimation.h"

void updateGateAnimation(GameState& gameState, sf::Time dt) {
    // Nếu cổng không đang trong trạng thái hoạt ảnh thì không làm gì
    if (!gameState.gate.isAnimating) return;

    // Cộng dồn thời gian từ frame trước
    gameState.gate.animationTimer += dt;

    // Cứ 50ms thì chuyển frame một lần
    if (gameState.gate.animationTimer.asMilliseconds() > 50) {
        if (gameState.gate.isOpening)   // Nếu cổng đang mở, tăng chỉ số frame
            gameState.gate.animationCellIndex++;
        else                            // Nếu cổng đang đóng, giảm chỉ số frame
            gameState.gate.animationCellIndex--;

        // Reset bộ đếm thời gian
        gameState.gate.animationTimer = sf::Time::Zero;
    }

    // Kiểm tra xem hoạt ảnh đã kết thúc chưa
    // Spritesheet cổng có 8 frame (từ 0 đến 7)
    // Nếu đang mở và đã đến frame cuối (>= 7)
    // Hoặc nếu đang đóng và đã về frame đầu (<= 0)
    if ((gameState.gate.isOpening && gameState.gate.animationCellIndex >= 7) ||
        (!gameState.gate.isOpening && gameState.gate.animationCellIndex <= 0))
    {

        // Chốt lại ở frame cuối cùng (7 cho mở, 0 cho đóng)
        gameState.gate.animationCellIndex = gameState.gate.isOpening ? 7 : 0;
        // Tắt cờ báo hiệu đang hoạt ảnh
        gameState.gate.isAnimating = false;

    }
}
