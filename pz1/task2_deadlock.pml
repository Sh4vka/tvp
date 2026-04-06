chan requests = [0] of {byte};
chan responses = [0] of {byte};

proctype Client() {
    byte req_id = 1;
    byte resp;
    
    printf("Client: starting\n");
    requests!req_id;

    req_id = 2;
    printf("Client: sending request %d (Wrong no wait)\n", req_id);
    requests!req_id;

    responses?resp;
    printf("Client: received responce for request %d\n", resp);

    responses?resp;
    printf("Client: received responce for request %d\n", resp);

    printf("Client finished\n");
}

proctype Server() {
    byte req;
    byte resp_id;

    printf("Server: starting\n");

    do
    :: requests?req ->
        printf("Server: received requests %d\n", req);

        resp_id = req;
        printf("Server: processing...\n");

        printf("Server: sending responce for %d\n", resp_id);
        responses!resp_id;
    od
}

init {
    run Server();
    run Client();
}
