#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_EXPR 1024
#define MAX_STACK 1024

typedef struct { double stack[MAX_STACK]; int top; } DoubleStack;
typedef struct { char stack[MAX_STACK][16]; int top; } StringStack;

void pushDouble(DoubleStack *s, double val) { s->stack[++s->top] = val; }
double popDouble(DoubleStack *s) { return s->stack[s->top--]; }
void pushString(StringStack *s, const char *val) { strcpy(s->stack[++s->top], val); }
char* popString(StringStack *s) { return s->stack[s->top--]; }

int isFunction(const char *t) { return strcmp(t,"sin")==0||strcmp(t,"cos")==0||strcmp(t,"tan")==0||strcmp(t,"log")==0||strcmp(t,"ln")==0||strcmp(t,"sqrt")==0||strcmp(t,"exp")==0; }
int precedence(const char *op) { if(strcmp(op,"+") == 0||strcmp(op,"-")==0) return 1; if(strcmp(op,"*")==0||strcmp(op,"/")==0) return 2; if(strcmp(op,"^")==0) return 3; return 0; }
int isOperator(const char *t) { return strcmp(t,"+")==0||strcmp(t,"-")==0||strcmp(t,"*")==0||strcmp(t,"/")==0||strcmp(t,"^")==0; }
double applyFunc(const char *f,double v){if(strcmp(f,"sin")==0)return sin(v);if(strcmp(f,"cos")==0)return cos(v);if(strcmp(f,"tan")==0)return tan(v);if(strcmp(f,"log")==0){if(v<=0){printf("Error\n");exit(1);}return log10(v);}if(strcmp(f,"ln")==0){if(v<=0){printf("Error\n");exit(1);}return log(v);}if(strcmp(f,"sqrt")==0){if(v<0){printf("Error\n");exit(1);}return sqrt(v);}if(strcmp(f,"exp")==0)return exp(v);return 0;}
double applyOp(double a,double b,const char *op){if(strcmp(op,"+")==0)return a+b;if(strcmp(op,"-")==0)return a-b;if(strcmp(op,"*")==0)return a*b;if(strcmp(op,"/")==0){if(b==0){printf("Error\n");exit(1);}return a/b;}if(strcmp(op,"^")==0)return pow(a,b);return 0;}

int getNextToken(const char *expr,int *pos,char *token){
    int i=*pos;while(isspace(expr[i]))i++;if(expr[i]=='\0')return 0;
    if(isdigit(expr[i])||expr[i]=='.'){int j=0;while(isdigit(expr[i])||expr[i]=='.')token[j++]=expr[i++];token[j]='\0';}
    else if(isalpha(expr[i])){int j=0;while(isalpha(expr[i]))token[j++]=expr[i++];token[j]='\0';}
    else{token[0]=expr[i++];token[1]='\0';}*pos=i;return 1;
}

double evaluate(const char *expr){
    StringStack opStack={.top=-1};
    DoubleStack valStack={.top=-1};
    char token[64];int pos=0;
    while(getNextToken(expr,&pos,token)){
        if(isdigit(token[0])||(token[0]=='.'&&isdigit(token[1])))pushDouble(&valStack,atof(token));
        else if(isFunction(token))pushString(&opStack,token);
        else if(strcmp(token,"(")==0)pushString(&opStack,token);
        else if(strcmp(token,")")==0){
            while(opStack.top!=-1&&strcmp(opStack.stack[opStack.top],"(")!=0){
                char *op=popString(&opStack);
                if(isFunction(op)){double v=popDouble(&valStack);pushDouble(&valStack,applyFunc(op,v));}
                else{double b=popDouble(&valStack);double a=popDouble(&valStack);pushDouble(&valStack,applyOp(a,b,op));}
            }
            if(opStack.top!=-1)popString(&opStack);
            if(opStack.top!=-1&&isFunction(opStack.stack[opStack.top])){char *f=popString(&opStack);double v=popDouble(&valStack);pushDouble(&valStack,applyFunc(f,v));}
        }
        else if(isOperator(token)){
            while(opStack.top!=-1&&((isOperator(opStack.stack[opStack.top])&&precedence(opStack.stack[opStack.top])>=precedence(token))||isFunction(opStack.stack[opStack.top]))){
                char *op=popString(&opStack);
                if(isFunction(op)){double v=popDouble(&valStack);pushDouble(&valStack,applyFunc(op,v));}
                else{double b=popDouble(&valStack);double a=popDouble(&valStack);pushDouble(&valStack,applyOp(a,b,op));}
            }
            pushString(&opStack,token);
        }else{printf("Unknown\n");exit(1);}
    }
    while(opStack.top!=-1){
        char *op=popString(&opStack);
        if(isFunction(op)){double v=popDouble(&valStack);pushDouble(&valStack,applyFunc(op,v));}
        else{double b=popDouble(&valStack);double a=popDouble(&valStack);pushDouble(&valStack,applyOp(a,b,op));}
    }
    return popDouble(&valStack);
}

int main(){
    char line[MAX_EXPR];
    if(!fgets(line,sizeof(line),stdin))return 1;
    line[strcspn(line,"\n")]=0;
    double result=evaluate(line);
    printf("%.6f\n",result);
    return 0;
}
