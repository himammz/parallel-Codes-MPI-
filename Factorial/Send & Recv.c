#include <mpi/mpi.h>
#include <stdio.h>
#include "time.h"


int main ()
{

    int p,rank;
    long long int finalRes=1;

    MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Status status;
    int n, part;
    clock_t start;
    if (rank==0)
    {
        printf ("Enter a number:");
        scanf ("%d",&n);
        start = clock();
        part = n / (p - 1 ) ;
        int i=1;
        for (i;i<p;i++)
        {
            MPI_Send(&n,1,MPI_INT,i,0,MPI_COMM_WORLD);
            MPI_Send(&part,1,MPI_INT,i,0,MPI_COMM_WORLD);


        }
    }


    long long int res=1;
    if (rank!=0)
    {

        MPI_Recv(&n,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
        MPI_Recv(&part,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);

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


            MPI_Send(&res,1,MPI_LONG_LONG_INT,0,0,MPI_COMM_WORLD);

    }

    if (rank==0)
    {
        int i=1;
        for (i;i<p;i++)
        {
            MPI_Recv(&res,1,MPI_LONG_LONG_INT,i,0,MPI_COMM_WORLD,&status);
            finalRes*=res;

        }
        printf ("Factorial is: %lld \n",finalRes);
        clock_t end = clock();
        float seconds = (float)(end - start) / CLOCKS_PER_SEC;
        printf("%f\n",seconds );
    }

    MPI_Finalize();


    return 0;

}

