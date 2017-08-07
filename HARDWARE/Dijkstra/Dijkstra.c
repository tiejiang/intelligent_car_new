#include "Dijkstra.h"
#include "Dynamic_Map.h"

//�Ͻ�˹�����㷨��������G��V0���㵽���ඥ��v�����·��p[v]����Ȩ����D[V]
//p[][]=-1��ʾû��·��,p[v][i]����Ǵ�v0��v��ǰ��õ����·�������ĵ�i+1������(���Ǵ�ӡ���·���Ĺؼ�),
//��v0��v�����·����Ϊp[v][0]��p[v][j]ֱ��p[v][j]=-1,·����ӡ���
//final[v]Ϊtrue���ҽ���v����S,���Ѿ���ô�v0��v�����·��
void ShortestPath_DIJ(int vexs[], int arcs[][10], int vexnum, int arcnum, int v0, int p[][MAX_VERTEX_NUM], int D[])
{
	int v, w, i, j, min;
	int final[10];
	
	for(v=0; v<vexnum; v++)
	{
		final[v] = 0;//���ֵ
		D[v]=arcs[v0][v];//D[]���v0��v����̾���,��ֵΪv0��v��ֱ�Ӿ���
		for(w=0; w<vexnum; w++)
			p[v][w]=-1;//��p[][]��ֵΪ-1,��û��·��
		if(D[v]<INFINITY)//v0��v��ֱ��·��
		{
			p[v][0]=v0;//v0��v���·���ĵ�һ������
			p[v][1]=v;//v0��v���·�������ĵڶ�������
		}
	}
	
	D[v0]=0;//v0��v0����Ϊ0
	final[v0] = 1;//v0���㲢��S����
	
	for(i=1; i<vexnum; i++)//����G.vexnum-1������
	{//��ʼ��ѭ����ÿ�����v0��ĳ������v�����·��,����v����S����,Ȼ�����p��D
		min=INFINITY;
		for(w=0; w<vexnum; w++)//�����ж�����
			if(!final[w] && D[w]<min)//��S����֮��(��final[]=false)�Ķ������ҵ���v0����ĵ�,���丳��v,���븳��min
			{
				v=w;
				min=D[w];
			}
			final[v] = 1;//v����S����
			for(w=0; w<vexnum; w++)//�����²���Ķ��㣬���²���S���ϵĶ��㵽v0�ľ����·������
			{
				if(!final[w] && min<INFINITY && arcs[v][w]<INFINITY && (min+arcs[v][w]<D[w]))
				{//w������S������v0-->v-->w�ľ���<Ŀǰv0-->w�ľ���
					D[w]=min+arcs[v][w];//����D[w]
					for(j=0; j<vexnum; j++)//�޸�p[w],v0��w�����Ķ������v0��v���������ж����ټ���w
					{
						p[w][j]=p[v][j];
						if(p[w][j]==-1)//��p[w][]��һ������-1�ĵط�����w
						{
							p[w][j]=w;
							break;
						}
					}						
				}
			}
	}	
}


