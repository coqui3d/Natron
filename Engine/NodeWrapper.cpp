//  Natron
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
 * Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012.
 * contact: immarespond at gmail dot com
 *
 */


#include "NodeWrapper.h"
#include "Engine/Node.h"

Effect::Effect(const boost::shared_ptr<Natron::Node>& node)
: _node(node)
{

}

int
Effect::getMaxInputCount() const
{
    return _node->getMaxInputCount();
}

bool
Effect::canSetInput(int inputNumber,const Effect& node) const
{

    ///Input already connected
    if (_node->getInput(inputNumber)) {
        return false;
    }
    
    ///No-one is allowed to connect to the other node
    if (!node._node->canOthersConnectToThisNode()) {
        return false;
    }
    
    ///Applying this connection would create cycles in the graph
    if (!_node->checkIfConnectingInputIsOk(node._node.get())) {
        return false;
    }
    
    ///Ok, following connection would be clean.
    return true;
}

bool
Effect::connectInput(int inputNumber,const Effect& input)
{
    if (canSetInput(inputNumber, input)) {
        return _node->connectInput(input._node, inputNumber);
    } else {
        return false;
    }
}

void
Effect::disconnectInput(int inputNumber)
{
    _node->disconnectInput(inputNumber);
}

Effect
Effect::getInput(int inputNumber) const
{
    boost::shared_ptr<Natron::Node> node = _node->getInput(inputNumber);
    return Effect(node);
}

std::string
Effect::getName() const
{
    return _node->getName_mt_safe();
}


std::string
Effect::getPluginID() const
{
    return _node->getPluginID();
}