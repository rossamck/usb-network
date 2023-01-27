#include <iostream>
#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>
#define kIOUSBDeviceClassName "IOUSBDevice"

int main() {
    // Create a matching dictionary to search for USB devices
    CFMutableDictionaryRef matchingDict = IOServiceMatching(kIOUSBDeviceClassName);
    if (!matchingDict) {
        std::cerr << "IOServiceMatching returned NULL" << std::endl;
        return 1;
    }

    // Get an iterator for the matching devices
    io_iterator_t iterator;
    if (IOServiceGetMatchingServices(kIOMainPortDefault, matchingDict, &iterator) != KERN_SUCCESS) {
        std::cerr << "IOServiceGetMatchingServices returned an error" << std::endl;
        return 1;
    }

    // Iterate over the devices
    io_service_t device;
    while ((device = IOIteratorNext(iterator))) {
        // Get the product name of the device
        CFStringRef productName = static_cast<CFStringRef>(IORegistryEntryCreateCFProperty(device, CFSTR("USB Product Name"), kCFAllocatorDefault, 0));
        if (productName) {
            std::cout << "Product Name: " << CFStringGetCStringPtr(productName, kCFStringEncodingUTF8) << std::endl;
            CFRelease(productName);
        }

        // Release the device
        IOObjectRelease(device);
    }

    return 0;
}
