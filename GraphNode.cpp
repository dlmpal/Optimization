#include <GraphNode.hpp>

#include <math.h>
#include <stdio.h>


GraphNode::GraphNode()
{
    x = 0.0;
    y = 0.0;
    NodeID = -1;
};

GraphNode::GraphNode(double x, double y, int ID)
    : x(x), y(y), NodeID(ID)
{

    // printf("Node %d , x: %lf , y: %lf\n", NodeID, x, y);
}

double GraphNode::GetEuclideanDistance(const GraphNode &Node2)
{
    return sqrt(pow(this->x - Node2.x, 2) + pow(this->y - Node2.y, 2));
}
void GraphNode::SetNodeVertices(double x, double y)
{
    this->x = x;
    this->y = y;
}
void GraphNode::PrintNode(int ID)
{
    printf("Node %d , x: %lf , y: %lf\n", ID, x, y);
}
