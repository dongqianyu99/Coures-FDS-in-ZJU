#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int ID;
    int click;
    struct Node* next, * prev;
}LQNode;

typedef struct
{
    LQNode* front, * rear;
}LQList;

LQNode* ENTER(LQList* r, int x, int click_pre)
{
    LQNode* n = (LQNode*)malloc(sizeof(LQNode));
    n->ID = x;
    n->click = ++click_pre;
    n->next = NULL, n->prev = r->rear, r->rear->next = n;
    r->rear = r->rear->next;

    return n;
}

int DELETE(LQNode* n, LQList* r)
{
    if (n->next)
    {
        n->next->prev = n->prev;
        n->prev->next = n->next;
    }
    else
    {
        n->prev->next = NULL;
        r->rear = n->prev;
    }

    return n->click;
}

int POP(LQList* r)
{
    int x = r->front->next->ID;
    r->front = r->front->next;
    return x;
}

struct Node* HAQ_pos[100005];
struct Node* CQ_pos[100005];

int main()
{
    int K, N, M;  //K stands for the number of hits for cache,N stands for the size of the queues(both),M stands for the number of referenced page IDs
    scanf("%d %d %d", &K, &N, &M);
    int len1 = 0, len2 = 0;
    int num = 0;
    int click = 0;

    //Historical Aaccess Queue,Cache Queue
    LQList* HAQ = (LQList*)malloc(sizeof(LQList));
    LQList* CQ = (LQList*)malloc(sizeof(LQList));

    //initialization
    LQNode* r1 = (LQNode*)malloc(sizeof(LQNode));
    r1->next = NULL, r1->prev = NULL;
    HAQ->front = r1, HAQ->rear = r1;

    LQNode* r2 = (LQNode*)malloc(sizeof(LQNode));
    r2->next = NULL, r1->prev = NULL;
    CQ->front = r2, CQ->rear = r2;

    //implementation
    for (int i = 1; i <= M; i++)
    {
        scanf("%d", &num);
        if (CQ_pos[num] != NULL) DELETE(CQ_pos[num], CQ), CQ_pos[num] = ENTER(CQ, num, 0);
        else if(K==1) 
        {
            if(len2<N)  CQ_pos[num] = ENTER(CQ, num, 0), len2++;
            else CQ_pos[POP(CQ)] = NULL, CQ_pos[num] = ENTER(CQ, num, 0);
        }
        else if (HAQ_pos[num] == NULL && len1 < N) HAQ_pos[num] = ENTER(HAQ, num, 0), len1++;
        else if (HAQ_pos[num] == NULL && len1 == N) HAQ_pos[POP(HAQ)] = NULL, HAQ_pos[num] = ENTER(HAQ, num, 0);
        else if (HAQ_pos[num] != NULL && HAQ_pos[num]->click<K-1) click=DELETE(HAQ_pos[num],HAQ),HAQ_pos[num]=ENTER(HAQ,num,click);
        else if (HAQ_pos[num]->click == K - 1)
        {
            len1 -= 1;
            DELETE(HAQ_pos[num], HAQ), HAQ_pos[num] = NULL;
            if (len2 < N && CQ_pos[num] == NULL) CQ_pos[num] = ENTER(CQ, num, 0), len2++;
            else if (len2 == N && CQ_pos[num] == NULL) CQ_pos[POP(CQ)] = NULL, CQ_pos[num] = ENTER(CQ, num, 0);
            //else DELETE(CQ_pos[num], CQ), CQ_pos[num] = ENTER(CQ, num, 0);
        }

    }

    LQNode* p = (LQNode*)malloc(sizeof(LQNode));
    p = HAQ->front->next;
    if (p != NULL)
    {
        while (p != HAQ->rear) printf("%d ", p->ID), p = p->next;
        printf("%d\n", p->ID);
    }
    else printf("-\n");
    p = CQ->front->next;
    if (p != NULL)
    {
        while (p != CQ->rear) printf("%d ", p->ID), p = p->next;
        printf("%d\n", p->ID);
    }
    else printf("-");


    return 0;
}
