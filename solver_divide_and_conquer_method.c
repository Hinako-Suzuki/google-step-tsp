#include<stdio.h>

/* http://www.geocities.jp/m_hiroi/light/pyalgo63.html */

void read_file(char file_name);
int divide_direction(float buff[][2], int buffNum);
void sort_buff (float buff[][2], int compare_x, int buffNum);
void divide(float buff[][2], float buff1[][2], float buff2[][2], int compare_x, int buffNum);
void divide_test(float buff[][2], int buffNum);
void divide_merge(float buff[][2], int buffNum);
void merge(float buff1[][2], float buff2[][2], int size_buff1, int size_buff2);
void search(float point[][2], float buff[][2], int buffNum, int b_pos[3]);
float distance(float point1[][2], float point2[][2]);
float differ(float point1[][2], float common_point[][2], float point2[][2]);

// FIXME: reading file
// [[x,y], [x,y],...]
void read_file(char file_name){
  printf("read file!");
}

/* 分割する方向を決定する */
/* 
  (max_x - min_x) - (max_y - min_y) > 0 : separate by x 
  (max_x - min_x) - (max_y - min_y) < 0 : separate by y 
*/
int divide_direction(float buff[][2], int buffNum){
  int i = 0;
  // buffNumは引数として渡さないといけない． 
  // buffの引数は最初のポインタを入れているだけなので，
  // const int buffNum = sizeof buff /sizeof buff[0];
  // => 関数内で書くと，1 になる
  
  float max_x = -1000000.0;
  float min_x = 1000000.0;
  float max_y = -1000000.0;
  float min_y = 1000000.0;
  
  for (i=0; i < buffNum; i++){
    if (max_x < buff[i][0]){
      max_x = buff[i][0]; //x
    }
    if (min_x > buff[i][0]){
      min_x = buff[i][0]; //x
    }
    if (max_y < buff[i][1]){
      max_y = buff[i][1]; //y
    }
    if (min_y > buff[i][1]){
      min_y = buff[i][1]; //y
    }
  }
  return (max_x - min_x) - (max_y - min_y);
}

/* x または y要素に注目しソートする*/
/* buff を返す */
void sort_buff (float buff[][2], int compare_x, int buffNum){
  int i, j, tmp_x, tmp_y;
  if (compare_x > 0){
    for (i = 0; i < buffNum; i++) {
      for (j = i+1; j < buffNum; j++) {
	if (buff[i][0] > buff[j][0]) {
	  tmp_x =  buff[i][0];
	  tmp_y =  buff[i][1];
	  buff[i][0] = buff[j][0];
	  buff[i][1] = buff[j][1];
	  buff[j][0] = tmp_x;
	  buff[j][1] = tmp_y;
	}
      }
    }
  }
  else
    {
      for (i = 0; i < buffNum; i++) {
	for (j = i+1; j < buffNum; j++) {
	  if (buff[i][1] > buff[j][1]) {
	    tmp_x =  buff[i][0];
	    tmp_y =  buff[i][1];
	    buff[i][0] = buff[j][0];
	    buff[i][1] = buff[j][1];
	    buff[j][0] = tmp_x;
	    buff[j][1] = tmp_y;
	  }
	}
      }
    }
}

/* 分割する */
void divide(float buff[][2], float buff1[][2], float buff2[][2], int compare_x, int buffNum){
  int n;
  n = buffNum / 2;
  sort_buff(buff, compare_x, buffNum);
  int i, j;
  // 0 - n
  for (i = 0; i < n+1; i++) {
    for (j = 0; j < 2; j++) {
      buff1[i][j] = buff[i][j];
    }
  }
  // n - buffNum
  for (i = n; i < buffNum; i++) {
    for (j = 0; j < 2; j++) {
      buff2[i-n][j] = buff[i][j];
    }
  }
}

/* 分割のテスト */
void divide_test(float buff[][2], int buffNum){
  int n;
  n = buffNum / 2;
  
  // 0 ~ n
  float b1[n+1][2];
  // n ~ buffNum -1
  float b2[buffNum-n][2]; 

  int i, j;
  if (buffNum < 3 || buffNum == 3){
    for (i = 0; i < buffNum; i++) {
      for (j = 0; j < 2; j++) {
	printf("%f \n", buff[i][j]);
      }
    }
    printf("\n");
  }
  else{
    divide(buff, b1, b2, divide_direction(buff, buffNum), buffNum);
    divide_test(b1, sizeof b1 /sizeof b1[0]);
    divide_test(b2, sizeof b2 /sizeof b2[0]);
  }
}

void divide_merge(float buff[][2], int buffNum){
  // 0 ~ n
  // n ~ buffNum -1
  int n;
  n = buffNum / 2;
  float b1[n+1][2]; 
  float b2[buffNum-n][2];

  int i, j;
  if (buffNum < 3 || buffNum == 3){
    return;
  }else{
    divide(buff, b1, b2, divide_direction(buff, buffNum), buffNum);
    
    divide_merge(b1, sizeof b1 /sizeof b1[0]);
    divide_merge(b2, sizeof b2 /sizeof b2[0]);

    //segmentation fault
    for (i=0;i<n+1;i++){
      for(j=0; j<2; j++){
    	printf ("%f ", b1[i][j]);
      }
      printf("\n");
    }
    
    for (i=0;i<buffNum-n;i++){
      for(j=0; j<2; j++){
    	printf ("%f ", b2[i][j]);
      }
      printf("\n");
    }
    printf("aaaa\n");
    merge(b1, b2, sizeof b1 /sizeof b1[0], sizeof b2 /sizeof b2[0]);
  }
}

void search(float point[][2], float buff[][2], int buffNum, int b_pos[3]){
  int i;
  for (i=0; i<buffNum; i++){
    if (buff[i][0] == point[0][0] && buff[i][1] == point[0][1]){
      if (i == 0){
	b_pos[0] = buffNum-1;
	b_pos[1] = i;
	b_pos[2] = i+1;
	break;
      }
      else if (i == buffNum -1){
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

float distance(float point1[][2], float point2[][2]){
  return (point1[0][0] - point2[0][0]) * (point1[0][0] - point2[0][0]) + (point1[0][1] - point2[0][1]) * (point1[0][1] - point2[0][1]);
}

float differ(float point1[][2], float common_point[][2], float point2[][2]){
  return distance(point1, common_point) + distance(point2, common_point) - distance(point1, point2);
}

void merge(float buff1[][2], float buff2[][2], int size_buff1, int size_buff2){
  int i,j;
  printf("%d ",size_buff1);
  for (i=0;i<size_buff1; i++){
    for(j=0; j<2; j++){
      printf("%f ", buff1[i][j]);
    }}
  

  float point[0][2];
  point[0][0] = buff2[0][0];
  point[0][1] = buff2[0][1];

  printf("%d ", point[0][0]);
  printf("%d ", point[0][1]);

  int b1_pos[3];
  int b2_pos[3];
  search(point, buff1, sizeof buff1 /sizeof buff1[0], b1_pos);
  search(point, buff2, sizeof buff2 /sizeof buff2[0], b2_pos);

  float d[4];
  //float point1[0][2] point2[0][2], common_point[0][2];
  //float common_point [0][2];
  //point1[0][0] = buff1[b1_pos[0]][0];
  //common_point[0][0] = buff2[0][0];
  //common_point[0][1] = buff2[0][1];

  d[0] = differ(buff1[b1_pos[0]], buff2[0], buff2[b2_pos[0]]);
  d[1] = differ(buff1[b1_pos[2]], buff2[0], buff2[b2_pos[2]]);
  d[2] = differ(buff1[b1_pos[0]], buff2[0], buff2[b2_pos[2]]);
  d[3] = differ(buff1[b1_pos[2]], buff2[0], buff2[b2_pos[0]]);

  //int i;
  float d_max = -100.0;
  for (i=0; i<4; i++) {
    printf("%f\n", d[i]);
    if (d_max < d[i]){
      d_max = d[i];
    }
  }

  printf ("%f ", d_max);

  /* TODO make new path*/
}

int main(int argc, char *args[])
{
  float buff[5][2] = { {214.98279057984195,762.6903632435094},
  		       {1222.0393903625825,229.56212316547953},
  		       {792.6961393471055,404.5419583098643},
  		       {1042.5487563564207,709.8510160219619},
  		       {150.17533883877582,25.512728869805677} };
  /* float buff[8][2] = {{20, 20}, */
  /* 		      {120, 20}, */
  /* 		      {220, 20}, */
  /* 		      {70, 120}, */
  /* 		      {170, 120}, */
  /* 		      {270, 120}, */
  /* 		      {20, 220}, */
  /* 		      {120, 220}}; */

  /* float buff[16][2] = {{214.98279057984195,762.6903632435094}, */
  /* 		       {1222.0393903625825,229.56212316547953}, */
  /* 		       {792.6961393471055,404.5419583098643}, */
  /* 		       {1042.5487563564207,709.8510160219619}, */
  /* 		       {150.17533883877582,25.512728869805677}, */
  /* 		       {1337.2241662717915,389.490361114548}, */
  /* 		       {1219.6481319327072,1.8954480159996234}, */
  /* 		       {712.6195104876823,649.3860291067043}, */
  /* 		       {366.0195540327242,850.7436259985301}, */
  /* 		       {1442.2839321783738,27.53098473019818}, */
  /* 		       {40.71337758953728,487.2712255141469}, */
  /* 		       {1502.638660445617,343.08381391939116}, */
  /* 		       {346.5590354089814,379.9049180244456}, */
  /* 		       {46.46526011978871,199.52249964573156}, */
  /* 		       {700.6201498409152,446.2310172436656}, */
  /* 		       {372.9351204121162,207.77988738688586}};  */
  int buffNum = sizeof buff /sizeof buff[0]; // 5
  //divide_test(buff, buffNum);
  divide_merge(buff, buffNum);
  return 0;
}
