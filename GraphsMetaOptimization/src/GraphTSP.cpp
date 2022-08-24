#include <GraphTSP.hpp>

#include <vector>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <random>

GraphTSP::GraphTSP(int nNodes)
    : nNodes(nNodes)
{
    GraphNodes.reserve(nNodes);
    GraphPath.reserve(nNodes);
    for(int i = 0 ; i<nNodes ;i++){
        GraphPath.push_back(i);
    }
    printf("Created TSP problem with %d Nodes\n", nNodes);
};
GraphTSP &GraphTSP::operator=(const GraphTSP &rhs)
{
    this->nNodes = rhs.nNodes;

    for (int i = 0; i < nNodes; i++)
    {
        this->GraphNodes.push_back(rhs.GraphNodes[i]);
        this->GraphPath.push_back(rhs.GraphPath[i]);
    }

    return *this;
};

void GraphTSP::CopyNodeVertices(double *x, double *y)
{
    for (int i = 0; i < nNodes; i++)
    {

        GraphNodes[i].SetNodeVertices(x[i], y[i]);
    }
    Status = 1;
};

void GraphTSP::PrintGraphNodes()
{

    if (Status == 0)
    {
        printf("Graph not initiliazed!\n");
        return;
    }
    for (int i = 0; i < nNodes; i++)
    {
        GraphNodes[i].PrintNode(i);
    }
}

void GraphTSP::SetPath(std::vector<int> Path)
{

    GraphPath = Path;
}
std::vector<int>& GraphTSP::GetPath(){

return GraphPath;

};
void GraphTSP::PrintPath()
{

    for (int i = 0; i < nNodes - 1; i++)
    {
        printf("%d->", GraphPath[i]);
    }
    printf("%d\n", GraphPath[nNodes - 1]);
}

double GraphTSP::GetPathLength(std::vector<int> Path)
{
    double result = 0;
    for (int i = 0; i < nNodes - 1; i++)
    {

        result += GraphNodes[Path[i]].GetEuclideanDistance(GraphNodes[Path[i + 1]]);
    }
    result += GraphNodes[Path[nNodes - 1]].GetEuclideanDistance(GraphNodes[Path[0]]);
    return result;
}
void GraphTSP::NearestNeighbours()
{
    static int PrintStatus = 0 ; 
    double NodeDistanceMatrix[nNodes][nNodes];
    int NodeStatus[nNodes]; // 1 if in the path , else 0
    memset(NodeStatus, 0, sizeof(NodeStatus));
    std::vector<int> NNPath;
    NNPath.reserve(nNodes);

    for (int i = 0; i < nNodes; i++)
    {
        for (int j = 0; j < nNodes; j++)
        {
            NodeDistanceMatrix[i][j] = GraphNodes[i].GetEuclideanDistance(GraphNodes[j]);
        }
    }

    for (int i = 0; i < nNodes; i++)
    {
        NodeDistanceMatrix[i][i] = std::numeric_limits<double>::max();
    }

    NNPath.push_back(0);
    NodeStatus[0] = 1;
    int NearestNode;
    double TempLength;
    // double NNTotalLength = 0;

    for (int i = 0; i < nNodes - 1; i++)
    {
        double NearestLength = NodeDistanceMatrix[i][i]; // std::numeric_limits<double>::max();
        for (int j = 0; j < nNodes; j++)
        {
            TempLength = NodeDistanceMatrix[NNPath[i]][j];
            if (TempLength < NearestLength)
            {
                if (NodeStatus[j] != 1)
                {
                    NearestNode = j;
                    NearestLength = TempLength;
                }
            }
        }
        NNPath.push_back(NearestNode);
        NodeStatus[NearestNode] = 1;
    }
    GraphPath = NNPath;
    if(PrintStatus == 0){
    PrintStatus = 1; 
    printf("Nearest Neighbours Path Length = %lf\n", GetPathLength(NNPath));
    }
}

double GraphTSP::SA(double StartTemp, double Factor, int max_outer_iters, int max_inner_iters, int verbose, int RenderGraph)
{
    // Render init
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    TTF_Font *NodeFont;
    int delay = 20;
    if (RenderGraph)
    {
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();
        NodeFont = TTF_OpenFont("OpenSans-Bold.ttf", 25);
        SDL_CreateWindowAndRenderer(1000, 1000, 0, &window, &renderer);
        SDL_SetWindowTitle(window, "Graph Network");
    }
    // Simulated Annealing

    NearestNeighbours();
    double UpperLengthBound = GetPathLength(GraphPath);
    int RestartCount = 0;
    int iter_data[3];
    double T = StartTemp;
    double TempPathLength, GraphPathLength, p;
    int iswp1, iswp2, temp;
    std::vector<int> TempPath;
    TempPath.reserve(nNodes);

    // print every...
    int PrintEvery1P = (int)max_outer_iters / 100;

    for (int i = 0; i < max_outer_iters; i++)
    {

        GraphPathLength = GetPathLength(GraphPath);
        LengthHistory.push_back(GraphPathLength);
        TempHistory.push_back(T);

        if (verbose > 0 && (i % PrintEvery1P == 0))
        {
            printf("Iter %d Length: %lf Temp: %lf\n", i, GraphPathLength, T);
        }
        if (verbose > 1 && (i % PrintEvery1P == 0))
        {
            PrintPath();
        }

        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_real_distribution<double> dist_p(0, 1);
        std::uniform_int_distribution<int> dist_indswap(0, nNodes - 1);

        for (int j = 0; j < max_inner_iters; j++)
        {
            TempPath = GraphPath;
            iswp1 = dist_indswap(generator);
            iswp2 = dist_indswap(generator);
            while (iswp1 == iswp2)
            {
                iswp2 = dist_indswap(generator);
            }
            temp = TempPath[iswp1];
            TempPath[iswp1] = TempPath[iswp2];
            TempPath[iswp2] = temp;
            TempPathLength = GetPathLength(TempPath);

            if (TempPathLength <= GraphPathLength)
            {
                GraphPath = TempPath;
                GraphPathLength = TempPathLength;
                continue;
            }
            p = dist_p(generator);
            if (p <= exp(-(TempPathLength - GraphPathLength) / T)) // if T>> , then exp(-DE/T)>>
            {
                GraphPath = TempPath;
                GraphPathLength = TempPathLength;
            }
        }
        T = T * Factor;

        if (T <= 0.6 && GraphPathLength >= UpperLengthBound)
        {
            RestartCount++;
            // T = StartTemp/(RestartCount+1);
        }

        if (RenderGraph && i % PrintEvery1P == 0)
        {
            iter_data[0] = i;
            iter_data[1] = (int)T;
            iter_data[2] = (int)GraphPathLength;
            // RenderGraphWindow(renderer, NodeFont, delay, iter_data);
        }
    }
    // end of SA loop
    if (RenderGraph)
    {
        iter_data[0] = max_outer_iters - 1;
        iter_data[1] = (int)T;
        iter_data[2] = (int)GraphPathLength;
        // RenderGraphWindow(renderer, NodeFont, 1000 * 10, iter_data);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    //printf("Termination with path length: %lf\n", GraphPathLength);
    return GraphPathLength;
}
