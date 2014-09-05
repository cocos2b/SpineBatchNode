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
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        _batch->setupVBOAndVAO();
    }
    
    for (auto child : _children) {
        SkeletonAnimation *skeletonNode = static_cast<SkeletonAnimation*>(child);
        if (skeletonNode) {
            skeletonNode->batchDrawSkeleton(_batch);
        }
    }
    _batch->flush();
}

// override visit
// don't call visit on it's children
void SpineBatchNode::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    if (! _visible)
    {
        return;
    }
    
    sortAllChildren();
    
    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    
    draw(renderer, _modelViewTransform, flags);
}

}