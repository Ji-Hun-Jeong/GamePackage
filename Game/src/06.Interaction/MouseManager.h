#pragma once

class CMouseManager
{
	SINGLE(CMouseManager)
public:
	void SetMousePosition(const Vector2& InMousePosition) { MousePosition = InMousePosition; }
	const Vector2& GetMousePosition() const { return MousePosition; }

private:
	Vector2 MousePosition;

};

