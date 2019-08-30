#include <unistd.h> //STDOUT_FILENO
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace std;
using namespace cv;

#define BUFLEN 4096

int main(int argc, char *argv[])
{
    Mat frame;
    std::vector<uchar> buf;
    //read image as grayscale
    frame = imread("./test.png",0);
    //encode image and put data into the vector buf
    imencode(".png",frame, buf);
    //send the total size of vector to parent
    //cout<<buf.size()<<endl;
    unsigned int written= 0;

    int i = 0;
    size_t toWrite = 0;
    //send until all bytes have been sent
    while (written<buf.size())
    {
        //send the current block of data
        toWrite = BUFLEN < (buf.size()-written) ? BUFLEN : (buf.size()-written);
        written += write(STDOUT_FILENO, buf.data()+written, toWrite);
        i++;
    }
    return 0;

}
