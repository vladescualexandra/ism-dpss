#include<thread>
#include<string>
#include<iostream>
#include<vector>
using namespace std;

// 0.
// Note: there is a huge performance difference between cout and printf.
// printf() is much faster and it's not atomic but generally we see the entire message, 
// but with cout if the message is concateneted it might get splitted in the console.

// 1. Threads with function that uses cout.
void printFromThread() {
	cout << "<part-1> " << "<part-2>" << endl;
}

// 3. Threads with function that uses printf with id.
void printIDFromThread(int id) {
	// cout << "thread with id: " << id << endl;
	printf("thread with id: %d \n",id);
}

// 4. Threads that "return" a value.
// In C++ we have pointers which are very powerful because I can return results from functions
// by writing at that address.
void computeArraySum(int array[], int n, int& result) {
	// result = 0;
	for (int i = 0; i < n; i++) {
		result += array[i];
	}
}

// 7. 
void printMessage(string msg, int id) {
	printf("%s from %d \n", msg.c_str(), id);
}



void main() {

	cout << "Main thread." << endl << endl;

	/* 1.
	* In C++ the thread starts automatically, without the need to call any start / run function.
	* Here you don't need an object or a class. Because C++ has the power of using function pointers,
	* this is actually a generic template constructor and here we need to pass the function that we 
	* want to execute on another thread.
	*/


	// 1.
	thread t1(printFromThread);
	thread t2(printFromThread);
	thread t3(printFromThread);
	thread t4(printFromThread);

	/* 1.
	* In C++ the name of the function is a pointer to itself.
	* So here the system is creating a thread, a different call stack and at the base of the
	* callstack is this printFromThread function.
	*/

	// 2.
	t1.join();
	t2.join();
	t3.join();
	t4.join();

	/* 2. 
	* We need to synchronize at the end.
	* The main thread needs to wait the other threads.
	* So we instruct the main to wait on t1, t2, t3, t4.
	*/

	// 3.
	thread t5(printIDFromThread, 5);
	thread t6(printIDFromThread, 6);
	thread t7(printIDFromThread, 7);
	thread t8(printIDFromThread, 8);

	// 3.
	t5.join();
	t6.join();
	t7.join();
	t8.join();

	// 4. 
	int values1[] = { 10, 20, 30 };
	int values2[] = { 40, 50, 60 };
	int n = 3;
	int sum = 0;

	// If we use the same variable to where threads have to "return" the value, 
	// there will be a race condition - aka mistake

	thread t_sum1(computeArraySum, values1, n, ref(sum));
	thread t_sum2(computeArraySum, values2, n, ref(sum));
	t_sum1.join();
	t_sum2.join();

	cout << "sum = " << sum << endl;

	// 5. Threads that share the same variable, but this example uses more and bigger numbers to 
	// better see the race condition.
	const int N = 1e5;
	int* values3 = new int[N];
	for (int i = 0; i < N; i++) {
		values3[i] = i + 1;
	}

	sum = 0;
	computeArraySum(values3, N, sum);
	computeArraySum(values3, N, sum);
	cout << "The result in a sequentially way: " << sum << endl;

	sum = 0;
	thread t_sum3(computeArraySum, values3, N, ref(sum));
	thread t_sum4(computeArraySum, values3, N, ref(sum));
	t_sum3.join();
	t_sum4.join();
	cout << "The result in a parallel way: " << sum << endl;

	// 6. To avoid race condition, just use different variables, so you don't have to use 
	// mutex or smth.
	int sumT5 = 0;
	int sumT6 = 0;

	thread t_sum5(computeArraySum, values3, N, ref(sumT5));
	thread t_sum6(computeArraySum, values3, N, ref(sumT6));
	t_sum5.join();
	t_sum6.join();

	cout << "sumT5: " << sumT5 << endl;
	cout << "sumT6: " << sumT6 << endl;

	sum = sumT5 + sumT6;

	cout << "concatenated sum: " << sum << endl;

	// 7. Handling multiple threads using a collection.
	vector<thread> threads;
	int noThreads = 10;
	for (int i = 0; i < noThreads; i++) {
		threads.push_back(thread(printMessage, "Bye", i));
	}
	
	for (int i = 0; i < noThreads; i++) {
		threads[i].join();
	}

	cout << endl << "End of main." << endl << endl;
}

