// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"

// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
	int num;
    for (num = 0; num < 5; num++) {
    printf("*** thread %d looped %d times with priority %d\n", currentThread->getTid(), num, currentThread->getPrio());
    }
}
//时间轮转测试
void 
SimpleTest_timerSlice_Thread(int which){
    for(int i = 0; i < 20;++i){
        //printf("sysTick is %d\n",stats->systemTicks);
        interrupt->OneTick();
        int ticks = stats->systemTicks - currentThread->timeSlice_start;
        if(ticks >= TimerSlice){
            printf("thread %d yield with running time %d \n",currentThread->getTid(),ticks);
            currentThread->Yield();
        }
    }
}
//抢占调度算法测试
void
ForkThread(int which)
{
    int num;
    
    for (num = 0; num < 2; num++) {
         printf("*** thread %d looped %d times with priority %d\n", currentThread->getTid(), num, currentThread->getPrio());
        Thread *t = new Thread("forked thread t",0);
        t->Fork(SimpleThread,t->getTid());
        currentThread->Yield();
    }
}

void TSCommand(){
    char *s;
    for(int i = 0;i < MaxThreadnum; ++i){
        if(threadID[i].t_flag == true){
            printf("thread_id %4d   thread_name %20s   thread_status %9s\n",threadID[i].thread->getTid(),threadID[i].thread->getName(),threadID[i].thread->getStatusString());
        }
    }
}

void TSTestThread(int which){
    int num;
    
    for (num = 0; num < 4; num++) {
    printf("*** thread %d looped %d times\n", which, num);
        TSCommand();
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

// 抢占调度ThreadTest

// void
// ThreadTest1()
// {
//     DEBUG('t', "Entering ThreadTest1");

//     Thread *t = new Thread("forked thread t",2);
//     Thread *t1 = new Thread("forked thread t1",4);
//     Thread *t2 = new Thread("forked thread t2",0);


//     t1->Fork(SimpleThread,t1->getTid());
//     t->Fork(ForkThread,t->getTid());
//     t2->Fork(SimpleThread,t2->getTid());

// }

// 时间片轮转ThreadTest

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread t",2);
    Thread *t1 = new Thread("forked thread t1",4);
    Thread *t2 = new Thread("forked thread t2",0);


    t->Fork(SimpleTest_timerSlice_Thread,t1->getTid());
    t1->Fork(SimpleTest_timerSlice_Thread,t->getTid());
    t2->Fork(SimpleTest_timerSlice_Thread,t2->getTid());

}


void
ThreadTest_limit()
{
    DEBUG('t', "Entering ThreadTest1");

    for(int i = 0;i < 129;++i){
        Thread *t = new Thread("fork thread");
        printf("*** thread %d looped %d times\n", t->getTid(), 0);
        currentThread->Yield();
    }

    //SimpleThread(0);
}

void
TSTest(){
    Thread *t = new Thread("forked thread t");
    Thread *t1 = new Thread("forked thread t1");
    Thread *t2 = new Thread("forked thread t2");
     t->Fork(TSTestThread, t->getTid());
    t1->Fork(TSTestThread, t1->getTid());
    t2->Fork(TSTestThread, t2->getTid());

    
}


//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
    case 2:
    ThreadTest_limit();
    break;
    case 3:
    TSTest();
    break;
    default:
	printf("No test specified.\n");
	break;
    }
}

