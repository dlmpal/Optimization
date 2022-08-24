

#ifndef GRAPHTSP_HPP
#define GRAPHTSP_HPP

#include <GraphNode.hpp>
#include <vector>

class GraphTSP
{

public:
    GraphTSP(){};
    GraphTSP(int nNodes);

    GraphTSP &operator=(const GraphTSP &rhs);

    ~GraphTSP(){};

    void CopyNodeVertices(double *x, double *y);

    void PrintGraphNodes();

    void SetPath(std::vector<int> Path);

    std::vector<int>& GetPath();

    void PrintPath();

    double GetPathLength(std::vector<int> Path);

    void NearestNeighbours();

    double SA(double StartTemp, double Factor, int max_outer_iters, int max_inner_iters, int verbose, int RenderGraph);

private:
    int nNodes;
    std::vector<GraphNode> GraphNodes;
    std::vector<int> GraphPath;
    int Status = 0;
    std::vector<double> TempHistory;
    std::vector<double> LengthHistory;
};

#endif