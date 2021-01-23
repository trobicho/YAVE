#pragma once

#include <stdexcept>

namespace YaveLib
{
	class FatalError: public exception
	{
		public:
  			explicit	FatalError(const string& what_arg);
  			explicit	FatalError(const char* what_arg);
	};

	class FatalVulkanInitError: public exception
	{
		public:
  			explicit	FatalVulkanInitError(const string& what_arg);
  			explicit	FatalVulkanInitError(const char* what_arg);
	};

	class YaveInvalidInstanceParam: public exception
	{
		public:
  			explicit	YaveInvalidInstanceParam(const string& what_arg);
  			explicit	YaveInvalidInstanceParam(const char* what_arg);
	};

	class YaveInstanceParamError: public exception
	{
		public:
  			explicit	YaveInstanceParamError(const string& what_arg);
  			explicit	YaveInstanceParamError(const char* what_arg);
	}
};