#include<stdio.h>
#include<math.h>

/* 
分割統治法
reference: http://www.geocities.jp/m_hiroi/light/pyalgo63.html 
*/

#define N 2048
struct point{
  float x;
  float y;
};
struct point_group{
  struct point data[N];
  int point_group_num;
};

void print_point(struct point *P){
  printf("%f ", P->x);
  printf("%f \n", P->y);
}  

void print_point_group(struct point_group *PG){
  int i;
  for (i=0; i<PG->point_group_num; i++){
    print_point(&PG->data[i]);
  }
  printf("\n");
}

int read_file(char *file_name, struct point_group *PG){
  FILE *fp;
  fp = fopen(file_name, "r");
  if(fp == NULL){
    printf("%s file not open\n", file_name);
    return -1;
  }
  int i = 0;
  float x, y;
  //各行の読み込み
  fscanf(fp,"%*s,%*s");//一行目を読み飛ばす
  while(fscanf(fp,"%f,%f", &x, &y)!=EOF){
    PG->data[i].x=x;
    PG->data[i].y=y;
    i++;
  }
  PG->point_group_num = i;
  fclose(fp);
  return;
}

/* 分割する方向を決定する */
/*
  (max_x - min_x) - (max_y - min_y) > 0 : separate by x 
  (max_x - min_x) - (max_y - min_y) < 0 : separate by y 
*/
int divide_direction(struct point_group *PG){
  int i = 0;
  
  float max_x = -1000000.0;
  float min_x = 1000000.0;
  float max_y = -1000000.0;
  float min_y = 1000000.0;
  
  for (i=0; i < PG->point_group_num; i++){
    if (max_x < PG->data[i].x){
      max_x = PG->data[i].x; //x
    }
    if (min_x > PG->data[i].x){
      min_x = PG->data[i].x; //x
    }
    if (max_y < PG->data[i].y){
      max_y = PG->data[i].y; //y
    }
    if (min_y > PG->data[i].y){
      min_y = PG->data[i].y; //y
    }
  }
  return (max_x - min_x) - (max_y - min_y);
}

/* x または y要素に注目しソートする*/
void sort_buff (struct point_group *PG, int compare_x){
  int i, j, tmp_x, tmp_y;
  int buffNum;
  buffNum = PG->point_group_num;
  for (i = 0; i < buffNum; i++) {
    for (j = i+1; j < buffNum; j++) {
      if ((compare_x > 0 && PG->data[i].x > PG->data[j].x) || ((compare_x < 0 || compare_x == 0) && PG->data[i].y > PG->data[j].y)){
	tmp_x =  PG->data[i].x;
	tmp_y =  PG->data[i].y;
	PG->data[i].x = PG->data[j].x;
	PG->data[i].y = PG->data[j].y;
	PG->data[j].x = tmp_x;
	PG->data[j].y = tmp_y;
      }
    }
  }
}

/* 分割する */
void divide(struct point_group *PG1, struct point_group *PG2, struct point_group *PG, int compare_x){
  int i, n;
  n = PG->point_group_num / 2;

  sort_buff(PG, compare_x);

  // 0 - n
  for (i = 0; i < n+1; i++) {
    PG1->data[i].x = (PG->data[i]).x;
    PG1->data[i].y = (PG->data[i]).y;
  }
  PG1->point_group_num = i;

  // n - buffNum
  for (i = n; i < PG->point_group_num; i++) {
    PG2->data[i-n].x = (PG->data[i]).x;
    PG2->data[i-n].y = (PG->data[i]).y;
  }
  PG2->point_group_num = PG->point_group_num -n;
}

/* 小さい固まりの２組のうち，共通の点がある．それを見つける．*/
void search(struct point_group *pg1, struct point_group *pg2, int b_pos[3], float point[2]){
  int i, j;
  for (i=0; i<pg1->point_group_num; i++){
    for (j=0; j<pg2->point_group_num; j++){
      if (pg1->data[i].x == pg2->data[j].x && pg1->data[i].y == pg2->data[j].y){
	point[0] = pg1->data[i].x;
	point[1] = pg1->data[i].y;
      }
    }
  }

  for (i=0; i<pg1->point_group_num; i++){
    if (pg1->data[i].x == point[0] && pg1->data[i].y == point[1]){
      if (i == 0){
	b_pos[0] = pg1->point_group_num-1;
	b_pos[1] = i;
	b_pos[2] = i+1;
	break;
      }
      else if (i == pg1->point_group_num -1){
	b_pos[0] = i-1;
	b_pos[1] = i;
	b_pos[2] = 0;
	break;
      }
      else{
	b_pos[0] = i-1;
	b_pos[1] = i;
	b_pos[2] = i+1;
	break;
      }
    }
  }
}

float distance(struct point *point1, struct point *point2){
  float tmp, tmp0, tmp1, tmp2;
  tmp1 = point1[0].x - point2[0].x;
  tmp2 = point1[0].y - point2[0].y;
  tmp = tmp1 * tmp1 + tmp2 * tmp2;
  return sqrt(tmp);
} 

float differ(struct point *point1, float common_point[2], struct point *point2){
  struct point cp;
  cp.x = common_point[0];
  cp.y = common_point[1];  
  return distance(point1, &cp) + distance(point2, &cp) - distance(point1, point2);
} 

/* 頂点の配列２つを１つにする */
void make_new_path(struct point_group *pg_new, struct point_group *pg1, struct point_group *pg2, int b1_pos, int b2_pos, int order){
  int i;
  i = b2_pos + order;
  int j;
  int threshold = pg2->point_group_num;

  pg_new->point_group_num = 0;
  for (j=0; j<b1_pos; j++){
    pg_new->data[j].x = pg1->data[j].x;
    pg_new->data[j].y = pg1->data[j].y;
    pg_new->point_group_num = pg_new->point_group_num +1;
  }

  while (1){
    if (i < 0){
      i = threshold -1;
    }
    else if (i > threshold || i == threshold){
      i=0;
    }
    if (i == b2_pos){
      break;
    }
    pg_new->data[j].x=pg2->data[i].x;
    pg_new->data[j].y=pg2->data[i].y;
    pg_new->point_group_num += 1;
    i = i+order;
    j = j+1;
  }
  
  int k;
  for (k=b1_pos; k<pg1->point_group_num; k++){
    pg_new->data[j].x = pg1->data[k].x;
    pg_new->data[j].y = pg1->data[k].y;
    pg_new->point_group_num += 1;
    j = j+1;
  }
}

/* 経路を見つける．蝶々の形をした三角形２つをどうつなぐか考える．
   TODO: 図を描く */
void merge(struct point_group *pg1, struct point_group *pg2, struct point_group *pg_new){
  int i;
  // search for common point
  int b1_pos[3];
  int b2_pos[3];
  float point[2];
  search(pg1, pg2, b1_pos, point);
  search(pg2, pg1, b2_pos, point);
    
  // calculate distance
  float d[4];
  d[0] = differ(&pg1->data[b1_pos[0]], point, &pg2->data[b2_pos[0]]);
  d[1] = differ(&pg1->data[b1_pos[2]], point, &pg2->data[b2_pos[2]]);
  d[2] = differ(&pg1->data[b1_pos[0]], point, &pg2->data[b2_pos[2]]);
  d[3] = differ(&pg1->data[b1_pos[2]], point, &pg2->data[b2_pos[0]]);
  
  float d_max = -100.0;
  for (i=0; i<4; i++) {
    if (d_max < d[i]){
      d_max = d[i];
    }
  }
  
  /* make new path*/
  if (d_max == d[0]){
    make_new_path(pg_new, pg1, pg2, b1_pos[1], b2_pos[1], -1);
  }
  else if (d_max == d[1]){
    make_new_path(pg_new, pg1, pg2, b1_pos[2], b2_pos[1], -1);
  }
  else if (d_max == d[2]){
    make_new_path(pg_new, pg1, pg2, b1_pos[1], b2_pos[1], 1);
  }
  else if (d_max == d[3]){
    make_new_path(pg_new, pg1, pg2, b1_pos[2], b2_pos[1], 1);
  }
}

/* 見つけた経路をファイルに書き込む */
int return_index(struct point_group *pg_new, char *file_name1, char *file_name){
  int i, j;
  float x, y;

  struct point_group dict;
  read_file(file_name1, &dict);

  FILE *fp;
  fp = fopen(file_name, "w");
  if (fp == NULL) {
    printf("cannot open\n");
    return -1;
  }
  fprintf(fp, "index\n");
  for (i=0; i<pg_new->point_group_num; i++){
    x = pg_new->data[i].x;
    y = pg_new->data[i].y;
    for(j=0; j<dict.point_group_num; j++){
      if ((int)dict.data[j].x == (int)x && (int)dict.data[j].y == (int)y){
	  fprintf(fp, "%d\n", j);
	}
      }
    }
  fclose(fp);
}

/* 
   再帰関数を使って，頂点の配列をばらしていく．(divide)
   順番につないでいく．(merge)
 */
void divide_merge(struct point_group *PG, struct point_group *pg_new, char *file_name1, char *file_name){
  struct point_group PG1, PG2;
  if (PG->point_group_num < 3 || PG->point_group_num == 3){
    return;
  }
  else{
    divide(&PG1, &PG2, PG, divide_direction(PG));
    divide_merge(&PG1, pg_new, file_name1, file_name);
    divide_merge(&PG2, pg_new, file_name1, file_name);
    merge(&PG1, &PG2, pg_new);
  }  
  return_index(pg_new, file_name1, file_name);
}


int main(int argc, char *args[])
{
  char input_file_name[] = "input_0.csv";
  char output_file_name[] = "output_0.csv";
  
  struct point_group PG, pg_new;
  read_file(input_file_name, &PG);
  divide_merge(&PG, &pg_new, input_file_name, output_file_name);
  return 0;
  }
