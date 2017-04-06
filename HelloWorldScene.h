#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SlideBlock.h"
#include <stack>

const Coor DESTINATION_COOR = Coor(5, 3);

struct Move
{
	Move(int id, int distance)
	{
		_id = id;
		_distance = distance;
	}
	int _id, _distance;
};

class HelloWorld : public cocos2d::Layer
{
private:
	std::vector<Coor> coor;
	std::vector<SlideBlock*> slideblocks;
	std::stack<int> historyMove;
	std::vector<Move> tracePath;
	bool found;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void getAllMove(std::vector<Move> &moves);
	void sortMove(std::vector<Move> &moves, std::vector<Move> &his);
	void searchPath();
	void searchPathRecursive();
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

};

#endif // __HELLOWORLD_SCENE_H__
