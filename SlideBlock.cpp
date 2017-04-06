#include "SlideBlock.h"

int SlideBlock::numberOfBlock = 0;

SlideBlock::SlideBlock(std::vector<Coor>& coor, TYPE type)
{
	_lenght = coor.size();
	_type = type;
	_id = numberOfBlock++;
	bodyCoor = coor;
	for(auto c : coor)
	{
		SlideBlock::_matrix[c.x][c.y] = true;
	}
}

SlideBlock * SlideBlock::create(std::vector<Coor>& coor, TYPE type)
{
	SlideBlock *ret = new (std::nothrow) SlideBlock(coor, type);
	if(ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool SlideBlock::init()
{
	for(auto c : bodyCoor)
	{
		auto body = Sprite::create("body.png");
		body->setPosition(c.x * 90 + 45, 210 + c.y * 90 + 45);
		auto label = Label::createWithTTF(std::to_string(_id), "fonts/Marker Felt.ttf", 30);
		label->setPosition(body->getContentSize() / 2);
		body->addChild(label);
		this->addChild(body);
		bodySprite.push_back(body);
	}
	return true;
}

bool SlideBlock::isValidPosition(const Coor &coor)
{
	return (coor.x >= 0 && coor.x < 6) && (coor.y >= 0 && coor.y < 6);
}

bool SlideBlock::isValidPosition(int x, int y)
{
	return (x >= 0 && x < 6) && (y >= 0 && y < 6);
}

bool SlideBlock::isFreePosition(const Coor &coor)
{
	return !_matrix[coor.x][coor.y];
}

bool SlideBlock::isFreePosition(int x, int y)
{
	return !_matrix[x][y];
}

bool SlideBlock::isValidMove(int distance)
{
	int d = 0;
	Coor coor, offset;
	if(distance > 0)
	{
		if(_type == TYPE::VERTICAL)
		{
			offset.x = 0;
			offset.y = 1;
		}
		else
		{
			offset.x = 1;
			offset.y = 0;
		}
		coor = bodyCoor.back();
	}
	else
	{
		if(_type == TYPE::VERTICAL)
		{
			offset.x = 0;
			offset.y = -1;
		}
		else
		{
			offset.x = -1;
			offset.y = 0;
		}
		coor = bodyCoor.front();
	}
	coor += offset;
	while(isValidPosition(coor) && isFreePosition(coor))
	{
		++d;
		coor += offset;
		if(d == ABS(distance))
			return true;
	}
	return false;
}

void SlideBlock::moveBy(int distance)
{
	Coor offset;
	if(_type == TYPE::VERTICAL)
	{
		offset = Coor(0, distance);
	}
	else
	{
		offset = Coor(distance, 0);
	}
	for(auto c : bodyCoor)
	{
		SlideBlock::_matrix[c.x][c.y] = false;
	}
	for(auto &c : bodyCoor)
	{
		c += offset;
		SlideBlock::_matrix[c.x][c.y] = true;
	}
	trace.push(distance);
}

void SlideBlock::refreshPosition()
{
	int i;
	for(i = 0; i < _lenght; ++i)
	{
		bodySprite[i]->setPosition(bodyCoor[i].x * 90 + 45, 210 + bodyCoor[i].y * 90 + 45);
	}
}

void SlideBlock::reverseMove()
{
	if(trace.empty())
		return;
	int v = trace.top();
	trace.pop();
	moveBy(-v);
}

bool SlideBlock::possibleReachTo(Coor coor)
{
	if(!isFreePosition(coor))
		return false;
	if(_type == TYPE::VERTICAL)
	{
		if(bodyCoor.front().x != coor.x)
			return false;
		return coor.y < bodyCoor.front().y ? isValidMove(coor.y - bodyCoor.front().y) : isValidMove(coor.y - bodyCoor.back().y);
	}
	else
	{
		if(bodyCoor.front().y != coor.y)
			return false;
		return coor.x < bodyCoor.front().x ? isValidMove(coor.x - bodyCoor.front().x) : isValidMove(coor.x - bodyCoor.back().x);
	}
}

void SlideBlock::getAllMove(std::vector<int>& move)
{
	int i;
	for(i = _lenght - 6; i <= 6 - _lenght; ++i)
		if(isValidMove(i))
			move.push_back(i);

}

void SlideBlock::getFreeCoorIfMove(std::vector<Coor>& coor, int distance)
{
	int i, n = ABS(distance) <= _lenght ? ABS(distance) : _lenght;
	if(distance > 0)
		for(i = 0; i < n; ++i)
			coor.push_back(bodyCoor[i]);
	else
		for(i = _lenght - n; i < _lenght; ++i)
			coor.push_back(bodyCoor[i]);
}

void SlideBlock::logdebug()
{
	int i;
	log("===================================");
	for(i = 0; i < _lenght; ++i)
		log("(%d, %d)", bodyCoor[i].x, bodyCoor[i].y);
	log("+++++++++++++++++++++++++++++++++++");
}

SlideBlock::~SlideBlock()
{
}
