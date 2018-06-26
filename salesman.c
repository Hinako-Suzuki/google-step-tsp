#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM 5
//各点の構造体
struct data{
    double x;//x座標
    double y;//y座標
};
//順番の構造体→本当は配列作るつもりだったが配列の引き渡しで詰みました
struct junban{
    int a;
    int b;
    int c;
    int d;
    int e;
};

//距離を計算する関数
double Distance(struct data n1,struct data n2){
    return sqrt((n1.x-n2.x)*(n1.x-n2.x)+(n1.y-n2.y)*(n1.y-n2.y));
}

//全合計を計算する関数
double SUM(struct data d[],struct junban j){
    double s=0;
    s+=Distance(d[j.a],d[j.b]);
    s+=Distance(d[j.b],d[j.c]);
    s+=Distance(d[j.c],d[j.d]);
    s+=Distance(d[j.d],d[j.e]);
    s+=Distance(d[j.e],d[j.a]);
    return s;
}

int main(void){
    FILE *fp;
    FILE *fp2;
    char fname[] = "input_0.csv";//←＊＊＊コマンドラインから入力に直す(?)＊＊＊
    char fname2[] = "N5.txt";//全通りの入力ファイル
    int A,B,C,D,E,j;
    double x,y,s,s_1;
    struct data d[NUM];
    int i=0;
    struct junban junban[200];
    //ファイルを開く
    fp = fopen(fname,"r");
    if(fp == NULL){
        printf("%s file not open\n",fname);
        return -1;
    }
    //各行の読み込み
    fscanf(fp,"%*s,%*s");//一行目を読み飛ばす
    while(fscanf(fp,"%lf,%lf",&x,&y)!=EOF){
        d[i].x=x;//構造体に代入
        d[i].y=y;
        i++;
    }
    fclose(fp);
    fp2=fopen(fname2,"r");
    if(fp2 == NULL){
        printf("%s file not open\n",fname2);
        return -1;
    }
    
    //本当は再帰関数で順番全パターン出力するつもりだったが予め作ったファイルから読み込むことにした
    i=0;
    while(fscanf(fp,"%d,%d,%d,%d,%d",&A,&B,&C,&D,&E)!=EOF){
        junban[i].a=A;
        junban[i].b=B;
        junban[i].c=C;
        junban[i].d=D;
        junban[i].e=E;
        i++;
    }
    fclose(fp2);
    
    //一番短いものを探す
    s=SUM(d,junban[0]);
    for(i=1;i<120;i++){
        s_1=SUM(d,junban[i]);
        if(s_1<s){
            s=s_1;
            j=i;
        }
    }
    
    printf("最短経路の距離は%lf\n",s);
    printf("道順は：%d %d %d %d %d\n",junban[j].a,junban[j].b,junban[j].c,junban[j].d,junban[j].e);
    
    
    return 0;
}
