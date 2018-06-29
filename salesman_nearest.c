#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM 5
//各点の構造体
struct data{
    double x;//x座標
    double y;//y座標
};

//経路とその距離を保存する構造体
struct keiro{
    double kyori[NUM];
    int michi[NUM];
};

//ファイルを読み込んで構造体に格納する関数
int fileyomi(struct data d[]){
	FILE *fp;
    char fname[] = "input_0.csv";//←＊＊＊コマンドラインから入力に直す(?)＊＊＊
    double x,y;
    int i;
    
	fp = fopen(fname,"r");
    if(fp == NULL){
        printf("%s file not open\n",fname);
        return 1;
    }
    //各行の読み込み
    fscanf(fp,"%*s,%*s");//一行目を読み飛ばす
    while(fscanf(fp,"%lf,%lf",&x,&y)!=EOF){
        d[i].x=x;//構造体に代入
        d[i].y=y;
        i++;
    }
    fclose(fp);
    return 0;
}

//結果を出力する関数
void output(struct keiro *min){
    int i=0;
    double sum=0;

    printf("最短経路の道順は：");
    for(i=0;i<NUM;i++){
        printf("%d ",*(min+i)->michi);
        sum+=*(min+i)->kyori;
    }

    printf("\nその距離は：%lf\n",sum);
}

//距離を計算する関数
double Distance(struct data n1,struct data n2){
    return sqrt((n1.x-n2.x)*(n1.x-n2.x)+(n1.y-n2.y)*(n1.y-n2.y));
}

//nowから最短距離の点をstruct keiroに格納
int shortest(int now, int how_many, struct data d[], struct keiro min[]){
    int i,j,check;
    double length;
    for(i=1;i<NUM;i++){
        length=Distance(d[now],d[i]);

        printf("%dlength=%lf\n",i,length);
        
        if(length!=0&&length<*min[how_many].kyori){
            check=0;
            for(j=0;j<how_many;j++){
                if(i==*min[j].michi)check++;
            }

            if(check==0){
                *min[how_many].kyori=length;
                *min[how_many].michi=i;
            }
        }
    }
    return *min[how_many].michi;
}

int main(){
	int i;
    double sum=0;
    struct data d[NUM];
    struct keiro min[NUM];
    
	i=fileyomi(d);
	if(i==1)return -1;
    
    for(i=0;i<NUM;i++){
        *min[i].kyori=10000;
    }
    
    int now=0;  //スタート地点を入力、以降現在地点を保存
    *min[0].michi=now;  //スタート地点を保存

    for(i=1;i<NUM;i++){
        now=shortest(now,i,d,min);
    }


    *min->kyori=Distance(d[*min[4].michi],d[*min[0].michi]); //終了地点からスタート地点に帰る距離

    output(min);
    
	return 0;

}
