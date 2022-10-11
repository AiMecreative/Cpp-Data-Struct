#include "Node.h"
#include <iostream>
#include <utility>

Node::Node(Node* parent, Action action, int cost, std::vector<int> state)
{
	this->parent = parent;
	this->action = action;
	this->cost = cost;
	this->state = std::move(state);
}

Node::Node()
{
	this->parent = nullptr;
	this->action = Action::FAILURE;
	this->cost = -1;
	this->state = std::vector<int>();
}

Node::~Node() = default;

bool Node::cmp::operator()(Node* a, Node* b) const
{
	return a->cost > b->cost;
}
