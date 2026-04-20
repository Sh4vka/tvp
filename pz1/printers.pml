byte printer_busy = 0;

proctype Task1() {
    printf("Task1: checking printer\n");

    if
    :: (printer_busy == 0) ->
        printf("Task1: printer is free, taking it\n");
        printer_busy = 1;

        printf("Task1: printing...\n");

        printf("Task1: releasing printer\n");
        printer_busy = 0;
    :: else ->
        printf("Task1: printer busy\n");
    fi
}

proctype Task2() {
    printf("Task2: checking printer\n");

    if
    :: (printer_busy == 0) ->
        printf("Task2: printer is free, taking it\n");
        printer_busy = 1;

        printf("Task2: printing...\n");

        printf("Task2: releasing printer\n");
        printer_busy = 0;
    :: else ->
        printf("Task2: printer busy\n");
    fi
}

init {
    atomic {
    run Task1();
    run Task2();
    }
}
