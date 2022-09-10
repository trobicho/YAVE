#pragma once

#include <stdexcept>
#include <string>
using std::string;
using std::exception;

namespace	YaveLib
{
  class	FatalError: public exception
  {
    public:
      //FatalError(const string& msg);
      FatalError(string msg): message(msg) {};

      const char * what () const throw ()
      {
        return message.c_str();
      }
    private:
      string message;
  };

  class	FatalVulkanInitError: public exception
  {
    public:
      FatalVulkanInitError(string msg): message(msg) {};

      const char * what () const throw ()
      {
        return message.c_str();
      }
    private:
      string message;
  };

  class	YaveRendererError: public exception
  {
    public:
      YaveRendererError(string msg): message(msg) {};

      const char * what () const throw ()
      {
        return message.c_str();
      }
    private:
      string message;
  };

  class	YaveInvalidInstanceParam: public exception
  {
    public:
      YaveInvalidInstanceParam(string msg): message(msg) {};

      const char * what () const throw ()
      {
        return message.c_str();
      }
    private:
      string message;
  };

  class	YaveInstanceParamError: public exception
  {
    public:
      YaveInstanceParamError(string msg): message(msg) {};

      const char * what () const throw ()
      {
        return message.c_str();
      }
    private:
      string message;
  };

  class	YaveHandlerError: public exception
  {
    public:
      YaveHandlerError(string msg): message(msg) {};

      const char * what () const throw ()
      {
        return message.c_str();
      }
    private:
      string message;
  };

  class	YaveHelperError: public exception
  {
    public:
      YaveHelperError(string msg): message(msg) {};

      const char * what () const throw ()
      {
        return message.c_str();
      }
    private:
      string message;
  };

  class	Error: public exception
  {
    public:
      Error(string msg): message(msg) {};

      const char * what () const throw ()
      {
        return message.c_str();
      }
    private:
      string message;
  };
}
