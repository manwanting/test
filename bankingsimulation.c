#include<stdio.h>
#include<stdlib.h>
 
    /*随机数初始值*/
#define MONEY  200000     /*个人业务值*/
#define TRUE  1
#define FALSE 0
#define OK   1
#define ERROR  0
#define OVERFLOW -2
#define CK  1    /*离开事件1：办理存款*/

typedef int status;

typedef struct{
 int arrivetime;   /*到达时间*/
 int OccurTime;   /*事件发生时间*/
 int NType;      /*事件类型*/
 int duration;   /*办理业务时间*/
 long int money;      /*数额*/
}Event,ElemType1;

typedef struct{
 int arrivetime;
 int duration;
 long int money;
}wait,ElemType2;       /*等待队列元素*/

typedef struct QNode1{
ElemType1 data;
struct QNode1 *next;
}QNode1,*Queue1;

typedef struct QNode2{
ElemType2 data;
struct QNode2 *next;
}QNode2,*Queue2;

typedef struct{
 Queue1 front;
 Queue1 rear;
}LinkQueue1;

typedef struct{
 Queue2 front;
 Queue2 rear;
}LinkQueue2;


/*全局变量*/
long int total_money;
int total_time;
int use_time;/*每个顾客所用时间*/
int money;/*每个顾客办理的款数*/
int closetime;/*银行营业时间*/
int INTERTIME; /*下一用户到达的时间间隔*/
int DURATION; /*办理业务所需时间*/
int pe_num;   /*办理业务的次序*/
int time1;/*系统现在时间*/


LinkQueue1 Q1;
LinkQueue2 Q2;
Event en;     /*事件*/
wait en1;    /*列表2元素*/

/*初始化队列1*/
status InitQueue1()
{Q1.front=Q1.rear=(Queue1)malloc(sizeof(QNode1));
 if(!Q1.front)exit(OVERFLOW);
 Q1.front->next=NULL;
 return OK;
}

/*初始化队列2*/

status InitQueue2()
{Q2.front=Q2.rear=(Queue2)malloc(sizeof(QNode2));
 if(!Q2.front)exit(OVERFLOW);
 Q2.front->next=NULL;
 return OK;
}

/*销毁队列1*/
status destroyqueue1()
{while(Q1.front)
    {Q1.rear=Q1.front->next;
     free(Q1.front);
     Q1.front=Q1.rear;
    }
return OK;
}

/*销毁队列2*/
status destroyqueue2()
{while(Q2.front)
    {Q2.rear=Q2.front->next;
     free(Q2.front);
     Q2.front=Q2.rear;
    }
return OK;
}

/*队列1，2 入队列*/
status EnQueue1()
{Queue1 p,r,r1;
 p=(Queue1)malloc(sizeof(QNode1));
 if(!p)exit(OVERFLOW);

p->data.arrivetime=en.arrivetime;
p->data.OccurTime=en.OccurTime;
p->data.NType=en.NType;
p->data.duration=en.duration;
p->data.money=en.money;

 r=Q1.front->next;
 while(r)
    {if(p->data.OccurTime < r->data.OccurTime)      /*当队列*/
       { if(r==Q1.front->next)
             {p->next=r;
              Q1.front->next=p;
              }
         else{r1->next=p;
              p->next=r;
              }

        }
       r1=r;r=r->next;
      }
  if(!r)
    { if(Q1.front->next==NULL)
         {Q1.front->next=p;
          Q1.rear=p;
          Q1.rear->next=NULL;
          }
      else{
       p->next=NULL;
       Q1.rear->next=p;
       Q1.rear=p;}
    }

return OK;
}


status EnQueue2()
{Queue2 p;
 p=(Queue2)malloc(sizeof(QNode2));
 if(!p)exit(OVERFLOW);
 p->data.arrivetime=en1.arrivetime;
 p->data.duration=en1.duration;
 p->data.money=en1.money;
 p->next=NULL;
 Q2.rear->next=p;
 Q2.rear=p;
return OK;
}

/*队列1，2 出队列,并用en返回值*/
status DeQueue1()
{  Queue1 p;
if(Q1.front==Q1.rear)return ERROR;
 p=Q1.front->next;
 en.arrivetime=p->data.arrivetime;
 en.OccurTime=p->data.OccurTime;
 en.NType=p->data.NType;
 en.duration=p->data.duration;
 en.money=p->data.money;
 Q1.front->next=p->next;
 if(Q1.rear==p)Q1.rear=Q1.front;
 free(p);
return OK;
}

status DeQueue2()
{ Queue2 p;
if(Q2.front==Q2.rear)return ERROR;
 p=Q2.front->next;
en1.arrivetime=p->data.arrivetime;
en1.duration=p->data.duration;
en1.money=p->data.money;
Q2.front->next=p->next;
if(Q2.rear==p)
  Q2.rear=Q2.front;
free(p);
return OK;
}


/*时间超时*/
  void  free_system()
   {destroyqueue1();
    destroyqueue2();
   }


/*随机数*/
status rand_ar(int *intertime,int *money, int *duration,int *NType)
{

  *intertime=rand()%INTERTIME+1;
  *money=rand()%MONEY+1;
  *duration=rand()%DURATION+1;
  *NType=rand()%2;
  return OK;
}


void OpenForDay()
{/*初始化操作*/
printf("请输入每个顾客办理的款数：");
scanf("%d",&total_money);   /*total_money=10000;*/
printf("请输入银行营业的总时间：");
scanf("%d",&closetime);     /*closetime=600*/
printf("请输入下一用户到达的时间间隔：");
scanf("%d",&INTERTIME);     /*INTERTIME=30*/
printf(" 请输入办理业务所需时间：");
scanf("%d",&DURATION);     /*DURATION=20*/
total_time=0;
pe_num=0;


InitQueue1();
InitQueue2();
 en.arrivetime=0;
 en.OccurTime=0;
 en.NType=0;    /*暂时值*/
 en.money=0;    /*暂时值*/
 en.duration=0;   /*暂时值*/

 EnQueue1(); /*事件进队列*/
}

 /*查找上一离开事件的发生时间*/
int find_dep()
{Queue1 p;
 int i=0;
 p=Q1.front->next;
 while(p!=NULL)
   {if(p->data.NType!=0)
    i=p->data.OccurTime;
    p=p->next;
   }
 return i;
}


void CustomerArrived()
{ 
  int intertime;
  int i;      /*前一离开时间*/
  time1=en.OccurTime;
  rand_ar(&intertime,&(en.money), &(en.duration),&(en.NType));


 /*设置一离开事件插入队列1*/
    en.NType++;   /* 0 1变1 2 */

  i=find_dep(); /*查找上一离开事件的发生时间*/
   if(i==0)/*第一位顾客*/
   en.OccurTime=en.arrivetime+en.duration;
   else
       if(i>=en.arrivetime)/*到达时还未离开*/
       en.OccurTime=i+en.duration;
       else
       en.OccurTime=en.arrivetime+en.duration;/*离开之前到达*/

  EnQueue1();

/*设置下一用户到达事件插入队列1*/

  en.arrivetime=en.arrivetime+intertime; /*下一客户到达时间*/
  en.OccurTime=en.arrivetime;
  en.NType=0;    /*暂时值*/
  en.money=0;    /*暂时值*/
  en.duration=0;   /*暂时值*/
  EnQueue1();

}


  /*返回队列2的长度*/
int getlong_q2()
{int i=0;
 Queue2 p;
 p=Q2.front->next;
 while(p)
   {i++;
    p=p->next;
   }

 return i;
}




/*检查队列2是否有满足条件者*/
status check_q2()
{

 int i;    /*队列长度*/
 int j,z=0;
 int time=0;
 i=getlong_q2();

  for(j=1;j<=i;j++)
      {DeQueue2();
            if(en1.money<=total_money)    /*en1为列表2元素*/
			 	if(time1+en1.duration>closetime) /*系统时间加业务办理的时间>营业时间*/
				{   use_time=closetime-en1.arrivetime;
	                pe_num++;
	                printf("--\t\t%d\t\t%d\t\t%d\t\t%d\t%d\n",z,use_time,pe_num,z,(en1.arrivetime),en1.money);                  
				      
				}
                else
                {time=time1+en1.duration;
				 use_time=time-en1.arrivetime;
				 total_time+=use_time;
                 total_money-=en1.money;
                 time1=time1+en1.duration;
                 pe_num++;
                 printf("%ld\t\t%d\t\t%d\t\t%d\t\t%d\t-%d\n",total_money,use_time,pe_num,time,(en1.arrivetime),en1.money);
                 }
		
			else {if(time1+en1.duration>=closetime) /*系统时间加业务办理的时间>营业时间*/
			         {use_time=closetime-en1.arrivetime;
	                  pe_num++;
	                  printf("--\t\t%d\t\t%d\t\t%d\t\t%d\t%d\n",z,use_time,pe_num,z,en1.arrivetime,z);
			         }
				      EnQueue2(); 
				}
					
      }

 return OK;
}
/*队列1离开事件减duration*/
status cut_duration(int e)
{Queue1 p;
 Queue1 q;
 Queue1 r;
 ElemType1 en;

 p=Q1.front->next;
 r=Q1.front;

   while(p)
 {if(p->data.NType!=0)
   {p->data.OccurTime=p->data.OccurTime-e;    /*事件发生时间减去该办理业务时间*/
    q=p->next;    /*删除结点*/
    r->next=q;
  en.arrivetime=p->data.arrivetime;   /*到达时间*/
  en.OccurTime=p->data.OccurTime;   /*事件发生时间*/
  en.NType=p->data.NType;      /*事件类型*/
  en.duration=p->data.duration;   /*办理业务时间*/
  en.money=p->data.money;      /*数额*/
    free(p);
    EnQueue1();
    }
    else
  {r=p;
   p=p->next;
  }

   }
    return OK;
}


void CustomerDeparture()
{ int i; /*业务类型*/

  i=en.NType;
  time1=en.OccurTime-en.duration;  /*系统现在时间*/
      if(i==CK)     /*离开事件1：办理存款*/
	  {    if(en.OccurTime>closetime)	           
	             free_system();
	                    
            else 
                { use_time=en.OccurTime-en.arrivetime;
				  total_time+=use_time;
                  total_money=total_money+en.money;

                   pe_num++;
                   time1=en.OccurTime;
    printf("%ld\t\t%d\t\t%d\t\t%d\t\t%d\t%d\n",total_money,use_time,pe_num,en.OccurTime,en.arrivetime,en.money);
                          /*检查队列2是否有满足条件者*/
                  check_q2();
                }
           }
      else {if(en.money>total_money)/*当钱不够时进入2队列等待*/

              { en1.arrivetime=en.arrivetime;
                en1.duration=en.duration;
                en1.money=en.money;
                EnQueue2();
              cut_duration(en.duration);/*同时删除该结点*/
              }
            else/*当钱够时进行队列1*/
                 {if(en.OccurTime>closetime)
                  free_system();
                  else
                     {use_time=en.OccurTime-en.arrivetime;
				      total_time+=use_time;
                      total_money-=en.money;
                      time1=en.OccurTime;
                      pe_num++;
     printf("%ld\t\t%d\t\t%d\t\t%d\t\t%d\t-%d\n",total_money,use_time,pe_num,en.OccurTime,en.arrivetime,en.money);
                     }

                 }
           }
  }

main()
    {
     float k=0;   /*i为总的时间，j为第几个客户，k为客户在银行内平均时间*/
	 int j=0,z=0,i=0;      /*重设随机函数的数值*/
     printf(" \n\n   =========================================================================\n");
	 printf("   =========================================================================\n");
     printf("                    Simulation of The Bank business\n");
     printf("   -------------------------------------------------------------------------\n\n\n");
	 printf("                   ---------------------------------                            \n");
     printf("                    NAME  ：满婉婷                                              \n");
	 printf("                    Number: 20171310640                                         \n");
	 printf("                    CLASS ：计科1701                                 \n");
     printf("   =========================================================================    \n");
     system("pause");
     system("cls");
	 while(i==0)
	 {
		 printf("   =========================================================================\n");
     printf("                    Simulation of The Bank business\n");
     printf("   -------------------------------------------------------------------------\n\n\n");
     OpenForDay();   /*初始化操作*/	 
	 
     printf("   -------------------------------------------------------------------------\n");
     printf("Total_money\tuse_time\tpe_num\ten.OccurTime\ten.arrivetime\tmoney\n");
	 
     {while(Q1.rear)
      {DeQueue1();      /*队列1出队列,并用en返回值*/
       if(en.NType==0)
           CustomerArrived();    /*客户到达*/
       else
           CustomerDeparture();   /*业务类型*/
       }
        z=total_time;
        j=pe_num;
        k=(float)z/j;
        printf("Total money:     %ld\n",total_money) ;
        printf("Total Time:      %d\n",total_time);
        printf("Total Custermer: %d\n",pe_num);
        printf("Average Time:    %f\n",k); 
		printf("按'0'再次输入数据 \n");
        printf("按任意键退出系统");
	 	scanf(" %d",&i);
		system("cls");
	 } 
	 }  
	    printf(" \n\n\n\n\n\n           谢谢使用^_^\n\n");
        system("pause");
        system("cls");


    }
