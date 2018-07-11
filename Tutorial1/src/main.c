#include <vulkan/vulkan.h>

#include <stdio.h>
#include <stdlib.h>

#ifndef _countof
#define _countof(x) (sizeof(x)/sizeof(x[0]))
#endif

#define APPLICATION_VERSION VK_MAKE_VERSION(0, 0, 1)
const char *APPLICATION_NAME = "Tutorial1";

void ExitIfFailed(VkResult result) {
    if (result != VK_SUCCESS) {
        printf("Error: %#X", result);
        exit(result);
    }
}

void PrintInstanceLayers() {
    uint32_t numInstanceLayers = 0;

    ExitIfFailed(vkEnumerateInstanceLayerProperties(&numInstanceLayers, NULL));

    VkLayerProperties *vkLayerProperties = (VkLayerProperties *) malloc(sizeof(VkLayerProperties) * numInstanceLayers);

    ExitIfFailed(vkEnumerateInstanceLayerProperties(&numInstanceLayers, vkLayerProperties));

    printf("+------------------------------------+---------+---------+--------------------------------------------------+\n");
    printf("|            Instance                |  Spec   |  Impl.  |                                                  |\n");
    printf("|           Layer Name               | Version | Version |                     Description                  |\n");
    printf("+------------------------------------+---------+---------+--------------------------------------------------+\n");
    for (int i = 0; i < numInstanceLayers; ++i) {
        uint32_t specVersion = vkLayerProperties[i].specVersion;
        char specVersionStr[10];
        snprintf(specVersionStr, 10, "%d.%d.%d",
                 VK_VERSION_MAJOR(specVersion),
                 VK_VERSION_MINOR(specVersion),
                 VK_VERSION_PATCH(specVersion));

        char implVersionStr[10];
        snprintf(implVersionStr, 10, "%d",
                 vkLayerProperties[i].implementationVersion);

        printf("|%-36.36s|%9.9s|%9.9s|%-50.50s|\n",
               vkLayerProperties[i].layerName,
               specVersionStr,
               implVersionStr,
               vkLayerProperties[i].description);
    }
    printf("+------------------------------------+---------+---------+--------------------------------------------------+\n");

    free(vkLayerProperties);
}

void PrintInstanceExtensions() {
    uint32_t numInstanceExtensions = 0;

    ExitIfFailed(vkEnumerateInstanceExtensionProperties(NULL, &numInstanceExtensions, NULL));

    VkExtensionProperties *vkExtensionProperties = (VkExtensionProperties *) malloc(
            sizeof(VkExtensionProperties) * numInstanceExtensions);

    ExitIfFailed(vkEnumerateInstanceExtensionProperties(NULL, &numInstanceExtensions, vkExtensionProperties));

    printf("+----------------------------------------+---------+\n");
    printf("|               Extension                |  Spec   |\n");
    printf("|                 Name                   | Version |\n");
    printf("+----------------------------------------+---------+\n");

    for (int i = 0; i < numInstanceExtensions; ++i) {
        uint32_t specVersion = vkExtensionProperties[i].specVersion;
        char specVersionStr[10];
        snprintf(specVersionStr, 10, "%d.%d.%d",
                 VK_VERSION_MAJOR(specVersion),
                 VK_VERSION_MINOR(specVersion),
                 VK_VERSION_PATCH(specVersion));

        printf("|%-40.40s|%9.9s|\n", vkExtensionProperties[i].extensionName, specVersionStr);
    }
    printf("+----------------------------------------+---------+\n");

    free(vkExtensionProperties);
}

void PrintPhysicalDeviceProperties(VkPhysicalDevice physicalDevice) {
    VkPhysicalDeviceProperties vkPhysicalDeviceProperties;

    vkGetPhysicalDeviceProperties(physicalDevice, &vkPhysicalDeviceProperties);

    uint32_t apiVersion = vkPhysicalDeviceProperties.apiVersion;

    char apiVersionStr[10];
    snprintf(apiVersionStr, 10, "%d.%d.%d",
             VK_VERSION_MAJOR(apiVersion),
             VK_VERSION_MINOR(apiVersion),
             VK_VERSION_PATCH(apiVersion));

    const char* physicalDeviceTypeStr[] = {
            "VK_PHYSICAL_DEVICE_TYPE_OTHER",
            "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU",
            "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU",
            "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU",
            "VK_PHYSICAL_DEVICE_TYPE_CPU"
    };

    printf("vkGetPhysicalDeviceProperties:\n");
    printf("============================================================\n");
    printf("Device Name:    %s\n", vkPhysicalDeviceProperties.deviceName );
    printf("Device Type:    %s\n", physicalDeviceTypeStr[vkPhysicalDeviceProperties.deviceType] );
    printf("API Version:    %s\n", apiVersionStr );
    printf("Driver Version: %#x\n", vkPhysicalDeviceProperties.driverVersion );
    printf("Vendor ID:      %#x\n", vkPhysicalDeviceProperties.vendorID );
    printf("Device ID:      %#x\n", vkPhysicalDeviceProperties.deviceID );
    printf("============================================================\n");

}

/**
 * Create the Vulkan instance.
 * @return The handle to the Vulkan instance.
 */
VkInstance CreateInstance() {
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
    const char *enabledExtensionNames[] = {
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

/**
 * Get the first physical device returned by the the vkEnumeratePhysicalDevices
 * @param instance A handle to the Vulkan Instance.
 * @return A handle to the physical device (GPU adapter).
 */
VkPhysicalDevice GetPhysicalDevice(VkInstance instance) {
    uint32_t physicalDeviceCount = 0;

    ExitIfFailed(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, NULL));

    VkPhysicalDevice *vkPhysicalDevices = (VkPhysicalDevice *) malloc(sizeof(VkPhysicalDevice) * physicalDeviceCount);

    ExitIfFailed(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, vkPhysicalDevices));

    for ( int i = 0; i < physicalDeviceCount; ++i )
    {
        printf("Device [%d]:\n", i);
        PrintPhysicalDeviceProperties(vkPhysicalDevices[i]);
        printf("\n");
    }

    VkPhysicalDevice vkPhysicalDevice = vkPhysicalDevices[0];

    free(vkPhysicalDevices);

    return vkPhysicalDevice;
}

int main(int argc, char *argv[]) {

    PrintInstanceLayers();
    PrintInstanceExtensions();

    VkInstance vkInstance = CreateInstance();

    VkPhysicalDevice vkPhysicalDevice = GetPhysicalDevice(vkInstance);

    return 0;
}
