#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

bool SlideBlock::_matrix[6][6] = {false};

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	if(!Layer::init())
	{
		return false;
	}
	int i, j;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
						   origin.y + closeItem->getContentSize().height / 2));

	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	auto ok = ui::Button::create("ok.png");
	ok->setPosition(Vec2(200, 100));
	ok->addTouchEventListener([&](Ref *ref, ui::Widget::TouchEventType type)
	{
		SlideBlock *sb;
		switch(type)
		{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				TYPE type;
				if(coor[0].x == coor[1].x)
					type = TYPE::VERTICAL;
				else
					type = TYPE::HORIZONTAL;
				sb = SlideBlock::create(coor, type);
				this->addChild(sb, 1);
				slideblocks.push_back(sb);
				coor.clear();
				break;
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				break;
			default:
				break;
		}
	});
	this->addChild(ok);

	auto done = ui::Button::create("done.png");
	done->setPosition(Vec2(340, 100));
	done->addTouchEventListener([&](Ref *ref, ui::Widget::TouchEventType type)
	{
		switch(type)
		{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				log("------------------- searching for path ---------------");
				searchPath();
				break;
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				break;
			default:
				break;
		}
	});
	this->addChild(done);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	for(i = 0; i < 6; ++i)
		for(j = 0; j < 6; ++j)
		{
			auto block = Sprite::create("slide.png");
			block->setPosition(90 * i + 45, 210 + 90 * j + 45);
			this->addChild(block);
		}
	return true;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	Rect r = Rect(0, 210, 540, 540);
	Vec2 touchLocation = touch->getLocation();
	if(r.containsPoint(touchLocation))
	{

	}
	return true;
}

void HelloWorld::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
	Rect r = Rect(0, 210, 540, 540);
	Vec2 touchLocation = touch->getLocation();
	if(r.containsPoint(touchLocation))
	{

	}
}

void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
	Rect r = Rect(0, 210, 540, 540);
	Vec2 touchLocation = touch->getLocation();
	if(r.containsPoint(touchLocation))
	{
		int x, y;
		x = touchLocation.x / 90;
		y = (touchLocation.y - 210) / 90;
		Coor c = Coor(x, y);
		coor.push_back(c);
	}
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::getAllMove(std::vector<Move>& moves)
{
	int i;
	for(i = 0; i < slideblocks.size(); ++i)
	{
		std::vector<int> dis;
		slideblocks[i]->getAllMove(dis);
		for(auto d : dis)
		{
			moves.push_back(Move(i, d));
		}
	}
}

void HelloWorld::sortMove(std::vector<Move> &moves, std::vector<Move> &his)
{
	int i, j;
	bool erase;
	std::vector<Coor> room;
	if(his.size() > 0)
	{
		auto hisMove = his.back();
		for(i = 0; i < moves.size(); ++i)
			if(moves[i]._id == hisMove._id && moves[i]._distance + hisMove._distance == 0)
			{
				moves.erase(moves.begin() + i);
				break;
			}
		slideblocks[hisMove._id]->reverseMove();
		slideblocks[hisMove._id]->getFreeCoorIfMove(room, hisMove._distance);
		slideblocks[hisMove._id]->moveBy(hisMove._distance);
	}
	if(room.size() > 0)
	{
		for(i = 0; i < slideblocks.size(); ++i)
		{
			erase = true;
			for(auto r : room)
				if(slideblocks[i]->possibleReachTo(r) && i != his.back()._id)
				{
					erase = false;
					break;
				}
			if(erase)
			{
				for(j = 0; j < moves.size(); ++j)
					if(moves[j]._id == i)
					{
						moves.erase(moves.begin() + j);
						--j;
					}
			}
		}
	}
	//for(j = 0; j < moves.size(); ++j)
	//{
	//	std::vector<Coor> coor;
	//	Move move = moves[j];
	//	erase = true;
	//	slideblocks[move._id]->getFreeCoorIfMove(coor, move._distance);
	//	slideblocks[move._id]->moveBy(move._distance);
	//	for(auto c : coor)
	//	{
	//		for(i = 0; i < slideblocks.size(); ++i)
	//			if(i != move._id && slideblocks[i]->possibleReachTo(c))
	//			{
	//				erase = false;
	//				break;
	//			}
	//		if(!erase)
	//			break;
	//	}
	//	slideblocks[move._id]->reverseMove();
	//	if(erase)
	//	{
	//		moves.erase(moves.begin() + j);
	//		--j;
	//	}
	//}
}

void HelloWorld::searchPath()
{
	std::vector<Move> allMoves, moves;
	std::vector<Move> paths;
	int i;
	bool found = false;
	while(true)
	{
		if(slideblocks.back()->possibleReachTo(DESTINATION_COOR))
		{
			log("------------- found path --------------");
			found = true;
			break;
		}
		moves.clear();
		getAllMove(moves);
		sortMove(moves, paths);
		if(moves.empty())
		{
			Move his = paths.back();
			paths.pop_back();
			slideblocks[his._id]->reverseMove();
		}
		else
		{
			allMoves.insert(allMoves.end(), moves.begin(), moves.end());
		}
		if(allMoves.empty())
		{
			log("------------- out of move -------------");
			break;
		}
		Move move = allMoves.back();
		allMoves.pop_back();
		paths.push_back(move);
		slideblocks[move._id]->reverseMove();
		slideblocks[move._id]->moveBy(move._distance);
	}
	for(i = paths.size() - 1; i >= 0; --i)
	{
		auto path = paths[i];
		slideblocks[path._id]->reverseMove();
	}
	if(found)
	{
		for(auto path : paths)
		{
			log("---- %d, %d ----->", path._id, path._distance);
			//slideblocks[path._id]->moveBy(path._distance);
			//slideblocks[path._id]->refreshPosition();
		}
	}
}

void HelloWorld::searchPathRecursive()
{
	std::vector<Move> moves;
	getAllMove(moves);
	sortMove(moves, tracePath);
	while(!moves.empty())
	{
		if(slideblocks.back()->possibleReachTo(DESTINATION_COOR))
		{
			found = true;
		}
		if(found)
		{
			break;
		}
		Move move = moves.back();
		moves.pop_back();
		tracePath.push_back(move);
		slideblocks[move._id]->moveBy(move._distance);
		searchPathRecursive();
		slideblocks[move._id]->reverseMove();
		if(!found)
			tracePath.pop_back();
	}
}