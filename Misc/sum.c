
//include of my personal path where apue.h library is located
#include "../../../src.3e/apue.3e/include/apue.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
int i,sum;
sum=0;
for (i=0; i<10; i++)
if (i<5)
sum=sum+i;
else
sum=sum+((i-3)/2+(i/3));
printf("sum=%d\n",sum);
}