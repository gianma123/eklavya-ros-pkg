#ifndef _EKLAVYA2_H_
#define _EKLAVYA2_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>   /* Needed for sleep() and usleep() */
#include <pthread.h>  /* Needed for all pthread library calls */
#include <vector>
#include <ros/ros.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "Modules/devices.h"
#include "Utils/SerialPortLinux/serial_lnx.h"

#define AUTO_CALIB 0

#define MAP_MAX 1000
#define LOOP_RATE 10
#define WAIT_TIME 100

enum Strategies {
    FollowNose = 0,
    TrackWayPoint =1,
    HectorSLAM =2,
    LaserTestOnly =3,
    PlannerTestOnly =4 ,
    FusionTestOnly =5,
    IGVCBasic =6
};

typedef struct Triplet {
    int x, y, z;
} Triplet;

typedef struct TripletFP {
    double x, y, z;
} TripletFP;

typedef struct Pose {
    Triplet position;
    TripletFP orientation; // Roll - Pitch - Yaw
} Pose;

typedef struct LatLong {
    double latitude;
    double longitude;
} LatLong;

typedef struct Odom {
    double left_velocity;
    double right_velocity;
} Odom;

/* Global data structures to be shared by all threads */
extern Pose pose; // Shared by IMU, EKF
extern LatLong lat_long; // Shared by GPS, EKF
extern Odom odom; // Shared by Encoder, EKF
extern unsigned char lidar_map[MAP_MAX][MAP_MAX];
extern unsigned char camera_map[MAP_MAX][MAP_MAX]; // Used by Camera
extern unsigned char global_map[MAP_MAX][MAP_MAX];
extern Triplet bot_location; // Shared by EKF, Planner
extern Triplet target_location; // Shared by EKF, Planner
extern std::vector<Triplet> path;

extern int strategy;

/* mutex for mutually exclusive updating of the shared data structures */
extern pthread_mutex_t pose_mutex;
extern pthread_mutex_t lat_long_mutex;
extern pthread_mutex_t odom_mutex;
extern pthread_mutex_t lidar_map_mutex;
extern pthread_mutex_t global_map_mutex;
extern pthread_mutex_t bot_location_mutex;
extern pthread_mutex_t target_location_mutex;
extern pthread_mutex_t path_mutex;
extern pthread_mutex_t camera_map_mutex;

void *imu_thread(void *arg);
void *lidar_thread(void *arg);
void *lane_thread(void *arg);
void *gps_thread(void *arg);
void *encoder_thread(void *arg);
void *ekf_thread(void *arg);
void *slam_thread(void *arg);
void *navigation_thread(void *arg);
void *planner_thread(void *arg);
void *diagnostics_thread(void *arg);
void *fusion_thread(void *arg);

using namespace std;

#endif
