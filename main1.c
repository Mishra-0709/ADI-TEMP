/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

// rps-demo
// Created using ./ai8xize.py --verbose --log --test-dir pytorch --prefix rps-demo --checkpoint-file trained/ai85-rps82-chw.pth.tar --config-file networks/rps-chw.yaml --softmax --embedded-code --device MAX78000 --compact-data --mexpress --timer 0 --display-checkpoint --fifo

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "mxc_device.h"
#include "mxc_sys.h"
#include "gcfr_regs.h"
#include "fcr_regs.h"
#include "icc.h"
#include "dma.h"
#include "led.h"
#include "tmr.h"
#include "pb.h"
#include "cnn.h"
#include "weights.h"
#include "sampledata.h"
#include "mxc_delay.h"
#include "camera.h"
#ifdef BOARD_EVKIT_V1
#include "bitmap.h"
#include "tft_ssd2119.h"
#endif
#ifdef BOARD_FTHR_REVA
#include "tft_ili9341.h"
#include "road_edge_lines.h"
#endif

// Comment out USE_SAMPLEDATA to use Camera module
#define USE_SAMPLEDATA

#define CAMERA_TO_LCD (1)
#define IMAGE_SIZE_X (64)
#define IMAGE_SIZE_Y (64)
#define CAMERA_FREQ (10 * 1000 * 1000)

#define TFT_BUFF_SIZE 35 // TFT buffer size
#define CNN_NUM_OUTPUTS 3 // number of classes



// Assuming these functions and variables are defined elsewhere in your project:
// extern void capture_camera_img(void);
// extern void process_camera_img(uint32_t *input0, uint32_t *input1, uint32_t *input2);
// extern void convert_img_signed_to_unsigned(uint32_t *input0, uint32_t *input1, uint32_t *input2);
// extern void MXC_TFT_ClearScreen(void);
// extern void TFT_Print(char *buff, int x, int y, void *font, int len);
// extern void MXC_Delay(uint32_t microseconds);

// Global variables for storing sample data and the TFT display buffer
char buff[128];  // Buffer for text on the display
void *font_2;    // Font pointer, replace with your actual font type
uint32_t input_0[1024], input_1[1024], input_2[1024];  // Placeholder for sample data arrays

// Function to simulate sample data for testing (to be replaced by actual camera capture)
void generate_sample_data(void) {
    for (int i = 0; i < 1024; i++) {
        input_0[i] = i;
        input_1[i] = i + 1;
        input_2[i] = i + 2;
    }
}

// Main function
int main(void)
{
    int frame = 0;
    uint32_t input_0_camera[1024];
    uint32_t input_1_camera[1024];
    uint32_t input_2_camera[1024];

    // Time interval between frames (in microseconds)
    const uint32_t frame_interval_us = 1000000; // 1 second

    // Initialize sample data or camera system
    generate_sample_data(); // Only needed if USE_SAMPLEDATA is enabled

    while (1) {
        printf("\nProcessing frame %d...\n", ++frame);

#ifdef USE_SAMPLEDATA
        // Use sample data
        printf("Using sample data for frame %d\n", frame);
        memcpy(input_0_camera, input_0, sizeof(input_0_camera));
        memcpy(input_1_camera, input_1, sizeof(input_1_camera));
        memcpy(input_2_camera, input_2, sizeof(input_2_camera));
        convert_img_signed_to_unsigned(input_0_camera, input_1_camera, input_2_camera);
#else
        // Capture and process camera frame
        printf("Capturing and processing camera frame %d\n", frame);
        capture_camera_img();  // Your camera capture function
        process_camera_img(input_0_camera, input_1_camera, input_2_camera);  // Process captured frame
#endif

#ifdef TFT_ENABLE
        // Display the frame on the LCD
        MXC_TFT_ClearScreen();
        printf("Displaying frame %d on LCD\n", frame);
        TFT_Print(buff, 10, 30, font_2, snprintf(buff, sizeof(buff), "User Move"));
        TFT_Print(buff, 152, 30, font_2, snprintf(buff, sizeof(buff), "Computer Move"));
#endif

        // Wait for the next frame interval
        MXC_Delay(frame_interval_us);
    }

    return 0;
}
