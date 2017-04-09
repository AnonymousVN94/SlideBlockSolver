#ifndef SLIDE_BLOCK_H
#define SLIDE_BLOCK_H

#include "cocos2d.h"
#include <stack>

USING_NS_CC;

#define ABS(X) ((X) > 0 ? (X) : -(X))

enum class TYPE
{
	VERTICAL,
	HORIZONTAL
};

enum class GO
{
	BACKWARD,
	FORWARD
};


struct Coor
{
	int x, y;
	Coor()
	{
		x = y = 0;
	}
	Coor(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	bool operator==(Coor &coor)
	{
		return (x == coor.x) && (y == coor.y);
	}
	Coor operator+(const Coor &coor)
	{
		return Coor(x + coor.x, y + coor.y);
	}
	Coor& operator+=(const Coor &coor)
	{
		x += coor.x;
		y += coor.y;
		return *this;
	}
};

class SlideBlock : public Layer
{
private:
	int _lenght;
	TYPE _type;
	std::vector<Coor> bodyCoor;
	std::vector<Sprite*> bodySprite;
	std::stack<int> trace;
	int _id;
	static int numberOfBlock;
	static bool _matrix[6][6];
public:
	SlideBlock(std::vector<Coor> &coor, TYPE type);
	static SlideBlock* create(std::vector<Coor> &coor, TYPE type);
	static unsigned int hashCurrentMatrix();
	virtual bool init();
	bool possibleReachTo(Coor coor);
	void getAllMove(std::vector<int> &move);
	void getFreeCoorIfMove(std::vector<Coor> &coor, int distance);
	bool inline isValidPosition(const Coor &coor);
	bool inline isValidPosition(int x, int y);
	bool inline isFreePosition(const Coor &coor);
	bool inline isFreePosition(int x, int y);
	bool isValidMove(int distance);
	void moveBy(int distance);
	void reverseMove();
	void refreshPosition(std::function<void()> cbOnFinish);
	void logdebug();
	~SlideBlock();
};

#endif // !SLIDE_BLOCK_H
