
#include <stdio.h>
#include <Windows.h>

#define MAX_WIDTH 10
#define MAX_HEIGHT 10
#define QUEUE_SIZE 100

char map[MAX_HEIGHT][MAX_WIDTH] = {
    {' ',' ','X','X','X','X','X','X',' ',' '},
    {' ',' ','X',' ',' ',' ',' ','X',' ',' '},
    {' ',' ','X',' ',' ',' ',' ','X',' ',' '},
    {' ','X','X',' ',' ',' ',' ',' ',' ',' '},
    {' ','X',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ','X',' ',' ',' ',' ','X',' ',' ',' '},
    {' ','X',' ',' ',' ',' ','X',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ','X',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ','X',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ','X',' ',' ',' '},
};

typedef struct {
    bool visit;
    int parentH;
    int parentW;
}pathInfo;

pathInfo data[MAX_HEIGHT][MAX_WIDTH];

typedef struct {
    int height;
    int width;
}position;

typedef struct {
    position pos[QUEUE_SIZE];
    int front, rear;
}structure;

void Init(structure* q)
{
    q->front = -1;
    q->rear = -1;
}

bool IsFull(structure* q) {
    if(q->rear == QUEUE_SIZE - 1)
        return true;
    return false;
}

bool IsEmpty(structure* q) {
    if(q->rear == q->front)
        return true; // 공백상태 
    return false;
}

bool Enqueue(structure* q, position position) {
    if (IsFull(q)) {
        return false;
    }
    q->pos[++q->rear] = position;  //rear값을 증가시킨 후 반환
    return true;
}

bool Dequeue(structure* q, position* position)
{
    if (IsEmpty(q)) {
        return false;
    }
    *position = q->pos[++q->front];
    return true;
}

bool Push(structure* s, position pos)
{
    if (IsFull(s)) {
        return false;
    }
    s->pos[++s->front] = pos;
    return true;
}

bool Pop(structure* s, position* pos)
{
    if (IsEmpty(s)) {
        return false;
    }
    *pos = s->pos[s->front--];
    return true;
}

void PrintMap()
{
    system("cls");

    for (int i = 0; i < MAX_HEIGHT; ++i)
    {
        for (int j = 0; j < MAX_WIDTH; ++j)
        {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

bool  Check(int H, int W)
{
    return false;
}

void bfs(int startH, int startW, int goalH, int goalW)
{
    // 시작점과 목표지점의 좌표가 'X'이면 탐색을 시작하지 않고 return
    if (map[startH][startW] == 'X' || map[goalH][goalW] == 'X')
        return;

    // 방문여부를 모두 false로 설정 
    for (int i = 0; i < MAX_HEIGHT; ++i)
    {
        for (int j = 0; j < MAX_WIDTH; ++j)
        {
            data[i][j].visit = false;
        }
    }

    structure queue;
    Init(&queue);

    // 시작지점 넣고 시작
    map[startH][startW] = '.';
    Enqueue(&queue, { startH,startW });
    data[startH][startW] = { true,-1,-1 };
    PrintMap();

    position pos;   // Dequeue를 통해 나온 좌표를 넣을 공간
    while (!IsEmpty(&queue))
    {
        Dequeue(&queue, &pos);
        int H = pos.height;
        int W = pos.width;

        if (H == goalH && W == goalW)   // goal에 도착하면
        {
            while (H != startH || W != startW)
            {
                map[H][W] = '*';
                int parentH = data[H][W].parentH;
                int parentW = data[H][W].parentW;
                H = parentH;
                W = parentW;
            }
            map[startH][startW] = '*';
            PrintMap();
            return;
        }

        if (W - 1 >= 0 && map[H][W - 1] != 'X' && !data[H][W - 1].visit)
        {
            map[H][W - 1] = '.';
            Enqueue(&queue, { H,W - 1 });
            data[H][W - 1] = { true,H,W };
            PrintMap();
        }
        if (W + 1 < MAX_WIDTH && map[H][W + 1] != 'X' && !data[H][W + 1].visit)
        {
            map[H][W + 1] = '.';
            Enqueue(&queue, { H,W + 1 });
            data[H][W + 1] = { true,H,W };
            PrintMap();
        }
        if (H - 1 >= 0 && map[H - 1][W] != 'X' && !data[H - 1][W].visit)
        {
            map[H - 1][W] = '.';
            Enqueue(&queue, { H - 1,W });
            data[H - 1][W] = { true,H,W };
            PrintMap();
        }
        if (H + 1 < MAX_HEIGHT && map[H + 1][W] != 'X' && !data[H + 1][W].visit)
        {
            map[H + 1][W] = '.';
            Enqueue(&queue, { H + 1,W });
            data[H + 1][W] = { true,H,W };
            PrintMap();
        }
    }
}

void dfs(int startH, int startW, int goalH, int goalW)
{
    // 시작점과 목표지점의 좌표가 'X'이면 탐색을 시작하지 않고 return
    if (map[startH][startW] == 'X' || map[goalH][goalW] == 'X')
    {
        PrintMap();
        return;
    }

    // 방문여부를 모두 false로 설정 
    for (int i = 0; i < MAX_HEIGHT; ++i)
    {
        for (int j = 0; j < MAX_WIDTH; ++j)
        {
            data[i][j].visit = false;
        }
    }

    structure stack;
    Init(&stack);

    // 시작지점 넣고 시작
    Push(&stack, { startH, startW });
    data[startH][startW] = { true,-1,-1 };
    map[startH][startW] = '.';
    PrintMap();

    position pos;
    while (!IsEmpty(&stack))
    {
        Pop(&stack, &pos);
        int H = pos.height;
        int W = pos.width;

        if (H == goalH && W == goalW)   // goal에 도착하면
        {
            while (H != startH || W != startW)
            {
                map[H][W] = '*';
                int parentH = data[H][W].parentH;
                int parentW = data[H][W].parentW;
                H = parentH;
                W = parentW;
            }
            map[startH][startW] = '*';
            PrintMap();
            return;
        }
        if (W - 1 >= 0 && map[H][W - 1] != 'X' && !data[H][W - 1].visit)
        {
            map[H][W - 1] = '.';
            Push(&stack, { H,W - 1 });
            data[H][W - 1] = { true,H,W };
            PrintMap();
        }
        if (W + 1 < MAX_WIDTH && map[H][W + 1] != 'X' && !data[H][W + 1].visit)
        {
            map[H][W + 1] = '.';
            Push(&stack, { H,W + 1 });
            data[H][W + 1] = { true,H,W };
            PrintMap();
        }
        if (H - 1 >= 0 && map[H - 1][W] != 'X' && !data[H - 1][W].visit)
        {
            map[H - 1][W] = '.';
            Push(&stack, { H - 1,W });
            data[H - 1][W] = { true,H,W };
            PrintMap();
        }
        if (H + 1 < MAX_HEIGHT && map[H + 1][W] != 'X' && !data[H + 1][W].visit)
        {
            map[H + 1][W] = '.';
            Push(&stack, { H + 1,W });
            data[H + 1][W] = { true,H,W };
            PrintMap();
        }
    }
}

int main()
{
    //bfs(0, 0, 9, 9);
    dfs(0, 2, 9, 9);
   
    return 0;
}