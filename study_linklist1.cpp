#include <stdio.h>
#include <math.h>
#include <malloc.h>
/*
ͬһ�涨
*/
#define ERROR 0
#define OK 1
typedef unsigned int Status ;

typedef struct 
{
	long num;
	float score;
}student;

/*���Ա����ʽ�洢�ṹ*/
typedef struct Lnode
{
	student data;
	struct Lnode *next;
}Lnode;
typedef Lnode * LinkList;



Status getelem(LinkList L, int i, student *e)
{

	return OK;
}

int main(void)
{
	LinkList L;
}