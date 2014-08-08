//
//  SpineBatchNode.h
//  Batching multi Spine sprite.
//
//  Created by Huang Sheng on 8/4/14.
//
//
/**
    usage:
    Layer someLayer = ...
    SpineBatchNode *spineBatch = SpineBatchNode::create();
    someLayer->addChild(spineBatch);
 
    spine::SkeletonAnimation* skeletonNode1 = SkeletonAnimation::createWithFile("someSkeleton.json", "someSkeleton.atlas", 0.6f);
    skeletonNode1->setPosition, and other stuff.....
    spineBatch->addChild(skeletonNode1);
 
     spine::SkeletonAnimation* skeletonNode2 = SkeletonAnimation::createWithFile("someSkeleton.json", "someSkeleton.atlas", 0.6f);
     skeletonNode2->setPosition, and other stuff.....
     spineBatch->addChild(skeletonNode2);
    ...........
    // more nodes
 */
#ifndef __Spine__SpineBatchNode__
#define __Spine__SpineBatchNode__

#include <spine/SkeletonRenderer.h>
#include <spine/SkeletonAnimation.h>

namespace spine {
    
class SpineBatchNode : public SkeletonRenderer {
    
public:
    virtual void update (float deltaTime) override;
	virtual void draw (cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t transformFlags) override;
	virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags) override;
    
    void drawBatchSkeleton (const cocos2d::Mat4& transform, uint32_t transformFlags);
    

    CREATE_FUNC (SpineBatchNode);
};

}

#endif /* defined(__Spine__SpineBatchNode__) */
