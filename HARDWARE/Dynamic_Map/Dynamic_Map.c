#include "Dynamic_Map.h"
#include "Dijkstra.h"
#include "LCD5110.h"

int vexs[15];//������Ϣ
int vexnum = 10;
int arcnum = 13;//�������ͱ���
int p[MAX_VERTEX_NUM][MAX_VERTEX_NUM];//���·������p
int D[MAX_VERTEX_NUM];//��̾�������D
int pre_Node_Count = 0;
int angle_Array[10];	//װ��·���滮������ڵ��Ӧ�ĽǶ�ֵ
//��ͼ����---�ڽӾ���
//int arcs[6][6] = {
//					{1000,1000,10,1000,30,100},
//					{1000,1000,5,1000,1000,1000},
//					{1000,1000,1000,50,1000,1000},
//					{1000,1000,1000,1000,1000,10},
//					{1000,1000,1000,20,1000,60},
//					{1000,1000,1000,1000,1000,1000}
//					};
int arcs[10][10] = {
		{1000,1000,1,1000,1000,1000,1000,1000,1000,1000},
		{1000,1000,4,1000,6,1000,1000,1000,1000,1000},
		{1,4,1000,5,1000,20,1000,1000,1000,1000},
		{1000,1000,5,1000,1000,1000,2,1000,1000,1000},
		{1000,6,1000,1000,1000,3,1000,7,1000,1000},
		{1000,1000,20,1000,3,1000,6,1000,1000,4},
		{1000,1000,1000,2,1000,6,1000,1000,3,1000},
		{1000,1000,1000,1000,7,1000,1000,1000,1000,2},
		{1000,1000,1000,1000,1000,1000,3,1000,1000,8},
		{1000,1000,1000,1000,1000,4,1000,2,8,1000}
		};
//�ڵ�λ�þ���
//int node_Position_Matrix[6][6] = {
//					{1000,1000,30,1000,40,150},
//					{1000,1000,50,1000,1000,1000},
//					{1000,1000,1000,50,1000,1000},
//					{1000,1000,1000,1000,1000,10},
//					{1000,1000,1000,20,1000,60},
//					{1000,1000,1000,1000,1000,1000}
//					};
int node_Position_Matrix[10][10] = {								//ֻ���õ��˵Ľڵ��Ӧ�ǶȽ����޸�
					{1000,1000,193,1000,1000,1000,1000,1000,1000,1000},
					{1000,1000,4,1000,193,1000,1000,1000,1000,1000},
					{1,135,1000,283,1000,20,1000,1000,1000,1000},
					{1000,1000,135,1000,1000,1000,193,1000,1000,1000},
					{1000,6,1000,1000,1000,283,1000,7,1000,1000},
					{1000,1000,20,1000,3,1000,6,1000,1000,193},
					{1000,1000,1000,2,1000,135,1000,1000,3,1000},
					{1000,1000,1000,1000,7,1000,1000,1000,1000,2},
					{1000,1000,1000,1000,1000,1000,3,1000,1000,8},
					{1000,1000,1000,1000,1000,4,1000,2,8,1000}
					};

//�㷨������ʼ���Լ��㷨����
void plan_Route(int startNode){
	int i;	
	//ȷ��������������
//	vexnum = 10;
//	arcnum = 13;	
	//���붥��
	for(i=0; i<vexnum; i++){
		vexs[i] = i;
	}	
	
	ShortestPath_DIJ(vexs, arcs, vexnum, arcnum, startNode, p, D);	
}
//��·���滮�Ľڵ����Ӧ�Ƕ��������Ϊ�µľ���
int is_path_prepare = 0;
void combine_Angle_Data(int vertex_num){
	int i;	
	
	for(i=0; i<vertex_num; i++){		
		if(p[vertex_num-1][i]>-1){
			pre_Node_Count++;				//��¼�����������Ҫ�����Ľڵ�����
			if(p[vertex_num-1][i+1]>-1){
					//		LCD5110ShowNum(35, 3, p[5][i]);
					//		delay_ms(3000);
				//----���ģ���p�����ж�Ӧ�����ڵ���Ϊnode_Position_Matrix[��][��]�Ĳ���ȡ����Ӧ���Ƕȸ���angle_Array�����Ӧλ��
				//ͬʱ��node_Position_Matrix[][]��arcs[][]���������Ӧ����λ����ͬ������Ƕ�̬�ı������������ݵ���ϵ�͹ؼ�
				angle_Array[i] = node_Position_Matrix[p[vertex_num-1][i]][p[vertex_num-1][i+1]];
				is_path_prepare = 1;			//�ڵ��Ӧ�ĽǶ�����׼�����
//							LCD5110ShowNum(35, i, angle_Array[i]); //---test
							
			}
//								LCD5110ShowNum(30, 5, pre_Node_Count); //---test
		}
	}
}			
//ͨ���ڵ����ݻ�ö�Ӧ�Ƕ�ֵ
int get_Angle_Value(int actual_Node_Num){
	return angle_Array[actual_Node_Num];
}
//��ȡ������
int get_vexnum(){
	return vexnum;
}
//�޸�������ʼ����Ķ�Ӧ�ڵ�����
void change_Map(int node_num){
	int node_number = get_vexnum();			//ȡ�õ�ͼ�Ķ�������
	node_Position_Matrix[p[node_number-1][node_num]][p[node_number-1][node_num+1]] = INFINITY;			//�޸Ľڵ�λ�þ����Ӧ�ĽǶ�ֵ
	arcs[p[node_number-1][node_num]][p[node_number-1][node_num+1]] = INFINITY;											//�޸ĵ�ͼ�����Ӧ�Ľڵ�֧·��Ϣ	
}
