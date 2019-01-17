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
    int people1,court1,treasury1;
    char ans2[200];
    int people2,court2,treasury2;
    int possibility;
    struct problem *next;
};typedef struct problems problem;

problem *create_new(problem new_problem)//create new nodes
{
    problem *new_node=(problem *)malloc(sizeof(problem));
    strcpy(new_node->question,new_problem.question);
    strcpy(new_node->ans1,new_problem.ans1);
    strcpy(new_node->ans2,new_problem.ans2);
    new_node->people1=new_problem.people1;
    new_node->people2=new_problem.people2;
    new_node->court1=new_problem.court1;
    new_node->court2=new_problem.court2;
    new_node->treasury1=new_problem.treasury1;
    new_node->treasury2=new_problem.treasury2;
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

problem *make_linkedlist(char name[])//makes a linked list using the name of the text file given
{
    char q_file[200];
    FILE *fp=fopen(name,"r");
    assert(fp!=NULL);//check if we can even open the choices file!
    problem new_val,*head=NULL;
    while(1)
    {
        printf("*\n");
        if(feof(fp)) break;
        fscanf(fp,"%s",q_file);
        FILE *fpp=fopen(q_file,"r");
        assert(fpp!=NULL);//check if we can even open the question file!
        //read the shit
        fgets(new_val.question,200,fpp);
        fgets(new_val.ans1,200,fpp);
        fscanf(fpp,"%d",&new_val.people1);
        fscanf(fpp,"%d",&new_val.court1);
        fscanf(fpp,"%d",&new_val.treasury1);
        fgets(new_val.ans2,200,fpp);
        fscanf(fpp,"%d",&new_val.people2);
        fscanf(fpp,"%d",&new_val.court2);
        fscanf(fpp,"%d",&new_val.treasury2);
        new_val.possibility=3;
        problem *new_prob=create_new(new_val);
        add_end(&head,new_prob);

    }
    return head;
}
int if_resum(char player_name)//checks if the game should start from the beginning or resume
{}
int main()
{
    int people,treasury,court;
    char player_name[200];
    scanf("%s",player_name);
    while(1)
    {

    }
    //Beep(300, 500);
    //printf("\a");
    return 0;
}
