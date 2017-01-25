#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libusb-1.0/libusb.h>

#define USB_LED_ON 1
#define USB_LED_OFF 0

#define CTRL_IN	(LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN)

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
    int err;
    unsigned char buffer[256];
    (void)argv; 

    if(argc < 2) {
        printf("Usage:\n");
        printf("usbtext.o on\n");
        printf("usbtext.o off\n");
        exit(1);
    }

    handle = usb_open_device(); 

    if(handle == NULL) {
        fprintf(stderr, "Could not find USB device!n");
        exit(1);
    }


    if(strcmp(argv[1], "on") == 0) { 
        err = libusb_control_transfer(handle,
                LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE  | LIBUSB_RECIPIENT_ENDPOINT, USB_LED_ON,
                0,
                0,
                buffer, 
                sizeof(buffer),
                5000); 
    } else if(strcmp(argv[1], "off") == 0) {
        err = libusb_control_transfer(handle,
                LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE  | LIBUSB_RECIPIENT_ENDPOINT, USB_LED_OFF,
                0,
                0,
                buffer, 
                sizeof(buffer),
                5000); 
    }

    if(err < 0) {
        fprintf(stderr, "USB error: %sn", libusb_error_name(LIBUSB_ERROR_IO));
    }

    libusb_close(handle); 
    return 0;
} 
