#include <stdio.h>

int a[4] = {5, 3, 2, 1};
int b = 0;

int sal()
{
    return 3;
}

int main()
{
    // b = 5;
    a[3] = b + 5;
    printf("%d", a[2]);
    return 5;
}