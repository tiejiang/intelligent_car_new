#ifndef _DYNAMIC_H
#define _DYNAMIC_H

#define INFINITY 1000//���ڵ�֮��û�б�ʱ��Ȩֵ
#define MAX_VERTEX_NUM 15//��󶥵���

void plan_Route(int startNode);
void combine_Angle_Data(int vertex_num);
int get_Angle_Value(int actual_Node_Num);
int get_vexnum(void);
void change_Map(int node_num);
#endif
extern int p[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

//extern int arcs[10][10];
extern int pre_Node_Count; //��һ��·���滮���õ��ܽڵ���
extern int is_path_prepare;
//extern int node_Position_Matrix[6][6];
