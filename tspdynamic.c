#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 8

double W[N][N];
double dp[N][1 << N] = {{-1}};

struct data{
    double x;//x座標
    double y;//y座標
};

double distance(struct data n1,struct data n2){
    return sqrt((n1.x-n2.x)*(n1.x-n2.x)+(n1.y-n2.y)*(n1.y-n2.y));
}

//visitedは今までの経路
double shortestPath(int current, int visited){

	//全てのcityに行った
	if(visited == (1 << N) -1)
		return W[current][0];

	//もう行ったことある
	if(dp[current][visited] >0){
		return dp[current][visited];
	}

	double ret = 987654321;

	//setから次のelementを選ぶ
	for(int next =0; next < N; next++){
		if(visited & (1 << next))
			continue;

		if(W[current][next] == 0)
			continue;

		double temp = W[current][next] + shortestPath(next, visited+ (1<<next));

		if(ret >= temp) ret =temp;
	}
	return ret;
}



int main(void){
    FILE *fp;
    FILE *fp2;
    double x,y;
    int i=0;
    char fname[] = "input_1.csv";//←＊＊＊コマンドラインから入力に直す(?)＊＊＊

    struct data d[N];

    fp = fopen(fname,"r");
    if(fp == NULL){
        printf("%s file not open\n",fname);
        return -1;
    }

    //各行の読み込み
    fscanf(fp,"%*s,%*s");//一行目を読み飛ばす
    while(fscanf(fp,"%lf,%lf",&x,&y)!=EOF){
    	d[i].x=x;
    	d[i].y=y;
    	i++;
    }
    fclose(fp);

    for (int i = 0; i < N; i++) {
    	for (int j = 0; j< N; j++) {
    				W[i][j] = distance(d[i],d[j]);
    	}
    }

    int start =0;
    printf("\n%lf\n", shortestPath(start, 0));

    return 0;

}




