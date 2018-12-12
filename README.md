# Distributed-OS
Programs for Distributed OS Lab for NSIT-IT
## Following is the list of Programs
 -  IPC using Pipes
 -  IPC using FIFO
 -  IPC using Message Queue
 -  IPC using Shared Memory
 -  Semaphore Demonstration
 -  Socket Programming
 -  RPC implementation
 
 ---
 ## How to run?
 All the programs are already compiled and just need to be run on a system having gcc. Open a terminal and type navigate to the folder of the program and use 
 ```
 ./{file_name}
 ```
 
 For RPC implementation, first check by typing
 ```
 $ rpcinfo
 ```
 If it gives an error use the following command
 ```
 $ sudo apt-get install rpcbind
 ```
 After that, use 
 ```
 ./IDL_server
 ```
 to run the server and open another terminal and type
 ```
 ./IDL_client localhost
 ```
 
