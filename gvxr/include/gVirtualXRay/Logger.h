#ifndef __Logger_h
#define __Logger_h


//******************************************************************************
//  Include
//******************************************************************************
#include <string>
#include <fstream>
#include <iostream>


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

class Logger
{
public:
    Logger(const char* aFileName = "gvxr.log");
    Logger(const std::string& aFileName = "gvxr.log");
    ~Logger();

    bool is_open() const;
    void open(const std::string& aFileName);
    void close();

private:

    Logger& operator=(const Logger& aLogger);
    Logger(const Logger& aLogger);

    void logNow();
    
    std::ofstream m_output;
    std::string m_filename;
};


}


#endif // __Logger_h
