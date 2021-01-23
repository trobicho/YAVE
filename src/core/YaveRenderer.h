#pragma once

#include "YaveParams.h"

class YaveRenderer
{
	public:
		YaveRenderer(YaveInstanceParams_t yaveInstanceParams);
		~YaveRenderer();

		void	init();

	private:
		void	createInstance();

	private:
		VkInstance				m_instance;
		YaveInstanceParams_t	m_yaveInstanceParams
		bool					m_wasInit; //XXX probably init() in constructor
};
