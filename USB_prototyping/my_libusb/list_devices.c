#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libusb-1.0/libusb.h>

int main(void) { 
    struct libusb_device *dev = NULL;;
    struct libusb_device **devlist = NULL;
    struct libusb_device_handle *devhandle = NULL;
    struct libusb_device_descriptor devdesc; 
    struct libusb_config_descriptor *cfgdesc;
    unsigned char strdesc[256];
    int number_of_devices; 
    int err = 0; 

    libusb_init(NULL); 
    number_of_devices = libusb_get_device_list(NULL, &devlist); 
    printf("Found %i usb-devices\n\n", number_of_devices); 

    if (number_of_devices < 0) {
        printf("Error, could not read devices");
        return -1;
    }
    
    for (int i = 0; i < number_of_devices; i++) {
        printf("Device %i\n", i);
        dev = *(devlist + i);
        err = libusb_open(dev, &devhandle);

        if (err) {
            printf("Could not open the device\n"); 
        } else { 
            err = libusb_get_device_descriptor(dev, &devdesc); 

            if (err) {
                printf("Could not get a descriptor\n"); 
            } else { 
                printf("USB-IF vendor ID: %d\n", devdesc.idVendor);
                printf("USB-IF product ID: %d\n", devdesc.idProduct);

                if (devdesc.iManufacturer > 0) {
                    err = libusb_get_string_descriptor_ascii(devhandle,
                            devdesc.iManufacturer,
                            strdesc,
                            256);
                    printf("Manufacturer: %s\n", strdesc);
                }


                if (devdesc.iProduct > 0) {
                    err = libusb_get_string_descriptor_ascii(devhandle,
                            devdesc.iProduct,
                            strdesc,
                            256);
                    printf("Product description: %s\n", strdesc);
                } 
            }

            err = libusb_get_active_config_descriptor(dev, &cfgdesc);

            if (err != 0) {
                printf("Could not get active config descriptor\n"); 
            } else { 
                if (cfgdesc->MaxPower > 0) {
                    printf("Max power %d mA\n", cfgdesc->MaxPower*2);   
                }
                
                if (cfgdesc->bNumInterfaces > 0) {
                    printf("Supported interfaces %d\n", cfgdesc->bNumInterfaces);   
                }
                libusb_free_config_descriptor(cfgdesc);
                cfgdesc = NULL;


            }
        }
        libusb_close(devhandle);
        devhandle = NULL;
        printf("\n");
    }



    libusb_free_device_list(devlist, 0); 
} 
