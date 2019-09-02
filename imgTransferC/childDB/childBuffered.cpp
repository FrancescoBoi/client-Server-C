#include <stdlib.h>
#include <unistd.h>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <time.h>
#define BUFLEN 4096

int main(int argc, char *argv[])
{
    //file descriptor to the child process
    std::cout<<"Child started\n";
    FILE *fp = fdopen(STDIN_FILENO, "r");
    cv::Mat frame;
    char temp[10] ={0};
    //size_t bytes_read_tihs_loop = 0;
    size_t total_bytes_read = 0;
    size_t total_bytesToRead;
    unsigned short int elRead = 0;
    size_t bytes2Copy = BUFLEN;
    cv::namedWindow( "win", cv::WINDOW_AUTOSIZE );
    while(1)
    {
        temp[10] ={0};
        total_bytes_read = 0;
        elRead = 0;
        bytes2Copy = BUFLEN;
        if (fread(temp, 10, 1, fp)!=1)
        {
            printf("error trying to read size");
            return 1;
        }
        total_bytesToRead = atoi((char*)temp); //115715;
        //allocate memory where to store encoded iamge data that will be received
        u_char *buf = (u_char*)malloc(total_bytesToRead*sizeof(u_char));

        //initialize the number of bytes read to 0
        printf ("child process total_bytesToRead: %ld\n",total_bytesToRead);
        while(total_bytes_read<total_bytesToRead)
        {
          if((elRead = fread(buf+total_bytes_read, bytes2Copy, 1, fp))!=1)
          {
              printf("error elRead!=1, elRead=%ld\n", elRead);
              return 0;
            }
            total_bytes_read += elRead*bytes2Copy;
            //printf("child bytes copied=%ld\n", elRead*bytes2Copy);
            bytes2Copy = BUFLEN<(total_bytesToRead-total_bytes_read)? BUFLEN : (total_bytesToRead-total_bytes_read);
            //printf("child total bytes read=%ld, bytes to be copied at next iter: %ld\n\n", total_bytes_read, bytes2Copy);

          }
          printf("child all bytes read\n");
          frame  = cv::imdecode(cv::Mat(3,total_bytes_read,CV_8UC3, buf), 1);
          cv::imshow("win", frame);
          cv::waitKey(50);
    }
    std::cout<<"Returning\n";
    pclose(fp);
    return 0;

}


/*
while(total_bytes_read<total_bytesToRead &&
    //(bytes_read_tihs_loop = read(fileno(fp), buf+total_bytes_read, bytes2Copy))
    (elRead = fread ( buf+total_bytes_read, bytes2Copy, 1, fp))
)
{
    bytes_read_tihs_loop = elRead*bytes2Copy;
    //update the number of bytes read
    total_bytes_read += bytes_read_tihs_loop;
    printf("child process: bytes read this loop %lu\n", bytes_read_tihs_loop);
    printf("child process: total_bytes_read<total_bytesToRead? %lu<%lu\n", total_bytes_read,total_bytesToRead);
    //bytes to be read at this iteration: either 4096 or the remaining (total_bytesToRead-total)
    bytes2Copy = BUFLEN < (total_bytesToRead-total_bytes_read) ? BUFLEN : (total_bytesToRead-total_bytes_read);
    printf("child: bytes to read at next iteration: %ld\n\n", bytes2Copy);
}*/
