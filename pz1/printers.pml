bool printer_busy = false;  

proctype Task1() {
    printf("Task1: starting\n");
    
    if
    :: printer_busy == false ->
        printf("Task1: printer is free, taking it\n");
        printer_busy = true;
        printf("Task1: printing... (critical section)\n");
        printf("Task1: printing... (simulated work)\n");
        printer_busy = false;
        printf("Task1: printer released\n");
    :: else ->
        printf("Task1: printer busy, waiting\n");
    fi
    
    printf("Task1: finished\n");
}

proctype Task2() {
    printf("Task2: starting\n");
    
    if
    :: printer_busy == false ->
        printf("Task2: printer is free, taking it\n");
        printer_busy = true;
        printf("Task2: printing... (critical section)\n");
        printf("Task2: printing... (simulated work)\n");
        printer_busy = false;
        printf("Task2: printer released\n");
    :: else ->
        printf("Task2: printer busy, waiting\n");
    fi
    
    printf("Task2: finished\n");
}

init {
    atomic {
        run Task1();
        run Task2();
    }
}
