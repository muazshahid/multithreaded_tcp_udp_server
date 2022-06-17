# Multithreaded TCP/UDP Server 
A multithreaded TCP/UDP server which can handle multiple clients using threads.

The code compiles into an exectubale which creates TCP and UDP server using threads, this is only the server side implementation.
Users can connect to the server using the netstat command which comes built-in in linux distribution.

**The code uses Port 1234 for TCP Server and port 1235 for the UDP Server**
## Compiling 
>Compiling on Linux
```
cc server.c -o server -lsocket -lnsl -lpthread 
```
## Connecting to server
>To install netstat
```
sudo apt-get install netstat
```

>To connect to TCP server using netstat
```
netstat localhost:1234
```
>To connect to UDP server using netstat
```
netstat -u localhost:1235
```

*For connecting multiple clients, follow the same procedure from a different instance of terminal or a different computer.*
