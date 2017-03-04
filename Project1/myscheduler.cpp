//myschedule.cpp
/*Define all the functions in 'myschedule.h' here.*/
#include "myscheduler.h"

void MyScheduler::CreateThread(int arriving_time, int remaining_time, int priority, int tid) //Thread ID not Process ID
{
	//Function to Create Thread(s) and insert them in the student
	//defined data structure
	ThreadNode *p = new ThreadNode();
	ThreadDescriptorBlock *d = new ThreadDescriptorBlock();
	d->arriving_time = arriving_time;
	d->remaining_time = remaining_time;
	d->priority = priority;
	d->tid = tid;
	p->data = d;

	if (WaitingList->head == NULL){
		WaitingList->head = p;
		WaitingList->tail = p;
	}
	else{
		p->next = WaitingList->tail;
		WaitingList->tail->back = p;
		WaitingList->tail = p;
		
		

	}
}

bool MyScheduler::Dispatch()
{
	//Todo: Check and remove finished threads
	//Todo: Check if all the threads are finished; if so, return false
	
	ThreadNode *thread = WaitingList->head;
	
	
	do{
		if (thread->data->arriving_time == timer){
			ThreadNode *temp = thread;
			if (thread == WaitingList->tail){}
			else if (thread->next != NULL){
				thread->back->next = thread->next;
				thread->next->back = thread->back;
				thread = thread->next;
			}			
			else{
				thread = new ThreadNode();
				thread->back = temp->back;

			}
			switch (policy)
			{
				
			case FCFS:		//First Come First Serve
				FCFSadd(temp->data);
				break;
			case STRFwoP:	//Shortest Time Remaining First, without preemption
				STRFwoPFunc(temp->data);
				break;
			case STRFwP:	//Shortest Time Remaining First, with preemption
				STRFwPFunc(temp->data);
				break;
			case PBS:		//Priority Based Scheduling, with preemption
				PBSFunc(temp->data);
				break;
			default:
				cout << "Invalid policy!";
				throw 0;
			}
		}
	} while ((thread = thread->back) != NULL);
	
	return EXECfunc();
}

void MyScheduler::FCFSadd(ThreadDescriptorBlock *x){
	ThreadNode *thread = new ThreadNode();
	thread->data = x;
	AddToBack(thread);
	
}
bool MyScheduler::EXECfunc(){
	int done=0;
	for (int i = 0; (unsigned)i < num_cpu; i++)
	{
		if (CPUs[i] == NULL){
			if (ReadyList->head != NULL)
				popHead(i);
			else
				done++;
			
		}
		
	}
	if ((unsigned)done == num_cpu)
			return false;
	else
		return true;
}


void MyScheduler::STRFwoPFunc(ThreadDescriptorBlock *x){
	
	ThreadNode *thread = new ThreadNode();
	thread->data = x;
	ThreadNode *CurrentNode = ReadyList->head;
	if (ReadyList->head == NULL){
		ReadyList->head = thread;
		ReadyList->tail = thread;
		

	}
	else{
		while (CurrentNode != NULL){
			if (CurrentNode->data->remaining_time > thread->data->remaining_time){
				swap(CurrentNode, thread);
				if (CurrentNode == ReadyList->head)
					ReadyList->head = thread;
				break;
			}
			else{
				CurrentNode = CurrentNode->back;
			}
		}
		if (CurrentNode == NULL)
			AddToBack(thread);
	}
	
}


void MyScheduler::STRFwPFunc(ThreadDescriptorBlock *x){
	int max = 0, loc=-1;

	for (int i = 0; (unsigned)i < num_cpu; i++){
		if (CPUs[i] == NULL){
			loc = i;
			break;
		}
		if (CPUs[i]->remaining_time > x->remaining_time)
		{
			if (max < CPUs[i]->remaining_time)
			{
				max = CPUs[i]->remaining_time;
				loc = i;
			}
		}

	}
	if (loc != -1){
		if (CPUs[loc] == NULL)
			CPUs[loc] = x;
		else{
			ThreadDescriptorBlock *temp = CPUs[loc];
			CPUs[loc] = x;
			STRFwoPFunc(temp);

		}
	}
	else
		STRFwoPFunc(x);

	
}

void MyScheduler::PBSFunc(ThreadDescriptorBlock *x){
	int max = 0, loc = -1;
	ThreadNode *thread = new ThreadNode();

	for (int i = 0; (unsigned)i < num_cpu; i++){
		if (CPUs[i] == NULL){
			loc = i;
			break;
		}
		if (CPUs[i]->priority > x->priority)
		{
			if (max < CPUs[i]->priority)
			{
				max = CPUs[i]->priority;
				loc = i;
			}
		}

	}
	if (loc != -1){
		if (CPUs[loc] == NULL)
			CPUs[loc] = x;
		else{
			ThreadDescriptorBlock *temp = CPUs[loc];
			CPUs[loc] = x;
			thread->data = temp;

		}
	}
	else
		thread->data = x;

	if (thread->data != NULL){
		ThreadNode *CurrentNode = ReadyList->head;
		if (ReadyList->head == NULL){
			ReadyList->head = thread;
			ReadyList->tail = thread;


		}
		else{
			while (CurrentNode != NULL){
				if (CurrentNode->data->priority > thread->data->priority){
					swap(CurrentNode, thread);
					if (CurrentNode == ReadyList->head)
						ReadyList->head = thread;
					break;
				}
				else{
					CurrentNode = CurrentNode->back;
				}
			}
			if (CurrentNode == NULL)
				AddToBack(thread);
		}

	}

	
}

void MyScheduler::popHead(int i){
	CPUs[i] = ReadyList->head->data;
	ReadyList->head = ReadyList->head->back;
}

void MyScheduler::swap(ThreadNode *CN, ThreadNode *TD){
	if (CN == ReadyList->head){
		CN->next = TD;
		TD->back = CN;
		ReadyList->head = TD;
	}
	else{
		TD->back = CN;
		TD->next = CN->next;
		TD->next->back = TD;
		CN->next = TD;
	}
}

void MyScheduler::AddToBack(ThreadNode *thread){
	if (ReadyList->head == NULL){
		ReadyList->head = thread;
		ReadyList->tail = thread;
	}
	else{
		thread->next = ReadyList->tail;
		thread->next->back = thread;
		thread->back = NULL;

		ReadyList->tail = thread;



	}
}
