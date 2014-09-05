
## spine使用中碰到的问题
### spine单个精灵是batch的，多个同样的精灵，是不会batch的
 单个精灵可能由很多部件组成，spine runtime实现了对单个精灵的batch。而多个同样的精灵，则不会batch。即：1个不管多复杂的精灵是1次draw call，10个同样的精灵是10次draw call。对场景中有大量小怪的情况，这将是个问题。


### cocos2dx（以下简称c2dx）3.x不是实现了自动batch了吗？
Renderer类只实现了对“QUAD_COMMAND”这个类型的渲染command做batch，而spine使用的是“CustomCommand”，即渲染是自己实现的。

### spine的渲染实现2个关键类“SkeletonRenderer”和“PolygonBatch”
“SkeletonRenderer”负责计算每个部件的顶点，uv，颜色信息，提交给“PolygonBatch”进行draw。所以单个精灵是batch的。

### 方案
实现一个类似于SpriteBatchNode的东西，命名SpineBatchNode。相同spine的精灵加入到该Node作为子节点，即可自动实现batch。

### 性能测试（iOS）
* 修改`SpineboyExample.cpp`的`USE_BATCH`，1为使用batch，0为不batch
* 修改`PolygonBatch.h`的`USE_VBO`，1为使用vbo，0为不使用vbo
#### 原来没有batch和vbo性能
使用iPhone5s，700个Node，fps=33
#### 使用batch和vbo性能
使用iPhone5s，700个Node，fps=52

可以提升50%+的`渲染`效率。
