#include <Servo.h>
#include <Adafruit_LSM6DSOX.h>
#include <math.h>
#include <stdio.h>

// For SPI mode, we need a CS pin
#define LSM_CS 10
// For software-SPI mode we need SCK/MOSI/MISO pins
#define LSM_SCK 13
#define LSM_MISO 12
#define LSM_MOSI 11

// Temporary test values for walking motion in txt file
#define ROWS = 174
#define COLS = 6

Adafruit_LSM6DSOX sox;

Servo left_hip_1;
Servo left_hip_2;
Servo left_hip_3;
Servo left_knee; // higher is back
Servo left_ankle_1;
Servo left_ankle_2;
Servo right_hip_1;
Servo right_hip_2;
Servo right_hip_3;
Servo right_knee; // Higher is forward
Servo right_ankle_1;
Servo right_ankle_2;

//const int stepdata[][12] PROGMEM = {
int stepdata[][12] = {
{-0.0, -8.98, -461.42, 947.1400000000001, -485.71999999999997, 8.98, -0.0, -8.98, -461.42, 947.1400000000001, -485.71999999999997, -485.71999999999997},
{-0.0, -37.75, -944.22, 1938.3, -994.0799999999999, 37.75, -0.0, -37.75, -944.22, 1938.3, -994.0799999999999, -994.0799999999999},
{-0.0, -84.11, -1404.6100000000001, 2883.6800000000003, -1479.0700000000002, 84.11, -0.0, -84.11, -1404.6100000000001, 2883.6800000000003, -1479.0700000000002, -1479.0700000000002},
{-0.0, -145.91, -1841.61, 3781.34, -1939.74, 145.91, -0.0, -145.91, -1841.61, 3781.34, -1939.74, -1939.74},
{-0.0, -221.02, -2254.21, 4629.29, -2375.08, 221.02, -0.0, -221.02, -2254.21, 4629.29, -2375.08, -2375.08},
{-0.0, -307.27, -2641.4100000000003, 5425.450000000001, -2784.04, 307.27, -0.0, -307.27, -2641.4100000000003, 5425.450000000001, -2784.04, -2784.04},
{-0.0, -402.38000000000005, -3002.15, 6167.660000000001, -3165.51, 402.38000000000005, -0.0, -402.38000000000005, -3002.15, 6167.660000000001, -3165.51, -3165.51},
{-0.0, -503.87, -3335.34, 6853.62, -3518.29, 503.87, -0.0, -503.87, -3335.34, 6853.62, -3518.29, -3518.29},
{-0.0, -609.01, -3639.82, 7480.9400000000005, -3841.1099999999997, 609.01, -0.0, -609.01, -3639.82, 7480.9400000000005, -3841.1099999999997, -3841.1099999999997},
{-0.0, -714.82, -3914.39, 8047.01, -4132.62, 714.82, -0.0, -714.82, -3914.39, 8047.01, -4132.62, -4132.62},
{-0.0, -818.02, -4157.7, 8549.0, -4391.3, 818.02, -0.0, -818.02, -4157.7, 8549.0, -4391.3, -4391.3},
{-0.0, -915.0600000000001, -4368.29, 8983.77, -4615.4800000000005, 915.0600000000001, -0.0, -915.0600000000001, -4368.29, 8983.77, -4615.4800000000005, -4615.4800000000005},
{-0.0, -1002.14, -4544.4400000000005, 9347.68, -4803.24, 1002.14, -0.0, -1002.14, -4544.4400000000005, 9347.68, -4803.24, -4803.24},
{-0.0, -1075.25, -4684.13, 9636.41, -4952.28, 1075.25, -0.0, -1075.25, -4684.13, 9636.41, -4952.28, -4952.28},
{-0.0, -1130.3100000000002, -4784.85, 9844.68, -5059.83, 1130.3100000000002, -0.0, -1130.3100000000002, -4784.85, 9844.68, -5059.83, -5059.83},
{-0.0, -1163.25, -4843.39, 9965.77, -5122.38, 1163.25, -0.0, -1163.25, -4843.39, 9965.77, -5122.38, -5122.38},
{-0.0, -1167.45, -4853.81, 9995.77, -5141.97, 1167.45, -0.0, -1153.6200000000001, -5257.1, 10660.730000000001, -5403.62, -5403.62},
{-0.0, -1149.04, -4835.76, 10003.5, -5167.75, 1149.04, -0.0, -1087.47, -6248.320000000001, 12200.75, -5952.429999999999, -5952.429999999999},
{-0.0, -1105.8, -4791.3, 10020.41, -5229.110000000001, 1105.8, -0.0, -957.8499999999999, -7347.8, 13663.88, -6316.08, -6316.08},
{-0.0, -1027.6200000000001, -4707.25, 10046.49, -5339.24, 1027.6200000000001, -0.0, -757.3199999999999, -8179.82, 14395.34, -6215.5199999999995, -6215.5199999999995},
{-0.0, -904.24, -4567.49, 10075.85, -5508.37, 904.24, -0.0, -509.5, -8397.24, 13928.6, -5531.36, -5531.36},
{-0.0, -725.31, -4351.28, 10092.9, -5741.62, 725.31, -0.0, -280.25, -8025.74, 12505.2, -4479.46, -4479.46},
{-0.0, -480.33, -4031.38, 10067.46, -6036.08, 480.33, -0.0, -117.19, -7343.28, 10719.83, -3376.54, -3376.54},
{-0.0, -158.89000000000001, -3571.97, 9948.26, -6376.29, 158.89000000000001, -0.0, -38.56, -6789.4, 9427.22, -2637.82, -2637.82},
{0.0, 32.95, -3328.2099999999996, 9914.960000000001, -6586.76, -32.95, 0.0, 357.18, -6431.240000000001, 9661.720000000001, -3230.48, -3230.48},
{0.0, 32.77, -3489.68, 10178.130000000001, -6688.45, -32.77, 0.0, 640.41, -6153.17, 9818.36, -3665.1800000000003, -3665.1800000000003},
{0.0, 32.46, -3740.53, 10577.54, -6837.01, -32.46, 0.0, 864.49, -5901.650000000001, 9887.69, -3986.04, -3986.04},
{0.0, 31.99, -4059.9, 11070.25, -7010.349999999999, -31.99, 0.0, 1041.41, -5682.76, 9908.61, -4225.849999999999, -4225.849999999999},
{0.0, 31.36, -4428.9400000000005, 11616.97, -7188.03, -31.36, 0.0, 1180.7, -5496.52, 9903.97, -4407.45, -4407.45},
{0.0, 30.55, -4831.32, 12183.51, -7352.19, -30.55, 0.0, 1289.87, -5340.160000000001, 9887.22, -4547.06, -4547.06},
{0.0, 29.56, -5252.97, 12740.66, -7487.6900000000005, -29.56, 0.0, 1374.81, -5209.780000000001, 9866.11, -4656.34, -4656.34},
{0.0, 28.36, -5681.349999999999, 13263.279999999999, -7581.93, -28.36, 0.0, 1440.1, -5101.21, 9845.04, -4743.820000000001, -4743.820000000001},
{0.0, 26.94, -6104.67, 13729.210000000001, -7624.54, -26.94, 0.0, 1489.22, -5010.45, 9826.369999999999, -4815.92, -4815.92},
{0.0, 25.31, -6511.0599999999995, 14118.269999999999, -7607.21, -25.31, 0.0, 1524.81, -4933.77, 9811.33, -4877.55, -4877.55},
{0.0, 23.479999999999997, -6887.870000000001, 14411.400000000001, -7523.530000000001, -23.479999999999997, 0.0, 1548.7500000000002, -4867.81, 9800.48, -4932.68, -4932.68},
{0.0, 21.450000000000003, -7221.14, 14589.91, -7368.77, -21.450000000000003, 0.0, 1562.3300000000002, -4809.47, 9794.08, -4984.61, -4984.61},
{0.0, 19.25, -7495.150000000001, 14634.67, -7139.51, -19.25, 0.0, 1566.2599999999998, -4755.900000000001, 9792.199999999999, -5036.31, -5036.31},
{0.0, 16.95, -7696.3099999999995, 14533.3, -6836.99, -16.95, 0.0, 1560.75, -4704.34, 9794.87, -5090.53, -5090.53},
{0.0, 14.62, -7824.370000000001, 14300.15, -6475.78, -14.62, 0.0, 1545.51, -4652.0, 9802.06, -5150.0599999999995, -5150.0599999999995},
{0.0, 12.32, -7882.91, 13954.49, -6071.58, -12.32, 0.0, 1519.73, -4595.9, 9813.68, -5217.78, -5217.78},
{0.0, 10.13, -7876.87, 13515.15, -5638.29, -10.13, 0.0, 1482.04, -4532.66, 9829.46, -5296.8, -5296.8},
{0.0, 8.09, -7812.85, 13001.3, -5188.45, -8.09, 0.0, 1430.42, -4458.25, 9848.76, -5390.509999999999, -5390.509999999999},
{0.0, 6.24, -7699.27, 12433.220000000001, -4733.95, -6.24, 0.0, 1362.13, -4367.61, 9870.25, -5502.639999999999, -5502.639999999999},
{0.0, 4.609999999999999, -7546.580000000001, 11833.37, -4286.79, -4.609999999999999, 0.0, 1273.5, -4254.16, 9891.32, -5637.15, -5637.15},
{0.0, 3.2, -7367.5199999999995, 11227.51, -3860.0, -3.2, 0.0, 1159.75, -4109.17, 9907.2, -5798.03, -5798.03},
{0.0, 2.04, -7177.46, 10646.08, -3468.62, -2.04, 0.0, 1014.77, -3920.69, 9909.47, -5988.780000000001, -5988.780000000001},
{0.0, 1.13, -6994.76, 10125.3, -3130.54, -1.13, 0.0, 830.71, -3672.21, 9883.619999999999, -6211.42, -6211.42},
{0.0, 0.48000000000000004, -6840.68, 9707.5, -2866.8199999999997, -0.48000000000000004, 0.0, 597.67, -3340.49, 9804.95, -6464.46, -6464.46},
{0.0, 0.1, -6738.33, 9439.300000000001, -2700.97, -0.1, 0.0, 303.23, -2892.31, 9631.41, -6739.1, -6739.1},
{-0.0, -357.18, -6423.330000000001, 9669.48, -3246.15, 357.18, -0.0, -32.95, -2395.67, 9422.64, -7026.97, -7026.97},
{-0.0, -640.41, -6146.129999999999, 9823.23, -3677.09, 640.41, -0.0, -32.77, -2573.71, 9713.48, -7139.78, -7139.78},
{-0.0, -864.49, -5895.59, 9890.77, -3995.18, 864.49, -0.0, -32.46, -2850.05, 10153.9, -7303.84, -7303.84},
{-0.0, -1041.41, -5677.63, 9910.550000000001, -4232.92, 1041.41, -0.0, -31.99, -3201.56, 10695.58, -7494.02, -7494.02},
{-0.0, -1180.7, -5492.240000000001, 9905.21, -4412.97, 1180.7, -0.0, -31.36, -3607.7, 11295.0, -7687.3, -7687.3},
{-0.0, -1289.87, -5336.59, 9888.0, -4551.41, 1289.87, -0.0, -30.55, -4051.18, 11915.03, -7863.85, -7863.85},
{-0.0, -1374.81, -5206.8, 9866.61, -4659.8099999999995, 1374.81, -0.0, -29.56, -4517.47, 12524.519999999999, -8007.06, -8007.06},
{-0.0, -1440.1, -5098.71, 9845.35, -4746.639999999999, 1440.1, -0.0, -28.36, -4993.9, 13097.05, -8103.15, -8103.15},
{-0.0, -1489.22, -5008.31, 9826.56, -4818.25, 1489.22, -0.0, -26.94, -5468.650000000001, 13609.500000000002, -8140.84, -8140.84},
{-0.0, -1524.81, -4931.91, 9811.44, -4879.54, 1524.81, -0.0, -25.31, -5929.759999999999, 14040.919999999998, -8111.17, -8111.17},
{-0.0, -1548.7500000000002, -4866.12, 9800.55, -4934.43, 1548.7500000000002, -0.0, -23.479999999999997, -6364.2699999999995, 14371.6, -8007.33, -8007.33},
{-0.0, -1562.3300000000002, -4807.88, 9794.11, -4986.23, 1562.3300000000002, -0.0, -21.450000000000003, -6757.62, 14582.23, -7824.6, -7824.6},
{-0.0, -1566.2599999999998, -4754.33, 9792.199999999999, -5037.87, 1566.2599999999998, -0.0, -19.25, -7093.120000000001, 14653.22, -7560.09, -7560.09},
{-0.0, -1560.75, -4702.71, 9794.84, -5092.13, 1560.75, -0.0, -16.95, -7355.7300000000005, 14571.92, -7216.1900000000005, -7216.1900000000005},
{-0.0, -1545.51, -4650.22, 9801.99, -5151.7699999999995, 1545.51, -0.0, -14.62, -7543.17, 14352.640000000001, -6809.469999999999, -6809.469999999999},
{-0.0, -1519.73, -4593.87, 9813.55, -5219.68, 1519.73, -0.0, -12.32, -7657.2300000000005, 14014.89, -6357.660000000001, -6357.660000000001},
{-0.0, -1482.04, -4530.27, 9829.25, -5298.98, 1482.04, -0.0, -10.13, -7701.49, 13577.980000000001, -5876.49, -5876.49},
{-0.0, -1430.42, -4455.35, 9848.43, -5393.08, 1430.42, -0.0, -8.09, -7681.67, 13061.7, -5380.04, -5380.04},
{-0.0, -1362.13, -4364.02, 9869.710000000001, -5505.7, 1362.13, -0.0, -6.24, -7605.75, 12487.16, -4881.41, -4881.41},
{-0.0, -1273.5, -4249.66, 9890.47, -5640.81, 1273.5, -0.0, -4.609999999999999, -7484.1, 11877.67, -4393.57, -4393.57},
{-0.0, -1159.75, -4103.46, 9905.859999999999, -5802.41, 1159.75, -0.0, -3.2, -7329.66, 11260.019999999999, -3930.3500000000004, -3930.3500000000004},
{-0.0, -1014.77, -3913.38, 9907.369999999999, -5993.99, 1014.77, -0.0, -2.04, -7158.24, 10665.75, -3507.5, -3507.5},
{-0.0, -830.71, -3662.77, 9880.31, -6217.54, 830.71, -0.0, -1.13, -6988.78, 10132.44, -3143.66, -3143.66},
{-0.0, -597.67, -3328.2000000000003, 9799.68, -6471.48, 597.67, -0.0, -0.48000000000000004, -6843.26, 9704.01, -2860.7500000000005, -2860.7500000000005},
{-0.0, -303.23, -2876.17, 9623.01, -6746.839999999999, 303.23, -0.0, -0.1, -6745.49, 9428.77, -2683.29, -2683.29},
{0.0, 32.95, -2374.79, 9409.79, -7035.0, -32.95, 0.0, 357.18, -6431.240000000001, 9661.720000000001, -3230.48, -3230.48},
{0.0, 32.77, -2552.99, 9701.210000000001, -7148.219999999999, -32.77, 0.0, 640.41, -6153.17, 9818.36, -3665.1800000000003, -3665.1800000000003},
{0.0, 32.46, -2829.55, 10142.45, -7312.9, -32.46, 0.0, 864.49, -5901.650000000001, 9887.69, -3986.04, -3986.04},
{0.0, 31.99, -3181.29, 10685.130000000001, -7503.84, -31.99, 0.0, 1041.41, -5682.76, 9908.61, -4225.849999999999, -4225.849999999999},
{0.0, 31.36, -3587.65, 11285.63, -7697.98, -31.36, 0.0, 1180.7, -5496.52, 9903.97, -4407.45, -4407.45},
{0.0, 30.55, -4031.3199999999997, 11906.779999999999, -7875.450000000001, -30.55, 0.0, 1289.87, -5340.160000000001, 9887.22, -4547.06, -4547.06},
{0.0, 29.56, -4497.79, 12517.41, -8019.62, -29.56, 0.0, 1374.81, -5209.780000000001, 9866.11, -4656.34, -4656.34},
{0.0, 28.36, -4974.39, 13091.08, -8116.69, -28.36, 0.0, 1440.1, -5101.21, 9845.04, -4743.820000000001, -4743.820000000001},
{0.0, 26.94, -5449.33, 13604.689999999999, -8155.360000000001, -26.94, 0.0, 1489.22, -5010.45, 9826.369999999999, -4815.92, -4815.92},
{0.0, 25.31, -5910.66, 14037.29, -8126.63, -25.31, 0.0, 1524.81, -4933.77, 9811.33, -4877.55, -4877.55},
{0.0, 23.479999999999997, -6345.48, 14369.15, -8023.67, -23.479999999999997, 0.0, 1548.7500000000002, -4867.81, 9800.48, -4932.68, -4932.68},
{0.0, 21.450000000000003, -6739.24, 14580.98, -7841.740000000001, -21.450000000000003, 0.0, 1562.3300000000002, -4809.47, 9794.08, -4984.61, -4984.61},
{0.0, 19.25, -7075.26, 14653.17, -7577.9, -19.25, 0.0, 1566.2599999999998, -4755.900000000001, 9792.199999999999, -5036.31, -5036.31},
{0.0, 16.95, -7338.53, 14573.08, -7234.54, -16.95, 0.0, 1560.75, -4704.34, 9794.87, -5090.53, -5090.53},
{0.0, 14.62, -7526.76, 14354.98, -6828.23, -14.62, 0.0, 1545.51, -4652.0, 9802.06, -5150.0599999999995, -5150.0599999999995},
{0.0, 12.32, -7641.679999999999, 14018.409999999998, -6376.72, -12.32, 0.0, 1519.73, -4595.9, 9813.68, -5217.78, -5217.78},
{0.0, 10.13, -7686.86, 13582.650000000001, -5895.780000000001, -10.13, 0.0, 1482.04, -4532.66, 9829.46, -5296.8, -5296.8},
{0.0, 8.09, -7667.99, 13067.51, -5399.5199999999995, -8.09, 0.0, 1430.42, -4458.25, 9848.76, -5390.509999999999, -5390.509999999999},
{0.0, 6.24, -7593.02, 12494.08, -4901.0599999999995, -6.24, 0.0, 1362.13, -4367.61, 9870.25, -5502.639999999999, -5502.639999999999},
{0.0, 4.609999999999999, -7472.299999999999, 11885.69, -4413.389999999999, -4.609999999999999, 0.0, 1273.5, -4254.16, 9891.32, -5637.15, -5637.15},
{0.0, 3.2, -7318.750000000001, 11269.11, -3950.36, -3.2, 0.0, 1159.75, -4109.17, 9907.2, -5798.03, -5798.03},
{0.0, 2.04, -7148.15, 10675.88, -3527.73, -2.04, 0.0, 1014.77, -3920.69, 9909.47, -5988.780000000001, -5988.780000000001},
{0.0, 1.13, -6979.410000000001, 10143.519999999999, -3164.11, -1.13, 0.0, 830.71, -3672.21, 9883.619999999999, -6211.42, -6211.42},
{0.0, 0.48000000000000004, -6834.46, 9715.869999999999, -2881.4, -0.48000000000000004, 0.0, 597.67, -3340.49, 9804.95, -6464.46, -6464.46},
{0.0, 0.1, -6737.06, 9441.15, -2704.08, -0.1, 0.0, 303.23, -2892.31, 9631.41, -6739.1, -6739.1},
{-0.0, -357.18, -6423.330000000001, 9669.48, -3246.15, 357.18, -0.0, -32.95, -2395.67, 9422.64, -7026.97, -7026.97},
{-0.0, -640.41, -6146.129999999999, 9823.23, -3677.09, 640.41, -0.0, -32.77, -2573.71, 9713.48, -7139.78, -7139.78},
{-0.0, -864.49, -5895.59, 9890.77, -3995.18, 864.49, -0.0, -32.46, -2850.05, 10153.9, -7303.84, -7303.84},
{-0.0, -1041.41, -5677.63, 9910.550000000001, -4232.92, 1041.41, -0.0, -31.99, -3201.56, 10695.58, -7494.02, -7494.02},
{-0.0, -1180.7, -5492.240000000001, 9905.21, -4412.97, 1180.7, -0.0, -31.36, -3607.7, 11295.0, -7687.3, -7687.3},
{-0.0, -1289.87, -5336.59, 9888.0, -4551.41, 1289.87, -0.0, -30.55, -4051.18, 11915.03, -7863.85, -7863.85},
{-0.0, -1374.81, -5206.8, 9866.61, -4659.8099999999995, 1374.81, -0.0, -29.56, -4517.47, 12524.519999999999, -8007.06, -8007.06},
{-0.0, -1440.1, -5098.71, 9845.35, -4746.639999999999, 1440.1, -0.0, -28.36, -4993.9, 13097.05, -8103.15, -8103.15},
{-0.0, -1489.22, -5008.31, 9826.56, -4818.25, 1489.22, -0.0, -26.94, -5468.650000000001, 13609.500000000002, -8140.84, -8140.84},
{-0.0, -1524.81, -4931.91, 9811.44, -4879.54, 1524.81, -0.0, -25.31, -5929.759999999999, 14040.919999999998, -8111.17, -8111.17},
{-0.0, -1548.7500000000002, -4866.12, 9800.55, -4934.43, 1548.7500000000002, -0.0, -23.479999999999997, -6364.2699999999995, 14371.6, -8007.33, -8007.33},
{-0.0, -1562.3300000000002, -4807.88, 9794.11, -4986.23, 1562.3300000000002, -0.0, -21.450000000000003, -6757.62, 14582.23, -7824.6, -7824.6},
{-0.0, -1566.2599999999998, -4754.33, 9792.199999999999, -5037.87, 1566.2599999999998, -0.0, -19.25, -7093.120000000001, 14653.22, -7560.09, -7560.09},
{-0.0, -1560.75, -4702.71, 9794.84, -5092.13, 1560.75, -0.0, -16.95, -7355.7300000000005, 14571.92, -7216.1900000000005, -7216.1900000000005},
{-0.0, -1545.51, -4650.22, 9801.99, -5151.7699999999995, 1545.51, -0.0, -14.62, -7543.17, 14352.640000000001, -6809.469999999999, -6809.469999999999},
{-0.0, -1519.73, -4593.87, 9813.55, -5219.68, 1519.73, -0.0, -12.32, -7657.2300000000005, 14014.89, -6357.660000000001, -6357.660000000001},
{-0.0, -1482.04, -4530.27, 9829.25, -5298.98, 1482.04, -0.0, -10.13, -7701.49, 13577.980000000001, -5876.49, -5876.49},
{-0.0, -1430.42, -4455.35, 9848.43, -5393.08, 1430.42, -0.0, -8.09, -7681.67, 13061.7, -5380.04, -5380.04},
{-0.0, -1362.13, -4364.02, 9869.710000000001, -5505.7, 1362.13, -0.0, -6.24, -7605.75, 12487.16, -4881.41, -4881.41},
{-0.0, -1273.5, -4249.66, 9890.47, -5640.81, 1273.5, -0.0, -4.609999999999999, -7484.1, 11877.67, -4393.57, -4393.57},
{-0.0, -1159.75, -4103.46, 9905.859999999999, -5802.41, 1159.75, -0.0, -3.2, -7329.66, 11260.019999999999, -3930.3500000000004, -3930.3500000000004},
{-0.0, -1014.77, -3913.38, 9907.369999999999, -5993.99, 1014.77, -0.0, -2.04, -7158.24, 10665.75, -3507.5, -3507.5},
{-0.0, -830.71, -3662.77, 9880.31, -6217.54, 830.71, -0.0, -1.13, -6988.78, 10132.44, -3143.66, -3143.66},
{-0.0, -597.67, -3328.2000000000003, 9799.68, -6471.48, 597.67, -0.0, -0.48000000000000004, -6843.26, 9704.01, -2860.7500000000005, -2860.7500000000005},
{-0.0, -303.23, -2876.17, 9623.01, -6746.839999999999, 303.23, -0.0, -0.1, -6745.49, 9428.77, -2683.29, -2683.29},
{0.0, 32.95, -2374.79, 9409.79, -7035.0, -32.95, 0.0, 357.18, -6431.240000000001, 9661.720000000001, -3230.48, -3230.48},
{0.0, 32.77, -2552.99, 9701.210000000001, -7148.219999999999, -32.77, 0.0, 640.41, -6153.17, 9818.36, -3665.1800000000003, -3665.1800000000003},
{0.0, 32.46, -2829.55, 10142.45, -7312.9, -32.46, 0.0, 864.49, -5901.650000000001, 9887.69, -3986.04, -3986.04},
{0.0, 31.99, -3181.29, 10685.130000000001, -7503.84, -31.99, 0.0, 1041.41, -5682.76, 9908.61, -4225.849999999999, -4225.849999999999},
{0.0, 31.36, -3587.65, 11285.63, -7697.98, -31.36, 0.0, 1180.7, -5496.52, 9903.97, -4407.45, -4407.45},
{0.0, 30.55, -4031.3199999999997, 11906.779999999999, -7875.450000000001, -30.55, 0.0, 1289.87, -5340.160000000001, 9887.22, -4547.06, -4547.06},
{0.0, 29.56, -4497.79, 12517.41, -8019.62, -29.56, 0.0, 1374.81, -5209.780000000001, 9866.11, -4656.34, -4656.34},
{0.0, 28.36, -4974.39, 13091.08, -8116.69, -28.36, 0.0, 1440.1, -5101.21, 9845.04, -4743.820000000001, -4743.820000000001},
{0.0, 26.94, -5449.33, 13604.689999999999, -8155.360000000001, -26.94, 0.0, 1489.22, -5010.45, 9826.369999999999, -4815.92, -4815.92},
{0.0, 25.31, -5910.66, 14037.29, -8126.63, -25.31, 0.0, 1524.81, -4933.77, 9811.33, -4877.55, -4877.55},
{0.0, 23.479999999999997, -6345.48, 14369.15, -8023.67, -23.479999999999997, 0.0, 1548.7500000000002, -4867.81, 9800.48, -4932.68, -4932.68},
{0.0, 21.450000000000003, -6739.24, 14580.98, -7841.740000000001, -21.450000000000003, 0.0, 1562.3300000000002, -4809.47, 9794.08, -4984.61, -4984.61},
{0.0, 19.25, -7075.26, 14653.17, -7577.9, -19.25, 0.0, 1566.2599999999998, -4755.900000000001, 9792.199999999999, -5036.31, -5036.31},
{0.0, 16.95, -7338.53, 14573.08, -7234.54, -16.95, 0.0, 1560.75, -4704.34, 9794.87, -5090.53, -5090.53},
{0.0, 14.62, -7526.76, 14354.98, -6828.23, -14.62, 0.0, 1545.51, -4652.0, 9802.06, -5150.0599999999995, -5150.0599999999995},
{0.0, 12.32, -7641.679999999999, 14018.409999999998, -6376.72, -12.32, 0.0, 1519.73, -4595.9, 9813.68, -5217.78, -5217.78},
{0.0, 10.13, -7686.86, 13582.650000000001, -5895.780000000001, -10.13, 0.0, 1482.04, -4532.66, 9829.46, -5296.8, -5296.8},
{0.0, 8.09, -7667.99, 13067.51, -5399.5199999999995, -8.09, 0.0, 1430.42, -4458.25, 9848.76, -5390.509999999999, -5390.509999999999},
{0.0, 6.24, -7593.02, 12494.08, -4901.0599999999995, -6.24, 0.0, 1362.13, -4367.61, 9870.25, -5502.639999999999, -5502.639999999999},
{0.0, 4.609999999999999, -7472.299999999999, 11885.69, -4413.389999999999, -4.609999999999999, 0.0, 1273.5, -4254.16, 9891.32, -5637.15, -5637.15},
{0.0, 3.2, -7318.750000000001, 11269.11, -3950.36, -3.2, 0.0, 1159.75, -4109.17, 9907.2, -5798.03, -5798.03},
{0.0, 2.04, -7148.15, 10675.88, -3527.73, -2.04, 0.0, 1014.77, -3920.69, 9909.47, -5988.780000000001, -5988.780000000001},
{0.0, 1.13, -6979.410000000001, 10143.519999999999, -3164.11, -1.13, 0.0, 830.71, -3672.21, 9883.619999999999, -6211.42, -6211.42},
{0.0, 0.48000000000000004, -6834.46, 9715.869999999999, -2881.4, -0.48000000000000004, 0.0, 597.67, -3340.49, 9804.95, -6464.46, -6464.46},
{0.0, 0.1, -6737.06, 9441.15, -2704.08, -0.1, 0.0, 303.23, -2892.31, 9631.41, -6739.1, -6739.1},
{-0.0, -357.18, -6423.330000000001, 9669.48, -3246.15, 357.18, -0.0, -32.95, -2395.67, 9422.64, -7026.97, -7026.97},
{-0.0, -640.41, -6146.129999999999, 9823.23, -3677.09, 640.41, -0.0, -32.77, -2573.71, 9713.48, -7139.78, -7139.78},
{-0.0, -864.49, -5895.59, 9890.77, -3995.18, 864.49, -0.0, -32.46, -2850.05, 10153.9, -7303.84, -7303.84},
{-0.0, -1041.41, -5677.63, 9910.550000000001, -4232.92, 1041.41, -0.0, -31.99, -3201.56, 10695.58, -7494.02, -7494.02},
{-0.0, -1180.7, -5492.240000000001, 9905.21, -4412.97, 1180.7, -0.0, -31.36, -3607.7, 11295.0, -7687.3, -7687.3},
{-0.0, -1289.87, -5336.59, 9888.0, -4551.41, 1289.87, -0.0, -30.55, -4051.18, 11915.03, -7863.85, -7863.85},
{-0.0, -1374.81, -5206.8, 9866.61, -4659.8099999999995, 1374.81, -0.0, -29.56, -4517.47, 12524.519999999999, -8007.06, -8007.06},
{-0.0, -1440.1, -5098.71, 9845.35, -4746.639999999999, 1440.1, -0.0, -28.36, -4993.9, 13097.05, -8103.15, -8103.15},
{-0.0, -1489.22, -5008.31, 9826.56, -4818.25, 1489.22, -0.0, -26.94, -5468.650000000001, 13609.500000000002, -8140.84, -8140.84},
{-0.0, -1524.81, -4931.91, 9811.44, -4879.54, 1524.81, -0.0, -25.31, -5929.759999999999, 14040.919999999998, -8111.17, -8111.17},
{-0.0, -1548.7500000000002, -4866.12, 9800.55, -4934.43, 1548.7500000000002, -0.0, -23.479999999999997, -6364.2699999999995, 14371.6, -8007.33, -8007.33},
{-0.0, -1562.3300000000002, -4807.88, 9794.11, -4986.23, 1562.3300000000002, -0.0, -21.450000000000003, -6757.62, 14582.23, -7824.6, -7824.6},
{-0.0, -1566.2599999999998, -4754.33, 9792.199999999999, -5037.87, 1566.2599999999998, -0.0, -19.25, -7093.120000000001, 14653.22, -7560.09, -7560.09},
{-0.0, -1560.75, -4702.71, 9794.84, -5092.13, 1560.75, -0.0, -16.95, -7355.7300000000005, 14571.92, -7216.1900000000005, -7216.1900000000005},
{-0.0, -1545.51, -4650.22, 9801.99, -5151.7699999999995, 1545.51, -0.0, -14.62, -7543.17, 14352.640000000001, -6809.469999999999, -6809.469999999999},
{-0.0, -1519.73, -4593.87, 9813.55, -5219.68, 1519.73, -0.0, -12.32, -7657.2300000000005, 14014.89, -6357.660000000001, -6357.660000000001},
{-0.0, -1482.04, -4530.27, 9829.25, -5298.98, 1482.04, -0.0, -10.13, -7701.49, 13577.980000000001, -5876.49, -5876.49},
{-0.0, -1430.42, -4455.35, 9848.43, -5393.08, 1430.42, -0.0, -8.09, -7681.67, 13061.7, -5380.04, -5380.04},
{-0.0, -1362.13, -4364.02, 9869.710000000001, -5505.7, 1362.13, -0.0, -6.24, -7605.75, 12487.16, -4881.41, -4881.41},
{-0.0, -1273.5, -4249.66, 9890.47, -5640.81, 1273.5, -0.0, -4.609999999999999, -7484.1, 11877.67, -4393.57, -4393.57},
{-0.0, -1159.75, -4103.46, 9905.859999999999, -5802.41, 1159.75, -0.0, -3.2, -7329.66, 11260.019999999999, -3930.3500000000004, -3930.3500000000004},
{-0.0, -1014.77, -3913.38, 9907.369999999999, -5993.99, 1014.77, -0.0, -2.04, -7158.24, 10665.75, -3507.5, -3507.5},
{-0.0, -830.71, -3662.77, 9880.31, -6217.54, 830.71, -0.0, -1.13, -6988.78, 10132.44, -3143.66, -3143.66},
{-0.0, -597.67, -3328.2000000000003, 9799.68, -6471.48, 597.67, -0.0, -0.48000000000000004, -6843.26, 9704.01, -2860.7500000000005, -2860.7500000000005},
{-0.0, -303.23, -2876.17, 9623.01, -6746.839999999999, 303.23, -0.0, -0.1, -6745.49, 9428.77, -2683.29, -2683.29}
};

float a[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

//float a = 106;
float b = 144.5;
float A = 90;
float B;
float kneeAngle;
float left_hip_2_angle;
float left_knee_angle;
float left_ankle_2_angle;
float right_hip_2_angle;
float right_knee_angle;
float right_ankle_2_angle;
bool increasing = false;
float increment = 0.15;
float squat_angle = 30;
float lowest_angle = 15;
float twerk_angle = 50;
int stepdataindex;

float getRadians(float x) {
  return x * M_PI / 180;
}

float getDegrees(float x) {
  return x * 180 / M_PI;
}

void translateangles(float a[], int a_size) {
  a[0] = (a[0] / 10000.0 * (-1) + 97 * M_PI / 180.0) * 180 / M_PI;
  a[1] = (a[1] / 10000.0 * (-1) + M_PI * 90 / 180.0) * 180 / M_PI;
  a[2] = (a[2] / 10000.0 + 93 * M_PI / 180.0) * 180 / M_PI;
  a[3] = (a[3] / 10000.0 + 32 * M_PI / 180.0) * 180 / M_PI;
  a[4] = (a[4] / 10000.0 + 95 * M_PI / 180.0) * 180 / M_PI;
  a[5] = (a[5] / 10000.0 * (-1) + 95 * M_PI / 180) * 180 / M_PI;
  a[6] = (a[6] / 10000.0 + 90 * M_PI / 180.0) * 180 / M_PI;
  a[7] = (a[7] / 10000.0 + M_PI * 90 / 180.0) * 180 / M_PI;
  a[8] = (a[8] / 10000.0 * (-1) + 105 * M_PI / 180.0) * 180 / M_PI;
  a[9] = (a[9] / 10000.0 * (-1) + 148 * M_PI / 180.0) * 180 / M_PI;
  a[10] = (a[11] / 10000.0 * (-1) + 90 * M_PI / 180.0) * 180 / M_PI;
  a[11] = (a[10] / 10000.0 + 75 * M_PI / 180.0) * 180 / M_PI;

  // Convert to degrees
  for (int i = 0; i < 12; i += 1) {
    //a[i] = a[i] * 180 / M_PI;
    Serial.print(a[i]);
    Serial.print(", ");
  }
  Serial.println();
}


void setup() {
  
  stepdataindex = 0;

  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit LSM6DSOX test!");

  if (!sox.begin_I2C()) {
    while (1) {
      delay(10);
    }
  }

  Serial.println("LSM6DSOX Found!");

  left_hip_1.attach(2);
  left_hip_2.attach(3);
  left_hip_3.attach(4);
  left_knee.attach(5);
  left_ankle_1.attach(6);
  left_ankle_2.attach(7);
  right_hip_1.attach(8);
  right_hip_2.attach(9);
  right_hip_3.attach(10);
  right_knee.attach(11);
  right_ankle_1.attach(12);
  right_ankle_2.attach(13);

  left_hip_1.write(97); //97 + is curve in - is curve out
  left_hip_2.write(93); //93 + is back - is forward
  left_hip_3.write(90); //90 + is in - is out
  left_knee.write(32); //32 + is back - is forward
  left_ankle_1.write(95); //95 + is in - is out
  left_ankle_2.write(95); //95 + is back - is forward

  right_hip_1.write(90); //90 + is curve out - is curve in
  right_hip_2.write(105); //105 + is forward - is back
  right_hip_3.write(90); //90 + is out - is in
  right_knee.write(148); //148 + is forward - is back
  right_ankle_1.write(90); //90 + is in - is out
  right_ankle_2.write(75); //75 + is forward - is back

  delay(5000);
}

void loop() {

  for (int i = 0; i < 12; i++) {
    a[i] = stepdata[stepdataindex][i];
    //Serial.print(stepdata[stepdataindex][i]);
    //Serial.print(a[i]);
    //Serial.print(", ");
    }
  //Serial.println(stepdataindex);

  stepdataindex = stepdataindex + 1;
  if (stepdataindex >= 174) {
    stepdataindex = 0;
  }
  //Serial.println();


  //float a[] = {0.000000, 0.001925, -0.749515, 1.463467, -0.713951, -0.001925, 0.000000, 0.001925, -0.749515, 1.463467, -0.713951, -0.001925};
  translateangles(a, sizeof(a));


  left_hip_1.write(a[0]);
  left_hip_2.write(a[2]);
  left_hip_3.write(a[1]);
  left_knee.write(a[3]);
  left_ankle_1.write(a[5]);
  left_ankle_2.write(a[4]);
  
  right_hip_1.write(a[6]);
  right_hip_2.write(a[8]);
  right_hip_3.write(a[7]);
  right_knee.write(a[9]);
  right_ankle_1.write(a[11]);
  right_ankle_2.write(a[10]);

  delay(40);
}