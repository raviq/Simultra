//
// Graph and path computations for routing logic
//
#include "graph.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <QList>

using namespace std;

Graph::Graph()
{
    bi_direction = true;
}

Graph::~Graph()
{
    reset();
}

// reset all the graph data
void Graph::reset()
{
  // delete the link pointers
  for ( map< pair<int,int>, Link*>::iterator it = link_map.begin(); it != link_map.end();  it++ )
  {
      Link* l = (*it).second;
      delete l;
      l = NULL;
  }
  link_map.erase ( link_map.begin(), link_map.end() );
}

void Graph::addLink( Link* l )
{
  // update one-to-one mapping between each link added and its unique node pair
  int s, d;
  l->GetSourceTargetNodes( s, d );
  pair<int,int> p( s, d );
  link_map[ p ] = l;
}

void Graph::addLink(int s, int d )
{
  if (!linkExists(s, d))
      addLink( new Link( s, d ) );
}

bool Graph::linkExists( int sourceID, int targetID )
{
  bool found = false;
  map< pair<int,int>, Link* >::iterator it1, it2;
  pair<int,int> p1( sourceID, targetID );
  pair<int,int> p2( targetID, sourceID );

  it1 = link_map.find( p1 );

  // Condition for bi-directional (full duplex) links
  if ( BiDirectional() )
  {
      it2 = link_map.find( p2 );
      found = it1 != link_map.end() || it2 != link_map.end();
  }
  // Otherwise directed links
  else
  {
      found =  it1 != link_map.end();
  }

  return found;
}

map<pair<int,int>, Link*> Graph::getLinkMap()
{
    return link_map;
}

vector< int > Graph::getAdjNodeIDs( int n )
{
  vector< int > adj;
  map< pair<int,int>, Link* >::iterator link_it = link_map.begin();
  for ( ; link_it != link_map.end(); link_it++ )
  {
      int start  = -1, target = -1;
      Link* l = (*link_it).second;
      l->GetSourceTargetNodes(start, target);

      if (linkExists(start, target))
      {
          if ( BiDirectional() && ( start == n || target == n ) )
          {
              adj.push_back( start == n ? target : start );
          }
          else if (start == n)
          {
              adj.push_back( target );
          }
      }
  }
  return adj;
}

bool Graph::BiDirectional()
{
  return bi_direction;
}

// Set bi-directional status of Graph links
void Graph::setBiDirection(bool bi)
{
  bi_direction = bi;
}

// Recursively search for paths (Depth-First Search Algorithm)

void Graph::depthFirst(vector<int>& visited, int end)
{
    int back = visited.back();
    vector< int > adjNode = this->getAdjNodeIDs( back );

    // check adjacent nodes
    for ( vector<int>::iterator node_it = adjNode.begin(); node_it != adjNode.end(); node_it++ )
    {
        int node = (*node_it);

        if(find(visited.begin(), visited.end(), node) != visited.end())
            continue;

        if ( node == end )
        {
            visited.push_back(*node_it);
            int hops = (int) visited.size();
           // cout << "hops=" << hops << " \t";
            QList<int> * l = new QList<int>;
            for ( int i = 0; i < hops; i++ )
            {
               // cout << visited[i] << "  ";
                l->append(visited[i]);
            }
            list.append(l);
           // cout << endl;
            int n = (int) visited.size() - 1;
            visited.erase( visited.begin() + n );
            break;
        }
    }
    // in BF recursion needs to come after visiting adjacent nodes
    for ( vector<int>::iterator node_it = adjNode.begin(); node_it != adjNode.end(); node_it++)
    {
        int node = (*node_it);
        if (  find(visited.begin(), visited.end(), node) != visited.end() || node == end )
            continue;
        visited.push_back(node);
        depthFirst(visited, end);
        int n = (int) visited.size() - 1;
        visited.erase( visited.begin() + n );
    }
}

QList<QList<int> *> Graph::getPaths()
{
    return list;
}

void Graph::graph_test()
{
    Graph* g = new Graph();
    g->setBiDirection(false);
    g->addLink(1, 2);
    g->addLink(1, 3);
    g->addLink(1, 4);
    g->addLink(2, 1);
    g->addLink(2, 4);
    g->addLink(2, 3);
    g->addLink(3, 5);
    g->addLink(4, 5);
    g->addLink(5, 3);

    int start  = 2, target = 5;
    vector<int> visited;
    visited.push_back(start);
    g->depthFirst(visited, target);
}


