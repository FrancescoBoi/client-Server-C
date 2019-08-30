#include <stdlib.h>
#include <unistd.h>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <time.h>
#define BUFLEN 4096

int main(int argc, char *argv[])
{
    cv::Mat frame;
    size_t img_size = 115715;
    u_char *buf = (u_char*)malloc(img_size*sizeof(u_char));
    size_t total_bytes_read = 0;
    size_t elRead =0;
    size_t bytes2Copy = BUFLEN;
    FILE * fp = fdopen(STDIN_FILENO, "r");


    elRead = fread ( buf, bytes2Copy, img_size/bytes2Copy, fp);
    total_bytes_read += elRead*bytes2Copy;
    bytes2Copy = img_size-total_bytes_read;
    printf("child received %ld\n", total_bytes_read);
    elRead = fread ( buf+total_bytes_read, bytes2Copy, 1, fp); //sostituire 1
    total_bytes_read += elRead*bytes2Copy;//bytes_read_tihs_loop;
    printf("child received %ld\n", total_bytes_read);

    cv::namedWindow( "win", cv::WINDOW_AUTOSIZE );
    frame  = cv::imdecode(cv::Mat(1,total_bytes_read,0, buf), 0);
    cv::imshow("win", frame);
    cv::waitKey(0);
    return 0;

}
/*
{
    FILE *fp = fdopen(STDIN_FILENO, "r");
    cv::Mat frame;

    size_t bytes_read = 0, imgSize, sent = 0;
    unsigned short int elRead = 0;
    size_t bytes2Copy = BUFLEN;

    imgSize = 115715; //atoi((char*)temp);
    //allocate memory where to store encoded iamge data that will be received
    u_char *img_data = (u_char*)malloc(imgSize*sizeof(u_char));

    printf ("child process imgSize: %ld\n",imgSize);
    while(bytes_read<imgSize)
    {
        printf("trying to read %ld bytes\n", bytes2Copy);
        if((elRead = fread(img_data+bytes_read, bytes2Copy, 1, fp))!=1)
                     fread (buf+total_bytes_read, bytes2Copy, 1, fp))
        {
            printf("error elRead!=1, elRead=%ld\n", elRead);
            return 0;
        }
        bytes_read += elRead*bytes2Copy;
        printf("child bytes copied=%ld\n", elRead*bytes2Copy);
        bytes2Copy = BUFLEN < (imgSize-bytes_read) ? BUFLEN : (imgSize-bytes_read);

        printf("child total bytes read=%ld, bytes to be copied at next iter: %ld\n\n", bytes_read, bytes2Copy);

    }
    printf("child all bytes read\n");
    pclose(fp);
    cv::namedWindow( "win", cv::WINDOW_AUTOSIZE );
    frame  = cv::imdecode(cv::Mat(1,bytes_read,0, img_data), 0);
    cv::imshow("win", frame);
    cv::waitKey(0);
    return 0;

}
*/
