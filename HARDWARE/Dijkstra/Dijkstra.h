#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

//#define INFINITY 1000//两节点之间没有边时的权值
#define MAX_VERTEX_NUM 15//最大顶点数

void ShortestPath_DIJ(int vexs[], int arcs[][10], int vexnum, int arcnum, int v0, int p[][MAX_VERTEX_NUM], int D[]);
#endif


