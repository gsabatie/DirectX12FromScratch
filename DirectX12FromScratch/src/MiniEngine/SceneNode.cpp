#include "MiniEngine/SceneNode.h"

using namespace MiniEngine;

SceneNode::SceneNode(SceneManager &manager) : _manager(manager), _parent(nullptr), _obj(nullptr), _scaling(1.0f, 1.0f, 1.0f)
{}

SceneNode::~SceneNode()
{
    while (_childs.size())
    {
        delete _childs.front();
        _childs.pop_front();
    }
}

SceneNode *SceneNode::createChild()
{
    return (addChild(new SceneNode(_manager)));
}

SceneNode *SceneNode::addChild(SceneNode *node)
{
    _childs.push_back(node);

    node->setParent(this);
    node->updateMatrix();

    return (node);
}

void SceneNode::attachObject(MovableObject *obj)
{
    delete _obj;
    _obj = obj;
}

SceneNode *SceneNode::getParent()
{
    return (_parent);
}

bool SceneNode::render(Camera &camera, CommandList &commandList)
{
    RenderableObject    *object = dynamic_cast<RenderableObject*>(_obj);

    if (object)
    {
        // TO-DO: Set model to world transformation

        if (!object->render(camera, commandList))
            return (false);
    }

    for (auto &&child : _childs)
        if (!child->render(camera, commandList))
            return (false);

    return (true);
}

Matrix4f const &SceneNode::getTransformationMatrix() const
{
    return (_localMatrix);
}

Matrix4f const &SceneNode::getWorldTransformationMatrix() const
{
    return (_worldMatrix);
}

void SceneNode::updateMatrix()
{
    _localMatrix = Matrix4f::createTranslation(_position.x, _position.y, _position.z) * _rotation.transform() * Matrix4f::createScale(_scaling.x, _scaling.y, _scaling.z);

    if (_parent)
        _worldMatrix = _localMatrix * _parent->getWorldTransformationMatrix();
    else
        _worldMatrix = _localMatrix;

    for (auto &&child : _childs)
        child->updateMatrix();

    if (_obj)
        _obj->updateMatrix();
}

void SceneNode::setParent(SceneNode *node)
{
    _parent = node;
}