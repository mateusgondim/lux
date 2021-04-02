#ifndef LUX_CORE_ERROR_H_
#define LUX_CORE_ERROR_H_

#include <iostream>

//TODO: See how to implement DEBUG_BREAK() on Linux
namespace lux {

#if ASSERTIONS_ENABLED
  #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define DEBUG_BREAK() asm { int 3 }
  #elif __APPLE__
    #define DEBUG_BREAK() __asm__("int $3")
  #else
    #define DEBUG_BREAK()
  #endif
  
  #define ASSERT(condition, message) \
    if (condition) {} \
    else { \
      lux::report_assertion_failure(#condition, message, __FILE__, __LINE__); \
      DEBUG_BREAK(); \
    } 
#else
  #define ASSERT(condition, message)
#endif

  inline void report_assertion_failure(const char * expr, const char * message, 
                                       const char * file_name, int line_number)
  {
    std::cerr << "Assertion " << expr << " Failed on file " << file_name << ", line " 
              << line_number << std::endl << message << std::endl;
  } 

}

#endif
