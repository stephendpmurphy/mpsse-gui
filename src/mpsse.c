#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include "mpsse.h"

#define APP_CHECK_STATUS(exp) {if(exp!=FT_OK){printf("%s:%d:%s(): status(0x%x) \
!= FT_OK\n",__FILE__, __LINE__, __FUNCTION__,exp);exit(1);}else{;}};
#define CHECK_NULL(exp){if(exp==NULL){printf("%s:%d:%s():  NULL expression \
encountered \n",__FILE__, __LINE__, __FUNCTION__);exit(1);}else{;}};

static int checkIfFtdiModuleLoaded(void) {
    FILE *fp;
    char modRes[128] = {0x00};

    // Run the lsmod command and check if the FTDI module is loaded
    fp = popen("lsmod | grep ftdi_sio", "r");

    // Make sure the file pointer isn't null
    if (fp == NULL) {
        printf("fp is null\n");
        return -1;
    }

    // Read out any resulst from running the lsmod command
    fgets(modRes, sizeof(modRes), fp);

    // Check if the result has any characters
    if (strlen(modRes) == 0) {
        // No characters, assume the module isn't loaded then.
        return 0;
    }
    else {
        // Some characters were found. The module must be loaded.
        return 1;
    }
}

#define delete_module(name, flags) syscall(__NR_delete_module, name, flags)
int removeFtdiModule(void) {
    if (delete_module("ftdi_sio", O_NONBLOCK) != 0) {
        perror("delete_module");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int mpsse_init(void) {
    FT_DEVICE_LIST_INFO_NODE device;
    uint32 chCount = 0;

    // Check if the FTDI serial module is loaded. If so, remove it.
    // This requires sudo when running after a build. Not required when installed.
    if (checkIfFtdiModuleLoaded() > 0) {
        if( removeFtdiModule() == EXIT_SUCCESS ) {
            printf("Removed FTDI module.\n");
        }
        else {
            return EXIT_FAILURE;
        }
    }

    Init_libMPSSE();

    chCount = mpsse_getChannelCount();

    printf("Found %d MPSSE capable channel(s).\n", chCount);
    for(uint8 i = 0; i < chCount; i++) {
        device = mpsse_getChannelInfo(i);
        printf("Information on channel number %d:\n", i);
        /* print the dev info */
        printf("    Flags=0x%x\n", device.Flags);
        printf("    Type=0x%x\n", device.Type);
        printf("    ID=0x%x\n", device.ID);
        printf("    LocId=0x%x\n", device.LocId);
        printf("    SerialNumber=%s\n", device.SerialNumber);
        printf("    Description=%s\n", device.Description);
        printf("    ftHandle=0x%x\n", (unsigned int)device.ftHandle); /*is 0 unless open*/
    }

    return EXIT_SUCCESS;
}

uint32 mpsse_getChannelCount(void) {
    FT_STATUS ftStatus;
    uint32 numDevs;
    uint32 channels = 0;
    // create the device information list
    ftStatus = FT_CreateDeviceInfoList(&numDevs);
    APP_CHECK_STATUS(ftStatus);
    // retrieve the number of devices that are capable of MPSSE
    ftStatus = SPI_GetNumChannels(&channels);
    APP_CHECK_STATUS(ftStatus);

    return channels;
}

FT_DEVICE_LIST_INFO_NODE mpsse_getChannelInfo(int index) {
    FT_DEVICE_LIST_INFO_NODE deviceInfo;
    FT_STATUS ftStatus;
    int count = mpsse_getChannelCount();

    if( (count <= 0) || (index >= count) ) {
        return deviceInfo;
    }

    ftStatus = SPI_GetChannelInfo(index, &deviceInfo);
    APP_CHECK_STATUS(ftStatus);

    return deviceInfo;
}