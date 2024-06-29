#pragma once
#include <map>
#include <utility>
#include <string>

struct Node {
	Node(int id) : nodeId(id) {}
	virtual void draw() = 0;
	int getId() const { return nodeId;}
	virtual std::string getOutputShaderCode(int ouputId) { return ""; };

	std::map<int, std::pair<Node*, int>> inputs;

	virtual ~Node() {};
protected:
	int nodeId;
};