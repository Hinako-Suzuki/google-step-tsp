#include<stdio.h>

/* http://www.geocities.jp/m_hiroi/light/pyalgo63.html */

void read_file(char file_name);
int divide_direction(float buff[][2], int buffNum);
void sort_buff (float buff[][2], int compare_x, int buffNum);
void divide(float buff[][2], float buff1[][2], float buff2[][2], int compare_x, int buffNum);
void divide_test(float buff[][2], int buffNum);

// FIXME: reading file
// [[x,y], [x,y],...]
void read_file(char file_name){
  printf("read file!");
}

/* 分割する方向を決定する */
/* 
   1 (true):  x kugiri
   0 (false): y kugiri
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
  //printf ("%f\n",max_x - min_x - (max_y - min_y));
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
  printf ("hogehoge\n");
  for (i = 0; i < buffNum; i++) {
    for (j = 0; j < 2; j++) {
      printf ("%f ", buff[i][j]);
    }
    printf(", ");
  }

  //int i,j;

  // 0 - n
  //printf("test \n");
  for (i = 0; i < n+1; i++) {
    for (j = 0; j < 2; j++) {
      buff1[i][j] = buff[i][j];
      //printf("%f ", buff1[i][j]);
    }
  }
  //printf("\n");

  // n - buffNum
  //printf("test start\n");
  for (i = n; i < buffNum; i++) {
    for (j = 0; j < 2; j++) {
      buff2[i-n][j] = buff[i][j];
      //printf("%f ", buff2[i-n][j]);
    }
  }
  //printf("test ends\n");
  
  /* for (i=0; i< buffNum-n; i++){ */
  /*   for (j = 0; j < 2; j++) { */
  /*     printf("%f ", buff2[i][j]); */
  /*   } */
  /* } */
}

/* 分割のテスト */
void divide_test(float buff[][2], int buffNum){
  //float p;
  
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
    
    /* */
    printf("devide is Ok? \n");
    for (i = 0; i < n+1; i++) {
      for (j = 0; j < 2; j++) {
      printf("%f ", b1[i][j]);
      }
      printf(", ");
    }
    printf("\n");
    for (i = 0; i < buffNum - n ; i++) {
      for (j = 0; j < 2; j++) {
      printf("%f ", b2[i][j]);
      }
      printf(", ");
    }
    printf("\n");

    int i = sizeof b1 /sizeof b1[0]; // 2
    printf("size b1 %d \n", i);
    int j = sizeof b2 /sizeof b2[0]; // 3
    printf("size b2 %d \n", j);
    divide_test(b1, sizeof b1 /sizeof b1[0]);
    divide_test(b2, sizeof b2 /sizeof b2[0]);
  }
}


int main(int argc, char *args[])
{
  /* float buff[5][2] = { {214.98279057984195,762.6903632435094}, */
  /* 		       {1222.0393903625825,229.56212316547953}, */
  /* 		       {792.6961393471055,404.5419583098643}, */
  /* 		       {1042.5487563564207,709.8510160219619}, */
  /* 		       {150.17533883877582,25.512728869805677} }; */
  float buff[8][2] = {{20, 20},
		      {120, 20},
		      {220, 20},
		      {70, 120},
		      {170, 120},
		      {270, 120},
		      {20, 220},
		      {120, 220}};

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
  divide_test(buff, buffNum);
  return 0;
}
