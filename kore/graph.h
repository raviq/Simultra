#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <QList>

using namespace std;

class Link
{
   public:
    Link( int src, int dest )
    {
        sourceNode = src;
        targetNode = dest;
    }
    void GetSourceTargetNodes( int & src, int & dest )
    {
        src = sourceNode;
        dest = targetNode;
    }
    protected:
        int sourceNode;
        int targetNode;
};

class Graph
{
    private:
        map<pair<int,int>, Link*> link_map;
        bool bi_direction;
        QList<QList<int> *> list; // paths

    public:
        Graph();
        ~Graph();

        void depthFirst(vector<int>& visited, int end);
        QList<QList<int> *> getPaths();

        void reset();
        void setBiDirection( bool bi );
        void addLink( Link* l );
        void addLink( int s, int d );
        bool linkExists( int sourceID, int targetID );
        bool BiDirectional();
        vector< int > getAdjNodeIDs( int n );
        map<pair<int,int>, Link*> getLinkMap();

        void graph_test();

};

#endif // GRAPH_H
