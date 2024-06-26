

//******************************************************************************
//  Include
//******************************************************************************
#include <chrono>
#include <ctime>
#include <sstream>
// #include<stdio.h>
#include<cstring>
#include<cmath>

#ifndef __Logger_h
#include <gVirtualXRay/Logger.h>
#endif


//******************************************************************************
//  Global variable
//******************************************************************************
gVirtualXRay::Logger gVirtualXRay::LOGGER("");


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
            m_output << "--------------------------------------------------------------------------------" << std::endl;
            logNow("Open log file") << "\t" << m_filename << std::endl;
            m_output << "--------------------------------------------------------------------------------" << std::endl;
        }
    }
}


//------------------
void Logger::close()
//------------------
{
    if (is_open())
    {
        m_output << "--------------------------------------------------------------------------------" << std::endl;
        logNow("Close log file") << "\t" << m_filename << std::endl;
        m_output << "--------------------------------------------------------------------------------" << std::endl;
        m_output.close();
        m_filename.clear();
    }
}


//------------------
void Logger::flush()
//------------------
{
    if (is_open())
    {
        m_output << std::flush;
    }
    else
    {
        std::cout << std::flush;
        std::cerr << std::flush;
        std::clog << std::flush;
    }
}


//--------------------------------------------
const std::string& Logger::getFilename() const
//--------------------------------------------
{
    return m_filename;
}


//-----------------------------------------
const std::string& Logger::getFname() const
//-----------------------------------------
{
    return m_filename;
}


//----------------------------------------------
std::ostream& Logger::logNow(const std::string& aMessage)
//----------------------------------------------
{
    if (is_open())
    {
        m_output << getNow() << " ---- " << aMessage;
        return m_output;
    }
    else
    {
        flush();

        std::clog << getNow() << " ---- " << aMessage;
        return std::clog;
    }
}


//------------------------------------------------
std::ostream& Logger::logError(const std::string& aMessage)
//------------------------------------------------
{
    if (is_open())
    {
        m_output << getNow() << " (EE) " << aMessage;
        return m_output;
    }
    else
    {
        flush();

        std::cerr << getNow() << " (EE) " << aMessage;
        return std::cerr;
    }
}


//--------------------------------------------------
std::ostream& Logger::logWarning(const std::string& aMessage)
//--------------------------------------------------
{
    if (is_open())
    {
        m_output << getNow() << " (WW) " << aMessage;
        return m_output;
    }
    else
    {
        flush();

        std::cerr << getNow() << " (WW) " << aMessage;
        return std::cerr;
    }
}


//----------------------------------------------------------
void Logger::progressBar(float aProgress, bool aNewLineFlag)
//----------------------------------------------------------
{
    const unsigned char total_length = 80;
    const unsigned char number_of_Xs = round(aProgress * total_length);

    if (is_open())
    {
        m_output <<
            "\r[" <<
            std::string(number_of_Xs, 'X') <<
            std::string(total_length - number_of_Xs, '-') <<
            "] " <<
            round(100.0 * aProgress) << "%" <<
            std::flush;

        if (aNewLineFlag) m_output << std::endl;
    }
    else
    {
        std::clog << 
            "\r[" <<
            std::string(number_of_Xs, 'X') <<
            std::string(total_length - number_of_Xs, '-') <<
            "] " <<
            round(100.0 * aProgress) << "%" <<
            std::flush;

        if (aNewLineFlag) std::clog << std::endl;
    }
}


//--------------------------------
std::string Logger::getNow() const
//--------------------------------
{
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    std::string message = std::ctime(&current_time);
    message.pop_back(); // Drop ENDL
    return message;
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
