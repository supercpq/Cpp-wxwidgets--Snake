#include "snake.h"

std::list<SnakeBody> snbodys;

int SnakeHead::getx()
{
	return dx;
}

int SnakeHead::gety()
{
	return dy;
}

void SnakeHead::iscrash()
{
	using namespace std;
	list<SnakeBody>::iterator thisbody = snbodys.begin();
	int xx(dx), yy(dy);
	thisbody = find_if(snbodys.begin(), snbodys.end(),
		[xx, yy](SnakeBody& item)-> bool { return (xx == item.getx() && yy == item.gety()); });
	if (thisbody != snbodys.end())
	{
		exit(0);
	}
	bool a = isend();
}

void SnakeHead::toNext()
{
	switch (direction)
	{
	case 1:
		dy--;
		break;
	case 2:
		dy++;
		break;
	case 3:
		dx--;
		break;
	case 4:
		dx++;
		break;
	}
}

void SnakeHead::turnUp()
{
	if (direction != 2)
		direction = 1;
}

void SnakeHead::turnDown()
{
	if (direction != 1)
		direction = 2;
}

void SnakeHead::turnLeft()
{
	if (direction != 4)
		direction = 3;
}

void SnakeHead::turnRight()
{
	if (direction != 3)
		direction = 4;
}

int SnakeBody::getx(std::list<SnakeBody>::iterator prebody)
{
	return prebody->dx;
}
int SnakeBody::gety(std::list<SnakeBody>::iterator prebody)
{
	return prebody->dy;
}

//链表连接蛇身,把坐标改为前一节身体的坐标，再刷新界面即可

void SnakeBody::follow()
{
	using namespace std;

	if (ifhead)//头后面的第一节身体
	{
		dx = head->getx();
		dy = head->gety();
	}
	else
	{
		list<SnakeBody>::iterator thisbody = snbodys.begin();
		int xx(dx), yy(dy);
		thisbody = find_if(snbodys.begin(), snbodys.end(),
			[xx, yy](SnakeBody& item)-> bool { return (xx == item.getx()&& yy == item.gety()); });
		try 
		{
			if (thisbody != snbodys.end())
			{
				dx = getlx(--thisbody);
				dy = getly(thisbody);
				setl(thisbody);
			}
			else
				throw("there is no previous snakebody,so it can't move correctly");
		}
		catch(char *err)
		{
			cout << err << endl;
			exit(1);
		}
	}
}
