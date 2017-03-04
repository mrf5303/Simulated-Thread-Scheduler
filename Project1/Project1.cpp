#include "myscheduler.h"
#include <time.h>

int main(int argc, char* argv[])
{
	srand(time(NULL));


	int numberOfCPUs = 5;
	// just change the policy to try different ones.
	MyScheduler ms = MyScheduler(Policy::PBS, numberOfCPUs);

	int totalThreads = 60;

	cout << "Testbench via Adam Geiger\n\n";

	cout << "\tUsing PBS, 5, random remaining times and arrivial time and priorities\n";

	int t_arriving_time;
	int t_remaining_time;
	int t_priority;
	int t_tid;

	for (int i = 0; i < totalThreads; i++)
	{
		t_arriving_time = i + rand() % 5;
		t_remaining_time = rand() % 40 + 1;
		t_priority = rand() % 20;
		t_tid = i;

		cout << "Thread Created: ";
		cout << "Arriving Time: " << t_arriving_time << " ";
		cout << "Remaining Time: " << t_remaining_time << " ";
		cout << "Priority: " << t_priority << " ";
		cout << "tid: " << t_tid << endl;

		ms.CreateThread(t_arriving_time, t_remaining_time, t_priority, t_tid);
	}

	ms.Go();

	return 0;
}