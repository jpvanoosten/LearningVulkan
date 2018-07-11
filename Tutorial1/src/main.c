#include <vulkan/vulkan.h>

#include <stdio.h>
#include <stdlib.h>

#ifndef _countof
#define _countof(x) (sizeof(x)/sizeof(x[0]))
#endif

#define APPLICATION_VERSION VK_MAKE_VERSION(0, 0, 1)
const char* APPLICATION_NAME = "Tutorial1";

void ExitIfFailed(VkResult result)
{
    if ( result != VK_SUCCESS )
    {
        printf("Error: %#X", result);
        exit(result);
    }
}

void PrintInstanceLayers()
{
    uint32_t numInstanceLayers = 0;

    ExitIfFailed(vkEnumerateInstanceLayerProperties(&numInstanceLayers , NULL ));

    VkLayerProperties* vkLayerProperties = (VkLayerProperties*)malloc(sizeof(VkLayerProperties) * numInstanceLayers  );

    ExitIfFailed(vkEnumerateInstanceLayerProperties(&numInstanceLayers, vkLayerProperties) );

    printf("+------------------------------------+---------+---------+--------------------------------------------------+\n");
    printf("|            Instance                |  Spec   |  Impl.  |                                                  |\n");
    printf("|           Layer Name               | Version | Version |                     Description                  |\n");
    printf("+------------------------------------+---------+---------+--------------------------------------------------+\n");
    for ( int i = 0; i < numInstanceLayers; ++i )
    {
        uint32_t specVersion = vkLayerProperties[i].specVersion;
        char specVersionStr[10];
        snprintf(specVersionStr, 10, "%d.%d.%d",
                VK_VERSION_MAJOR(specVersion),
                VK_VERSION_MINOR(specVersion),
                VK_VERSION_PATCH(specVersion) );

        char implVersionStr[10];
        snprintf(implVersionStr, 10, "%d", vkLayerProperties[i].implementationVersion);

        printf("|%-36.36s|%9.9s|%9.9s|%-50.50s|\n",
               vkLayerProperties[i].layerName,
               specVersionStr,
               implVersionStr,
               vkLayerProperties[i].description );
    }
    printf("+------------------------------------+---------+---------+--------------------------------------------------+\n");

    free(vkLayerProperties);
}

void PrintInstanceExtensions()
{

}

VkInstance CreateInstance()
{
    VkInstance vkInstance = VK_NULL_HANDLE;

    // Initialize the application info structure.
    VkApplicationInfo vkApplicationInfo;
    vkApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    vkApplicationInfo.pNext = NULL;
    vkApplicationInfo.pApplicationName = APPLICATION_NAME;
    vkApplicationInfo.applicationVersion = APPLICATION_VERSION;
    vkApplicationInfo.pEngineName = APPLICATION_NAME;
    vkApplicationInfo.engineVersion = APPLICATION_VERSION;
    vkApplicationInfo.apiVersion = VK_API_VERSION_1_0;

    // Initialize the instance create info structure.
    VkInstanceCreateInfo vkInstanceCreateInfo;
    vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    vkInstanceCreateInfo.pNext = NULL;
    vkInstanceCreateInfo.flags = 0;
    vkInstanceCreateInfo.pApplicationInfo = &vkApplicationInfo;

#if _DEBUG
    const char* enabledExtensionNames[] = {
            "VK_LAYER_LUNARG_standard_validation"
    };
    vkInstanceCreateInfo.enabledLayerCount = _countof(enabledExtensionNames);
    vkInstanceCreateInfo.ppEnabledLayerNames = enabledExtensionNames;
#else
    vkInstanceCreateInfo.enabledLayerCount = 0;
    vkInstanceCreateInfo.ppEnabledExtensionNames = NULL;
#endif
    vkInstanceCreateInfo.enabledExtensionCount = 0;
    vkInstanceCreateInfo.ppEnabledExtensionNames = NULL;

    // Create the instance.
    ExitIfFailed(vkCreateInstance(&vkInstanceCreateInfo, NULL, &vkInstance));

    return vkInstance;
}

int main( int argc, char *argv[] )
{
    PrintInstanceLayers();

    VkInstance vkInstance = CreateInstance();



    return 0;
}
