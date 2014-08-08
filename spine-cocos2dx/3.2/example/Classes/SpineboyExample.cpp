/******************************************************************************
 * Spine Runtimes Software License
 * Version 2.1
 * 
 * Copyright (c) 2013, Esoteric Software
 * All rights reserved.
 * 
 * You are granted a perpetual, non-exclusive, non-sublicensable and
 * non-transferable license to install, execute and perform the Spine Runtimes
 * Software (the "Software") solely for internal use. Without the written
 * permission of Esoteric Software (typically granted by licensing Spine), you
 * may not (a) modify, translate, adapt or otherwise create derivative works,
 * improvements of the Software or develop new applications using the Software
 * or (b) remove, delete, alter or obscure any trademarks or any copyright,
 * trademark, patent or other intellectual property or proprietary rights
 * notices on or in the Software, including any copy thereof. Redistributions
 * in binary or source form must include this license and terms.
 * 
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "SpineboyExample.h"
#include "GoblinsExample.h"
#include <iostream>
#include <fstream>
#include <string.h>

#include <spine/SpineBatchNode.h>

USING_NS_CC;
using namespace spine;
using namespace std;

Scene* SpineboyExample::scene () {
	Scene *scene = Scene::create();
	scene->addChild(SpineboyExample::create());
	return scene;
}

bool SpineboyExample::init () {
	if (!LayerColor::initWithColor(Color4B(128, 128, 128, 255))) return false;

    SpineBatchNode *spineBatch = SpineBatchNode::create();
    
	skeletonNode = SkeletonAnimation::createWithFile("spineboy.json", "spineboy.atlas", 0.6f);
    

	skeletonNode->setStartListener( [this] (int trackIndex) {
		spTrackEntry* entry = spAnimationState_getCurrent(skeletonNode->getState(), trackIndex);
		const char* animationName = (entry && entry->animation) ? entry->animation->name : 0;
		log("%d start: %s", trackIndex, animationName);
	});
	skeletonNode->setEndListener( [] (int trackIndex) {
		log("%d end", trackIndex);
	});
	skeletonNode->setCompleteListener( [] (int trackIndex, int loopCount) {
		log("%d complete: %d", trackIndex, loopCount);
	});
	skeletonNode->setEventListener( [] (int trackIndex, spEvent* event) {
		log("%d event: %s, %d, %f, %s", trackIndex, event->data->name, event->intValue, event->floatValue, event->stringValue);
	});

	skeletonNode->setMix("walk", "jump", 0.2f);
	skeletonNode->setMix("jump", "run", 0.2f);
	skeletonNode->setAnimation(0, "run", true);
//	spTrackEntry* jumpEntry = skeletonNode->addAnimation(0, "jump", false, 3);
	skeletonNode->addAnimation(0, "walk", true);

//	skeletonNode->setTrackStartListener(jumpEntry, [] (int trackIndex) {
//		log("jumped!");
//	});

	// skeletonNode->addAnimation(1, "test", true);
	// skeletonNode->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1), FadeIn::create(1), DelayTime::create(5), NULL)));

	Size windowSize = Director::getInstance()->getWinSize();
	skeletonNode->setPosition(Vec2(100, 30));
    skeletonNode->setScale(1.1);
    spineBatch->setPosition(0, 0);
	addChild(spineBatch);
    spineBatch->addChild(skeletonNode);

//    addChild(skeletonNode);
    
#define MORE_NODE 76
    for (int i=1; i<MORE_NODE; i++) {
        createNode(this, i);
    }
    
	scheduleUpdate();
	
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this] (Touch* touch, Event* event) -> bool {
		if (!skeletonNode->getDebugBonesEnabled())
			skeletonNode->setDebugBonesEnabled(true);
		else if (skeletonNode->getTimeScale() == 1) {
            skeletonNode3->setTimeScale(.5);
            skeletonNode4->setTimeScale(2.5);
			skeletonNode->setTimeScale(1.3f);
        }
		else
			Director::getInstance()->replaceScene(GoblinsExample::scene());
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

//创建更多骨骼对象
void SpineboyExample::createNode(Node *parent, int i) {

    skeletonNode3 = SkeletonAnimation::createWithFile("spineboy.json", "spineboy.atlas", 0.6f);
    
	skeletonNode3->setStartListener( [this] (int trackIndex) {
		spTrackEntry* entry = spAnimationState_getCurrent(skeletonNode3->getState(), trackIndex);
		const char* animationName = (entry && entry->animation) ? entry->animation->name : 0;
		log("%d start: %s", trackIndex, animationName);
	});
	skeletonNode3->setEndListener( [] (int trackIndex) {
		log("%d end", trackIndex);
	});
	skeletonNode3->setCompleteListener( [] (int trackIndex, int loopCount) {
		log("%d complete: %d", trackIndex, loopCount);
	});
	skeletonNode3->setEventListener( [] (int trackIndex, spEvent* event) {
		log("%d event: %s, %d, %f, %s", trackIndex, event->data->name, event->intValue, event->floatValue, event->stringValue);
	});
    
	skeletonNode3->setMix("run", "jump", 0.2f);
	skeletonNode3->setMix("jump", "run", 0.2f);
	skeletonNode3->setAnimation(0, "walk", true);
	spTrackEntry* jumpEntry = skeletonNode3->addAnimation(0, "jump", false, 2*i);
	skeletonNode3->addAnimation(0.2*i, "run", true);
    
	skeletonNode3->setTrackStartListener(jumpEntry, [] (int trackIndex) {
		log("jumped!");
	});
    
	// skeletonNode->addAnimation(1, "test", true);
	// skeletonNode->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1), FadeIn::create(1), DelayTime::create(5), NULL)));
    
	Size windowSize = Director::getInstance()->getWinSize();

	skeletonNode3->setPosition(Vec2(100 + 40*(i%17), 10*(i%5) + 40));
    skeletonNode3->setScale(-0.001*i + 1);
	parent->addChild(skeletonNode3);
}

void SpineboyExample::createNode2(Node *parent, int i) {
    
    skeletonNode4 = SkeletonAnimation::createWithFile("spineboy.json", "spineboy.atlas", 0.6f);
    
	skeletonNode4->setStartListener( [this] (int trackIndex) {
		spTrackEntry* entry = spAnimationState_getCurrent(skeletonNode4->getState(), trackIndex);
		const char* animationName = (entry && entry->animation) ? entry->animation->name : 0;
		log("%d start: %s", trackIndex, animationName);
	});
	skeletonNode4->setEndListener( [] (int trackIndex) {
		log("%d end", trackIndex);
	});
	skeletonNode4->setCompleteListener( [] (int trackIndex, int loopCount) {
		log("%d complete: %d", trackIndex, loopCount);
	});
	skeletonNode4->setEventListener( [] (int trackIndex, spEvent* event) {
		log("%d event: %s, %d, %f, %s", trackIndex, event->data->name, event->intValue, event->floatValue, event->stringValue);
	});
    
	skeletonNode4->setMix("run", "jump", 0.2f);
	skeletonNode4->setMix("jump", "run", 0.2f);
	skeletonNode4->setAnimation(0, "walk", true);
	spTrackEntry* jumpEntry = skeletonNode4->addAnimation(0, "jump", false, 2*2);
	skeletonNode4->addAnimation(0.2*2, "run", true);
    
	skeletonNode4->setTrackStartListener(jumpEntry, [] (int trackIndex) {
		log("jumped!");
	});
    
	// skeletonNode->addAnimation(1, "test", true);
	// skeletonNode->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1), FadeIn::create(1), DelayTime::create(5), NULL)));
    
	Size windowSize = Director::getInstance()->getWinSize();
    //这里设置position没用
	skeletonNode4->setPosition(Vec2(540 + 40*i, 40*i + 120));
	parent->addChild(skeletonNode4);
}

void SpineboyExample::update (float deltaTime) {
	// Test releasing memory.
	// Director::getInstance()->replaceScene(SpineboyExample::scene());
}

