#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

int checkfile(char*);

//各点の構造体
struct data{
    double x;//x座標
    double y;//y座標
};

struct kyori{
    int shiten;
    int syuten;
    double len;
};
int remove_data(struct kyori*,int,int);
//距離を計算する関数
double Distance(struct data n1,struct data n2){
    return sqrt((n1.x-n2.x)*(n1.x-n2.x)+(n1.y-n2.y)*(n1.y-n2.y));
}



int main(int argc,char*argv[]){
    FILE *fp;
    char*fname = argv[1];
    double x,y;
    int file_size=0;
        
    int i=0,j=0;
    
    if (argc != 2){
        printf("Usage : %s <FileName>\n" ,argv[0]);
        exit(EXIT_FAILURE);
    }

    //ファイル名のチェック
    file_size = checkfile(fname);
    printf("file_size : %d\n",file_size);

    struct data d[file_size];
    int tour[file_size];
    //メモリの問題で2048のときはsegになってしまった
    int array_size = file_size * file_size ; //file_sizeのシグマだけ必要
    struct kyori dist[array_size];//若干無駄なメモリが存在

    //ファイルを開く
    fp = fopen(fname,"r");
    if(fp == NULL){
        printf("%s file not open\n",fname);
        exit(EXIT_FAILURE);
    }


    //各行の読み込み
    fscanf(fp,"%*s,%*s");//一行目を読み飛ばす
    while(fscanf(fp,"%lf,%lf",&x,&y)!=EOF){
        printf("%lf %lf\n",x,y);//確かめ用出力(あとで消す)
        d[i].x=x;//構造体に代入
        d[i].y=y;
        i++;
    }

    for(i=0;i<file_size;i++){
        for(j=i;j<file_size;j++){
            //距離を配列に入れる
            dist[i * file_size + j].len = dist[i + file_size * j].len = Distance(d[i],d[j]);
            dist[i * file_size + j].shiten = dist[i + file_size * j].syuten = i;
            dist[i * file_size + j].syuten = dist[i + file_size * j].shiten = j;
            }
    }

    int current_city = 0; //初期の場所
    int unvisited_cities[file_size-1]; //訪れていない場所のセット
    int nextcity;
    for(i=0;i<file_size-1;i++){
        unvisited_cities[i]=i+1; //行ってないとこを入れていく
    }

    tour[0]=current_city; //初期値をツアーに入れる
   

    for(i=1;i<file_size;i++){

        double tmplen= DBL_MAX;
        for(j=0;j<file_size;j++){//距離の比較を行う
         if(dist[current_city * file_size + j].len < tmplen && dist[current_city * file_size + j].len >0){
             tmplen=dist[current_city * file_size + j].len;
             nextcity = j;
         }
        }
        printf("nextcity :%d\n",nextcity);
        remove_data(dist,current_city,file_size);//unvisitedcityの配列を減らし、unvisitedの中身を減らす
        
        tour[i]=nextcity;
        current_city=nextcity;
    }
    for(i=0;i<file_size;i++){
    printf("%d\n",tour[i]);
    }
    fclose(fp);
    return 0;
}

int checkfile(char*file_name){
    int len=0;
    if(!strcmp(file_name,"input_0.csv")){
        len=5;
    }
    else if(!strcmp(file_name,"input_1.csv")){
        len=8;
    }
    else if(!strcmp(file_name,"input_2.csv")){
        len=16;
    }
    else if(!strcmp(file_name,"input_3.csv")){
        len=64;
    }
    else if(!strcmp(file_name,"input_4.csv")){
        len=128;
    }
    else if(!strcmp(file_name,"input_5.csv")){
        len=512;
    }
    else if(!strcmp(file_name,"input_6.csv")){
        len=2048;
    }
    else{
        perror(file_name);
        exit(EXIT_FAILURE);
    }
    return len;
}
//移動したら、配列のデータを0にする
int remove_data(struct kyori*dist,int city,int file_size){
    int i=0,j=0;
    for(i=0;i<file_size;i++){
        for(j=i;j<file_size;j++){
            if(i == city || j==city){
                dist[i * file_size + j].len = dist[i + file_size * j].len = 0;
            }
            }
    }
    return 0;
}
