
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<omp.h>
#include<time.h>


#define NUM_ITEMS 100000000

void mergeSort(int numbers[], int temp[], int array_size);
void m_sort(int numbers[], int temp[], int left, int right);
void merge(int numbers[], int temp[], int left, int mid, int right);

int numbers[NUM_ITEMS];
int temp[NUM_ITEMS];
int temps[NUM_ITEMS];
int s1, s2, s3;
clock_t start,end;


int main()
{
  int i;
  FILE *fp;
  
fp = fopen("Parallel.txt","r"); //ファイル読み込み
for (i = 0; i < NUM_ITEMS; i++)
    fscanf(fp, "%d", &numbers[i]);

  fclose(fp);



  start = clock();
  mergeSort(numbers, temp, NUM_ITEMS);
  end = clock();
 
    printf("%.2f秒かかりました\n",(double)(end-start)/CLOCKS_PER_SEC);


  return 0;
}


void mergeSort(int numbers[], int temp[], int array_size)
{
s1 = array_size / 4;
s2 = array_size / 2;
s3 = (array_size / 4) * 3; 

int mid ;
 
 mid = array_size / 2;
 

   
 #pragma omp parallel
  {
    if(omp_get_thread_num() == 0){
 		 m_sort(numbers, temp, 0, s1 -1 );
     } else if(omp_get_thread_num() == 1){
    	 m_sort(numbers, temp, s1 , s2 -1 );
   } else if(omp_get_thread_num() == 2){
    	m_sort(numbers, temp, s2, s3 - 1);	
    } else if(omp_get_thread_num() == 3){
    	m_sort(numbers, temp, s3 , array_size - 1);
    }
 }
 
 

 #pragma omp parallel
  {
  	 if(omp_get_thread_num() == 0){
		merge(numbers, temp, 0, s1, s2 - 1);
		 } else if(omp_get_thread_num() == 1){
        merge(numbers, temps, s2 , s3, array_size - 1);
   }
  }
	    merge(numbers, temp, 0, mid+1, array_size);	
}




void m_sort(int numbers[], int temp[], int left, int right)
{
  int mid;

  if (right > left)
  {
    mid = (right + left) / 2;
    m_sort(numbers, temp, left, mid);
    m_sort(numbers, temp, mid+1, right);
    merge(numbers, temp, left, mid+1, right);
  }
}


void merge(int numbers[], int temp[], int left, int mid, int right)
{
  int i, left_end, num_elements, tmp_pos;

  left_end = mid - 1;
  tmp_pos = left;
  num_elements = right - left + 1;

 

  while ((left <= left_end) && (mid <= right))
  {
    if (numbers[left] <= numbers[mid])
    {
      temp[tmp_pos] = numbers[left];
      tmp_pos = tmp_pos + 1;
      left = left +1;
    }
    else
    {
      temp[tmp_pos] = numbers[mid];
      tmp_pos = tmp_pos + 1;
      mid = mid + 1;
    }
  }

  while (left <= left_end)
  {
    temp[tmp_pos] = numbers[left];
    left = left + 1;
    tmp_pos = tmp_pos + 1;
  }

  while (mid <= right)
  {
    temp[tmp_pos] = numbers[mid];
    mid = mid + 1;
    tmp_pos = tmp_pos + 1;
  }
 
 
  for (i=0; i <= num_elements; i++)
  {
    numbers[right] = temp[right];
    right = right - 1;
  }
  
  }