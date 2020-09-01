#include <stdio.h>

void
run(int i)
{
    if (i % 5 == 0 && i % 3 == 0)
    {
        printf("TIK TOK\n");
    }

    else if (i % 3 == 0)
    {
        printf("TIK\n");
    }

    else if (i % 5 == 0)
    {
        printf("TOK\n");
    }


    if (i != 100)
    {
        run(i + 1);
    }

    return;
}

int
main()
{
    run(1);
}
