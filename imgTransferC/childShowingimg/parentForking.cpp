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
