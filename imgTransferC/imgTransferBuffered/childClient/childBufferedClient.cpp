#include <stdlib.h>
#include <unistd.h>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <time.h>

using namespace std;

int main(int argc, char *argv[])
{
    //file descriptor to the child process
    std::cout<<"Child started\n";
    FILE *fp = fdopen(STDIN_FILENO, "r");
    cv::Mat frame;
    char temp[10] ={0};
    size_t total_bytes;
    unsigned short int elRead = 0;
    cv::namedWindow( "win", cv::WINDOW_AUTOSIZE );
    u_char *buf ;
    while(1)
    {
        temp[10] ={0};
        elRead = 0;
        if (fread(temp, 10, 1, fp)!=1)
        {
            printf("error trying to read size");
            return 1;
        }
        total_bytes = atoi((char*)temp); //115715;
        //allocate memory where to store encoded iamge data that will be received
        buf = (u_char*)malloc(total_bytes*sizeof(u_char));

        //initialize the number of bytes read to 0
        printf ("child process total_bytes: %ld\n",total_bytes);
        if((elRead = fread(buf, total_bytes, 1, fp))!=1)
        {
            printf("error elRead!=1, elRead=%hu\n", elRead);
            return 0;
        }
        std::vector<u_char> vec_img;
        vec_img.assign(buf, buf+total_bytes);
        cout<<"----"<<CV_8UC3<<endl;
        printf("child all bytes read\n");
        try
        {
            frame = cv::imdecode(vec_img, 1);
            //frame  = cv::imdecode(cv::Mat(3, total_bytes, CV_8UC3, buf), 1);
        }
        catch(const char*msg)
        {
            std::cerr<<msg<<std::endl;
        }
        cout<<"frame decoded"<<endl;
        try
        {
            cv::imshow("win", frame);
            if(cv::waitKey(2)>2){;}
        }
        catch (const char* msg)
        {
            std::cerr<<"total bytes "<< total_bytes<<std::endl;
            std::cerr << msg << std::endl;
        }

        free(buf);
    }
    std::cout<<"Returning\n";
    pclose(fp);
    return 0;

}


/*
while(total_bytes_read<total_bytes &&
    //(bytes_read_tihs_loop = read(fileno(fp), buf+total_bytes_read, bytes2Copy))
    (elRead = fread ( buf+total_bytes_read, bytes2Copy, 1, fp))
)
{
    bytes_read_tihs_loop = elRead*bytes2Copy;
    //update the number of bytes read
    total_bytes_read += bytes_read_tihs_loop;
    printf("child process: bytes read this loop %lu\n", bytes_read_tihs_loop);
    printf("child process: total_bytes_read<total_bytes? %lu<%lu\n", total_bytes_read,total_bytes);
    //bytes to be read at this iteration: either 4096 or the remaining (total_bytes-total)
    bytes2Copy = BUFLEN < (total_bytes-total_bytes_read) ? BUFLEN : (total_bytes-total_bytes_read);
    printf("child: bytes to read at next iteration: %ld\n\n", bytes2Copy);
}*/
