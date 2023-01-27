#include <iostream>
#include <IOKit/IOKitLib.h>

int main() {
    CFMutableDictionaryRef matchingDict = IOServiceMatching(kIOUSBDeviceClassName);
    io_iterator_t iterator;
    kern_return_t kr = IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDict, &iterator);
    if (kr != KERN_SUCCESS) {
        std::cerr << "Error: IOServiceGetMatchingServices()" << std::endl;
        return 1;
    }

    io_service_t device;
    while ((device = IOIteratorNext(iterator))) {
        CFStringRef deviceName;
        kr = IORegistryEntryCreateCFProperty(device, CFSTR(kUSBProductString), kCFAllocatorDefault, 0, &deviceName);
        if (kr != KERN_SUCCESS) {
            std::cerr << "Error: IORegistryEntryCreateCFProperty()" << std::endl;
            IOObjectRelease(device);
            continue;
        }

        char deviceNameCStr[256];
        CFStringGetCString(deviceName, deviceNameCStr, sizeof(deviceNameCStr), kCFStringEncodingASCII);
        std::cout << "Device name: " << deviceNameCStr << std::endl;

        CFRelease(deviceName);
        IOObjectRelease(device);
    }
    IOObjectRelease(iterator);
    return 0;
}
