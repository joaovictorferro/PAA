#include<stdio.h>
#include<stdlib.h>

int max_size = 1000;
int** create(int,FILE*,int,int);
int** createZeroMatrix(int);
int** strassensMultiplication(int**, int**,int);
int** strassensMultRec(int**, int**,int n);
int** divide(int** matrixA,int n, int row,int col);
void printMatrix(int**, int,int,int);
int** addMatrix(int**,int**,int);
int** subMatrix(int**,int**,int);
void compose(int**,int**,int,int,int);

int main() {
	int i=0,j=0,n=0;
	int n1,n2,n3,n4;
	int pow = 1;

	FILE *in;
	in = fopen("in.txt", "r");

	if (in == NULL){
   		printf("Nao abriu\n");
   		return 0;
	}

	fscanf(in,"%d %d %d %d",&n1,&n2,&n3,&n4);
	
	if (n2 != n3){
		printf("Numero de colunas de A é diferente do numero de colunas de B\n\n");
		return 0;
	}

	if(n1 >= n2 && n1 >= n3 && n1 >= n4){
		n = n1;
	}else if(n2 >= n3 && n2 >= n4){
		n = n2;
	}else if(n3 >= n4){
		n = n3;
	}else{
		n = n4;
	}

	while(pow < n){
		pow *= 2;
	}

	int** matrixA = create(pow,in,n1,n2);
	int** matrixB = create(pow,in,n3,n4);
	n = pow;
	int ** standardRes,**strassenRes;

	int ** strassensRes = strassensMultiplication(matrixA,matrixB,n);
	printMatrix(strassensRes,n,n1,n4);

	fclose(in);
	return 0;
}

int ** create(int pow,FILE *in,int m,int n){
	int ** array = createZeroMatrix(pow);	
	int i,j;
	int k;
	for(i = 0;i < m; i++) {
   	 	for(j = 0; j < n; j++) {
	    		fscanf(in,"%d",&k);
	    		array[i][j] = k;
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

void printMatrix(int ** matrix,int n,int n1,int n4){
	int i,j;
	FILE* out;
	out = fopen("out.txt","w");
	if(out == NULL){
		printf("Não existe o arquivo\n");
		return ;
	}
	fprintf(out,"17212343\n");
	fprintf(out,"%d %d\n",n1,n4);
	for(i=0;i<n1;i++){
		for(j=0;j<n4;j++){
			if (j != 0){
				fprintf(out, " ");
			}
			fprintf(out,"%d",matrix[i][j]);
		}
		fprintf(out,"\n");
	}
	fclose(out);
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
