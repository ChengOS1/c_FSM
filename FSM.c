/*state.c*/
#include <stdio.h>


enum year_state{
    SPRING = 1,
    SUMMER,
    AUTUMN,
    WINTER
};



enum year_event{
    EVENT1 = 1,
    EVENT2,
    EVENT3,
    EVENT4,
};


typedef struct FsmTable_s{
    int event;   //事件
    int CurState;  //当前状态
    void (*eventActFun)();  //函数指针
    int NextState;  //下一个状态
}FsmTable_t;




void spring_thing()
{
    printf("this is spring\n");
}
void summer_thing()
{
    printf("this is summer\n");
}
void autumn_thing()
{
    printf("this is autumn\n");
}
void winter_thing()
{
    printf("this is winter\n");
}


FsmTable_t year_table[] =
{
    //{到来的事件，当前的状态，将要要执行的函数，下一个状态}
    { EVENT1,  SPRING,    summer_thing,  SUMMER },
    { EVENT2,  SUMMER,    autumn_thing,  AUTUMN },
    { EVENT3,  AUTUMN,    winter_thing,  WINTER },
    { EVENT4,  WINTER,    spring_thing,  SPRING },
    //add your codes here
};







typedef struct FSM_s{
    int curState;//当前状态
    FsmTable_t * pFsmTable;//状态表
    int size;//表的项数
}FSM_t;

/*状态机注册,给它一个状态表*/
void FSM_Regist(FSM_t* pFsm, FsmTable_t* pTable)
{
    pFsm->pFsmTable = pTable;
}

/*状态迁移*/
void FSM_StateTransfer(FSM_t* pFsm, int state)
{
    pFsm->curState = state;
}

/*事件处理*/
void FSM_EventHandle(FSM_t* pFsm, int event)
{
    FsmTable_t* pActTable = pFsm->pFsmTable;
    void (*eventActFun)() = NULL;  //函数指针初始化为空
    int NextState;
    int CurState = pFsm->curState;
    int g_max_num = pFsm->size;
    int flag = 0; //标识是否满足条件
    int i;

    /*获取当前动作函数*/
    for (i = 0; i<g_max_num; i++)
    {
        //当且仅当当前状态下来个指定的事件，我才执行它
        if (event == pActTable[i].event && CurState == pActTable[i].CurState)
        {
            flag = 1;
            eventActFun = pActTable[i].eventActFun;
            NextState = pActTable[i].NextState;
            break;
        }
    }


    if (flag) //如果满足条件了
    {
        /*动作执行*/
        if (eventActFun)
        {
            eventActFun();
        }

        //跳转到下一个状态
        FSM_StateTransfer(pFsm, NextState);
    }
    else
    {
        printf("there is no match\n");
    }
}


int main()
{
    FSM_t year_fsm;
    FSM_Regist(&year_fsm,year_table);
    year_fsm.curState = SPRING;
    year_fsm.size = sizeof(year_table)/sizeof(FsmTable_t);


    printf("\n-------1--init spring------\n");
    printf("state:%d\n",year_fsm.curState);

    printf("\n-------2--spring->summer------\n");
    FSM_EventHandle(&year_fsm,EVENT1);
    printf("state:%d\n",year_fsm.curState);

    printf("\n-------3--summer->autumn------\n");
    FSM_EventHandle(&year_fsm,EVENT2);
    printf("state:%d\n",year_fsm.curState);

    printf("\n-------4--autumn->winter------\n");
    FSM_EventHandle(&year_fsm,EVENT3);
    printf("state:%d\n",year_fsm.curState);

    printf("\n-------5--winter->spring------\n");
    FSM_EventHandle(&year_fsm,EVENT4);
    printf("state:%d\n",year_fsm.curState);

    printf("\n-------6--receive EVENT2 not EVENT1------\n");
    FSM_EventHandle(&year_fsm,EVENT2);
    printf("state:%d\n",year_fsm.curState);

    return 0;
}