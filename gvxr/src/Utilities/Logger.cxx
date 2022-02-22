

//******************************************************************************
//  Include
//******************************************************************************
#include <ctime>

#ifndef __Logger_h
#include <gVirtualXRay/Logger.h>
#endif


//******************************************************************************
//  namespace
//******************************************************************************
using namespace gVirtualXRay;


//-----------------------------------
Logger::Logger(const char* aFileName)
//-----------------------------------
{
    open(aFileName);
}


//------------------------------------------
Logger::Logger(const std::string& aFileName)
//------------------------------------------
{
    open(aFileName);
}


//---------------
Logger::~Logger()
//---------------
{
    close();
}


//--------------------------
bool Logger::is_open() const
//--------------------------
{
    return m_output.is_open();
}



//---------------------------------------------
void Logger::open(const std::string& aFileName)
//---------------------------------------------
{
    // The filename has changed, close the previous log file if any
    if (m_filename != aFileName)
    {
        close();
    }

    // Update the filename
    m_filename = aFileName;

    // Open the stream if needed
    if (m_filename.size())
    {
        m_output.open (m_filename, std::ofstream::out | std::ofstream::app);

        // It is not open
        if (!is_open())
        {
            std::clog << "Warning, cannot open the log file \"" << m_filename << "\"." << std::endl;
        }
        // It is open
        else
        {

            m_output << std::endl;
            m_output << std::endl;
            logNow();
            m_output << "Open log file " << m_filename << std::endl;
        }
    }
}


//------------------
void Logger::close()
//------------------
{
    if (is_open())
    {
        m_output.close();
    }
}


//----------------------------------------------
Logger& Logger::operator=(const Logger& aLogger)
//----------------------------------------------
{
    // Do nothing
    return *this;
}


//-----------------------------------
Logger::Logger(const Logger& aLogger)
//-----------------------------------
{
    // Do nothing
}


//-------------------
void Logger::logNow()
//-------------------
{
    if (is_open())
    {
        auto now = std::chrono::system_clock::now();
        std::time_t current_time = std::chrono::system_clock::to_time_t(now);

        m_output << std::ctime(&current_time) << " --- ";
    }
}
