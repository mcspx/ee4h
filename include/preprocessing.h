#include "../include/stdafx.h"

//Include guard
#ifndef PREPROCESSING_H
#define PREPROCESSING_H

cv::Mat binary_threshold(cv::Mat input, float threshold_value, int min, int max);

cv::Mat make_background_black(cv::Mat input, int white_level);

cv::Mat filter_red_channel(cv::Mat input, int new_value);

bool is_red_suit(cv::Mat input, int base_threshold);

bool is_red_suit_by_corners(cv::Mat input, float horiz_margin_perc, float vert_margin_perc, int base_threshold, int target_regions, float perc_red);

#endif