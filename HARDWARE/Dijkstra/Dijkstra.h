#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

//#define INFINITY 1000//���ڵ�֮��û�б�ʱ��Ȩֵ
#define MAX_VERTEX_NUM 15//��󶥵���

void ShortestPath_DIJ(int vexs[], int arcs[][10], int vexnum, int arcnum, int v0, int p[][MAX_VERTEX_NUM], int D[]);
#endif


