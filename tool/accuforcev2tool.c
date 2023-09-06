/*
 * accuforcev2tool.c
 *
 * ACCUFORCE V2 LINUX CONFIGURATION UTILITY
 *
 * Copyright (C) 2023 Paul Dino Jones
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#include <linux/input.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>


#include <hidapi/hidapi.h>


/* Default values for the options */
#define DEFAULT_DEVICE_NAME "/dev/hidraw4"

#define VENDOR_ID      0x1fc9
#define PRODUCT_ID     0x804c

/* Options */
const char* device_name = DEFAULT_DEVICE_NAME;
const char* command = "calibrate";

/* Global variables about the initialized device */
int device_handle;


/* Parse command line arguments */
void parse_args(int argc, char* argv[])
{
    int i;

    int help = (argc < 2);

    for (i=1; i<argc && !help; i++)
    {
        if (!strcmp(argv[i],"-d"))
        {
            if (i<argc-1)
            {
                device_name = argv[++i];
            }
            else
            {
                help = 1;
            }
        }
        else
            command = argv[i];
    }


    if (help)
    {
        printf("-------- accuforcev2linux - Accuforce Setup Tool For Linux --------\n");
        printf("Description:\n");
        printf("  This program is for configuring basic settinges on an Accuforce Wheel\n");
        printf("  from linux.\n");
        printf("  THIS HAS BEEN TESTED ON MY WHEEL, BUT I MAKE NO GUARANTEES THIS WILL NOT BLOW YOURS UP\n");
        printf("  SENDING ARBITRARY HID COMMANDS TO YOUR DEVICE CAN BRICK A DEVICE. YOU HAVE BEEN WARNED.\n");
        printf("Usage:\n");
        printf("  %s <option> [<option>...] <command>\n",argv[0]);
        printf("Options:\n");
        printf("  -d <string>  device name (default: %s)\n",DEFAULT_DEVICE_NAME);
        printf("  -o           dummy option (useful because at least one option is needed)\n");
        printf("  center  ( position the wheel to its center position, and run command to set center to firmware )\n");
        printf("  calibrate  ( resets wheel by turning full left to full right. Do not hold onto the wheel during this task! )\n");

        exit(1);
    }
}


void center(const char* device_name)
{

    printf("\nHold the wheel center and wait a moment for the program to exit...\n");
    printf("\n        position                   \n");


    printf("\nSetting wheel center...\n");

    sleep(2);

    int buf_size = 17;
    int bytes_to_write = 17;
    unsigned char bytes[buf_size];

    for (int x = 0; x < buf_size; x++)
    {
        bytes[x] = 0x0;
    }

    bytes[0] = 0x02;
    bytes[1] = 0x01;
    bytes[2] = 0x57;
    bytes[3] = 0x01;
    bytes[4] = 0x84;
    bytes[5] = 0x03;
    bytes[6] = 0x10;
    bytes[7] = 0x00;
    bytes[8] = 0xff;
    bytes[9] = 0x01;
    bytes[10] = 0x00;
    bytes[11] = 0x06;
    bytes[12] = 0x06;
    bytes[13] = 0x06;
    bytes[14] = 0x7d;
    bytes[15] = 0x03;

    hid_device *handle;
    int i;

    int res = 0;
    res = hid_init();
    if (res != 0 )
    {
        fprintf(stderr, "could not initialize hid device layer\n");
    }


    handle = hid_open_path(device_name);
    //handle = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
    if (handle == NULL)
    {
        fprintf(stderr, "Could not open device %s\n", device_name);
        res = hid_exit();
        //return 0;
    }


    res = hid_write(handle, bytes, bytes_to_write);
    if (res < 0)
    {
    	fprintf(stderr, "Unable to write(): %ls\n", hid_error(handle));
    }
    // Close the device
    hid_close(handle);
    res = hid_exit();
}

void calibrate(const char* device_name)
{
    printf("\nPlease step a safe distance away from the wheel...\n");


    printf("\nCalibrating wheel...\n");

    sleep(2);

    int buf_size = 17;
    int bytes_to_write = 17;
    unsigned char bytes[buf_size];

    for (int x = 0; x < buf_size; x++)
    {
        bytes[x] = 0x0;
    }

    bytes[0] = 0x02;
    bytes[2] = 0x58;
    bytes[14] = 0x7d;
    bytes[15] = 0x03;

    hid_device *handle;
    int i;

    int res = 0;
    res = hid_init();
    if (res != 0 )
    {
        fprintf(stderr, "could not initialize hid device layer\n");
    }


    handle = hid_open_path(device_name);
    //handle = hid_open(VENDOR_ID, PRODUCT_ID, NULL);
    if (handle == NULL)
    {
        fprintf(stderr, "Could not open device %s\n", device_name);
        res = hid_exit();
        //return 0;
    }


    res = hid_write(handle, bytes, bytes_to_write);
    if (res < 0)
    {
    	fprintf(stderr, "Unable to write(): %ls\n", hid_error(handle));
    }
    // Close the device
    hid_close(handle);
    res = hid_exit();

    // hidraw can simply use open and write these bytes at the tradeoff of portability
    //int fd = open(device_name, O_RDWR);
    //lseek(fd, 1, SEEK_SET);
    //write(fd, &bytes, bytes_to_write);
    //close(fd);
}

int main(int argc, char* argv[])
{

    /* Parse command line arguments */
    parse_args(argc,argv);


    int tasksran = 0;

    if (strcmp(command,"calibrate") == 0)
    {
        calibrate(device_name);
        tasksran++;
    }
    if (strcmp(command,"center") == 0)
    {
        center(device_name);
        tasksran++;
    }

    if (tasksran == 0)
    {
        printf("Nothing to do, exiting\n");
    }

}
