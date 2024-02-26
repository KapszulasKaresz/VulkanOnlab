#pragma once

struct Node {
	Node(int id) : nodeId(id) {}
	virtual void draw() = 0;
	int getId() const { return nodeId;}

	virtual ~Node() {};
private:
	int nodeId;
};