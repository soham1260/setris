#include "raylib.h"

class ToggleButton {
private:
    Rectangle rect;
    bool isToggled;

public:
    ToggleButton(float x, float y, float width, float height);
    
    void Update(Vector2 mouse);
    void Draw() const;

    bool GetState() const;
    void SetState(bool state);

    Rectangle GetRect() const;
};