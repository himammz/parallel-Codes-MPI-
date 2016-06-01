#include <mpi/mpi.h>
#include <stdio.h>
#include "time.h"


int main ()
{

    int p,rank;
    long long int finalRes;

    MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    //MPI_Status status;
    int n, part;
    clock_t start;
    if (rank==0)
    {
        printf ("Enter a number:");
        scanf ("%d",&n);
        start = clock();
        part = n / (p - 1 ) ;


    }
    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&part,1,MPI_INT,0,MPI_COMM_WORLD);

    long long int res=1;
    if (rank!=0)
    {
        int rem=n % (p - 1) ;
        int i= part * (rank - 1) + 1;
        if ((p-rank) <= rem)
        {
            part++;
            i+=rem-(p-rank);
        }
         res=1;
        int m=0;
//	printf("%d %d\n",i,rank);
        for (m;m<part;m++,i++)
        {
            res*=i;
        }



    }
//print
    MPI_Reduce(&res,&finalRes,1,MPI_LONG_LONG_INT,MPI_PROD,0,MPI_COMM_WORLD);

    if (rank==0)
    {

        printf ("Factorial is: %lld \n",finalRes);
        clock_t end = clock();
        float seconds = (float)(end - start) / CLOCKS_PER_SEC;
        printf("%f\n",seconds );
    }

    MPI_Finalize();


    return 0;

}

