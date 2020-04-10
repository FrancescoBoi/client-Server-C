#define MAXLINE 4096
#ifdef __unix__
  #define CAM_BIAS CAP_V4L
#else
  #define CAM_BIAS 0
#endif
