#include<stdio.h>
#include<math.h>

/* 
分割統治法
reference: http://www.geocities.jp/m_hiroi/light/pyalgo63.html 
*/

#define N 100

struct array{
  float data[N][2];
  int num;
};
struct point{
  float x;
  float y;
};
struct point_group{
  struct point data[N];
  int point_group_num;
};
struct queue{
  struct point_group data_group[N];
  int head;
  int tail;
};
void initialize_point(struct point *point, float buff[2]){
  point->x=buff[0];
  point->y=buff[1];
}
void initialize_point_group(struct point_group *pg, struct array *ar){
  int i;
  pg->point_group_num = ar->num;
  for(i=0; i < ar->num; i++){
    initialize_point(&(pg->data[i]), ar->data[i]);
  }
}
void initialize_queue(struct queue *q, struct array *ar, int queueNum){
  int i;
  q->head=0;
  q->tail=queueNum;
  for(i=0; i < queueNum; i++){
    initialize_point_group(&(q->data_group[i]), ar);
  }
}
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
void print_queue(struct queue *Q){
  int i;
  for (i=Q->head; i<Q->tail; i++){
    print_point_group(&Q->data_group[i]);
  }
}

void enqueue(struct queue *q, struct point_group *pg){
  if (q->tail >= N) {
    printf("This queue is full! \n");
  }else{
    int i;
    struct array AR;
    AR.num = pg->point_group_num;
    for (i=0; i<AR.num; i++){
	AR.data[i][0] = (pg->data)[i].x;
	AR.data[i][1] = (pg->data)[i].y;
    }
    initialize_point_group(&(q->data_group[q->tail]), &AR);
    q->tail = q->tail +1;
  }
}

int dequeue(struct queue *q){
  struct point_group pg;
  pg.point_group_num = (q->data_group[q->head]).point_group_num;
  if(q->head == q->tail){
    return -1;
  }else{
    //print_point_group(&(q->data_group[q->head]));
    q->head = q->head + 1;
    return q->head;
  }
}
// FIXME: reading file
// [[x,y], [x,y],...]
int read_file(char *file_name, float buff[][2]){
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
    buff[i][0]=x;
    buff[i][1]=y;
    i++;
  }
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
void divide(struct point_group *PG, struct queue *Q, int compare_x){
  int n;
  n = PG->point_group_num / 2;

  sort_buff(PG, compare_x);
  //print_point_group(PG);

  struct point_group b1;
  struct point_group b2;
  int i;

  struct array AR1;
  struct array AR2;
  // 0 - n
  for (i = 0; i < n+1; i++) {
    AR1.data[i][0] = (PG->data[i]).x;
    AR1.data[i][1] = (PG->data[i]).y;
  }
  AR1.num = n+1;
  initialize_point_group(&b1, &AR1);
  //printf("b1\n");
  //print_point_group(&b1);

  // n - buffNum
  for (i = n; i < PG->point_group_num; i++) {
    AR2.data[i-n][0] = (PG->data[i]).x;
    AR2.data[i-n][1] = (PG->data[i]).y;
  }
  AR2.num = PG->point_group_num -n;
  initialize_point_group(&b2, &AR2);
  //printf("b2\n");
  //print_point_group(&b2);

  enqueue(Q, &b1);
  enqueue(Q, &b2);
  //print_queue(Q);
}

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
  printf("p: %d, i: %d, n: %d\n", b_pos[0], b_pos[1], b_pos[2]);
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

  /* printf("%f ", cp.x); */
  /* printf("%f \n", cp.y); */


  /* printf("%f ", distance(point1, &cp)); */
  /* printf("%f ", distance(point2, &cp)); */
  /* printf("%f \n", distance(point1, point2)); */
  
  return distance(point1, &cp) + distance(point2, &cp) - distance(point1, point2);
} 

void make_new_path(struct point_group *pg_new, struct point_group *pg1, struct point_group *pg2, int b1_pos, int b2_pos, int order){
  int i;
  i = b2_pos + order;
  int j;
  int threshold = pg2->point_group_num; //3

  //printf("b1_pos: %d \n", b1_pos); // 0
  //printf("b2_pos: %d \n", b2_pos); // 0
  //printf("%d \n", pg_new->point_group_num);

  pg_new->point_group_num = 0;
  //for (j=0; j<b1_pos+1; j++){
  for (j=0; j<b1_pos; j++){
    pg_new->data[j].x = pg1->data[j].x;
    pg_new->data[j].y = pg1->data[j].y;
    pg_new->point_group_num = pg_new->point_group_num +1;
  }

  //printf("%d \n", pg_new->point_group_num);
  //printf("first: \n");
  //print_point_group(pg_new);

  while (1){
    if (i < 0){
      i = threshold -1;
    }
    else if (i > threshold || i == threshold){
      i=0;
    }
    if (i == b2_pos){
      //printf("break\n");
      break;
    }
    //printf("i: %d \n",i);

    pg_new->data[j].x=pg2->data[i].x;
    pg_new->data[j].y=pg2->data[i].y;
    pg_new->point_group_num += 1;
    i = i+order;
    j = j+1;
  }
  
  //printf("middle: \n");
  //print_point_group(pg_new);

  /* okashii */
  int k;
  //printf("j: %d\n", j);
  //for (k=b1_pos+1; k<pg1->point_group_num; k++){
  for (k=b1_pos; k<pg1->point_group_num; k++){
    //for (j=pg_new->point_group_num+1; j<pg1->point_group_num; j++){
    //printf("j: %d", j);
    pg_new->data[j].x = pg1->data[k].x;
    pg_new->data[j].y = pg1->data[k].y;
    pg_new->point_group_num += 1;
    j = j+1;
  }
  //printf("end: \n");
  //print_point_group(pg_new);
}

void merge(struct point_group *pg1, struct point_group *pg2, struct point_group *pg_new){
  int i,j;
  //printf("1\n");
  //print_point_group(&pg1);
  //printf("2\n");
  //print_point_group(&pg2);
    
  // search for common point
  int b1_pos[3];
  int b2_pos[3]; 
  float point[2];
  search(pg1, pg2, b1_pos, point);
  search(pg2, pg1, b2_pos, point);
    
  /* for(i=0;i<3;i++){ */
  /*   printf("%d ", b1_pos[i]); */
  /* } */
  /* printf("\n"); */
  /* for(i=0;i<3;i++){ */
  /*   printf("%d ", b2_pos[i]); */
  /* } */
  /* printf("\n"); */

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
  
  //printf ("d_max %f \n", d_max);
 
  /* TODO make new path*/
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
  /* printf("CREATED pg_new: \n"); */
  /* print_point_group(pg_new); */
  /* printf("\n"); */
}

int return_index(struct queue *Q, struct array *AR, char *file_name){
  int i, j, k;
  float x, y;
  FILE *fp;
  fp = fopen(file_name, "w");
  if (fp == NULL) {          // オープンに失敗した場合
    printf("cannot open\n");         // エラーメッセージを出して
    return -1;                         // 異常終了
  }
  fprintf(fp, "index\n");
  for (i=Q->head; i<Q->tail; i++){
    for (j=0; j<(Q->data_group[i]).point_group_num; j++){
      x = (Q->data_group[i]).data[j].x;
      y = (Q->data_group[i]).data[j].y;
      for(k=0; k<AR->num; k++){
	//printf("X: %f, Y: %f\n\n", AR->data[k][0], AR->data[k][1]);
	//printf("x: %f, y: %f\n", x, y);
	if ((int)AR->data[k][0] == (int)x && (int)AR->data[k][1] == (int)y){
	  fprintf(fp, "%d\n", k);
	  printf("%d \n", k);
	}
      }
    }
  }
  fclose(fp);
}

void divide_merge(struct queue *Q, struct array *AR, char *file_name){
  //void divide_merge(struct queue *Q){
  // 0 ~ n
  // n ~ buffNum -1
  //float buff_list[100];
  struct queue Q_new;
  
  //printf("%d\n", (Q->data_group[Q->head]).point_group_num);
  // 5

  while ((Q->data_group[Q->head]).point_group_num > 3){
    divide(&(Q->data_group[Q->head]), Q, divide_direction(&(Q->data_group[Q->head])));
    dequeue(Q);
    //printf("%d\n", Q->head); // 1
    //printf("%d\n", (Q->data_group[Q->head]).point_group_num);
  }
  
  /* add Q_new */
  int i;
  for(i=Q->head; i<Q->tail; i++){
    enqueue(&Q_new, &Q->data_group[i]);
  }
  printf("Q_new\n");
  print_queue(&Q_new);
  
  //struct queue final_path;
  /* merge Q_new*/
  while (Q_new.head + 1 < Q_new.tail){
    i = Q_new.head;
    
    struct point_group pg1;
    struct point_group pg2;
    struct point_group pg_new;
    pg1 = Q_new.data_group[i];
    pg2 = Q_new.data_group[i+1];

    /* printf("pg1: \n"); */
    /* print_point_group(&pg1); */
    /* printf("\n"); */
    /* printf("pg2: \n"); */
    /* print_point_group(&pg2); */
    /* printf("\n"); */

    merge(&pg1, &pg2, &pg_new);

    //printf("created pg_new: \n"); 
    //print_point_group(&pg_new); 
    //printf("\n");


    enqueue(&Q_new, &pg_new);  
    dequeue(&Q_new);
    dequeue(&Q_new); 
  }
  printf("final path\n");
  //print_queue(&Q_new);
  return_index(&Q_new, AR, file_name);
}


int main(int argc, char *args[])
{
  float buff[16][2];
  char input_file_name[] = "input_2.csv";
  char output_file_name[] = "output_2.csv";
  
  read_file(input_file_name, buff);
  int buffNum = sizeof buff /sizeof buff[0];
  struct queue Q;
  struct array AR;
  AR.num =buffNum;
  int i, j;
  for (i=0; i<AR.num; i++){
    for(j=0; j<2; j++){
      AR.data[i][j] = buff[i][j];
    }
  }
  initialize_queue(&Q, &AR, 1);
  printf ("initialize\n");
  print_queue(&Q);

  divide_merge(&Q, &AR, output_file_name);
  return 0;
  }
