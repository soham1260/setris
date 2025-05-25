#include "ToggleButton.h"

ToggleButton::ToggleButton(float x, float y, float width, float height)
    : rect{ x, y, width, height }, isToggled(false) {}

void ToggleButton::Update(Vector2 mouse) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(mouse, rect)) {
        isToggled = !isToggled;
    }
}

void ToggleButton::Draw() const {
    Color color = isToggled ? GREEN : RED;
    DrawRectangleRec({rect.x,rect.y,rect.width/2,rect.height}, isToggled ? GREEN : WHITE);
    DrawRectangleRec({rect.x+rect.width/2,rect.y,rect.width/2,rect.height}, isToggled ? WHITE : RED);
    DrawRectangleLinesEx({rect.x,rect.y,rect.width/2,rect.height}, 2, BLACK);
    DrawRectangleLinesEx({rect.x+rect.width/2,rect.y,rect.width/2,rect.height}, 2, BLACK);
}

bool ToggleButton::GetState() const {
    return isToggled;
}

void ToggleButton::SetState(bool state) {
    isToggled = state;
}

Rectangle ToggleButton::GetRect() const {
    return rect;
}
