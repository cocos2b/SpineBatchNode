//
//  SpineBatchNode.cpp
//  Spine
//
//  Created by build on 8/4/14.
//
//

#include "SpineBatchNode.h"
#include <spine/PolygonBatch.h>

USING_NS_CC;

namespace spine {
    
//void SpineBatchNode::addChild(SkeletonAnimation* child) {
//    this->allChildren.push_back(child);
//}
    
void SpineBatchNode::update (float deltaTime) {
    //spSkeleton_update(_skeleton, deltaTime * _timeScale);
}

void SpineBatchNode::draw (Renderer* renderer, const Mat4& transform, uint32_t transformFlags) {
    _drawCommand.init(_globalZOrder);
    _drawCommand.func = CC_CALLBACK_0(SpineBatchNode::drawBatchSkeleton, this, transform, transformFlags);
    renderer->addCommand(&_drawCommand);
}

void SpineBatchNode::drawBatchSkeleton (const Mat4 &transform, uint32_t transformFlags) {
    getGLProgramState()->apply(transform);
    
    for (auto child : _children) {
        SkeletonAnimation *skeletonNode = static_cast<SkeletonAnimation*>(child);
        if (skeletonNode) {
            skeletonNode->drawSkeletonToBatch(_batch);
        }
    }
    _batch->flush();
}

// override visit
// don't call visit on it's children
void SpineBatchNode::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    
    // CAREFUL:
    // This visit is almost identical to CocosNode#visit
    // with the exception that it doesn't call visit on it's children
    //
    // The alternative is to have a void Sprite#visit, but
    // although this is less maintainable, is faster
    //
    if (! _visible)
    {
        return;
    }
    
    sortAllChildren();
    
    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    
    // IMPORTANT:
    // To ease the migration to v3.0, we still support the Mat4 stack,
    // but it is deprecated and your code should not rely on it
    Director* director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
    
    draw(renderer, _modelViewTransform, flags);
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    // FIX ME: Why need to set _orderOfArrival to 0??
    // Please refer to https://github.com/cocos2d/cocos2d-x/pull/6920
    //    setOrderOfArrival(0);
    
    CC_PROFILER_STOP_CATEGORY(kProfilerCategoryBatchSprite, "CCSpriteBatchNode - visit");
}

}