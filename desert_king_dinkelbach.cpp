#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<cmath>
#include<vector>
#include<queue>
#include<iostream>
#include<vector>
#include<sstream>
#include<fstream>
using namespace std;
const double INF = 1E6;
const double EPS=1E-6;

typedef struct Village
{
    int x,y,z;
} Village;

typedef struct Channel
{
    int srcID,destID;
    double dist,altitude,cost;
} Channel;

typedef pair<int,double> village_cost_pair;

typedef struct minHeapComp {
    int operator()(
        pair<int,double> const& a,
        pair<int,double> const& b)
    {
        return a.second > b.second;
    }
} minHeapComp;
int V;
// Initialization -- Construct Adjacency Matrix
vector<vector<Channel>> channelMap;

double primMST(double r,priority_queue<village_cost_pair,vector<village_cost_pair>, minHeapComp> &pq, vector<int> &keys,vector<int> &prev_country,vector<bool> &inMST)
{
    /* Looping till priority queue becomes empty */
    while (!pq.empty()) {
        int villageID = pq.top().first;
        pq.pop();
        
        
        if (inMST[villageID] == true) {
            continue;
        }
    
        inMST[villageID] = true;  // Include vertex in MST

        // Iterate through all neighbors of current node
        vector<Channel>::iterator it;
        
        for (it=channelMap[villageID].begin(); it!=channelMap[villageID].end();++it){
            int neighborID = it->destID;
            double curr_cost = it->altitude - r*it->dist;
            
            if (inMST[neighborID]==false && keys[neighborID] > curr_cost) {
                keys[neighborID] = curr_cost;
                pq.push(make_pair(neighborID,curr_cost));
                prev_country[neighborID] = villageID;
            }
        }
        
    }

    // Update Dinkelbach Ratio using backtrack
    double altitude_route = 0, dist_route = 0;
    int src, dest;
    for (int i=0; i<V; i++) {
        src = prev_country[i]; 
        dest = i;
        altitude_route += channelMap[src][dest].altitude;
        dist_route += channelMap[src][dest].dist;
    }
    return altitude_route / dist_route;
}

double computeDist(Village a, Village b) {
    double distance = (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
    return sqrt(distance);
}
double computeAltitude(Village a, Village b) {
    double altitude = abs(a.z-b.z);
    return altitude;
}

int main() 
{
    // Read in file
    scanf("%d", &V);
    vector<Village> village_list;
    for (int v=0; v<V; v++){
        Village new_village = {};
        scanf("%d%d%d",&new_village.x,&new_village.y,&new_village.z);
        village_list.push_back(new_village);
    }

    double dist,altitude;
    for (int i=0; i<V; i++) {
        vector<Channel> channelList;
        for (int j=0; j<V; j++) {
            dist = computeDist(village_list[i],village_list[j]);
            altitude = computeAltitude(village_list[i],village_list[j]);
            Channel tmp_channel = {i,j,dist,altitude,0}; // i=srcID;j=destID
            channelList.push_back(tmp_channel);
        }
        channelMap.push_back(channelList);
    }
    
    double rate_dinkelbach = 0;
    int src = 0; // Taking vertex 0 as source
    
    while (true) {
        priority_queue<village_cost_pair,vector<village_cost_pair>, minHeapComp> pq;
        vector<int> keys(V, INF);
        // To store parent array which in turn store MST
        vector<int> prev_country(V,-1);
        // To keep track of vertices included in MST
        vector<bool> inMST(V, false);
        pq.push(make_pair(src,0));
        keys[src] = 0; // Keep track of the latest least cost
        prev_country[0] = 0;
    
        double rate_tmp = primMST(rate_dinkelbach,pq,keys,prev_country,inMST);
        if (fabs(rate_dinkelbach - rate_tmp) < EPS) {
            printf("%.3f\n",rate_dinkelbach);
            break;
        } else {
            rate_dinkelbach = rate_tmp;
            // printf("%.3f\n",rate_tmp);
        }
    }

    return 0;
}