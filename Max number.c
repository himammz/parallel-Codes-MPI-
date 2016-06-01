
#include <mpich/mpi.h>
#include <stdio.h>

int main(void) {


	int pNum,myRank;
	int arr[100];
	int part,rem;
	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,&pNum);
	MPI_Comm_rank(MPI_COMM_WORLD,&myRank);
	int MAXn=0,MaxINd=-1;
	int c;
	MPI_Status status;
	if (myRank!=0)
	{		
		MPI_Recv(&part,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(arr,part,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&c,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		int i=0;
		int newpart=part-(c>0);
		for (i;i<part;i++)
		{
			if (arr[i]>MAXn)
			{
				MaxINd=(myRank-1)*(newpart)+c-(c>0)+i;
				MAXn=arr[i];
			}

		}
		MPI_Send(&MaxINd,1,MPI_INT,0,0,MPI_COMM_WORLD);
		MPI_Send(&MAXn,1,MPI_INT,0,0,MPI_COMM_WORLD);
	}
	else
	{
		int n;
		scanf("%d",&n);
		int i=0;
		for (i;i<n;i++)
		{
			scanf("%d",&arr[i]);
		}
		i=1;
		part = n/(pNum-1);
		rem=n%(pNum-1);
		int ch =0;
		int newPart= part+1;
		int count =0;
		for (i;i<pNum;i++)
		{

			if ((pNum-i==rem||ch>0)&&rem!=0)
			{
				MPI_Send(&newPart,1,MPI_INT,i,0,MPI_COMM_WORLD);

				MPI_Send(&arr[((i-1)*part)+ch],newPart,MPI_INT,i,0,MPI_COMM_WORLD);
				ch+=1;
				MPI_Send(&ch,1,MPI_INT,i,0,MPI_COMM_WORLD);

			}
			else
			{
				MPI_Send(&part,1,MPI_INT,i,0,MPI_COMM_WORLD);

				MPI_Send(&arr[(i-1)*part],part,MPI_INT,i,0,MPI_COMM_WORLD);
				MPI_Send(&ch,1,MPI_INT,i,0,MPI_COMM_WORLD);
			}
		}
		i=1;
		int ind,num;
		for (i;i<pNum;i++)
		{
			MPI_Recv(&ind,1,MPI_INT,i,0,MPI_COMM_WORLD,&status);
			MPI_Recv(&num,1,MPI_INT,i,0,MPI_COMM_WORLD,&status);
			if (num>MAXn)
			{
				MaxINd =ind ;
				MAXn=num;
			}
			
		}

		printf ("Max number in index %d with value %d \n",MaxINd,MAXn );
	}
	MPI_Finalize();
	return 0;
}

