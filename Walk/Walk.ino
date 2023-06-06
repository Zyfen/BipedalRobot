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

// the sensor
Adafruit_LSM6DSOX sox;

// the servos
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

//this is the array of data for movement from the simulink simulation
int step_data[][12] = {
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 2, 0, -3, 0, 2, 0, -2, 0, 3, 0, -2},
{0, 23, 0, -46, 0, 24, 0, -23, 0, 46, 0, -24},
{0, 145, 0, -297, 0, 152, 0, -145, 0, 297, 0, -152},
{0, 321, 0, -658, 0, 338, 0, -321, 0, 658, 0, -338},
{0, 366, 1, -751, -1, 385, 0, -366, 1, 751, -1, -385},
{0, 541, 1, -1110, -1, 569, 0, -541, 1, 1110, -1, -569},
{0, 745, 2, -1528, -2, 784, 0, -745, 2, 1528, -2, -784},
{0, 963, 4, -1978, -4, 1014, 0, -963, 4, 1978, -4, -1014},
{0, 1200, 6, -2462, -6, 1263, 0, -1200, 6, 2462, -6, -1263},
{0, 1550, 10, -3182, -10, 1632, 0, -1550, 10, 3182, -10, -1632},
{0, 1797, 13, -3689, -13, 1892, 0, -1797, 13, 3689, -13, -1892},
{0, 2026, 17, -4158, -17, 2132, 0, -2026, 17, 4158, -17, -2132},
{0, 2356, 23, -4837, -23, 2480, 0, -2356, 23, 4837, -23, -2480},
{0, 2586, 27, -5308, -27, 2722, 0, -2586, 27, 5308, -27, -2722},
{0, 2891, 35, -5934, -35, 3043, 0, -2891, 35, 5934, -35, -3043},
{0, 3240, 44, -6650, -44, 3410, 0, -3240, 44, 6650, -44, -3410},
{0, 3513, 51, -7213, -51, 3699, 0, -3513, 51, 7213, -51, -3699},
{0, 3886, 64, -7976, -64, 4090, 0, -3886, 64, 7976, -64, -4090},
{0, 4315, 79, -8858, -79, 4542, 0, -4315, 79, 8858, -79, -4542},
{0, 4756, 95, -9762, -95, 5006, 0, -4756, 95, 9762, -95, -5006},
{0, 5292, 118, -10864, -118, 5571, 0, -5292, 118, 10864, -118, -5571},
{0, 5533, 129, -11357, -129, 5824, 0, -5533, 129, 11357, -129, -5824},
{0, 5797, 142, -11900, -142, 6103, 0, -5797, 142, 11900, -142, -6103},
{0, 6152, 160, -12628, -160, 6476, 0, -6152, 160, 12628, -160, -6476},
{0, 6522, 180, -13388, -180, 6866, 0, -6522, 180, 13388, -180, -6866},
{0, 6941, 204, -14248, -204, 7307, 0, -6941, 204, 14248, -204, -7307},
{0, 7321, 225, -15028, -225, 7707, 0, -7321, 225, 15028, -225, -7707},
{0, 7802, 257, -16014, -257, 8213, 0, -7802, 257, 16014, -257, -8213},
{0, 8250, 288, -16936, -288, 8686, 0, -8250, 288, 16936, -288, -8686},
{0, 8889, 334, -18248, -334, 9359, 0, -8889, 334, 18248, -334, -9359},
{0, 9204, 359, -18894, -359, 9690, 0, -9204, 359, 18894, -359, -9690},
{0, 9598, 390, -19702, -390, 10105, 0, -9598, 390, 19702, -390, -10105},
{0, 9980, 422, -20486, -422, 10507, 0, -9980, 422, 20486, -422, -10507},
{0, 10375, 456, -21298, -456, 10923, 0, -10375, 456, 21298, -456, -10923},
{0, 10769, 492, -22107, -492, 11338, 0, -10769, 492, 22107, -492, -11338},
{0, 11298, 542, -23192, -542, 11895, 0, -11298, 542, 23192, -542, -11895},
{0, 11849, 596, -24324, -596, 12475, 0, -11849, 596, 24324, -596, -12475},
{0, 12642, 680, -25956, -680, 13313, 0, -12642, 680, 25956, -680, -13313},
{0, 13218, 744, -27136, -744, 13918, 0, -13218, 744, 27136, -744, -13918},
{0, 13777, 809, -28283, -809, 14507, 0, -13777, 809, 28283, -809, -14507},
{0, 14590, 909, -29953, -909, 15364, 0, -14590, 909, 29953, -909, -15364},
{0, 15548, 1034, -31924, -1034, 16375, 0, -15548, 1034, 31924, -1034, -16375},
{0, 16438, 1157, -33750, -1157, 17312, 0, -16438, 1157, 33750, -1157, -17312},
{0, 17267, 1279, -35452, -1279, 18185, 0, -17267, 1279, 35452, -1279, -18185},
{0, 18074, 1404, -37110, -1404, 19037, 0, -18074, 1404, 37110, -1404, -19037},
{0, 19039, 1562, -39094, -1562, 20054, 0, -19039, 1562, 39094, -1562, -20054},
{0, 20010, 1729, -41088, -1729, 21078, 0, -20010, 1729, 41088, -1729, -21078},
{0, 21101, 1929, -43330, -1929, 22229, 0, -21101, 1929, 43330, -1929, -22229},
{0, 23535, 2417, -48333, -2417, 24799, 0, -23535, 2417, 48333, -2417, -24799},
{0, 28469, 3596, -58481, -3596, 30012, 0, -28469, 3596, 58481, -3596, -30012},
{0, 35869, 5899, -73719, -5899, 37850, 0, -35869, 5899, 73719, -5899, -37850},
{0, 40612, 7760, -83498, -7760, 42886, 0, -40612, 7760, 83498, -7760, -42886},
{0, 47064, 10873, -96826, -10873, 49761, 0, -47064, 10873, 96826, -10873, -49761},
{0, 48561, 11705, -99920, -11705, 51359, 0, -48561, 11705, 99920, -11705, -51359},
{0, 48570, 11710, -99938, -11710, 51369, 0, -48570, 11710, 99938, -11710, -51369},
{0, 48571, 11711, -99942, -11711, 51370, 0, -48574, 11711, 99948, -11711, -51373},
{0, 48573, 11711, -99942, -11711, 51369, 0, -48598, 11710, 99991, -11710, -51393},
{0, 48578, 11710, -99942, -11710, 51364, 0, -48661, 11707, 100105, -11707, -51444},
{0, 48582, 11710, -99942, -11710, 51359, 0, -48709, 11706, 100191, -11706, -51482},
{0, 48592, 11709, -99943, -11709, 51351, 0, -48829, 11701, 100408, -11701, -51579},
{0, 48599, 11708, -99943, -11708, 51344, 0, -48917, 11697, 100567, -11697, -51649},
{0, 48614, 11707, -99944, -11707, 51330, 0, -49105, 11690, 100906, -11690, -51800},
{0, 48633, 11705, -99945, -11705, 51312, 0, -49344, 11680, 101336, -11680, -51991},
{0, 48658, 11702, -99946, -11702, 51288, 0, -49658, 11666, 101898, -11666, -52240},
{0, 48684, 11699, -99947, -11699, 51263, 0, -49977, 11652, 102469, -11652, -52492},
{0, 48701, 11697, -99948, -11697, 51247, 0, -50180, 11643, 102831, -11643, -52652},
{0, 48736, 11693, -99949, -11693, 51214, 0, -50604, 11623, 103587, -11623, -52984},
{0, 48777, 11687, -99952, -11687, 51174, 0, -51100, 11599, 104468, -11599, -53369},
{0, 48827, 11680, -99954, -11680, 51127, 0, -51685, 11569, 105504, -11569, -53818},
{0, 48873, 11673, -99957, -11673, 51083, 0, -52219, 11541, 106447, -11541, -54227},
{0, 48934, 11664, -99960, -11664, 51026, 0, -52925, 11502, 107685, -11502, -54760},
{0, 49003, 11652, -99964, -11652, 50961, 0, -53700, 11458, 109039, -11458, -55339},
{0, 49094, 11636, -99970, -11636, 50876, 0, -54706, 11397, 110783, -11397, -56077},
{0, 49189, 11617, -99976, -11617, 50786, 0, -55742, 11329, 112568, -11329, -56826},
{0, 49271, 11600, -99981, -11600, 50710, 0, -56606, 11270, 114046, -11270, -57438},
{0, 49367, 11579, -99988, -11579, 50620, 0, -57618, 11196, 115760, -11196, -58142},
{0, 49483, 11552, -99996, -11552, 50513, 0, -58802, 11104, 117750, -11104, -58947},
{0, 49570, 11530, -100001, -11530, 50432, 0, -59667, 11032, 119188, -11032, -59522},
{0, 49681, 11501, -100011, -11501, 50329, 0, -60761, 10936, 120991, -10936, -60231},
{0, 49816, 11462, -100021, -11462, 50205, 0, -62053, 10814, 123096, -10814, -61043},
{0, 49919, 11431, -100030, -11431, 50110, 0, -63019, 10717, 124649, -10717, -61631},
{0, 50042, 11392, -100041, -11392, 49999, 0, -64139, 10596, 126428, -10596, -62289},
{0, 50177, 11346, -100052, -11346, 49876, 0, -65345, 10456, 128315, -10456, -62970},
{0, 50271, 11311, -100062, -11311, 49789, 0, -66175, 10353, 129595, -10353, -63420},
{0, 50368, 11275, -100071, -11275, 49704, 0, -66989, 10246, 130834, -10246, -63844},
{0, 50502, 11221, -100085, -11221, 49583, 0, -68111, 10090, 132514, -10090, -64403},
{0, 50603, 11179, -100096, -11179, 49493, 0, -68929, 9967, 133717, -9967, -64788},
{0, 50731, 11121, -100111, -11121, 49379, 0, -69950, 9803, 135188, -9803, -65237},
{0, 50878, 11051, -100128, -11051, 49251, 0, -71073, 9607, 136766, -9607, -65693},
{0, 50997, 10990, -100144, -10990, 49147, 0, -71954, 9440, 137970, -9440, -66015},
{0, 51101, 10934, -100157, -10934, 49057, 0, -72703, 9288, 138964, -9288, -66261},
{0, 51234, 10858, -100177, -10858, 48943, 0, -73621, 9085, 140144, -9085, -66523},
{0, 51320, 10806, -100190, -10806, 48870, 0, -74200, 8947, 140862, -8947, -66662},
{0, 51423, 10740, -100207, -10740, 48784, 0, -74863, 8777, 141656, -8777, -66793},
{0, 51539, 10663, -100227, -10663, 48688, 0, -75575, 8577, 142468, -8577, -66893},
{0, 51598, 10621, -100237, -10621, 48639, 0, -75927, 8472, 142851, -8472, -66924},
{0, 51679, 10561, -100252, -10561, 48573, 0, -76392, 8322, 143336, -8322, -66944},
{0, 51776, 10487, -100272, -10487, 48496, 0, -76918, 8137, 143846, -8137, -66928},
{0, 51887, 10396, -100295, -10396, 48408, 0, -77483, 7917, 144339, -7917, -66856},
{0, 51979, 10316, -100316, -10316, 48337, 0, -77913, 7728, 144663, -7728, -66750},
{0, 52071, 10231, -100338, -10231, 48267, 0, -78310, 7529, 144905, -7529, -66595},
{0, 52174, 10130, -100365, -10130, 48191, 0, -78706, 7296, 145066, -7296, -66360},
{0, 52269, 10030, -100391, -10030, 48123, 0, -79022, 7072, 145101, -7072, -66080},
{0, 52370, 9915, -100421, -9915, 48052, 0, -79298, 6821, 145005, -6821, -65707},
{0, 52451, 9817, -100449, -9817, 47998, 0, -79470, 6612, 144819, -6612, -65349},
{0, 52518, 9729, -100472, -9729, 47955, 0, -79580, 6430, 144585, -6430, -65005},
{0, 52564, 9667, -100489, -9667, 47926, 0, -79635, 6303, 144384, -6303, -64749},
{0, 52653, 9537, -100525, -9537, 47873, 0, -79698, 6044, 143884, -6044, -64187},
{0, 52737, 9402, -100563, -9402, 47826, 0, -79696, 5788, 143273, -5788, -63576},
{0, 52810, 9274, -100599, -9274, 47789, 0, -79640, 5551, 142611, -5551, -62971},
{0, 52886, 9125, -100640, -9125, 47754, 0, -79517, 5289, 141767, -5289, -62250},
{0, 52942, 9006, -100674, -9006, 47732, 0, -79381, 5087, 141042, -5087, -61661},
{0, 53013, 8833, -100723, -8833, 47709, 0, -79131, 4806, 139920, -4806, -60789},
{0, 53077, 8656, -100774, -8656, 47696, 0, -78823, 4534, 138713, -4534, -59890},
{0, 53132, 8479, -100825, -8479, 47693, 0, -78468, 4275, 137453, -4275, -58984},
{0, 53189, 8248, -100891, -8248, 47702, 0, -77955, 3961, 135766, -3961, -57811},
{0, 53230, 8033, -100953, -8033, 47722, 0, -77432, 3687, 134156, -3687, -56724},
{0, 53259, 7817, -101015, -7817, 47755, 0, -76870, 3429, 132507, -3429, -55637},
{0, 53281, 7568, -101086, -7568, 47804, 0, -76199, 3154, 130614, -3154, -54416},
{0, 53291, 7299, -101163, -7299, 47872, 0, -75443, 2878, 128560, -2878, -53117},
{0, 53285, 7006, -101245, -7006, 47960, 0, -74602, 2604, 126345, -2604, -51742},
{0, 53264, 6737, -101320, -6737, 48055, 0, -73821, 2373, 124340, -2373, -50519},
{0, 53235, 6481, -101391, -6481, 48156, 0, -73075, 2169, 122464, -2169, -49389},
{0, 53186, 6178, -101472, -6178, 48287, 0, -72204, 1950, 120310, -1950, -48106},
{0, 53113, 5835, -101563, -5835, 48450, 0, -71234, 1726, 117954, -1726, -46720},
{0, 53013, 5461, -101658, -5461, 48644, 0, -70207, 1507, 115501, -1507, -45294},
{0, 52891, 5071, -101753, -5071, 48862, 0, -69182, 1306, 113084, -1306, -43904},
{0, 52763, 4719, -101835, -4719, 49072, 0, -68304, 1146, 111042, -1146, -42738},
{0, 52668, 4483, -101888, -4483, 49220, 0, -67742, 1049, 109744, -1049, -42002},
{0, 52564, 4241, -101940, -4241, 49375, 0, -67193, 957, 108484, -957, -41291},
{0, 52459, 4009, -101987, -4009, 49529, 0, -66690, 877, 107334, -877, -40645},
{0, 52289, 3660, -102055, -3660, 49768, 0, -65985, 768, 105735, -768, -39750},
{0, 52093, 3290, -102123, -3290, 50030, 0, -65303, 668, 104197, -668, -38893},
{0, 51893, 2935, -102182, -2935, 50288, 0, -64723, 587, 102892, -587, -38170},
{0, 51697, 2609, -102231, -2609, 50534, 0, -64249, 523, 101836, -523, -37587},
{0, 51544, 2365, -102264, -2365, 50721, 0, -63936, 481, 101137, -481, -37201},
{0, 51326, 2033, -102305, -2033, 50979, 0, -63564, 434, 100313, -434, -36748},
{0, 51162, 1794, -102331, -1794, 51169, 0, -63336, 406, 99808, -406, -36472},
{0, 50919, 1452, -102363, -1452, 51444, 0, -63071, 374, 99224, -374, -36152},
{0, 50693, 1148, -102386, -1148, 51691, 0, -62895, 352, 98835, -352, -35940},
{0, 50502, 897, -102401, -897, 51898, 0, -62790, 340, 98603, -340, -35813},
{0, 50261, 591, -102414, -591, 52154, 0, -62715, 331, 98438, -331, -35723},
{0, 50141, 444, -102419, -444, 52278, 0, -62698, 329, 98401, -329, -35703},
{0, 50063, 348, -102421, -348, 52359, 0, -62672, 296, 98403, -296, -35731},
{0, 49792, 25, -102424, -25, 52633, 0, -62500, 21, 98519, -21, -36019},
{0, 49630, -163, -102424, 163, 52794, 0, -62383, -169, 98604, 169, -36220},
{0, 49518, -296, -102425, 296, 52907, 0, -62253, -378, 98696, 378, -36443},
{0, 49499, -327, -102438, 327, 52939, 0, -62028, -734, 98847, 734, -36819},
{0, 49530, -328, -102496, 328, 52964, 0, -61704, -1234, 99045, 1234, -37341},
{0, 49596, -328, -102614, 328, 53018, 0, -61336, -1784, 99245, 1784, -37908},
{0, 49704, -328, -102809, 328, 53103, 0, -60929, -2375, 99438, 2375, -38508},
{0, 49816, -327, -103010, 327, 53194, 0, -60611, -2827, 99571, 2827, -38960},
{0, 49979, -327, -103301, 327, 53323, 0, -60234, -3347, 99708, 3347, -39473},
{0, 50159, -327, -103624, 327, 53464, 0, -59884, -3820, 99818, 3820, -39934},
{0, 51554, -325, -106110, 325, 54555, 0, -58080, -6102, 100153, 6102, -42073},
{0, 52847, -323, -108388, 323, 55540, 0, -56954, -7417, 100200, 7417, -43246},
{0, 53978, -321, -110361, 321, 56384, 0, -56159, -8302, 100171, 8302, -44011},
{0, 57172, -315, -115847, 315, 58675, 0, -54412, -10127, 99954, 10127, -45542},
{0, 64986, -293, -128591, 293, 63603, 0, -51554, -12756, 99267, 12756, -47712},
{0, 72015, -262, -138849, 262, 66834, 0, -49723, -14104, 98739, 14104, -49016},
{0, 74263, -247, -141715, 247, 67453, 0, -49188, -14404, 98605, 14404, -49417},
{0, 76140, -232, -143826, 232, 67686, 0, -48727, -14607, 98511, 14607, -49785},
{0, 78196, -207, -145536, 207, 67340, 0, -48140, -14762, 98438, 14762, -50297},
{0, 79628, -159, -144059, 159, 64431, 0, -47175, -14665, 98484, 14665, -51309},
{0, 74701, -67, -127323, 67, 52622, 0, -44991, -12819, 99245, 12819, -54254},
{0, 71200, -41, -118544, 41, 47344, 0, -43730, -11322, 99697, 11322, -55967},
{0, 66136, -13, -106698, 13, 40562, 0, -40968, -8024, 100186, 8024, -59218},
{0, 62488, 0, -98534, 0, 36046, 0, -33451, -152, 98598, 152, -65147},
{0, 62481, 11, -98536, -11, 36056, 0, -33347, -53, 98554, 53, -65207},
{0, 62404, 133, -98590, -133, 36186, 0, -33158, 125, 98471, -125, -65314},
{0, 62291, 314, -98670, -314, 36379, 0, -32987, 286, 98396, -286, -65408},
{0, 62117, 591, -98789, -591, 36672, 0, -32949, 327, 98385, -327, -65436},
{0, 61781, 1113, -99000, -1113, 37219, 0, -32978, 328, 98437, -328, -65459},
{0, 61201, 1978, -99312, -1978, 38111, 0, -33100, 328, 98650, -328, -65550},
{0, 60335, 3206, -99673, -3206, 39338, 0, -33447, 327, 99253, -327, -65806},
{0, 58060, 6124, -100156, -6124, 42096, 0, -35335, 325, 102486, -325, -67152},
{0, 54459, 10079, -99963, -10079, 45504, 0, -41718, 315, 112982, -315, -71263},
{0, 48452, 14696, -98469, -14696, 50017, 0, -67490, 221, 145264, -221, -77774},
{0, 48325, 14727, -98454, -14727, 50129, 0, -68206, 216, 145693, -216, -77487},
{0, 48110, 14766, -98436, -14766, 50326, 0, -69366, 206, 146234, -206, -76868},
{0, 47656, 14775, -98431, -14775, 50775, 0, -71481, 184, 146437, -184, -74955},
{0, 46929, 14557, -98534, -14557, 51605, 0, -73511, 146, 143808, -146, -70297},
{0, 45758, 13647, -98932, -13647, 53174, 0, -73438, 92, 134731, -92, -61293},
{0, 43986, 11633, -99616, -11633, 55630, 0, -70225, 45, 120862, -45, -50637},
{0, 42641, 10006, -99975, -10006, 57333, 0, -67866, 27, 113335, -27, -45470},
{0, 41572, 8732, -100139, -8732, 58567, 0, -66369, 18, 108965, -18, -42596},
{0, 40013, 6936, -100196, -6936, 60182, 0, -64752, 9, 104481, -9, -39729},
{0, 38656, 5434, -100088, -5434, 61432, 0, -63791, 5, 101909, -5, -38119},
{0, 37406, 4099, -99876, -4099, 62470, 0, -63188, 3, 100329, -3, -37141},
{0, 36055, 2708, -99536, -2708, 63481, 0, -62778, 1, 99268, -1, -36490},
{0, 34751, 1409, -99110, -1409, 64359, 0, -62563, 0, 98718, 0, -36154},
{0, 33869, 555, -98772, -555, 64903, 0, -62500, 0, 98558, 0, -36057},
{0, 33386, 95, -98570, -95, 65184, 0, -62488, -2, 98532, 2, -36043},
{0, 33156, -123, -98471, 123, 65315, 0, -62408, -131, 98587, 131, -36179},
{0, 33011, -259, -98407, 259, 65396, 0, -62322, -269, 98648, 269, -36326},
{0, 32947, -321, -98381, 321, 65434, 0, -62202, -461, 98732, 461, -36530},
{0, 32959, -328, -98411, 328, 65452, 0, -61898, -936, 98929, 936, -37031},
{0, 33000, -328, -98483, 328, 65481, 0, -61610, -1376, 99098, 1376, -37489},
{0, 33080, -328, -98621, 328, 65541, 0, -61259, -1898, 99283, 1898, -38024},
{0, 33181, -328, -98796, 328, 65616, 0, -60938, -2363, 99434, 2363, -38495},
{0, 33992, -327, -100199, 327, 66207, 0, -59469, -4366, 99927, 4366, -40458},
{0, 35960, -324, -103549, 324, 67590, 0, -57551, -6730, 100189, 6730, -42638},
{0, 39801, -318, -109905, 318, 70105, 0, -55294, -9225, 100086, 9225, -44793},
{0, 47229, -304, -121469, 304, 74241, 0, -52595, -11856, 99553, 11856, -46958},
{0, 55615, -279, -133104, 279, 77489, 0, -50583, -13519, 98983, 13519, -48400},
{0, 61275, -257, -139773, 257, 78499, 0, -49521, -14224, 98686, 14224, -49165},
{0, 71035, -189, -146522, 189, 75487, 0, -47763, -14782, 98428, 14782, -50665},
{0, 71594, -183, -146403, 183, 74809, 0, -47627, -14771, 98433, 14771, -50805},
{0, 72036, -176, -146191, 176, 74155, 0, -47508, -14754, 98441, 14754, -50932},
{0, 72556, -168, -145750, 168, 73194, 0, -47349, -14720, 98458, 14720, -51109},
{0, 73306, -152, -144449, 152, 71143, 0, -47045, -14612, 98509, 14612, -51464},
{0, 73874, -125, -141136, 125, 67263, 0, -46532, -14316, 98645, 14316, -52113},
{0, 73770, -105, -137570, 105, 63800, 0, -46090, -13960, 98802, 13960, -52712},
{0, 71757, -61, -126513, 61, 54756, 0, -44773, -12567, 99331, 12567, -54557},
{0, 67115, -22, -111124, 22, 44008, 0, -42140, -9403, 100066, 9403, -57926},
{0, 66434, -18, -109161, 18, 42727, 0, -41629, -8797, 100133, 8797, -58504},
{0, 65378, -12, -106200, 12, 40823, 0, -40693, -7707, 100196, 7707, -59502},
{0, 65084, -11, -105394, 11, 40310, 0, -40392, -7362, 100201, 7362, -59809},
{0, 64739, -9, -104454, 9, 39715, 0, -40004, -6922, 100196, 6922, -60193},
{0, 64159, -6, -102897, 6, 38738, 0, -39247, -6077, 100153, 6077, -60906},
{0, 63888, -6, -102174, 6, 38286, 0, -38828, -5617, 100110, 5617, -61283},
{0, 63500, -4, -101152, 4, 37652, 0, -38124, -4857, 100011, 4857, -61887},
{0, 63213, -3, -100401, 3, 37188, 0, -37478, -4172, 99892, 4172, -62414},
{0, 63069, -2, -100027, 2, 36958, 0, -37092, -3768, 99808, 3768, -62715},
{0, 62939, -2, -99695, 2, 36754, 0, -36694, -3356, 99711, 3356, -63017},
{0, 62814, -1, -99370, 1, 36555, 0, -36229, -2881, 99587, 2881, -63357},
{0, 62685, -1, -99037, 1, 36352, 0, -35622, -2267, 99405, 2267, -63783},
{0, 62603, 0, -98827, 0, 36224, 0, -35103, -1751, 99235, 1751, -64132},
{0, 62538, 0, -98661, 0, 36123, 0, -34514, -1174, 99024, 1174, -64509},
{0, 62495, 0, -98551, 0, 36056, 0, -33772, -458, 98733, 458, -64959},
{0, 62487, 1, -98533, -1, 36046, 0, -33406, -109, 98579, 109, -65173},
{0, 62463, 38, -98548, -38, 36084, 0, -33287, 4, 98528, -4, -65241},
{0, 62347, 224, -98631, -224, 36284, 0, -33055, 222, 98426, -222, -65371},
{0, 62266, 355, -98688, -355, 36422, 0, -32971, 302, 98390, -302, -65419},
{0, 62058, 683, -98827, -683, 36769, 0, -32951, 328, 98390, -328, -65439},
{0, 61686, 1257, -99055, -1257, 37369, 0, -32992, 328, 98462, -328, -65469},
{0, 60819, 2529, -99486, -2529, 38666, 0, -33228, 328, 98874, -328, -65645},
{0, 60343, 3195, -99671, -3195, 39328, 0, -33443, 327, 99246, -327, -65803},
{0, 59626, 4159, -99889, -4159, 40264, 0, -33880, 327, 100001, -327, -66120},
{0, 57677, 6579, -100184, -6579, 42506, 0, -35796, 324, 103267, -324, -67471},
{0, 53013, 11472, -99659, -11472, 46644, 0, -45824, 307, 119358, -307, -73534},
{0, 47457, 14745, -98445, -14745, 50988, 0, -72216, 174, 146069, -174, -73853},
{0, 45657, 13544, -98973, -13544, 53316, 0, -73302, 88, 133853, -88, -60551},
{0, 41107, 8187, -100177, -8187, 59070, 0, -65820, 14, 107420, -14, -41600},
{0, 39901, 6809, -100191, -6809, 60290, 0, -64659, 9, 104230, -9, -39571},
{0, 39467, 6322, -100169, -6322, 60702, 0, -64320, 6, 103318, -6, -38998},
{0, 38630, 5405, -100085, -5405, 61456, 0, -63775, 5, 101867, -5, -38092},
{0, 37297, 3984, -99853, -3984, 62556, 0, -63147, 3, 100221, -3, -37075},
{0, 36613, 3275, -99690, -3275, 63078, 0, -62920, 2, 99633, -2, -36713},
{0, 34886, 1542, -99158, -1542, 64271, 0, -62580, 0, 98760, 0, -36180},
{0, 33942, 625, -98802, -625, 64859, 0, -62504, 0, 98566, 0, -36062},
{0, 33447, 152, -98596, -152, 65150, 0, -62491, 0, 98533, 0, -36042},
{0, 33364, 74, -98561, -74, 65197, 0, -62487, -5, 98533, 5, -36046},
{0, 33170, -110, -98477, 110, 65307, 0, -62416, -119, 98582, 119, -36166},
{0, 33043, -229, -98421, 229, 65378, 0, -62344, -232, 98632, 232, -36288},
{0, 32970, -298, -98389, 298, 65419, 0, -62276, -343, 98681, 343, -36406},
{0, 32945, -327, -98385, 327, 65440, 0, -62093, -632, 98804, 632, -36711},
{0, 32969, -328, -98428, 328, 65459, 0, -61815, -1064, 98979, 1064, -37165},
{0, 33017, -328, -98512, 328, 65495, 0, -61523, -1507, 99146, 1507, -37624},
{0, 33120, -328, -98691, 328, 65571, 0, -61121, -2100, 99350, 2100, -38230},

};

// variable for the rolling index of the steps
int step_data_index;

//turns a slice of the movement array into angles for the motors to read, a is for angles
void translateangles(float a[], int a_size) {
  a[0] = (a[0] / 100000.0 * (-1) + 97 * M_PI / 180.0) * 180 / M_PI;
  a[1] = (a[1] / 100000.0 * (-1) + M_PI * 93 / 180.0) * 180 / M_PI;
  a[2] = (a[2] / 100000.0 + 90 * M_PI / 180.0) * 180 / M_PI;
  a[3] = (a[3] / 100000.0 + 32 * M_PI / 180.0) * 180 / M_PI;
  a[4] = (a[4] / 100000.0 + 95 * M_PI / 180.0) * 180 / M_PI;
  a[5] = (a[5] / 100000.0 * (-1) + 95 * M_PI / 180) * 180 / M_PI;
  a[6] = (a[6] / 100000.0 + 90 * M_PI / 180.0) * 180 / M_PI;
  a[7] = (a[7] / 100000.0 + M_PI * 105 / 180.0) * 180 / M_PI;
  a[8] = (a[8] / 100000.0 * (-1) + 90 * M_PI / 180.0) * 180 / M_PI;
  a[9] = (a[9] / 100000.0 * (-1) + 148 * M_PI / 180.0) * 180 / M_PI;
  a[10] = (a[10] / 100000.0 * (-1) + 90 * M_PI / 180.0) * 180 / M_PI;
  a[11] = (a[11] / 100000.0 + 75 * M_PI / 180.0) * 180 / M_PI;

  // print the angles
  for (int i = 0; i < 12; i += 1) {
    Serial.print(a[i]);
    Serial.print(", ");
  }
  Serial.println();
}

//runs once as it first starts
void setup() {
  
  // initialize step_data_index as 0
  step_data_index = 0;

  // begin a serial connection to print values to the computer
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  // once it gets past this, the arduino is connected
  Serial.println("Connected!");

  // connect to the I2C protocol which the sensor uses
  if (!sox.begin_I2C()) {
    while (1) {
      delay(10);
    }
  }

  // once it passes this, say the sensor is found
  Serial.println("LSM6DSOX Found!");

  // attach all of the servos to the correct pin
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

  // start the robot standing
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

  // wait five seconds
  delay(5000);
}

float a[12];
// this part of the code runs continuously
void loop() {
  
  // make a list, a, for the angles inside a slice of the movement data array
  for (int i = 0; i < 12; i++) {
    a[i] = step_data[step_data_index][i];
    //Serial.print(step_data[step_data_index][i]);
    //Serial.print(a[i]);
    //Serial.print(", ");
    }
  Serial.println(step_data_index);

  // increment the movement data index, or reset it
  step_data_index = step_data_index + 1;
  if (step_data_index >= 174) {
    step_data_index = 0;
  }
  //Serial.println();

  // turn the int angles into angles for the servos to read
  translateangles(a, sizeof(a));

  // write the angles to each servo
  // left_hip_1.write(a[0]);
  // left_hip_2.write(a[1]);
  // left_hip_3.write(a[2]);
  // left_knee.write(a[3]);
  // left_ankle_1.write(a[4]);
  // left_ankle_2.write(a[5]);
  
  right_hip_1.write(a[6]);
  right_hip_2.write(a[7]);
  right_hip_3.write(a[8]);
  right_knee.write(a[9]);
  right_ankle_1.write(a[10]);
  right_ankle_2.write(a[11]);


  delay(16);
}