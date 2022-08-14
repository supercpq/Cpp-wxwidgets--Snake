#pragma once
class food
{
private:
	int xx;
	int yy;
public:
	food(int x,int y): xx(x), yy(y) { }
	int getx()
	{
		return xx;
	}
	int gety()
	{
		return yy;
	}
};

