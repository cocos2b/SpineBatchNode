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
    
#define NODE_COUNT 150
#define USE_BATCH 1
    
#if USE_BATCH
    spineBatch = SpineBatchNode::create();
    spineBatch->setPosition(0, 0);
	addChild(spineBatch);
#endif
    
    for (int i = 1; i <= NODE_COUNT; i ++) {
#if USE_BATCH
        createNode(spineBatch, i);
#else
        createNode(this, i);
#endif
    }
    
	scheduleUpdate();

	return true;
}

//创建更多骨骼对象
void SpineboyExample::createNode(Node *parent, int i) {

    skeletonNode = SkeletonAnimation::createWithFile("bat.json", "bat.atlas", 0.6f);
    
    skeletonNode->setMix("idle", "stirke", 0.2f);
	skeletonNode->setAnimation(0, "idle", true);
	skeletonNode->addAnimation(0, "stirke", false, 2*i);
	skeletonNode->addAnimation(0.2*i, "idle", true);
    
	skeletonNode->setPosition(Vec2(70 + 45*(i%17), 60*(i%8) + 70));
	parent->addChild(skeletonNode);
}

void SpineboyExample::update (float deltaTime) {
	// Test releasing memory.
	// Director::getInstance()->replaceScene(SpineboyExample::scene());
}

