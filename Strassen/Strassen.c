#include<stdio.h>
#include<stdlib.h>

int max_size = 1000;
int** create(int,int);
int** createZeroMatrix(int);
int** strassensMultiplication(int**, int**,int);
int** standardMultiplication(int**,int**,int);
int** strassensMultRec(int**, int**,int n);
int** divide(int** matrixA,int n, int row,int col);
void printMatrix(int**, int);
int** addMatrix(int**,int**,int);
int** subMatrix(int**,int**,int);
void compose(int**,int**,int,int,int);

int main() {
	int i=0,j=0,n=0;

	printf("Digite o tamanho da matriz:\n");
	scanf("%d",&n);

	int pow = 1;
	while(pow < n){
		pow *= 2;
	}
	int** matrixA = create(n,pow);
	int** matrixB = create(n,pow);
	n = pow;
	int ** standardRes,**strassenRes;
	
	printf("\nMatrix A\n");
	printMatrix(matrixA,n);
	
	printf("\nMatrix B\n");
	printMatrix(matrixB,n);
	
	int ** stdRes = standardMultiplication(matrixA,matrixB,n);
	printf("\nStandard Multiplication Output:\n");
	printMatrix(stdRes,n);

	printf("\nStrassen's Multiplication Output:\n");
	int ** strassensRes = strassensMultiplication(matrixA,matrixB,n);
	printMatrix(strassensRes,n);

	return 0;
}

int ** create(int n,int pow){
	int ** array = createZeroMatrix(pow);	
	int i,j;
	for(i = 0;i < n; i++) {
   	 	for(j = 0; j < n; j++) {
	    		array[i][j] = rand() % max_size;
			}
	}
	return array;
} 

int** createZeroMatrix(int n){
	int ** array = (int**)malloc(n*sizeof(int *));	
	int i,j;
	for(i = 0;i < n; i++) {
	    	array[i] = (int*)malloc(n*sizeof(int));
   	 	for(j = 0; j < n; j++) {
	        	array[i][j] = 0.0;
	    	}
	}
	return array;
} 

void printMatrix(int ** matrix,int n) {
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			printf("%d ",matrix[i][j]);
		}
		printf("\n");
	}
}

int ** standardMultiplication(int ** matrixA,int ** matrixB,int n) {
	int ** result;
	int i,j,k;

	result = (int**)malloc(n*sizeof(int *));
	for(i=0;i<n;i++){
		result[i]=(int*)malloc(n*sizeof(int));
		for(j=0;j<n;j++){
			result[i][j]=0;
			for(k=0;k<n;k++) {
				result[i][j]=result[i][j]+(matrixA[i][k]*matrixB[k][j]);
			}		
		}
	}
	return result;
}

int ** strassensMultiplication(int** matrixA, int** matrixB,int n){
	int ** result = strassensMultRec(matrixA,matrixB,n);
	return result;
}

int** strassensMultRec(int** matrixA, int** matrixB,int n){
	int** result = createZeroMatrix(n);
	
	if(n>1) {
		
		int** a11 = divide(matrixA, n, 0, 0);
		int** a12 = divide(matrixA, n, 0, (n/2));
		int** a21 = divide(matrixA, n, (n/2), 0);
		int** a22 = divide(matrixA, n, (n/2), (n/2));	
		int** b11 = divide(matrixB, n, 0, 0);
		int** b12 = divide(matrixB, n, 0, n/2);
		int** b21 = divide(matrixB, n, n/2, 0);
		int** b22 = divide(matrixB, n, n/2, n/2);
		
		int** m1= strassensMultRec(addMatrix(a11,a22,n/2),addMatrix(b11,b22,n/2),n/2);
		int** m2= strassensMultRec(addMatrix(a21,a22,n/2),b11,n/2);
		int** m3= strassensMultRec(a11,subMatrix(b12,b22,n/2),n/2);
		int** m4= strassensMultRec(a22,subMatrix(b21,b11,n/2),n/2);
		int** m5= strassensMultRec(addMatrix(a11,a12,n/2),b22,n/2);
		int** m6= strassensMultRec(subMatrix(a21,a11,n/2),addMatrix(b11,b12,n/2),n/2);
		int** m7= strassensMultRec(subMatrix(a12,a22,n/2),addMatrix(b21,b22,n/2),n/2);

		int** c11 = addMatrix(subMatrix(addMatrix(m1,m4,n/2),m5,n/2),m7,n/2);
		int** c12 = addMatrix(m3,m5,n/2);
		int** c21 = addMatrix(m2,m4,n/2);
		int** c22 = addMatrix(subMatrix(addMatrix(m1,m3,n/2),m2,n/2),m6,n/2);
		//Compose the matrix
		compose(c11,result,0,0,n/2);
		compose(c12,result,0,n/2,n/2);
		compose(c21,result,n/2,0,n/2);
		compose(c22,result,n/2,n/2,n/2);
	} 
	else {
		//This is the terminating condition for recurssion.
		result[0][0]=matrixA[0][0]*matrixB[0][0];
	}
	return result;
}

void compose(int** matrix,int** result,int row,int col,int n){
	int i,j,r=row,c=col;
	for(i=0;i<n;i++){
		c=col;
		for(j=0;j<n;j++){
			result[r][c]=matrix[i][j];	
			c++;	
		}
		r++;
	}
}

int** divide(int** matrix,int n, int row,int col) {
	int n_new=n/2;	
	
	int ** array = createZeroMatrix(n_new);	
	int i,j,r=row,c=col;
	for(i = 0;i < n_new; i++) {
		c=col;
   	 	for(j = 0; j < n_new; j++) {
        		array[i][j] = matrix[r][c];
			c++;
    		}
		r++;
	}
	return array;
}

int ** addMatrix(int** matrixA,int** matrixB,int n){
	int ** res = createZeroMatrix(n);
	int i,j;	
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			res[i][j]=matrixA[i][j]+matrixB[i][j];
	
	return res;
}

int** subMatrix(int** matrixA,int** matrixB,int n){
	int ** res = createZeroMatrix(n);
	int i,j;	
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			res[i][j]=matrixA[i][j]-matrixB[i][j];
	
	return res;
}