#include <stdio.h>
#include <mpi/mpi.h>

int main ()
{

/*

    equations:
                4X + Y - Z  = 10
                3X + 2Y + Z = 13
                X + Y + Z   = 6

    then  we have
            4   1  -1          X         10
            3   2   1     *    Y      =  13
            1   1   1          Z         6



    then we get inverse matrix of M 3*3
            1.0   -2.0    3.0
           -2.0    5.0   -7.0
            1.0   -3.0    5.0
    then we multiply it by
                    10
                    13
                    6

    then we get
        X =  2
        Y =  3
        Z =  1

 this code give user two options: get input from File input.txt or get it from console.
    get :
        Original Matrix M 3*3
        then the inverse matrix M1 3*3  of M 
        then the vector v 3*1


    input.txt intialized with matrices that  discussed in the first of this comment .     

*/



    int myRank ;
    int p;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&myRank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    int partRows;
    double M1[9];
    double M[9];
    double v[3];
    double Mall[3][4];
    int i=0;
    if (myRank==0)
    {
        int choice ;
        printf ("Enter 1 for reading from file.\nEnter 2 for get input from console.\n");
        scanf("%d",&choice);
        if (choice==1)
        {

            FILE *fp;

            fp = fopen("input.txt", "r");
             i=0;




             i=0;
            for (; i < 9 ;i++)
            {
                fscanf(fp,"%lf",&M[i]);


            }

            i=0;
            for (; i < 9 ;i++)
            {
                fscanf(fp,"%lf",&M1[i]);
            }
            i=0;
            for (;i<3;i++)
            {
                fscanf(fp,"%lf",&v[i]);
            }
            i=0;
            for (;i<3;i++)
            {
                int j =0;
                for (;j<3;j++)
                {
                    Mall[i][j]= M1[(i*3)+j];
                }

                Mall[i][j]=v[i];

            }


            fclose(fp);
        }
        else if (choice==2)
        {
            i=0;
            printf ("Enter Matrix M 3*3 of  X,Y,Z coefficients.\n");

            for (; i < 9 ;i++)
            {
                scanf("%lf",&M[i]);


            }

            i=0;
            printf ("Enter inverse Matrix of M.\n");

            for (; i < 9 ;i++)
            {
                scanf("%lf",&M1[i]);
            }
            i=0;
            printf ("Enter vector v.\n");

            for (;i<3;i++)
            {
                scanf("%lf",&v[i]);
            }

            i=0;
            for (;i<3;i++)
            {
                int j =0;
                for (;j<3;j++)
                {
                    Mall[i][j]= M1[(i*3)+j];
                }

                Mall[i][j]=v[i];

            }

        }


    } // end if of master





    double toRecv[4];
    double gatheredVector[3];
    MPI_Scatter(Mall,4,MPI_DOUBLE,toRecv,4,MPI_DOUBLE,0,MPI_COMM_WORLD);
    //printf("%d    %lf %lf %lf %lf \n", myRank,toRecv[0],toRecv[1],toRecv[2],toRecv[3]);


    MPI_Allgather(&toRecv[3],1,MPI_DOUBLE,gatheredVector,1,MPI_DOUBLE,MPI_COMM_WORLD);
    //printf("%d         %lf %lf %lf  \n", myRank,gatheredVector[0],gatheredVector[1],gatheredVector[2]);

// calculate the mult ;
    i=0;
    double sum=0;
    double finRes[3];


        for (;i<3;i++)
        {

            sum+=toRecv[i]*gatheredVector[i];

        }

    MPI_Gather(&sum,1,MPI_DOUBLE,finRes,1,MPI_DOUBLE,0,MPI_COMM_WORLD);


    if (myRank==0)
    {


        printf("\nResult Matrix: \n");
        int i=0;
        for (;i<3;i++)
        {



            printf("variable_%d = %lf\n", i+1,finRes[i]);
        }
        printf("\n");


    }







    MPI_Finalize();
}

