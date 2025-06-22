OS-Sim: Operating System Simulation

This project is a simulation of a mini-operating system designed to implement CPU scheduling and memory management techniques. Developed as part of an advanced systems programming course, the simulator supports multiple scheduling algorithms including Round Robin (RR), Highest Priority First (HPF), and Shortest Remaining Time Next (SRTN).

Processes are generated dynamically by a process_generator component based on their arrival times and are then handed over to the scheduler. Each process is responsible for terminating itself once its runtime completes. Memory management features are introduced in later phases, requiring efficient and modular code design. 
Execution Example:
make
./os-sim -s rr -f processes.txt
The project emphasizes modularity and extensibility to support future enhancements and follows a team-based development approach (3-4 members per team) 



Workload Distribution:
•	Mohamed Yasser: Process Generator + Data structures


•	Aser Mohamed: Scheduler + Semaphores sync


•	Beshoy Sorial: HPF + SRTN + RR

