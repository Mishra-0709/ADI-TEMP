#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include <math.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 64
#define HEIGHT 64
#define GAUSSIAN_KERNEL_SIZE 5
#define LOW_THRESHOLD 20
#define HIGH_THRESHOLD 40
#define THETA_RES 1
#define MIN_LINE_LENGTH 10
#define EDGE_THRESHOLD  200

// Struct to represent a line segment
typedef struct {
    int x1, y1;
    int x2, y2;
} LineSegment;
//mask


// Function prototypes

void gaussian_blur(unsigned int image[HEIGHT][WIDTH], unsigned int result[HEIGHT][WIDTH]);
void gradient_magnitude_direction(unsigned int image[HEIGHT][WIDTH], float gradient[HEIGHT][WIDTH], float direction[HEIGHT][WIDTH]);
void non_maximum_suppression(float gradient[HEIGHT][WIDTH], float direction[HEIGHT][WIDTH], unsigned int result[HEIGHT][WIDTH]);
void double_threshold(unsigned int image[HEIGHT][WIDTH], unsigned int result[HEIGHT][WIDTH], int low, int high);
void hysteresis(unsigned int image[HEIGHT][WIDTH], unsigned int result[HEIGHT][WIDTH]);
float probabilistic_hough_transform(unsigned int edges[HEIGHT][WIDTH], LineSegment *lines, int max_lines);
float Detect_Lines(unsigned int image[HEIGHT][WIDTH]);

#endif // EDGE_DETECTION_H
