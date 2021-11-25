	This program is written to solve a different version of Dining-Philosophers Problem. It covers multithreaded programming and Process Synchronization (mutex, semaphors).

	N philosophers enter the famous Diyar-ı Kaşığı Kırık restaurant with its pilaf. A philosopher at a time eats only one serving of rice, thinks a little, and re-enters the eating-thinking cycle. They eat until the restaurant closes. 
There are also M waiters serving rice in the restaurant. Each of the waiters brings a portion of rice to the tray in the middle at a time, then hangs out on whatsapp a little and continues the serve-hang on whatsapp cycle.

	At most, one person, whether a philosopher or a waiter, should be able to access the tray in the middle. Tray capacity is 2N portions.

	This program counts how many servings of rice a philosopher ate until the restaurant closes and prints it on the screen. 

	(The printf functions, in which the current actions of the philosophers and waiters are specified, have been commented. If it is desired to follow the current movements of each person, they can be removed from the comment line.)


Compile
gcc -pthread diningPhilosophers.c -o diningPhilosophers


Run

	At the start, program takes the number of philosophers(N), the number of waiters(M), the thinking time of the philosophers in milliseconds, the time the waiters hang out on WhatsApp in milliseconds, and the time remaining until the closing of the restaurant in seconds from the command line.

./diningPhilosophers [Number of Philosophers] [Number of Waiters] [Thinking Time(ms) for Philosophers] [Hanging on Whatsapp Time(ms) for Waiters] [Restaurant closing Time(s)]

Sample Run Command
./diningPhilosophers 10 7 1500 1000 10
