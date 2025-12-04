#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define  MAX_ITERATION 10000
#define PI 3.1415

struct FuncPart;

typedef enum {eNull,eNumber,ePoly,eTrig,eArcTrig,eLog,eExpo,ePara}TermType;

union Term;
struct Poly; // xˆ(), x, 5xˆ2...
struct Trig; // sin(), cos()...
struct ArcTrig; // arcsin(),arccos()...
struct Log; // log_5(), log_()(), log_x()...
struct Expo; // (5+x)ˆ(sin(4x)), 5ˆ()...

union Term{
    double num;
    struct Trig *trig;
    struct ArcTrig *arcTrig;
    struct Poly *pol;
    struct Log *log;
    struct Expo *expo;
    struct FuncPart *inPara; // parantez onceligi icin gerekli
};

struct FuncPart{ // farkli fonksiyon tipleri icin linked list

    TermType type;
    union Term term;

    struct FuncPart *addedWith;
    struct FuncPart *multipWith;
    int negative;
    int inverse;
};

struct Poly{ // xˆ2 , 2xˆ2...
    struct FuncPart *coefficent;
    struct FuncPart *power;
};

struct Trig{
    struct FuncPart *inside;
    int mode; // 0-sin 1-cos 2-tan 3-cot
};

struct ArcTrig{
    struct FuncPart *inside;
    int mode; // 0-arcsin 1-arccos 2-arctan 3-arccot
};

struct Log{
    struct FuncPart *inside;
    struct FuncPart *base;
};

struct Expo{
    struct FuncPart *expo;
    struct FuncPart *base;
};

int doesStringStartWith(char* testString,int size,char* stringLiteral);
int findMatchingParanthesis(char* text,int size);
int findMatchingParanthesisReverse(char* text,int size);

double evaluateFunc(struct FuncPart *func,double valOfX);
double evaluateTrig(struct Trig *trig,double valOfX);
double evaluateArcTrig(struct ArcTrig *arcTrig,double valOfX);
double evaluatePoly(struct Poly *poly,double valOfX);
double evaluateExpo(struct Expo *expo,double valOfX);
double evaluateLog(struct Log *loga,double valOfX);

void freeFunc(struct FuncPart *func);
void freeTrig(struct Trig *trig);
void freeArcTrig(struct ArcTrig *arcTrig);
void freePoly(struct Poly *poly);
void freeExpo(struct Expo *expo);
void freeLog(struct Log *log);

struct FuncPart* getDynamicEmptyFunc();
struct Log* getDynamicEmptyLog();
struct Trig* getDynamicEmptyTrig();
struct ArcTrig* getDynamicEmptyArcTrig();
struct Poly* getDynamicEmptyPoly();
struct Expo* getDynamicEmptyExpo();

struct FuncPart* getFuncFromString();
struct FuncPart* cutMultips(char* text,int size,int negative);
struct FuncPart* cutAdditions(char* text,int size);
struct FuncPart* convertTermToStruct(char* text,int size,int inverse,int negative);

int handleCutTrig(struct FuncPart** newStruct,char* text,int size, int mode);
int handleCutArcTrig(struct FuncPart** newStruct,char* text,int size, int mode);

double** makeSquareMatrix(int size);
double** getMatrixFromUser(int size);
double** makeIdentityMatrix(int size);
void releaseSquareMatrix(double** matrix, int size);
void printSquareMatrix(double** matrix, int size);
void multiplyRow(double** matrix,int size,int row,double mult);
void multAndAddRowFromRow(double** matrix,int size,int startRow,int targetRow,double mult);
void swapRows(double** matrix,int size,int firstRow, int secondRow);
void makeDiagonalMax(double** matrix,double* unknowns, int size);
double findDeterminant(double** matrix,int size);

void bisection(struct FuncPart *func,double lowerLimit,double higherLimit,double error,int maxLoop);
void regulaFalsi(struct FuncPart *func, double lowerLimit,double upperLimit,double error,int maxLoop);
void newtonRaphson(struct FuncPart  *func, double startingPoint,double error,int maxLoop);

double backwardsDerivative(struct FuncPart *func,double x, double h);
double centerDerivative(struct FuncPart *func,double x, double h);
double forwardDerivative(struct FuncPart *func,double x, double h);

double trapez(struct FuncPart *func,double lowerLimit,double upperLimit,int numberOfInter);
double simpsonOneThree(struct FuncPart *func,double lowerLimit,double upperLimit,int numberOfInter);
double simpsonThreeEight(struct FuncPart *func,double lowerLimit,double upperLimit,int numberOfInter);

double** inverseMatrix(double** matrix,int size);
void cholesky(double** matrix,int size,double* constants);
void gaussSeidel(double** matrix,int size,double* unknowns,double* startingPoints,double error,int maxIteration);

void gregoryNewton(double startingPoint,double stepSize,int size,double* yValues,double xVal);

double factorial(int val);

int main()
{
    int mode;
    printf("Please choose a mode:\n\n"
           "1 - Bisection\n"
           "2 - Regula-Falsi\n"
           "3 - Newton-Raphson\n"
           "4 - Inverse of an NxN Matrix\n"
           "5 - Cholesky\n"
           "6 - Gauss Seidel\n"
           "7 - Numeric Derivative\n"
           "8 - Simpson\n"
           "9 - Trapez\n"
           "10 - Gregory Newton\n");
    scanf("%d",&mode);
    while(mode < 1 || mode > 11){
        printf("Invalid input! Please try again!\n");
        scanf("%d",&mode);
    }
    if (mode == 1) {
        double error, lowerLimit, upperLimit;
        struct FuncPart *func = getFuncFromString();
        printf("Please enter the error: ");
        scanf("%lf", &error);
        printf("Please enter the lower limit: ");
        scanf("%lf", &lowerLimit);
        printf("Please enter the upper limit: ");
        scanf("%lf", &upperLimit);
        bisection(func,lowerLimit,upperLimit,error,MAX_ITERATION);
    }
    else if (mode == 2) {
        double error, lowerLimit, upperLimit;
        struct FuncPart *func = getFuncFromString();
        printf("Please enter the error: ");
        scanf("%lf", &error);
        printf("Please enter the lower limit: ");
        scanf("%lf", &lowerLimit);
        printf("Please enter the upper limit: ");
        scanf("%lf", &upperLimit);
        regulaFalsi(func,lowerLimit,upperLimit,error,MAX_ITERATION);
    }
    else if (mode == 3) {
        double error,startingPoint;
        struct FuncPart *func = getFuncFromString();
        printf("Please enter the error: ");
        scanf("%lf", &error);
        printf("Please enter the starting point: ");
        scanf("%lf", &startingPoint);
        newtonRaphson(func,startingPoint,error,MAX_ITERATION);
    }
    else if (mode == 4) {
        int size;
        printf("Please enter the size of the matrix: ");
        scanf("%d",&size);
        double **matrix = getMatrixFromUser(size);
        if(fabs(findDeterminant(matrix,size)) < 0.000000001){
            printf("This matrix has no inverse. (The determinant is 0)");
        }
        else{
            double **inverse = inverseMatrix(matrix,size);
            printf("Inverse matrix: \n");
            printSquareMatrix(inverse,size);
            releaseSquareMatrix(inverse,size);
        }
        releaseSquareMatrix(matrix,size);
    }
    else if (mode == 5) {
        int size,i;
        printf("Please enter the size of the matrix: ");
        scanf("%d",&size);
        double* unknowns = (double*)calloc(size,sizeof(double));
        double* startingPoints = (double*)calloc(size,sizeof(double));
        double **matrix = getMatrixFromUser(size);
        printf("Please enter the constants:\n");
        for(i=0;i<size;i++){
            printf("%dth row: ",i+1);
            scanf("%lf",&unknowns[i]);
        }
        cholesky(matrix,size,unknowns);
    }
    else if (mode == 6) {
        int size,i;
        double error;
        printf("Please enter the size of the matrix: ");
        scanf("%d",&size);
        double* unknowns = (double*)calloc(size,sizeof(double));
        double* startingPoints = (double*)calloc(size,sizeof(double));
        double **matrix = getMatrixFromUser(size);
        printf("Please enter the constants:\n");
        for(i=0;i<size;i++){
            printf("%dth row: ",i+1);
            scanf("%lf",&unknowns[i]);
        }
        printf("Please enter the starting points:\n");
        for(i=0;i<size;i++){
            printf("%dth row: ",i+1);
            scanf("%lf",&startingPoints[i]);
        }
        printf("Please enter the error: ");
        scanf("%lf", &error);
        gaussSeidel(matrix,size,unknowns,startingPoints,error,MAX_ITERATION);
        free(unknowns);
        free(startingPoints);
        releaseSquareMatrix(matrix,size);
    }
    else if (mode == 7) {
        double stepSize,xVal;
        struct FuncPart *func = getFuncFromString();
        printf("Please enter the step size: ");
        scanf("%lf", &stepSize);
        printf("Please enter the point: ");
        scanf("%lf", &xVal);
        printf("\nBackward: %lf\nCenteral: %lf\nForward: %lf\n",backwardsDerivative(func,xVal,stepSize),centerDerivative(func,xVal,stepSize),forwardDerivative(func,xVal,stepSize));

    }
    else if (mode == 8) {
        double lowerLimit, upperLimit,intervalOneThree,intervalThreeEight;
        struct FuncPart *func = getFuncFromString();
        printf("Please enter the lower limit: ");
        scanf("%lf", &lowerLimit);
        printf("Please enter the upper limit: ");
        scanf("%lf", &upperLimit);
        printf("Please enter the interval count for 1/3 (Should be a multiple of 2): ");
        scanf("%lf", &intervalOneThree);
        printf("Please enter the N for 3/8: ");
        scanf("%lf", &intervalThreeEight);
        printf("\n1/3 Result: %lf\n3/8 Result: %lf\n",simpsonOneThree(func,lowerLimit,upperLimit,intervalOneThree),simpsonThreeEight(func,lowerLimit,upperLimit,intervalThreeEight));
    }
    else if (mode == 9) {
        double lowerLimit, upperLimit,intervalCount;
        struct FuncPart *func = getFuncFromString();
        printf("Please enter the lower limit: ");
        scanf("%lf", &lowerLimit);
        printf("Please enter the upper limit: ");
        scanf("%lf", &upperLimit);
        printf("Please enter the interval count: ");
        scanf("%lf", &intervalCount);
        printf("\nResult: %lf\n",trapez(func,lowerLimit,upperLimit,intervalCount));
    }
    else if (mode == 10) {
        int i;
        double startingPoint, stepSize, stepCount,xVal;
        printf("Please enter the starting point: ");
        scanf("%lf", &startingPoint);
        printf("Please enter the difference between x values: ");
        scanf("%lf", &stepSize);
        printf("Please enter the amount of x values: ");
        scanf("%lf", &stepCount);
        double *yValues = calloc(stepCount,sizeof(double));
        for(i = 0;i < stepCount;i++){
            printf("Please enter the y value for x=%lf: ",(stepSize*i+startingPoint));
            scanf("%lf",&yValues[i]);
        }
        printf("Please enter the x value to interpolate: ");
        scanf("%lf", &xVal);
        gregoryNewton(startingPoint,stepSize,stepCount,yValues,xVal);
        free(yValues);
    }
    else if (mode == 11) { // Debug Mode
        struct FuncPart *func = getFuncFromString();
        double x;
        printf("Value of x: ");
        scanf("%lf", &x);
        printf("\n%lf", evaluateFunc(func, x));
    }
    return 0;
}

int doesStringStartWith(char* testString,int size,char* stringLiteral){
    int i,literalSize = strlen(stringLiteral);
    if(size < literalSize){
        return 0;
    }
    for(i = 0; i < literalSize;i++){
        if(testString[i] != stringLiteral[i]){
            return 0;
        }
    }
    return 1;
}

int findMatchingParanthesis(char* text,int size){
    int parCount = 1,j = 1;
    while(parCount > 0 && j < size){
        if(text[j] == '('){
            parCount++;
        }
        else if(text[j] == ')'){
            parCount--;
        }
        j++;
    }
    if(parCount>0){
        return -1;
    }
    return (j-1);
}

int findMatchingParanthesisReverse(char* text,int size){
    int parCount = 1,j = size-2;
    while(parCount > 0 && j > 0){
        if(text[j] == ')'){
            parCount++;
        }
        else if(text[j] == '('){
            parCount--;
        }
        j--;
    }
    if(parCount>0){
        return -1;
    }
    return (j+1);
}

double evaluateFunc(struct FuncPart *func,double valOfX){
    double result = 0;
    if(func->type == eNull){
        printf("DEBUG LOG:FuncPart struct's type is eNull.");
        return 0;
    }

    if(func->type == eLog){
        result += evaluateLog(func->term.log,valOfX);
    }
    else if(func->type == eTrig){
        result += evaluateTrig(func->term.trig,valOfX);
    }
    else if(func->type == eArcTrig){
        result += evaluateArcTrig(func->term.arcTrig,valOfX);
    }
    else if(func->type == ePoly){
        result += evaluatePoly(func->term.pol,valOfX);
    }
    else if(func->type == ePara){
        result += evaluateFunc(func->term.inPara,valOfX);
    }
    else if(func->type == eExpo){
        result += evaluateExpo(func->term.expo,valOfX);
    }
    else if(func->type == eNumber){
        result += func->term.num;
    }
    if(func->negative){
        result *= -1;
    }
    if(func->inverse){
        if(result == 0){
            printf("\nDivide by 0 error.\n");
            return NAN;
        }
        else{
            result = 1 / result;
        }
    }
    if(func->multipWith != NULL){
        result *= evaluateFunc(func->multipWith,valOfX);
    }
    if(func->addedWith != NULL){
        result += evaluateFunc(func->addedWith,valOfX);
    }
    return result;
}

double evaluateTrig(struct Trig *trig,double valOfX){
    if(trig->mode == 0){
        return sin(evaluateFunc(trig->inside,valOfX));
    }
    else if(trig->mode == 1){
        return cos(evaluateFunc(trig->inside,valOfX));
    }
    else if(trig->mode == 2){
        return tan(evaluateFunc(trig->inside,valOfX));
    }
    else if(trig->mode == 3){
        return (1/tan(evaluateFunc(trig->inside,valOfX)));
    }
    return 0;
}

double evaluateArcTrig(struct ArcTrig *arcTrig,double valOfX){
    if(arcTrig->mode == 0){
        return asin(evaluateFunc(arcTrig->inside,valOfX));
    }
    else if(arcTrig->mode == 1){
        return acos(evaluateFunc(arcTrig->inside,valOfX));
    }
    else if(arcTrig->mode == 2){
        return atan(evaluateFunc(arcTrig->inside,valOfX));
    }
    else if(arcTrig->mode == 3){
        return (1/atan(evaluateFunc(arcTrig->inside,valOfX)));
    }
    return 0;
}

double evaluatePoly(struct Poly *poly,double valOfX){
    return evaluateFunc(poly->coefficent,valOfX) * pow(valOfX,evaluateFunc(poly->power,valOfX));
}

double evaluateExpo(struct Expo *expo,double valOfX){
    return pow(evaluateFunc(expo->base,valOfX),evaluateFunc(expo->expo,valOfX));
}

double evaluateLog(struct Log *loga,double valOfX){
    return (log(evaluateFunc(loga->inside,valOfX))/log(evaluateFunc(loga->base,valOfX)));
}

void freeFunc(struct FuncPart *func){
    if(func == NULL){
        return;
    }
    else if(func->type == eLog){
        freeLog(func->term.log);
    }
    else if(func->type == eTrig){
        freeTrig(func->term.trig);
    }
    else if(func->type == eArcTrig){
        freeArcTrig(func->term.arcTrig);
    }
    else if(func->type == ePoly){
        freePoly(func->term.pol);
    }
    else if(func->type == ePara){
        freeFunc(func->term.inPara);
    }
    else if(func->type == eExpo){
        freeExpo(func->term.expo);
    }
    if(func->multipWith != NULL){
        freeFunc(func->multipWith);
    }
    if(func->addedWith != NULL){
        freeFunc(func->addedWith);
    }
    free(func);
}

void freeTrig(struct Trig *trig){
    freeFunc(trig->inside);
    free(trig);
}
void freeArcTrig(struct ArcTrig *arcTrig){
    freeFunc(arcTrig->inside);
    free(arcTrig);
}
void freePoly(struct Poly *poly){
    freeFunc(poly->coefficent);
    freeFunc(poly->power);
    free(poly);
}
void freeExpo(struct Expo *expo){
    freeFunc(expo->base);
    freeFunc(expo->expo);
    free(expo);
}
void freeLog(struct Log *log){
    freeFunc(log->base);
    freeFunc(log->inside);
    free(log);
}

struct FuncPart* getDynamicEmptyFunc(){
    struct FuncPart *newStruct;
    newStruct = (struct FuncPart*)malloc(sizeof(struct FuncPart));
    newStruct->type = eNull;
    newStruct->inverse = 0;
    newStruct->negative = 0;
    newStruct->addedWith = NULL;
    newStruct->multipWith = NULL;
    return newStruct;
}

struct Log* getDynamicEmptyLog(){
    struct Log *newStruct;
    newStruct = (struct Log*)malloc(sizeof(struct Log));
    newStruct->base = NULL;
    newStruct->inside = NULL;
    return newStruct;
}

struct Trig* getDynamicEmptyTrig(){
    struct Trig *newStruct;
    newStruct = (struct Trig*)malloc(sizeof(struct Trig));
    newStruct->inside = NULL;
    newStruct->mode = -1;
    return newStruct;
}

struct ArcTrig* getDynamicEmptyArcTrig(){
    struct ArcTrig *newStruct;
    newStruct = (struct ArcTrig*)malloc(sizeof(struct ArcTrig));
    newStruct->inside = NULL;
    newStruct->mode = -1;
    return newStruct;
}

struct Poly* getDynamicEmptyPoly(){
    struct Poly *newStruct;
    newStruct = (struct Poly*)malloc(sizeof(struct Poly));
    newStruct->coefficent = NULL;
    newStruct->power = NULL;
    return newStruct;
}

struct Expo* getDynamicEmptyExpo(){
    struct Expo *newStruct;
    newStruct = (struct Expo*)malloc(sizeof(struct Expo));
    newStruct->base = NULL;
    newStruct->expo = NULL;
    return newStruct;
}

struct FuncPart* getFuncFromString(){
    char text[200];
    struct FuncPart *func;
    do{
        printf("Please enter the function: ");
        scanf("%s",text);
        func = cutAdditions(text,strlen(text));
        if(func == NULL){
            printf("Invalid syntax. Please try again.\n");
        }
    }while(func == NULL);
    return func;
}


struct FuncPart* cutAdditions(char* text,int size){
    struct FuncPart *head = getDynamicEmptyFunc();
    struct FuncPart *tail = head;
    head->term.num = 0;
    head->type = eNumber;
    int i,lastIndex = 0, nextIsNegative = 0;
    for(i = 0;i < size; i++){
        if(text[i] == '('){
            int offset = findMatchingParanthesis(text+i,size-i);
            if(offset==-1){
                printf("Invalid Syntax!\n");
                freeFunc(head);
                return NULL;
            }
            i=(offset+i)-1;
        }
        else if(text[i] == '+' || text[i] == '-'){
            if(i != 0){
                struct FuncPart *temp = cutMultips((text+lastIndex),i-lastIndex,nextIsNegative);
                if(temp == NULL){
                    freeFunc(head);
                    return NULL;
                }
                tail->addedWith = temp;
                tail = temp;
            }
            lastIndex = i+1;
            nextIsNegative = (text[i] ==  '-');
        }
        else if(i == size-1){
            struct FuncPart *temp = cutMultips((text+lastIndex),i-lastIndex+1,nextIsNegative);
            if(temp == NULL){
                freeFunc(head);
                return NULL;
            }
            tail->addedWith = temp;
            tail = temp;
        }
    }
    return head;
}


struct FuncPart* cutMultips(char* text,int size,int negative){
    struct FuncPart *head = getDynamicEmptyFunc();
    struct FuncPart *tail = head;
    head->term.num = 1;
    head->type = eNumber;
    int i,lastIndex = 0, nextIsInverse = 0, nextIsNegative = negative;
    for(i = 0;i < size; i++){
        if(text[i] == '('){
            int offset = findMatchingParanthesis(text+i,size-i);
            if(offset==-1){
                printf("Invalid input! Please try again.\n");
                freeFunc(head);
                return NULL;
            }
            i=(offset+i)-1;
        }
        else if(text[i] == '*' || text[i] == '/'){
            struct FuncPart *temp = convertTermToStruct((text+lastIndex),i-lastIndex,nextIsInverse,nextIsNegative);
            if(temp == NULL){
                freeFunc(head);
                return NULL;
            }
            tail->multipWith = temp;
            tail = temp;

            lastIndex = i+1;
            nextIsInverse = (text[i] == '/');
            nextIsNegative = 0;

        }
        else if(i == size-1){
            struct FuncPart *temp = convertTermToStruct((text+lastIndex),i-lastIndex+1,nextIsInverse,nextIsNegative);
            if(temp == NULL){
                freeFunc(head);
                return NULL;
            }
            tail->multipWith = temp;
            tail = temp;
        }
    }
    return head;
}


struct FuncPart* convertTermToStruct(char* text,int size,int inverse,int negative){
    struct FuncPart *newStruct;
    if(doesStringStartWith(text,size,"log_")){
        if(text[size-1] == ')'){
            int index = findMatchingParanthesisReverse(text,size);
            if(index == -1){
                return NULL;
            }
            struct FuncPart* base = convertTermToStruct(text+4,index-4,0,0);
            struct FuncPart* inside = convertTermToStruct(text+index,size-index,0,0);
            if(base != NULL && inside != NULL){
                newStruct = getDynamicEmptyFunc();
                struct Log *log = getDynamicEmptyLog();
                newStruct->term.log = log;
                newStruct->type = eLog;
                log->base = base;
                log->inside = inside;
            }
            else{
                freeFunc(base);
                freeFunc(inside);
                return NULL;
            }
        }
        else{
            return NULL;
        }
    }
    else if(text[0] == '(' && findMatchingParanthesis(text,size) == size-1){
        struct FuncPart *para;
        para = cutAdditions(text+1,size-2);
        if(para == NULL){
            return NULL;
        }
        newStruct = getDynamicEmptyFunc();
        newStruct->term.inPara = para;
        newStruct->type = ePara;
    }
    else if(doesStringStartWith(text,size,"sin")){
        if(!handleCutTrig(&newStruct,text,size,0)){
            return NULL;
        }
    }
    else if(doesStringStartWith(text,size,"cos")){
        if(!handleCutTrig(&newStruct,text,size,1)){
            return NULL;
        }
    }
    else if(doesStringStartWith(text,size,"tan")){
        if(!handleCutTrig(&newStruct,text,size,2)){
            return NULL;
        }
    }
    else if(doesStringStartWith(text,size,"cot")){
        if(!handleCutTrig(&newStruct,text,size,3)){
            return NULL;
        }
    }
    else if(doesStringStartWith(text,size,"arcsin")){
        if(!handleCutArcTrig(&newStruct,text,size,0)){
            return NULL;
        }
    }
    else if(doesStringStartWith(text,size,"arccos")){
        if(!handleCutArcTrig(&newStruct,text,size,1)){
            return NULL;
        }
    }
    else if(doesStringStartWith(text,size,"arctan")){
        if(!handleCutArcTrig(&newStruct,text,size,2)){
            return NULL;
        }
    }
    else if(doesStringStartWith(text,size,"arccot")){
        if(!handleCutArcTrig(&newStruct,text,size,3)){
            return NULL;
        }
    }
    else{
        int i = 0;
        while(text[i]!='^' &&i<size){
            if(text[i] == '('){
                int offset = findMatchingParanthesis(text+i,size-i);
                if(offset==-1){
                    printf("Invalid Syntax!\n");
                    return NULL;
                }
                i=(offset+i)-1;
            }
            i++;
        }
        if(i == size){ // ^ isareti yok
            if(text[size-1] == 'x'){ // ^ icermeyen x'li terim: 5x, x gibi
                struct FuncPart *coefficent,*one;
                if(size != 1){
                    coefficent = convertTermToStruct(text,size-1,0,0);
                    if(coefficent == NULL){
                        return NULL;
                    }
                    one = getDynamicEmptyFunc();
                }
                else{
                    one = getDynamicEmptyFunc();
                    coefficent = one;
                }
                struct Poly *poly = getDynamicEmptyPoly();
                one->term.num = 1;
                one->type = eNumber;
                poly->power = one;
                poly->coefficent = coefficent;
                newStruct = getDynamicEmptyFunc();
                newStruct->term.pol = poly;
                newStruct->type = ePoly;
            }
            else{ // number
                if(size == 1 && !(text[0]>= '0' && text[0] <= '9')){ // ozel sayi (e ve pi)
                    if(text[0] == 'e'){
                        newStruct = getDynamicEmptyFunc();
                        newStruct->term.num = exp(1);
                        newStruct->type = eNumber;
                    }
                    else if(text[0] == 'p'){
                        newStruct = getDynamicEmptyFunc();
                        newStruct->term.num = PI;
                        newStruct->type = eNumber;
                    }
                    else{
                        return NULL;
                    }
                }
                else{
                    int j,dotCount = 0,dotIndex;
                    for(j=0;j<size;j++){
                        if(text[j] == '.'){
                            dotCount++;
                            dotIndex = j;
                        }
                    }
                    if(dotCount == 0){//integer
                        int value,number = 0;
                        for(j=size-1;j>=0;j--){
                            value = text[j] - '0';
                            if(value>=0 && value < 10){
                                number += pow(10,(size-1)-j)*value;
                            }
                            else{
                                return NULL;
                            }
                        }
                        newStruct = getDynamicEmptyFunc();
                        newStruct->term.num = number;
                        newStruct->type = eNumber;
                    }
                    else if(dotCount == 1){// float
                        int value;
                        double number = 0;
                        for(j=size-1;j>=0;j--){
                            value = text[j] - '0';
                            if(value>=0 && value < 10){
                                if(j>dotIndex){
                                    number += pow(10,(dotIndex-j))*value;
                                }
                                else{
                                    number += pow(10,(dotIndex-j-1))*value;
                                }
                            }
                            else if(text[j] != '.'){
                                return NULL;
                            }
                        }
                        newStruct = getDynamicEmptyFunc();
                        newStruct->term.num = number;
                        newStruct->type = eNumber;
                    }
                    else{//undefined
                        return NULL;
                    }
                }
            }
        }
        else{ // ^ isareti var
            if(text[i-1] == 'x'){ //poly
                struct FuncPart *power = convertTermToStruct(text+i+1,size-i-1,0,0);
                if(power == NULL){
                     return NULL;
                }
                struct FuncPart *coefficent;
                if(i-1 == 0){
                    struct FuncPart *one = getDynamicEmptyFunc();
                    one->term.num = 1;
                    one->type = eNumber;
                    coefficent = one;
                }
                else{
                    coefficent = convertTermToStruct(text,i-1,0,0);
                    if(coefficent == NULL){
                        return NULL;
                    }
                }

                struct Poly *poly = getDynamicEmptyPoly();
                poly->coefficent = coefficent;
                poly->power = power;
                newStruct = getDynamicEmptyFunc();
                newStruct->term.pol = poly;
                newStruct->type = ePoly;
            }
            else{ //expo

                struct FuncPart *base = convertTermToStruct(text,i,0,0);
                struct FuncPart *power = convertTermToStruct(text+i+1,size-i-1,0,0);
                if(base == NULL || power == NULL){
                    freeFunc(base);
                    freeFunc(power);
                    return NULL;
                }
                newStruct = getDynamicEmptyFunc();
                struct Expo *expo = getDynamicEmptyExpo();
                expo->base = base;
                expo->expo = power;
                newStruct->term.expo = expo;
                newStruct->type = eExpo;
            }
        }
    }
    newStruct->negative = negative;
    newStruct->inverse = inverse;
    return newStruct;
}

int handleCutTrig(struct FuncPart** newStruct,char* text,int size, int mode){
    struct FuncPart* inside = convertTermToStruct(text+3,size-3,0,0);
    if(inside == NULL){
        return 0;
    }
    *newStruct = getDynamicEmptyFunc();
    struct Trig *trig = getDynamicEmptyTrig();
    trig->mode = mode;
    trig->inside = inside;
    (*newStruct)->term.trig = trig;
    (*newStruct)->type = eTrig;
    return 1;
}

int handleCutArcTrig(struct FuncPart** newStruct,char* text,int size, int mode){
    struct FuncPart* inside = convertTermToStruct(text+6,size-6,0,0);
    if(inside == NULL){
        return 0;
    }
    *newStruct = getDynamicEmptyFunc();
    struct ArcTrig *arcTrig = getDynamicEmptyArcTrig();
    arcTrig->mode = mode;
    arcTrig->inside = inside;
    (*newStruct)->term.arcTrig = arcTrig;
    (*newStruct)->type = eArcTrig;
    return 1;
}

double** makeSquareMatrix(int size){
    double **matrix;
    int i;
    matrix = (double**)calloc(size,sizeof(double*));
    for(i = 0;i < size;i++){
        *(matrix+i) = (double*)calloc(size,sizeof(double));
    }
    return matrix;
}

double** getMatrixFromUser(int size){
    int i,j;
    double** matrix = makeSquareMatrix(size);
    for(i = 0;i < size;i++){
        for(j = 0;j < size;j++){
            printf("%dth row %dth column: ",i+1,j+1);
            scanf("%lf",&matrix[i][j]);
        }
    }
    return matrix;
}

double** makeIdentityMatrix(int size){
    int i;
    double **matrix = makeSquareMatrix(size);
    for(i = 0;i < size;i++){
        matrix[i][i] = 1;
    }
    return matrix;
}

void releaseSquareMatrix(double** matrix, int size){
    int i;
    for(i = 0;i < size;i++){
        free(*(matrix+i));
    }
    free(matrix);
}

void printSquareMatrix(double** matrix, int size){
    int i,j;
    printf("\n");
    for(i = 0;i < size;i++){
        for(j = 0;j < size;j++){
            printf("%lf ",matrix[i][j]);
        }
        printf("\n");
    }
}

void multiplyRow(double** matrix,int size,int row,double mult){
    int i;
    for(i = 0;i < size;i++){
        matrix[row][i] *= mult;
    }
}
void multAndAddRowFromRow(double** matrix,int size,int startRow,int targetRow,double mult){
    int i;
    for(i = 0;i < size;i++){
        matrix[targetRow][i] += (matrix[startRow][i] * mult);
    }
}

void swapRows(double** matrix, int size,int firstRow,int secondRow){
    double temp;
    int i;
    for(i = 0;i < size;i++){
        temp = matrix[firstRow][i];
        matrix[firstRow][i] = matrix[secondRow][i];
        matrix[secondRow][i] = temp;
    }
}

void makeDiagonalMax(double** matrix,double* unknowns, int size){
    int i,j,maxRow;
    double maxNum,temp;
    for(i = 0;i < size;i++){
        maxNum = fabs(matrix[0][i]);
        maxRow = 0;
        for(j = 1;j < size;j++){
            if(maxNum < fabs(matrix[j][i])){
                maxNum = fabs(matrix[j][i]);
                maxRow = j;
            }
        }
        if(fabs(matrix[i][i]*matrix[maxRow][maxRow]) < fabs(matrix[i][maxRow]*matrix[maxRow][i])){ // degisim olumlu mu diye kontrol
            swapRows(matrix,size,maxRow,i);
            temp = unknowns[i];
            unknowns[i] = unknowns[maxRow];
            unknowns[maxRow] = temp;
        }
    }
}

double findDeterminant(double** matrix,int size){
    if(size == 1) {
        return matrix[0][0];
    }
    else if(size == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    else{
        double **subMatrix = makeSquareMatrix(size-1);
        double result = 0.0;
        int i,j,k,flag;
        for(i = 0;i < size;i++){
            for(j=1;j<size;j++){
                flag = 0;
                for(k=0;k<size-1;k++){
                    if(i == k){
                        flag = 1;
                    }
                    subMatrix[j-1][k] = matrix[j][k+flag];
                }
            }

            result += findDeterminant(subMatrix,size-1)* matrix[0][i] * (i % 2 == 0 ? 1.0 : -1.0);
        }
        releaseSquareMatrix(subMatrix,size-1);
        return result;
    }
}

void bisection(struct FuncPart *func,double lowerLimit,double higherLimit,double error,int maxLoop){
    if(lowerLimit >= higherLimit){
        printf("\nLower limit should be lower than the higher limit!\n");
        return;
    }
    double startLower,startHigher;
    startLower = evaluateFunc(func,lowerLimit);
    startHigher = evaluateFunc(func,higherLimit);

    if(startLower*startHigher<0){
        printf("\nf(a) * f(b) < 0: Root between lower and upper limit is confirmed. Starting bisection.\n");
        double positive,negative,newX,oldX,result;
        int iteration = 0;
        if(startHigher < 0){
            negative = higherLimit;
            positive = lowerLimit;
        }
        else{
            negative = lowerLimit;
            positive = higherLimit;
        }
        newX = startLower;
        do{
            oldX = newX;
            iteration++;
            newX = (negative + positive) / 2.0;
            result = evaluateFunc(func,newX);
            printf("Result of %dth iteration: y=%lf at x=%lf\n",iteration,result,newX);
            if(result < 0){
                negative = newX;
            }
            else{
                positive = newX;
            }
        }while(fabs(newX-oldX)>error && maxLoop>iteration);
        printf("Final result: y=%lf x=%lf\n",result,newX);
    }
    else{
        printf("\nf(a) * f(b) > 0: Root between lower and upper limit cannot be confirmed.\n");
        return;
    }
}

void regulaFalsi(struct FuncPart *func,double lowerLimit,double upperLimit,double error,int maxLoop){
    if(lowerLimit >= upperLimit){
        printf("\nLower limit should be lower than the higher limit!\n");
        return;
    }
    if(evaluateFunc(func,lowerLimit)*evaluateFunc(func,upperLimit)<0){
        printf("\nf(a) * f(b) < 0: Root between lower and upper limit is confirmed. Starting Regula-Falsi.\n");
        double resultAtUpper,resultAtLower,result,newX = 0,oldX = 0,newLower = lowerLimit,newUpper = upperLimit;
        int iteration = 0;
        do{
            oldX = newX;
            iteration++;
            resultAtLower = evaluateFunc(func,newLower);
            resultAtUpper = evaluateFunc(func,newUpper);
            newX = (newUpper*resultAtLower-newLower*resultAtUpper)/(resultAtLower-resultAtUpper);
            result = evaluateFunc(func,newX);
            printf("Result of %dth iteration: y=%lf at x=%lf\n",iteration,result,newX);
            if(result*resultAtLower < 0){
                newUpper = newX;
            }
            else{
                newLower = newX;
            }
        }while((iteration == 1|| fabs(newX-oldX)>error) && maxLoop>iteration);
        printf("Final result: y=%lf x=%lf\n",result,newX);
    }
    else{
        printf("\nf(a) * f(b) > 0: Root between lower and upper limit cannot be confirmed.\n");
        return;
    }
}

void newtonRaphson(struct FuncPart  *func, double startingPoint,double error,int maxLoop){
    double oldX,result,newX = startingPoint;
    int iteration = 0;
    do{
        iteration++;
        oldX = newX;
        newX = oldX - (evaluateFunc(func,oldX) / centerDerivative(func,oldX,0.001));
        result = evaluateFunc(func,newX);
        printf("Result of %dth iteration: y=%lf at x=%lf\n",iteration,result,newX);
    }while(fabs(newX-oldX)>error && maxLoop>iteration);
    printf("Final result: y=%lf x=%lf\n",result,newX);
}

double backwardsDerivative(struct FuncPart *func,double x, double h){
    return ((evaluateFunc(func,x) - (evaluateFunc(func,x-h))) / h);
}

double centerDerivative(struct FuncPart *func,double x, double h){
    return ((evaluateFunc(func,x+h) - (evaluateFunc(func,x-h))) / (2*h));
}

double forwardDerivative(struct FuncPart *func,double x, double h){
    return ((evaluateFunc(func,x+h) - (evaluateFunc(func,x))) / h);
}

double trapez(struct FuncPart *func,double lowerLimit,double upperLimit,int numberOfInter){
    double result = 0, stepSize = (upperLimit-lowerLimit)/numberOfInter;
    int i;
    result += (evaluateFunc(func,lowerLimit)+evaluateFunc(func,upperLimit))/ 2.0;
    for(i = 1; i<numberOfInter;i++){
        result += evaluateFunc(func,lowerLimit + i*stepSize);
    }
    result *= stepSize;
    return result;
}

double simpsonOneThree(struct FuncPart *func,double lowerLimit,double upperLimit,int numberOfInter){
    if(numberOfInter%2 != 0){
        printf("\nNumber of intervals should be even in Simpson 1/3!\n");
        return 0;
    }
    double result = 0, stepSize = (upperLimit-lowerLimit)/numberOfInter;
    int i;
    result += (evaluateFunc(func,lowerLimit)+evaluateFunc(func,upperLimit));
    for(i = 1; i<numberOfInter;i += 2){
        result += 4*evaluateFunc(func,lowerLimit + i*stepSize);
    }
    for(i = 2; i<numberOfInter;i += 2){
        result += 2*evaluateFunc(func,lowerLimit + i*stepSize);
    }
    result *= (stepSize/3);
    return result;
}

double simpsonThreeEight(struct FuncPart *func,double lowerLimit,double upperLimit,int numberOfInter){
    double result = 0, stepSize = (upperLimit-lowerLimit)/(numberOfInter*3);
    int i;
    result += (evaluateFunc(func,lowerLimit)+evaluateFunc(func,upperLimit));
    for(i = 1; i<(numberOfInter*3);i++){
        if(i % 3 == 0){
            result += 2*evaluateFunc(func,lowerLimit + i*stepSize);
        }
        else{
            result += 3*evaluateFunc(func,lowerLimit + i*stepSize);
        }
    }
    result *= (stepSize*3/8);
    return result;
}

double** inverseMatrix(double** matrix,int size){
    int i,j;
    double **identity = makeIdentityMatrix(size);
    for(i = 0;i < size;i++){
        multiplyRow(identity,size,i,1/matrix[i][i]);
        multiplyRow(matrix,size,i,1/matrix[i][i]);
        for(j = 0;j < size;j++){
            if(j != i){
                multAndAddRowFromRow(identity,size,i,j,-1*matrix[j][i]);
                multAndAddRowFromRow(matrix,size,i,j,-1*matrix[j][i]);
            }
        }
    }
    return identity;
}


void cholesky(double** matrix,int size,double* constants){
    double** lowerMatrix = makeSquareMatrix(size);
    double** upperMatrix = makeSquareMatrix(size);
    int i, j, k;
    double temp = 0;
    for(i = 0;i < size;i++){
        upperMatrix[i][i] = 1;
    }
    for(i = 0;i < size;i++){
        for(j = 0;j < size;j++){
            if(j <= i){
                temp = 0;
                for(k = 0;k < j; k++){
                    temp += lowerMatrix[i][k] * upperMatrix[k][j];
                }
                lowerMatrix[i][j] = matrix[i][j] - temp;
            }
            else{
                temp = 0;
                for(k = 0;k < i; k++){
                    temp += lowerMatrix[i][k] * upperMatrix[k][j];
                }
                upperMatrix[i][j] = (matrix[i][j] - temp) / lowerMatrix[i][i];
            }
        }
    }
    printf("\nLower Matrix:\n");
    printSquareMatrix(lowerMatrix,size);
    printf("\nUpper Matrix:\n");
    printSquareMatrix(upperMatrix,size);
    double* y = (double*)calloc(size,sizeof(double));
    double* results = (double*)calloc(size,sizeof(double));
    for(i = 0;i < size;i++){
        temp = 0;
        for(j = 0;j < i;j++){
            temp += y[j] * lowerMatrix[i][j];
        }
        y[j] = (constants[j] - temp)/lowerMatrix[i][i];
    }
    for(i = size-1;i >= 0;i--){
        temp = 0;
        for(j = size-1 ;i < j;j--){
            temp += results[j] * upperMatrix[i][j];
        }
        results[j] = (y[j] - temp)/upperMatrix[i][i];
    }
    for(i = 0;i < size;i++){
        printf("X%d = %lf\n",i+1,results[i]);
    }
    free(y);
    free(results);
    releaseSquareMatrix(lowerMatrix,size);
    releaseSquareMatrix(upperMatrix,size);
}

void gaussSeidel(double** matrix,int size,double* unknowns,double* startingPoints,double error,int maxIteration){
    makeDiagonalMax(matrix,unknowns,size);
    int allIsSmaller,i,j,iteration = 0;
    double *results = (double*)calloc(size,sizeof(double));
    for(i = 0;i< size;i++){
        results[i] = startingPoints[i];
    }
    double oldVal;
    do{
        iteration++;
        allIsSmaller = 1;
        printf("%dth iteration:\n",iteration);
        for(i = 0;i < size;i++){
            oldVal = results[i];
            results[i] = unknowns[i];
            for(j = 0;j < size;j++){
                if(i != j){
                    results[i] -= matrix[i][j]*results[j];
                }
            }
            results[i] /= matrix[i][i];
            printf("%dth variable = %lf\n",i+1,results[i]);
            if(fabs(oldVal-results[i]) >= error){
                allIsSmaller = 0;
            }
        }

    }while(!allIsSmaller && maxIteration > iteration);
    printf("\nFinal values:\n");
    for(i = 0;i < size;i++){
        printf("%dth variable = %lf\n",i+1,results[i]);
    }
    free(results);
}

void gregoryNewton(double startingPoint,double stepSize,int size,double* yValues,double xVal){
    double** differenceMatrix = (double**)calloc(size-1,sizeof(double*));
    int i,j ,allIsSame = 0, differenceSize = 0;
    double testNum;
    while(!allIsSame && differenceSize < size - 1){
        differenceSize++;
        differenceMatrix[differenceSize-1] = (double*)calloc(size-differenceSize,sizeof(double));

        for(i = 0;i < size-differenceSize;i++){
            if(differenceSize == 1){
                differenceMatrix[differenceSize-1][i] = yValues[i+1]-yValues[i];
            }
            else{
                differenceMatrix[differenceSize-1][i] = differenceMatrix[differenceSize-2][i+1] - differenceMatrix[differenceSize-2][i];
            }
        }

        testNum = differenceMatrix[differenceSize-1][0];
        allIsSame = 1;
        for(i = 1; i < size-differenceSize;i++){
            if(testNum != differenceMatrix[differenceSize-1][i]){
                allIsSame = 0;
            }
        }
    }
    double result = 0, part = 1;
    for(i = 0;i <= differenceSize;i++){
        part = 1;
        for(j = 0; j < i;j++){
            part *= xVal - ((stepSize*j)+startingPoint);
        }
        if(i == 0){
            result += yValues[0];
        }
        else{
            result += (differenceMatrix[i-1][0]/factorial(i))*(part/pow(stepSize,i));
        }
    }
    printf("Result = %lf\n",result);

    for(i = 0;i < differenceSize;i++){
        free(differenceMatrix[i]);
    }
    free(differenceMatrix);
}

double factorial(int val){
    int i;
    double result = 1;
    for(i = 2; i <= val;i++){
        result *= i;
    }
    return result;
}
