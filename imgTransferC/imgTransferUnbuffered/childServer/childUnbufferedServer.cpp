#include <unistd.h> //STDOUT_FILENO
#include "opencv2/opencv.hpp"
#include <iostream>
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
    //Before starting the camera redirect the STDOUT to null
    // since when starting the camera OpenCV prints undesired
	//characters
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
        //size_t toWrite = 0;
        //send until all bytes have been sent
        while (written<buf.size())
        {
            //send the current block of data
            //toWrite = BUFLEN < (buf.size()-written) ? BUFLEN : (buf.size()-written);
            written = write(STDOUT_FILENO, buf.data(), buf.size());
        }
    }
    return 0;

}
