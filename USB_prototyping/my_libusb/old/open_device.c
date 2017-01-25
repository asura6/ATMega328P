#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libusb-1.0/libusb.h>

struct libusb_device_handle *usb_open_device() { 
    struct libusb_device *dev = NULL;;
    struct libusb_device **devlist = NULL;
    struct libusb_device_handle *devhandle = NULL;
    struct libusb_device_descriptor devdesc; 
    unsigned char strdesc[256];
    int number_of_devices; 
    int err = 0; 
    unsigned char match;
    const char dev_manufacturer[] = "codeandlife.com";
    const char dev_description[] = "USBexample"; 

    libusb_init(NULL); 
    number_of_devices = libusb_get_device_list(NULL, &devlist); 

    if (number_of_devices < 0) {
        printf("Error, could not read devices");
        return NULL;
    }

    for (int i = 0; i < number_of_devices; i++) { 
        dev = *(devlist + i);
        err = libusb_open(dev, &devhandle);

        if (err) {
            printf("Could not open some devices\n"); 
        } else { 
            err = libusb_get_device_descriptor(dev, &devdesc); 

            if (err) {
                printf("Could not get a descriptor\n"); 
            } else { 
                if (devdesc.iManufacturer > 0) {
                    err = libusb_get_string_descriptor_ascii(devhandle,
                            devdesc.iManufacturer,
                            strdesc,
                            256); 

                    if (strcmp((char *)strdesc, dev_manufacturer) == 0) {
                        printf("Matching manufacturer: %s\n", strdesc); 
                        match = 1;
                    } else {
                        match = 0;
                    }
                } 

                if (devdesc.iProduct > 0) {
                    err = libusb_get_string_descriptor_ascii(devhandle,
                            devdesc.iProduct,
                            strdesc,
                            256);
                    if (strcmp((char *)strdesc, dev_description) == 0 && match == 1) {
                        printf("Matching description: %s\n", strdesc); 
                        libusb_free_device_list(devlist, 0); 
                        return devhandle;
                    } 
                } 
            } 
        } 
    } 
    libusb_free_device_list(devlist, 0); 
    return NULL;
} 

int main(int argc, char **argv) {
    libusb_device_handle *handle = NULL;
    bRequest = LIBUSB_REQUEST_TYPE_STANDARD;

    if(argc < 2) {
        printf("Usage:\n");
        printf("usbtext.o on\n");
        printf("usbtext.o off\n");
        exit(1);
    }

    handle = usb_open_device(); 

    if(handle == NULL) {
        fprintf(stderr, "Could not find USB device!n");
        usb_close(handle);
        exit(1);
    }

//    int err = libusb_control_transfer(handle,
//		uint8_t  	bmRequestType,
//		uint8_t  	bRequest,
//		uint16_t  	wValue,
//		uint16_t  	wIndex,
//		unsigned char *  	data,
//		uint16_t  	wLength,
//		unsigned int  	timeout 
//	) 	
//    usb_close(handle);

    printf("%d 
    usb_close(handle);
    return 0;
} 
