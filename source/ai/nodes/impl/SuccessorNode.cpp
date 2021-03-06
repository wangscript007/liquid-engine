#include "SuccessorNode.h"

namespace liquid {
namespace ai {

    SuccessorNode::SuccessorNode() :
        DecoratorNode()
    {
    }

    SuccessorNode::~SuccessorNode()
    {
    }

    bool SuccessorNode::process(BehaviourContext* context)
    {
        if (mChildren.empty() == false)
            mChildren[0]->process(context);

        return true;
    }

}}
