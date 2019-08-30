#include <stdlib.h>
#include <unistd.h>//read
#include "opencv2/opencv.hpp"
#include <iostream>
#include <sys/wait.h>
#include "opencv2/opencv.hpp"
#define BUFLEN 4096

using namespace cv;

int main(int argc, char *argv[])
{
    int status;
    Mat frame;
    std::vector<uchar> img;
    //read image as grayscale
    frame = imread("../childP/test.png",0);
    //encode image and put data into the vector buf
    imencode(".png",frame, img);
    //send the total size of vector to parent

    size_t toWrite = 0;
    size_t written= 0;
    FILE * f = popen("./childP/openCV", "w");
    printf("forked\n");
    while (written<img.size())
    {
        //send the current block of data
        toWrite = BUFLEN < (img.size()-written) ? BUFLEN : (img.size()-written);
        //written += write(STDOUT_FILENO, buf.data()+written, toWrite);
        written += toWrite*fwrite ( img.data()+written, toWrite, 1, f );
        printf("written: %ld\n", written);
    }
    //wait(&status);
    return 0;

}

/*
int main(int argc, char *argv[])
{
    int status;
    FILE* fp;
    size_t sent = 0;
    size_t is_sent = 0;
    size_t img_size, n;
    cv::Mat frame;
    std::vector<uchar> img;

    frame = imread("/home/fra/Desktop/imgTransferC/childP/test.png",0);
    imencode(".png",frame, img);
    img_size = img.size();
    if ((fp = popen("/home/fra/Desktop/imgTransferC/childShowingimg/childP/openCV", "w")) == NULL)
    {
        //error
        return 1;
    }
    //printf(" forked\n");
    sleep(2);
    n = BUFLEN;

    //printf("image size=%ld\n", img.size());
    //printf("sent=%ld, img_size=%ld\n", sent, img_size);
    while(sent<img_size)
    {
        //printf("to send: %ld\n", n);

        n = BUFLEN<(img_size-sent) ? BUFLEN:(img_size-sent);
        if((is_sent = fwrite(img.data()+sent, n, 1, fp)) !=1)
        {
            printf("error sending size, exiting program: %d!=1\n", is_sent);
            return 1;
        }
        usleep(5000);
        sent += is_sent*n;
        printf("parent: sent %ld bytes over %ld\n", sent, img_size);
    }
        wait(&status);
    //cv::waitKey(0);
    return 0;

}
*/
