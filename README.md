# client-Server-C



dserver/server.c and dclient/client.c form a TCP client-server application the return the current time by the server call to /usr/bin/uptime.
This is the same example as given in the book Advanced Programming in the Unix Environment.

The book example was modified for a TCP client-server application to stream a video. The scripts involved are:
 - dimgClient/client.c
 - dimgServer/server.c
 - imgTransferC/childP/childBuffered.cpp
 - imgTransferC/childDB/childBuffered.cpp

When the server receives a connection request, it forks a new process and overwrite it with the one obtained by compiling
childP/childBuffered.cpp. The latter opens the camera takes an image and encode it. Since it is a child process of the dimgServer/server.c,
they can communicate. The childBuffered write to dimgServer/server.c by writing the encoded image to STDOUT_FILENO. The communication is done
in block sizes (it is buffered). Once the server process receives all the data, it transfers it to the client.
The client does the same thing: forks a process, this time overwriting the process obtained from imgTransferC/childDB/childBuffered.cpp.
Now is client that has to transfer its data to the child process: so the child reads from STDIN_FILENO and finally the image is shown on the 
client side. 

dimgClient/client.c and dimgServer/server.c are C programs, imgTransferC/childP/childBuffered.cpp and imgTransferC/childDB/childBuffered.cpp
are C++ programs. Being launched with a fork (popen actually) there is neither mismatch nor dependemce between C and C++ programs.

Required libraries:
  OpenCV

In the folder there are already the objects and executables compiled on Ubuntu. To recompile first clear the files with :
  make clean
  
Then run 'make' from the outer project. The command will run the Makefiles specific in each folder.
