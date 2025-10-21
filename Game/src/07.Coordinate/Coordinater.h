#pragma once

class CPivot
{
	friend class CCoordinateTransformer;
public:
	CPivot() {}
	~CPivot() {}
private:
	Vector2 Position;

};

class CCoordinateTransformer
{
public:
	CCoordinateTransformer() {}
	~CCoordinateTransformer() = default;

public:
	void Update()
	{

	}

private:


};

