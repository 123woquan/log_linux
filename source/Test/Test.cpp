#pragma once
#include "unistd.h"
#include "../Logging.h"
#include "string"
#include "iostream"
#include "time.h"
int main()
{

    timeval start;
    timeval end;
    ::gettimeofday(&start,NULL);
  
    for(int i=0;i<1500000;++i)
    {
        LOG_INFO<<"hello world";
       // LOG_INFO<<"1";
    }
    
    ::gettimeofday(&end,NULL);
    auto delta_time = (end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000; 
    // delta_time = (end.tv_sec-start.tv_sec); 
    std::cout<<delta_time<<std::endl;
    int a=0;
    std::cin>>a;
    return 1;
}