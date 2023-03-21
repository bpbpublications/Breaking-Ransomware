// SampleDLL.cpp 
// compile with: /EHsc /LD 
  
#include "SampleDLL.h" 
  
#include <stdexcept>
  
using namespace std; 
  
namespace SampleDLLFuncs 
{ 
    double MySampleDLLFuncs::Add(double a, double b) 
    { 
        return a + b; 
    } 
  
    double MySampleDLLFuncs::Subtract(double a, double b) 
    { 
        return a - b; 
    } 
  
    double MySampleDLLFuncs::Multiply(double a, double b) 
    { 
        return a * b; 
    } 
  
    double MySampleDLLFuncs::Divide(double a, double b) 
    { 
        if (b == 0) 
        { 
            throw new invalid_argument("b cannot be zero!"); 
        } 
  
        return a / b; 
    } 
} 