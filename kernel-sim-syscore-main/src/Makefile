all: process_ clk_ scheduler_
	gcc -o os-sim process_generator.c clk.c clk.h helper.h utilites/*.h utilites/*.c memory_manager.h memory_manager.c -lm

	

process_:
	gcc -o process process.c process.h clk.h clk.c


clk_:
	gcc -o clk clkRun.c clk.c clk.h


scheduler_:
	gcc -o scheduler scheduler.c scheduler.h Algos/*.h Algos/*.c utilites/*.c utilites/*.h helper.h clk.c clk.h -lm



clean:
	rm -f process clk scheduler os-sim scheduler.log scheduler.perf memory.log 
