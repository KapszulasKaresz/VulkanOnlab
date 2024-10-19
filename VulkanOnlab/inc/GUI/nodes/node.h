#pragma once
#include "imnodes.h"
#include <map>
#include <string>
#include <utility>

struct Node {
    Node(int id)
        : nodeId(id)
    {
    }
    virtual void draw() = 0;
    int getId() const { return nodeId; }
    void setPosition(const ImVec2& pos) { ImNodes::SetNodeGridSpacePos(getId(), pos);}
    virtual std::string getOutputShaderCode(int ouputId) { return ""; };
    virtual std::string getFunctionDefinitions() { return std::string(); }

    std::map<int, std::pair<Node*, int>> inputs;

    virtual ~Node() {};

protected:
    int nodeId;
};