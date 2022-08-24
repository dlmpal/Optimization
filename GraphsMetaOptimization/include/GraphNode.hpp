#ifndef GRAPHNODE_HPP
#define GRAPHNODE_HPP

class GraphNode
{
public:
    GraphNode();
    GraphNode(double x, double y, int NodeID);
    double GetEuclideanDistance(const GraphNode &Node2);
    void SetNodeVertices(double x, double y);
    void PrintNode(int ID);
private: 
    double x , y; 
    int NodeID; 

};

#endif