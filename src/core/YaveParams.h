#pragma once

/*
static const int	g_numValidationLayers = 1;
static const char	*g_validationLayers[g_numValidatinLayers] = {
	"VK_LAYER_LUNARG_standard_validation"
};
*/

struct	YaveInstanceParams_t
{
	const char* const*	applicationName;
	uint32_t			validationLayerCount = 0;
	const char* const*	validationLayerNames;
	uint32_t			extensionCount = 0;
	const char* const*	extensionNames;
};

namespace	YaveParamsValidator
{
	void	instanceParamsChecker(YaveInstanceParams_t &yaveInstanceParams);
	void	validationLayerCheck(uint32_t validationLayerCount
		, const char* const* validationLayerNames);
	void	extensionCheck(uint32_t extensionCount
		, const char* const* extensionNames);
}
