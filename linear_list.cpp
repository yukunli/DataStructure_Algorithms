#include <stdio.h>
#include <malloc.h>

#define ACTICE_LIKER 1
#define ERROR 0
#define OK    1
#define NULL 0
#define LEN sizeof(struct student)
//typedef enum {ERROR, OK} Status;

struct student
{
	long num;
	float score;
	struct student * next;
};
/*  静态链表  所有节点在程度中定义，不是临时开辟的
void main()
{
	struct student a,b,c,*head,*p;
	a.num = 100; a.score = 33.4;
	b.num = 101; b.score = 22.5;
	c.num = 102; c.score = 44.2;
	head = &a;
	a.next = &b;
	b.next = &c;
	c.next = NULL;
	p = head;
	do
	{
		printf("%ld  %5.2f\n",p -> num, p-> score);
		p = p->next;
	}while(p != NULL);

}
end 静态链表程序段*/

//建立动态链表
# if ACTICE_LIKER

int n;
struct student * creat(void) //建立链表
{
	struct student *head,*p1, *p2;
	n =0;
	p1 = p2 = (struct student *)malloc(LEN); //开辟一个新单元
	scanf("%ld,%f",&p1->num,&p1->score);
	head = NULL;							 //初始化的新单元为空链表
	while(p1->num != 0)
	{
		n++;
		if(n ==1) head = p1;
		else
			p2->next = p1;
		p2 = p1;
		p1 = (struct student *)malloc(LEN); //再次开辟一个新单元
		scanf("%ld,%f",&p1->num,&p1->score);
	}
	p2 ->next = NULL;
	return head;
}
void print_linker(struct student * head)
{
	struct student *p;
	printf("\n Now these %d records are: \n",n);
	p = head;
	if(p != NULL)
	{
		do
		{
			printf("%ld %5.2f\n",p->num,p->score);
			p = p->next;
		}while( p!= NULL);
	}
}

struct student * del(struct student *head,long num)
{
	struct student *p1,*p2;
	if(head == NULL) printf("\n List is null!\n");
	p1 = head;
	while( num != p1->num && p1->next != NULL)
	{
		p2 = p1;
		p1 = p1 ->next;
	}
	if(p1 ->num == num)
	{
		if(p1 == head ) head = p1->next;
		else
		{
			p2->next = p1 ->next;
		}
		printf("delete: %ld\n",num);
		n--;
	}
	else
		printf("Can not find the num %ld\n",num);
	return head;
}
struct student *insert(struct student *head ,struct student *stud)
{
	struct student *p0,*p1,*p2;
	p1 = head;
	p0 = stud;
	if(head == NULL) 
	{
		head = p0;
		p0 ->next = NULL;
		return head;
	}
	else
	{
		while(p0 ->num > p1 ->num && p1->next != NULL)
		{
			p2 =p1;
			p1 = p1->next;
		}
		if(p0->num <= p1->num)
		{
			if(head == p1) head = p0;
			else
				p2->next = p0;
			p0->next = p1;
		}
		else
		{
			p1->next = p0;
			p0->next = NULL;
		}
		n++;
		return head;
	}
}
void main()
{
	struct student *p;
	long num;
	p = creat();
	print_linker(p);
	printf("please input one num to del: ");
	scanf("%ld",&num);
	del(p,num);
}
# endif