#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int firstMatrixRows,firstMatrixColumns,secondMatrixRows,secondMatrixColumns;
int unsortedLength;


void matrixMultiplication();
void firstProcedureMultiply();
void secondProcedureMultiply();

void* firstMultiply();
void* secondMultiply();

void mergeSort();
void divideArray();
void merge();

void* mergeSortThread();

//parameters for thread of first procedure in matrix multiplication

typedef struct firstMultiplyPars{

    int i;
    int j;
    int arr1[100][100];
    int arr2[100][100];
    int result[100][100];

}firstMultiplyPars;

//parameters for thread of second procedure in matrix multiplication

typedef struct secondMultiplyPars{

    int i;
    int arr1[100][100];
    int arr2[100][100];
    int result[100][100];

}secondMultiplyPars;

//parameters for thread of merge sort

typedef struct mergeSortPars{

    int low;
    int high;
    int arr[100];

}mergeSortPars;


int main()
{

    matrixMultiplication();             //make matrix multiplication
    mergeSort();                        //make merge sort

    return 0;
}

void matrixMultiplication(){

//read file of matrices

    FILE *f;

    f = fopen("input.txt", "r");

    if(f == NULL){
        printf("error\n");
    }

//read first matrix

    fscanf(f,"%d",&firstMatrixRows);
    fscanf(f,"%d",&firstMatrixColumns);

    int firstMatrix[firstMatrixRows][firstMatrixColumns];

    int i,j;

    for(i=0;i<firstMatrixRows;i++){
        for(j=0;j<firstMatrixColumns;j++){

            fscanf(f,"%d",&firstMatrix[i][j]);

        }
    }

//read second matrix

    fscanf(f,"%d",&secondMatrixRows);
    fscanf(f,"%d",&secondMatrixColumns);

    int secondMatrix[secondMatrixRows][secondMatrixColumns];

    for(i=0;i<secondMatrixRows;i++){
        for(j=0;j<secondMatrixColumns;j++){

            fscanf(f,"%d",&secondMatrix[i][j]);

        }
    }

    fclose(f);

    if(firstMatrixColumns == secondMatrixRows){

        //implement first procedure

        firstProcedureMultiply(firstMatrix,secondMatrix);

        //implement second procedure

        secondProcedureMultiply(firstMatrix,secondMatrix);
    }

}


void firstProcedureMultiply(int arr1[][firstMatrixColumns],int arr2[][secondMatrixColumns]){

    int result[firstMatrixRows][secondMatrixColumns];

    int i,j;
	firstMultiplyPars pars;
    int m,n;
    for(m=0;m<firstMatrixRows;m++){
        for(n=0;n<firstMatrixColumns;n++){
            pars.arr1[m][n] = arr1[m][n];
        }
    }
    for(m=0;m<secondMatrixRows;m++){
        for(n=0;n<secondMatrixColumns;n++){
            pars.arr2[m][n] = arr2[m][n];
        }
    }

//calculate execution time of first procedure
	clock_t begin = clock();

    for(i=0;i<firstMatrixRows;i++){
        for(j=0;j<secondMatrixColumns;j++){

            //calculate each element of result array in thread
            pthread_t tid;

            pars.i = i;
            pars.j = j;

            int ret = pthread_create(&tid,NULL,firstMultiply,(void*)&pars);

            if(ret != 0){
                printf("error\n");
            }

            pthread_join(tid,NULL);

        }
    }

	clock_t end = clock();
    float time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

//print result array and time spent in output file

    FILE *f;

    f = fopen("output.txt", "a+");

    if(f == NULL){
        printf("error\n");
    }

    for(i=0;i<firstMatrixRows;i++){
        for(j=0;j<secondMatrixColumns;j++){
            fprintf(f,"%d ",pars.result[i][j]);

        }
    fprintf(f,"\n");
    }
	fprintf(f,"\n");
    fprintf(f,"%s   ","END1");
    fprintf(f,"%f s",time_spent);
    fprintf(f,"\n\n");

    fclose(f);


}


void secondProcedureMultiply(int arr1[][firstMatrixColumns],int arr2[][secondMatrixColumns]){

    int result[firstMatrixRows][secondMatrixColumns];

	secondMultiplyPars pars;

	int m,n;
    for(m=0;m<firstMatrixRows;m++){
        for(n=0;n<firstMatrixColumns;n++){
            pars.arr1[m][n] = arr1[m][n];
        }
    }
    for(m=0;m<secondMatrixRows;m++){
        for(n=0;n<secondMatrixColumns;n++){
            pars.arr2[m][n] = arr2[m][n];
        }
    }

    int i,j;

//calculate execution time of second procedure

	clock_t begin = clock();

    for(i=0;i<firstMatrixRows;i++){

    //calculate each row of result array in thread

        pthread_t tid;

        pars.i = i;

        int ret = pthread_create(&tid,NULL,secondMultiply,(void*)&pars);

        if(ret != 0){
            printf("error\n");
        }

        pthread_join(tid,NULL);
    }

	clock_t end = clock();
    float time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

//print result array and time spent in output file

    FILE *f;

    f = fopen("output.txt", "a+");

    if(f == NULL){
        printf("error\n");
    }

    for(i=0;i<firstMatrixRows;i++){
        for(j=0;j<secondMatrixColumns;j++){
            fprintf(f,"%d ",pars.result[i][j]);

        }
    fprintf(f,"\n");
    }
    fprintf(f,"\n");
    fprintf(f,"%s   ","END2");
    fprintf(f,"%f s",time_spent);
    fprintf(f,"\n\n");

    fclose(f);


}

void mergeSort(){

//read array from input file

    FILE *f;

    f = fopen("input2.txt", "r");

    if(f == NULL){
        printf("error\n");
    }

    fscanf(f,"%d",&unsortedLength);

    int arr[unsortedLength];

    int i;

    for(i=0;i<unsortedLength;i++){
        fscanf(f,"%d",&arr[i]);
    }

    fclose(f);

    //sort array

    divideArray(arr,0,unsortedLength-1);

    //print sorted array

    FILE *f1;

    f1 = fopen("output2.txt", "a+");

    if(f1 == NULL){
        printf("error\n");
    }

    for(i=0;i<unsortedLength;i++){
        fprintf(f1,"%d ",arr[i]);
    }
    fprintf(f1,"\n");


    fclose(f1);

}

void divideArray(int arr[],int low,int high){

    int mid;
	mergeSortPars pars;
	int i;

	for(i=0;i<unsortedLength;i++){
		pars.arr[i] = arr[i];

	}
    if(low<high){
        mid = (high + low)/2;

    //sort first half of array in thread
	pthread_t tid;


    pars.low = low;
	pars.high = mid;

    int ret = pthread_create(&tid,NULL,mergeSortThread,(void*)&pars);

    if(ret != 0){
        printf("error\n");
    }

    pthread_join(tid,NULL);


    //sort second half of array in thread

	pthread_t tid2;

    pars.low = mid+1;
	pars.high = high;

    int ret2 = pthread_create(&tid2,NULL,mergeSortThread,(void*)&pars);

    if(ret2 != 0){
        printf("error\n");
    }

    pthread_join(tid2,NULL);

	for(i=0;i<unsortedLength;i++){
		arr[i] = pars.arr[i];

	}

    merge(arr,low,mid,high);


    }




}

void merge(int arr[],int low,int mid,int high){


    int tmp[high-low+1];

    int i = low;

    int j = mid+1;
    int r=0;

    while(i<=mid && j<=high){
        if(arr[i]<=arr[j]){
            tmp[r] = arr[i];
            i++;
        }
        else{
            tmp[r] = arr[j];
            j++;
        }
        r++;
    }

    if(i>mid){
        while(j<=high){
            tmp[r] = arr[j];
            r++;
            j++;
        }

    }
    else{
        while(i<=mid){
            tmp[r] = arr[i];
            r++;
            i++;
        }


    }


    int k=low;
    int s=0;
    while(k<=high){
        arr[k] = tmp[s];
        k++;
        s++;

    }


}

//thread to implement first procedure matrix multiplication

void* firstMultiply(void* parameter){

    firstMultiplyPars *pars = (firstMultiplyPars*)parameter;

    int k,tmp=0;
    int m,n;

    int i = pars->i;
    int j = pars->j;

    for(k=0;k<firstMatrixColumns;k++){
        tmp  = tmp + (pars->arr1[i][k] * pars->arr2[k][j]);
    }

	pars->result[i][j]=tmp;

}

//thread to implement second procedure matrix multiplication

void* secondMultiply(void* parameter){

    secondMultiplyPars *pars = (secondMultiplyPars*)parameter;

    int j,k,tmp;
    int m,n;

    int i = pars->i;

    for(j=0;j<secondMatrixColumns;j++){
        tmp=0;
        for(k=0;k<firstMatrixColumns;k++){
            tmp  = tmp + (pars->arr1[i][k] * pars->arr2[k][j]);
        }
        pars->result[i][j]=tmp;
    }

}

//thread to implement merge sort

void* mergeSortThread(void* parameter){

	mergeSortPars *pars = (mergeSortPars*)parameter;

	int low = pars->low;
	int high = pars->high;
	int arr[unsortedLength];
	int i;

	for(i=0;i<unsortedLength;i++){
		arr[i] = pars->arr[i];

	}

	divideArray(arr,low,high);

	for(i=0;i<unsortedLength;i++){
		pars->arr[i] = arr[i];

	}


}


