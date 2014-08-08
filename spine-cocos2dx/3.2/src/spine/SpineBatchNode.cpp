//
//  SpineBatchNode.cpp
//  Spine
//
//  Created by Huang Sheng on 8/4/14.
//
//

#include "SpineBatchNode.h"
#include <spine/PolygonBatch.h>

USING_NS_CC;

namespace spine {
    
void SpineBatchNode::update (float deltaTime) {
}

void SpineBatchNode::draw (Renderer* renderer, const Mat4& transform, uint32_t transformFlags) {
    _drawCommand.init(_globalZOrder);
    _drawCommand.func = CC_CALLBACK_0(SpineBatchNode::drawBatchSkeleton, this, transform, transformFlags);
    renderer->addCommand(&_drawCommand);
}

// draw all children to batch. Recursive iteration not support yet.
void SpineBatchNode::drawBatchSkeleton (const Mat4 &transform, uint32_t transformFlags) {
    getGLProgramState()->apply(transform);
    for (auto child : _children) {
        SkeletonAnimation *skeletonNode = static_cast<SkeletonAnimation*>(child);
        if (skeletonNode) {
            skeletonNode->batchDrawSkeleton(_batch);
        }
    }
    _batch->flush();
}

// override visit（copy from SpriteBatchNode）
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
    
    draw(renderer, _modelViewTransform, flags);
}

}