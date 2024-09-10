* Unlike processes (created by for fork()), threads (created with p_thread* functions) share the same memory and variables with eachother. 
* Same applies to open files
* pthread_create() --> pthread_join()
    OR
    pthread_create() --> pthread_detach() --> pthread_exit()
    OR pass attribute PTHREAD_CREATE_DETACHED
* "protect the forks state with a mutex for *each* of them"
* think -> eat -> sleep -> think
