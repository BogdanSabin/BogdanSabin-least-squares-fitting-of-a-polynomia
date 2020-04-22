#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 3
 //must be computed
    double a[3];

void printMatrix(double** m, int number){
    number < 0 ? printf("\nM = \n") : printf("\nM%d = \n", number); 
    for (int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%f ",m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

double get3X3Det(double **m){
    double det = 0;
    double prod = 1;
    
    for (int i = 0; i < N; i++)
        prod *= m[i][i];
    det += prod;

    det += m[0][2] * m[1][0] * m[2][1];
    det += m[2][0] * m[0][1] * m[1][2];
    
    prod = 1;
    for (int i = 0; i < N; i++)
        prod *= m[i][N - i - 1];
    det -= prod;

    det -= m[0][1] * m[1][0] * m[2][2];
    det -= m[1][2] * m[2][1] * m[0][0];

    return det;
}

double getDetSwitch (int row, double ** m, double *b){
    double **mat = (double **)malloc(N * sizeof(double*));
    for(int i = 0; i < 3; i++) mat[i] = (double *)malloc(N * sizeof(double));

    
    for (int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(j == row) 
                mat[i][j] = b[i];
            else
                mat[i][j] = m[i][j];
        }
    }

    printMatrix(mat, row);
    return get3X3Det(mat);
}

void printFunc(){
    printf("y = %fx^2 ",a[2]);
    if(a[1] < 0)
        printf("%fx ",a[1]);
    else
        printf("+ %fx ",a[1]);
    
    if(a[0] < 0)
        printf("%f",a[0]);
    else
        printf("+ %f ",a[0]);
    printf("\n");
}

double func(double x){
    return a[2] * x * x + a[1] * x + a[0]; 
}

void evaluateFunc(double* v, int size){
    for(int i = 0; i < size; i ++)
        printf("f(%f) = %f\n",v[i], func(v[i]));
}

int main (int argc, char *argv[])
{
 if(argc >= 4){
     char * path = malloc(strlen(argv[1]) * sizeof(char));
     strcpy(path, argv[1]);
     
     int entries;
     sscanf(argv[2], "%d", &entries); //convert to int
     
     int numbersToEvaluateFromFile = 0;
     sscanf(argv[3], "%d", &numbersToEvaluateFromFile); //convert to int
     if(numbersToEvaluateFromFile < 0 || numbersToEvaluateFromFile > entries){
        printf("\033[1;31m"); //red output
        printf("numbersToEvaluateFromFile: %d is invalid min is 0 max is %d\n", numbersToEvaluateFromFile, entries);
        printf("\033[0m"); //reset to default color
        exit(-1);
     }
    
     double* x = malloc(entries * sizeof(double));
     double* y = malloc(entries * sizeof(double));

     double* vals;
     int thereAreCustomNumbersToEvaluate  = 0;
     if(argc > 4){ //have number to evaluate for fuction
        thereAreCustomNumbersToEvaluate = 1;
        int lenght = argc - 4;
        vals = malloc( lenght * sizeof(double));
        int k = 4; 
        for(int i = 0; i < lenght; i ++)
            sscanf(argv[k++], "%lf", &vals[i]); //convert to double
        
        for(int i = 0; i < lenght; i ++) 
            printf("vals(%d) = %f\n",i, vals[i]);
     } 

    FILE* file = fopen(path, "r");
    if (file < 0 || file == NULL){
        printf("Error while reading File\n");
        exit (0);
    }

    for (int  i = 0; i < entries; i++){
        fscanf(file,"%lf",&x[i]);
        fscanf(file," ");
        fscanf(file,"%lf",&y[i]);
    }
    
    printf("Data read from file: \n");
    for(int i = 0; i < entries; i++){
        printf("%f %f\n",x[i], y[i]);
    }
    
    //M*a=b where M -matrix
    //must be computed
    //double a[3];

    //compute b
    double b[N];
    for(int i = 0; i < numbersToEvaluateFromFile; i ++) b[0] += y[i];
    for(int i = 0; i < numbersToEvaluateFromFile; i ++) b[1] += x[i] * y[i];
    for(int i = 0; i < numbersToEvaluateFromFile; i ++) b[2] += x[i] * x[i] * y[i];
    
    printf("\nb0 = %f\n",b[0]);
    printf("b1 = %f\n",b[1]);
    printf("b2 = %f\n",b[2]);

    //compute matrix
    double **matrix = (double **)malloc(N * sizeof(double*));
    for(int i = 0; i < 3; i++) matrix[i] = (double *)malloc(N * sizeof(double));


    double sumXi = 0;
    double sumXiSquare = 0;
    double sumXiCube = 0;
    double sumXiTo4 = 0;

    for(int i = 0; i < numbersToEvaluateFromFile; i ++) sumXi += x[i];
    for(int i = 0; i < numbersToEvaluateFromFile; i ++) sumXiSquare += x[i] * x[i];
    for(int i = 0; i < numbersToEvaluateFromFile; i ++) sumXiCube += x[i] * x[i] * x[i];
    for(int i = 0; i < numbersToEvaluateFromFile; i ++) sumXiTo4 += x[i] * x[i] * x[i] * x[i];

    for (int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(i == 0 && j == 0 ) matrix[i][j] = numbersToEvaluateFromFile;
            else
                if(i == 2 && j == 2) matrix[i][j] = sumXiTo4;
                else
                    if(j == N - i -1) matrix[i][j] = sumXiSquare;
                    else
                     if( (i == 0 && j == 1) || (j == 0 && i == 1)) matrix[i][j] = sumXi;  
                     else
                          matrix[i][j] = sumXiCube;                       
                
        }
    }
    printMatrix(matrix, -1);

    double detM = get3X3Det(matrix);
    double detM0 = getDetSwitch(0, matrix, b);
    double detM1 = getDetSwitch(1, matrix, b);
    double detM2 = getDetSwitch(2, matrix, b);
    
    printf("detM = %f\n",detM);
    printf("detM0 = %f\n",detM0);
    printf("detM1 = %f\n",detM1);
    printf("detM2 = %f\n\n",detM2);

    a[0] = detM0/detM;
    a[1] = detM1/detM;
    a[2] = detM2/detM;
    
    printf("a0 = %f\n", a[0]);
    printf("a1 = %f\n", a[1]);
    printf("a2 = %f\n\n", a[2]);

    printFunc();
    if(thereAreCustomNumbersToEvaluate == 0){
        //some default vals
        printf("f(50) = %f\n", func(50));
        printf("f(300)= %f\n", func(300));
    }
    else
        evaluateFunc(vals, argc - 4);
 }
 else {
     printf("\033[1;31m"); //red output
     printf("USAGE: ./app.exe pathToDataFile numberOfEntriesInFile numbersToEvaluateFromFile [numbersToEvaluateFunction(optional) f(X)] \n");
     printf("\033[0;32m"); //green output
     printf("Ex: ./app.exe ./data/infected1.dat 6 6 3 4.5 2.4 9 0\n");
     printf("\033[0m"); //reset to default color
 }
 return 0;
}