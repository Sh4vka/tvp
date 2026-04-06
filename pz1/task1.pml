proctype ProcessA() {
    printf("A: start\n");
    printf("A: step1\n");
    printf("A: step2\n");
    printf("A: step3\n");
    printf("A: finish\n");
}
    
proctype ProcessB() {
    printf("B: start\n");
    printf("B: step1\n");
    printf("B: step2\n");
    printf("B: step3\n");
    printf("B: finish\n");
}

init {
    run ProcessA();
    run ProcessB();
}
