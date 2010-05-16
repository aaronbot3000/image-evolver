#ifndef SHAPELISTNODE_H
#define SHAPELISTNODE_H

class ShapeListNode
{
    public:
        ShapeListNode(){}
        ShapeListNode(ShapeListNode *p, ShapeListNode *n, MutableColorShape *v);
        ShapeListNode *prev, *next, *value;
};

#endif // SHAPELISTNODE_H
