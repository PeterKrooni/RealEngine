#pragma once
class EntityPositionComponent
{
public:
	void setX(int x);
	void setY(int y);
	int getX();
	int getY();

	int x = 0;
	int y = 0;
};

