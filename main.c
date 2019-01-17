#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dos.h>
#include <windows.h>

struct problems//the entire shit
{
    char question[200];
    char ans1[200];
    int people[2],court[2],treasury[2];
    char ans2[200];
    int possibility;
    struct problem *next;
};typedef struct problems problem;

problem *create_new(problem new_problem)//create new nodes
{
    problem *new_node=(problem *)malloc(sizeof(problem));
    strcpy(new_node->question,new_problem.question);
    strcpy(new_node->ans1,new_problem.ans1);
    strcpy(new_node->ans2,new_problem.ans2);
    new_node->people[0]=new_problem.people[0];
    new_node->people[1]=new_problem.people[1];
    new_node->court[0]=new_problem.court[0];
    new_node->court[1]=new_problem.court[1];
    new_node->treasury[0]=new_problem.treasury[0];
    new_node->treasury[1]=new_problem.treasury[1];
    new_node->possibility=new_problem.possibility;
    new_node->next=NULL;
}

int add_end(problem **head,problem *new_problem)//add nodes to the end of link list
{
    problem *cur=*head;
    if(*head==NULL)
    {
        *head=new_problem;
        return 1;
    }
    while(cur->next != NULL)
        cur=cur->next;
    cur->next=new_problem;
    return 1;
}

int delete_problem(problem **head,problem *waste)//delete nodes by their address
{
    problem *cur=*head,*prev;
    if(*head==NULL) return -1;
    while(cur != waste && cur!=NULL)
    {
        prev=cur;
        cur=cur->next;
    }
    if(cur==NULL) return -2;
    if(cur==*head)
    {
        *head=NULL;
        free(cur);
        return 1;
    }
    prev->next=cur->next;
    free(cur);
    return 1;
}

problem *make_linkedlist(char name[],int *number)//makes a linked list using the name of the text file given
{
    char q_file[200];
    FILE *fp=fopen(name,"r");
    assert(fp!=NULL);//check if we can even open the choices file!
    problem new_val,*head=NULL;
    while(1)
    {
        if(feof(fp)) break;
        fscanf(fp,"%s",q_file);
        FILE *fpp=fopen(q_file,"r");
        assert(fpp!=NULL);//check if we can even open the question file!
        //read the shit:
        fgets(new_val.question,200,fpp);
        fgets(new_val.ans1,200,fpp);
        fscanf(fpp,"%d",&new_val.people[0]);
        fscanf(fpp,"%d",&new_val.court[0]);
        fscanf(fpp,"%d",&new_val.treasury[0]);
        fgets(new_val.ans2,200,fpp);
        fgets(new_val.ans2,200,fpp);
        fscanf(fpp,"%d",&new_val.people[1]);
        fscanf(fpp,"%d",&new_val.court[1]);
        fscanf(fpp,"%d",&new_val.treasury[1]);
        new_val.possibility=3;
        problem *new_prob=create_new(new_val);
        add_end(&head,new_prob);
        (*number)++;

    }
    return head;
}
int if_resum(char player_name)//checks if the game should start from the beginning or resume
{}
void play_music(void)
{
    Beep(300,400);
    Beep(350,300);
    Beep(400,300);
}
int main()
{
    int people=50,treasury=50,court=50, *num,number;
    *num=0;
    time_t seed=time(NULL);
    srand(seed);
    play_music();//get the name
    printf("Choose a name: \n");
    char player_name[200];
    scanf("%s",player_name);
    problem *head=make_linkedlist("CHOICES.txt",num);//make the linked list
    number=*num;
    problem *cur=head;
    while(1)//the entire GAME!
    {
        printf("|People: %d| |Treasury: %d| |Court: %d| \n",people,treasury,court);
        cur=head;
        int i,end_loop=rand()%number,choice;
        for(i=0;i<end_loop;i++)
            cur=cur->next;
        cur->possibility--;
        printf("%s\n|1| %s\n|2| %s",cur->question,cur->ans1,cur->ans2);
        if(cur->possibility==0) delete_problem(&head,cur);
        scanf("%d",&choice);
        choice--;
        //do the changes to the government
        people+=cur->people[choice];
        treasury+=cur->treasury[choice];
        court+=cur->court[choice];
    }
    //Beep(300, 500);
    //printf("\a");
    return 0;
}
