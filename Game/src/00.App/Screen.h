#pragma once

class CScreen
{
public:
	CScreen() = default;
	~CScreen() = default;

public:
	void SetScreenPosition(const Vector2& InScreenPosition)
	{
		ScreenPosition = InScreenPosition;
	}
	const Vector2& GetScreenPosition() const { return ScreenPosition; }

private:
	Vector2 ScreenPosition;

};

