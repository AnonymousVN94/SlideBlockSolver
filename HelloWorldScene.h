#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SlideBlock.h"
#include <stack>
#include <set>

const Coor DESTINATION_COOR = Coor(5, 3);
using std::string;

struct Move
{
	Move()
	{

	}
	Move(int id, int distance)
	{
		_id = id;
		_distance = distance;
	}
	Move& operator=(Move &other)
	{
		_id = other._id;
		_distance = other._distance;
		return *this;
	}
	bool operator==(const Move &other)
	{
		return (_id == other._id) && (_distance == other._distance);
	}
	int _id, _distance;
};

class HelloWorld : public cocos2d::Layer
{
private:
	std::vector<Coor> coor;
	std::vector<SlideBlock*> slideblocks;
	std::vector<std::vector<Coor>> originSlideBlocks;
	std::vector<Move> tracePath, paths;
	//std::set<unsigned int> stateMatrix;
	std::set<string> stateMatrixString;
	//std::vector<unsigned int> hashMove;
	std::vector<string> hashMoveString;
	bool found;
	int index;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void resetOriginalMatrix();
	bool checkPathValid();
	void getAllMove(std::vector<Move> &moves);
	void sortBodys();
	void sortMove(std::vector<Move> &moves, std::vector<Move> &his);
	void searchPath();
	void searchPathRecursive();
	void takeAMove();
	void finalOptimizePath();
	void optimizePathFrom(int index_path);
	void minimizePathFrom();
	void goBack();
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

};

#endif // __HELLOWORLD_SCENE_H__
