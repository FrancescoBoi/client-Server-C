#include <unistd.h> //STDOUT_FILENO
#include "opencv2/opencv.hpp"
#include <iostream>
//#include <sys/types.h>
#include <fcntl.h>
#include "constants.h"
using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    Mat frame;
    std::vector<uchar> buf;
    int bak, temp;

    //read image as grayscale
    namedWindow( "Camera", WINDOW_AUTOSIZE );
    //sTAMPA stringhe che potrebbero incasinare tutto il resto: in un altro
    //programa avevo dirottato l'stdout
    fflush(stdout);
    bak = dup(1);
    temp = open("/dev/null", O_WRONLY);
    dup2(temp, 1);
    close(temp  );
    VideoCapture cam(0 + CAM_BIAS);
    if (!cam.isOpened())
    {
        cout << "\nCould not open reference " << 0 << endl;
        return -1;
    }
    //for (int i=0; i<2; i++)
    //{
        //cam>>frame;
    //}
    //frame = imread("/home/fra/Desktop/imgTransferC/childP/test2.png",0);
    /*Set the normal STDOUT back*/
    fflush(stdout);
    dup2(bak, 1);
    close(bak);
    FILE * f = fdopen(STDOUT_FILENO, "w");
    fflush(f);
    while(1)
    {
        cam>>frame;
        //encode image and put data into the vector buf
        imencode(".png",frame, buf);
        //send the total size of vector to parent
        cout<<buf.size()<<endl;
        unsigned int written= 0;

        //send until all bytes have been sent

        written = fwrite ( buf.data(), buf.size(), 1, f );
        fflush(f);
    }
    return 0;

}
