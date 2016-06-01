#include <stdio.h>
#include <mpi/mpi.h>

int main ()
{




    int myRank ;
    int p;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&myRank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    int partRows;
    int row1,col1,row2,col2;
    int *M1 ;
    int **M2 ;

    if (myRank==0)
    {
        int choice ;
        printf ("Enter 1 for reading from file.\nEnter 2 for get input from the console.\n");
        scanf("%d",&choice);
        if (choice==1)
        {
            char str[20];
            printf ("Enter name of File.\n");
            scanf("%s",str);

            FILE *fp;

            fp = fopen(str, "r+");
            int i=0;
            fscanf(fp,"%d",&row1);
            fscanf(fp,"%d",&col1);
            fscanf(fp,"%d",&row2);
            fscanf(fp,"%d",&col2);
            if (row2!=col1)
            {
                printf("can't multiply.\n");
            }

            M1= malloc(row1*col1*sizeof(int));
            i=0;
            for (;i<row1*col1;i++)
            {
                fscanf(fp,"%d",&M1[i]);
            }
            int *temp = malloc(row2*col2*sizeof(int));
            M2= malloc(row2*sizeof(int *));

            i=0;
            for (;i<row2;i++)
                M2[i]= &temp[i*col2];

                i=0;

                for (;i<row2;i++)
                {
                    int j=0;
                    for (;j<col2;j++)
                        fscanf(fp,"%d",&M2[i][j]);
                }
            fclose(fp);
        }



        else if (choice==2)
        {
            printf("enter #rows #coloums of matrix1 : \n");

            scanf("%d %d",&row1,&col1);
            printf("enter #rows #coloums of matrix2 : \n");
            scanf("%d %d",&row2,&col2);
            if (row2!=col1)
            {
                printf("can't multiply.\n");
            }
            else
            {
                M1= malloc(row1*col1*sizeof(int));
                int i=0;
                printf("enter matrix1 element: \n");
                for (;i<row1*col1;i++)
                {
                    scanf("%d",&M1[i]);
                }
                int *temp = malloc(row2*col2*sizeof(int));
                M2= malloc(row2*sizeof(int *));

                i=0;
                for (;i<row2;i++)
                    M2[i]= &temp[i*col2];
                i=0;
                printf("enter matrix2 element: \n");

                for (;i<row2;i++)
                {
                    int j=0;
                    for (;j<col2;j++)
                        scanf("%d",&M2[i][j]);
                }
            }
        }

        partRows= row1/p;
    } // end if of master


    MPI_Bcast(&col1,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&row2,1,MPI_INT,0,MPI_COMM_WORLD);
    if (row2!=col1)
    {
        MPI_Finalize();
        return 0;
    }
    MPI_Bcast(&col2,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&partRows,1,MPI_INT,0,MPI_COMM_WORLD);
    int finRes[row1*col2];

    if (myRank!=0)
    {


        int *temp = malloc(row2*col2*sizeof(int));
        M2= malloc(row2*sizeof(int *));

        int i=0;
        for (;i<row2;i++)
            M2[i]= &temp[i*col2];

    }
    MPI_Bcast(M2[0],row2*col2,MPI_INT,0,MPI_COMM_WORLD);
    int toRecv [partRows*col1];
    MPI_Scatter(M1,partRows*col1,MPI_INT,toRecv,partRows*col1,MPI_INT,0,MPI_COMM_WORLD);

   
// calculate the mult ;
    int i=0;
    int j=0;
    int sum=0;
    int res[partRows*col2];
    int k=0;

     for (; i<partRows; i++)   // loop on rows per process
     {
        j=0;

        for (;j<col2;j++)
        {
            k=(i*col1);
            //printf("");
            sum=0;
            for (;k<(i*col1)+col1;k++)
            {
                sum+=toRecv[ k ]*M2[k%col1][j];
            }
            res[(i*col2)+j]=sum;
        }
     }

    MPI_Gather(res,partRows*col2,MPI_INT,finRes,partRows*col2,MPI_INT,0,MPI_COMM_WORLD);


    if (myRank==0)
    {

        i = partRows*p;

        for (;i< row1;i++)
        {
            j=0;

            for (;j<col2;j++)
            {
                k=(i*col1);
                sum=0;
                for (;k<(i*col1)+col1;k++)
                {
                    sum+=M1[ k ] * M2[k%col1][j];
                }
                finRes[(i*col2)+j]=sum;
            }
        }
        i=0;
        printf("\nResult Matrix: \n");
        for (;i<row1*col2;i++)
        {

            if (i%col2==0)
                printf("\n");

            printf("%d ", finRes[i]);
        }
        printf("\n");


    }







    MPI_Finalize();
}
