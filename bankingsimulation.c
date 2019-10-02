#include<stdio.h>
#include<stdlib.h>
 
    /*�������ʼֵ*/
#define MONEY  200000     /*����ҵ��ֵ*/
#define TRUE  1
#define FALSE 0
#define OK   1
#define ERROR  0
#define OVERFLOW -2
#define CK  1    /*�뿪�¼�1��������*/

typedef int status;

typedef struct{
 int arrivetime;   /*����ʱ��*/
 int OccurTime;   /*�¼�����ʱ��*/
 int NType;      /*�¼�����*/
 int duration;   /*����ҵ��ʱ��*/
 long int money;      /*����*/
}Event,ElemType1;

typedef struct{
 int arrivetime;
 int duration;
 long int money;
}wait,ElemType2;       /*�ȴ�����Ԫ��*/

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


/*ȫ�ֱ���*/
long int total_money;
int total_time;
int use_time;/*ÿ���˿�����ʱ��*/
int money;/*ÿ���˿Ͱ���Ŀ���*/
int closetime;/*����Ӫҵʱ��*/
int INTERTIME; /*��һ�û������ʱ����*/
int DURATION; /*����ҵ������ʱ��*/
int pe_num;   /*����ҵ��Ĵ���*/
int time1;/*ϵͳ����ʱ��*/


LinkQueue1 Q1;
LinkQueue2 Q2;
Event en;     /*�¼�*/
wait en1;    /*�б�2Ԫ��*/

/*��ʼ������1*/
status InitQueue1()
{Q1.front=Q1.rear=(Queue1)malloc(sizeof(QNode1));
 if(!Q1.front)exit(OVERFLOW);
 Q1.front->next=NULL;
 return OK;
}

/*��ʼ������2*/

status InitQueue2()
{Q2.front=Q2.rear=(Queue2)malloc(sizeof(QNode2));
 if(!Q2.front)exit(OVERFLOW);
 Q2.front->next=NULL;
 return OK;
}

/*���ٶ���1*/
status destroyqueue1()
{while(Q1.front)
    {Q1.rear=Q1.front->next;
     free(Q1.front);
     Q1.front=Q1.rear;
    }
return OK;
}

/*���ٶ���2*/
status destroyqueue2()
{while(Q2.front)
    {Q2.rear=Q2.front->next;
     free(Q2.front);
     Q2.front=Q2.rear;
    }
return OK;
}

/*����1��2 �����*/
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
    {if(p->data.OccurTime < r->data.OccurTime)      /*������*/
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

/*����1��2 ������,����en����ֵ*/
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


/*ʱ�䳬ʱ*/
  void  free_system()
   {destroyqueue1();
    destroyqueue2();
   }


/*�����*/
status rand_ar(int *intertime,int *money, int *duration,int *NType)
{

  *intertime=rand()%INTERTIME+1;
  *money=rand()%MONEY+1;
  *duration=rand()%DURATION+1;
  *NType=rand()%2;
  return OK;
}


void OpenForDay()
{/*��ʼ������*/
printf("������ÿ���˿Ͱ���Ŀ�����");
scanf("%d",&total_money);   /*total_money=10000;*/
printf("����������Ӫҵ����ʱ�䣺");
scanf("%d",&closetime);     /*closetime=600*/
printf("��������һ�û������ʱ������");
scanf("%d",&INTERTIME);     /*INTERTIME=30*/
printf(" ���������ҵ������ʱ�䣺");
scanf("%d",&DURATION);     /*DURATION=20*/
total_time=0;
pe_num=0;


InitQueue1();
InitQueue2();
 en.arrivetime=0;
 en.OccurTime=0;
 en.NType=0;    /*��ʱֵ*/
 en.money=0;    /*��ʱֵ*/
 en.duration=0;   /*��ʱֵ*/

 EnQueue1(); /*�¼�������*/
}

 /*������һ�뿪�¼��ķ���ʱ��*/
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
  int i;      /*ǰһ�뿪ʱ��*/
  time1=en.OccurTime;
  rand_ar(&intertime,&(en.money), &(en.duration),&(en.NType));


 /*����һ�뿪�¼��������1*/
    en.NType++;   /* 0 1��1 2 */

  i=find_dep(); /*������һ�뿪�¼��ķ���ʱ��*/
   if(i==0)/*��һλ�˿�*/
   en.OccurTime=en.arrivetime+en.duration;
   else
       if(i>=en.arrivetime)/*����ʱ��δ�뿪*/
       en.OccurTime=i+en.duration;
       else
       en.OccurTime=en.arrivetime+en.duration;/*�뿪֮ǰ����*/

  EnQueue1();

/*������һ�û������¼��������1*/

  en.arrivetime=en.arrivetime+intertime; /*��һ�ͻ�����ʱ��*/
  en.OccurTime=en.arrivetime;
  en.NType=0;    /*��ʱֵ*/
  en.money=0;    /*��ʱֵ*/
  en.duration=0;   /*��ʱֵ*/
  EnQueue1();

}


  /*���ض���2�ĳ���*/
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




/*������2�Ƿ�������������*/
status check_q2()
{

 int i;    /*���г���*/
 int j,z=0;
 int time=0;
 i=getlong_q2();

  for(j=1;j<=i;j++)
      {DeQueue2();
            if(en1.money<=total_money)    /*en1Ϊ�б�2Ԫ��*/
			 	if(time1+en1.duration>closetime) /*ϵͳʱ���ҵ������ʱ��>Ӫҵʱ��*/
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
		
			else {if(time1+en1.duration>=closetime) /*ϵͳʱ���ҵ������ʱ��>Ӫҵʱ��*/
			         {use_time=closetime-en1.arrivetime;
	                  pe_num++;
	                  printf("--\t\t%d\t\t%d\t\t%d\t\t%d\t%d\n",z,use_time,pe_num,z,en1.arrivetime,z);
			         }
				      EnQueue2(); 
				}
					
      }

 return OK;
}
/*����1�뿪�¼���duration*/
status cut_duration(int e)
{Queue1 p;
 Queue1 q;
 Queue1 r;
 ElemType1 en;

 p=Q1.front->next;
 r=Q1.front;

   while(p)
 {if(p->data.NType!=0)
   {p->data.OccurTime=p->data.OccurTime-e;    /*�¼�����ʱ���ȥ�ð���ҵ��ʱ��*/
    q=p->next;    /*ɾ�����*/
    r->next=q;
  en.arrivetime=p->data.arrivetime;   /*����ʱ��*/
  en.OccurTime=p->data.OccurTime;   /*�¼�����ʱ��*/
  en.NType=p->data.NType;      /*�¼�����*/
  en.duration=p->data.duration;   /*����ҵ��ʱ��*/
  en.money=p->data.money;      /*����*/
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
{ int i; /*ҵ������*/

  i=en.NType;
  time1=en.OccurTime-en.duration;  /*ϵͳ����ʱ��*/
      if(i==CK)     /*�뿪�¼�1��������*/
	  {    if(en.OccurTime>closetime)	           
	             free_system();
	                    
            else 
                { use_time=en.OccurTime-en.arrivetime;
				  total_time+=use_time;
                  total_money=total_money+en.money;

                   pe_num++;
                   time1=en.OccurTime;
    printf("%ld\t\t%d\t\t%d\t\t%d\t\t%d\t%d\n",total_money,use_time,pe_num,en.OccurTime,en.arrivetime,en.money);
                          /*������2�Ƿ�������������*/
                  check_q2();
                }
           }
      else {if(en.money>total_money)/*��Ǯ����ʱ����2���еȴ�*/

              { en1.arrivetime=en.arrivetime;
                en1.duration=en.duration;
                en1.money=en.money;
                EnQueue2();
              cut_duration(en.duration);/*ͬʱɾ���ý��*/
              }
            else/*��Ǯ��ʱ���ж���1*/
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
     float k=0;   /*iΪ�ܵ�ʱ�䣬jΪ�ڼ����ͻ���kΪ�ͻ���������ƽ��ʱ��*/
	 int j=0,z=0,i=0;      /*���������������ֵ*/
     printf(" \n\n   =========================================================================\n");
	 printf("   =========================================================================\n");
     printf("                    Simulation of The Bank business\n");
     printf("   -------------------------------------------------------------------------\n\n\n");
	 printf("                   ---------------------------------                            \n");
     printf("                    NAME  ��������                                              \n");
	 printf("                    Number: 20171310640                                         \n");
	 printf("                    CLASS ���ƿ�1701                                 \n");
     printf("   =========================================================================    \n");
     system("pause");
     system("cls");
	 while(i==0)
	 {
		 printf("   =========================================================================\n");
     printf("                    Simulation of The Bank business\n");
     printf("   -------------------------------------------------------------------------\n\n\n");
     OpenForDay();   /*��ʼ������*/	 
	 
     printf("   -------------------------------------------------------------------------\n");
     printf("Total_money\tuse_time\tpe_num\ten.OccurTime\ten.arrivetime\tmoney\n");
	 
     {while(Q1.rear)
      {DeQueue1();      /*����1������,����en����ֵ*/
       if(en.NType==0)
           CustomerArrived();    /*�ͻ�����*/
       else
           CustomerDeparture();   /*ҵ������*/
       }
        z=total_time;
        j=pe_num;
        k=(float)z/j;
        printf("Total money:     %ld\n",total_money) ;
        printf("Total Time:      %d\n",total_time);
        printf("Total Custermer: %d\n",pe_num);
        printf("Average Time:    %f\n",k); 
		printf("��'0'�ٴ��������� \n");
        printf("��������˳�ϵͳ");
	 	scanf(" %d",&i);
		system("cls");
	 } 
	 }  
	    printf(" \n\n\n\n\n\n           ллʹ��^_^\n\n");
        system("pause");
        system("cls");


    }
