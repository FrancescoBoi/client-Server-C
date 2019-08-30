#include <stdlib.h>
#include <unistd.h>//read
#include "opencv2/opencv.hpp"
#include <iostream>
#include <time.h>
#define BUFLEN 4096

int main(int argc, char *argv[])
{
    //file descriptor to the child process
    FILE *fp;
    cv::Mat frame;
    char temp[10] = {0};
    //size_t bytes_read_tihs_loop = 0;
    size_t total_bytes_read = 0;
    unsigned short int elRead =0;
    size_t bytes2Copy = BUFLEN;
    //launch the child process with popen
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    if ((fp = popen("./childP/openCV", "r")) == NULL)
    {
        //error
        return 1;
    }

    //read the number of btyes of encoded image data
    fgets(temp, 10, fp);
    //convert the string to int
    size_t bytesToRead = atoi((char*)temp);
    //some prints
    std::cout<<bytesToRead<<std::endl;

    //allocate memory where to store encoded iamge data that will be received
    u_char *buf = (u_char*)malloc(bytesToRead*sizeof(u_char));

    //initialize the number of bytes read to 0
    printf ("bytesToRead: %ld\n",bytesToRead);
    elRead = fread ( buf+total_bytes_read, bytes2Copy, bytesToRead/bytes2Copy, fp);
    total_bytes_read += elRead*bytes2Copy;
    bytes2Copy = bytesToRead-total_bytes_read;
    elRead = fread ( buf+total_bytes_read, bytes2Copy, bytesToRead/bytes2Copy, fp);
    total_bytes_read += elRead*bytes2Copy;//bytes_read_tihs_loop;
    /*while(total_bytes_read<bytesToRead &&
        //(bytes_read_tihs_loop = read(fileno(fp), buf+total_bytes_read, bytes2Copy))
        (elRead = fread ( buf+total_bytes_read, bytes2Copy, 1, fp))
    )
    {
        bytes_read_tihs_loop = elRead*bytes2Copy;
        printf("%ld bytes to be copied\n", bytes2Copy);
        //read the bytes
        printf("%ld bytes read\n", bytes_read_tihs_loop);

        //update the number of bytes read
        total_bytes_read += bytes_read_tihs_loop;
        //bytes to be read at this iteration: either 4096 or the remaining (bytesToRead-total)
        bytes2Copy = BUFLEN < (bytesToRead-total_bytes_read) ? BUFLEN : (bytesToRead-total_bytes_read);
        printf("%ld btytes to copy at next iteration\n", bytes2Copy);
        printf("total_bytes_read<bytesToRead? %lu<%lu\n", total_bytes_read,bytesToRead);
        printf("%lu total bytes read\n\n", total_bytes_read);
    }*/
    printf("%lu bytes received over %lu expected\n", total_bytes_read, bytesToRead);
    printf("%lu final bytes read\n", total_bytes_read);
    pclose(fp);
        cv::namedWindow( "win", cv::WINDOW_AUTOSIZE );
    frame  = cv::imdecode(cv::Mat(1,total_bytes_read,0, buf), 0);
    cv::imshow("win", frame);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time with %d buffer length: %f\n", BUFLEN, cpu_time_used);
    cv::waitKey(0);
    return 0;

}
