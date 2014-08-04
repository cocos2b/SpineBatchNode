//
//  SpineBatchNode.h
//  Spine
//
//  Created by Huang Sheng on 8/4/14.
//
//

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
    
protected:
    std::vector<SkeletonAnimation*> allChildren;
};

}

#endif /* defined(__Spine__SpineBatchNode__) */
