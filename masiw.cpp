#include <stdio.h>

 
int main(void){
    int arr[] = { 1, 2, 3 }, i;
 
    for ( i = 0; i < sizeof(arr) / sizeof(*arr); ++i )
      printf("%d\n", arr[2]);
 
    return 0;
}