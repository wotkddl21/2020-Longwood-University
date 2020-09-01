g++ -o lab3 main.cpp dcurses.c -lpthread

./lab3 <seed> <threadsnum>

Implement
2. i from 0 to vector.size(), I swapped ith vector with rand()%size()th vector.
rand()%size() is between [0,size()).

8. The command is ./Lab3 <seed> <threadsnum>

9.Even if I had only one single thread, I have to use join()method. Without that, the thread would be just ready to run.

10.If I had two or more threads, then I need to use a mutex.
Because the point to color is critical information, without the mutex , the cursor will move before coloring the supposed point.
Ex) 	thread 1 :	dcurses_moveTo(1,2); 
					dcurses_bg_pallet(1);
		thread 2 :	dcurses_moveTo(2,1);
					dcurses_bg_pallet(2);
The code above is what we want for thread 1,2 to do.
But without mutex, the code would be like below where point(2,1) could be colored with 1
	thread 1 :	dcurses_moveTo(1,2);
	thread 2 :	dcurses_moveTo(2,1);
				dcurses_bg_pallet(2);
	thread 1 :	dcurses_bg_pallet(1);
So I need to use a mutex with two or more threads in this case.

12.
Threadsnum  	time(msec)
1				36433.3
2				18749.3
5				10200.8
10				7921.53
20				7784.24
50				7796.4
100				7749.9
200				7759.4
500				7732.97
1000			7832.97
2000			7844.13
5000			7912.97
10000			8146.88

It looks like the time decreases when the threadsnum increases between 1 and 500.
But when the threadsnum is over 500, it seems like the time doesn't decrease even if the threads num increases rapidly.

At some extent, increasing threadsnum helps to reduce the time.
But above some extent, the huge threadsnum distract program to run coloring function.
This is because the huge threadsnum means the program should make a lot of threads and manage them.


