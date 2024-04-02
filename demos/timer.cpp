#include"timer.h"
#include <stdlib.h>
#include <sys/time.h>

typedef unsigned long long	LONGLONG;

static bool qpc_flag;

unsigned system_time() {
    struct timeval tv;
    gettimeofday(&tv, 0);
   	return tv.tv_sec * 1000 + tv.tv_usec/1000;
}

unsigned timer::get_time() {
    return system_time();
}

unsigned long timer::get_clock() {
    struct timeval tv;
    gettimeofday(&tv, 0);
   	return tv.tv_sec * 1000 + tv.tv_usec/1000;
}

void init_time() {
    qpc_flag = false;
}

static timer *time = NULL;

timer& timer::get() {
    return (timer&)*time;
}

void timer::update() {
    if(!time) return;

    if(!time->is_paused) {
        time->frame_number++;
    }

    unsigned this_time = system_time();
    time->last_frame_duration = this_time - time->last_frame_timestamp;
    time->last_frame_timestamp = this_time;

    unsigned long this_clock = get_clock();
    time->last_frame_clock_ticks = this_clock - time->last_frame_clockstamp;
    time->last_frame_clockstamp = this_clock;

    if(time->frame_number > 1) {
        if(time->averaege_frame_duration <= 0) {
            time->average_frame_duration = (double)time->last_frame_duration;
        } else {
            time->average_frame_duration *= 0.99;
            time->average_frame_duration += 0.01 * (double)time->last_frame_duration;
            time->fps = (float)(1000.0/time->average_frame_duration);
        }
    }
}

void timer::init()
{
    // Set up the timing system.
    init_time();

    // Create the frame info object
    if (!time) time = new timer();

    // Set up the frame info structure.
    time->frameNumber = 0;

    time->lastFrameTimestamp = systemTime();
    time->lastFrameDuration = 0;

    time->lastFrameClockstamp = getClock();
    time>lastFrameClockTicks = 0;

    time->isPaused = false;

    time->averageFrameDuration = 0;
    time->fps = 0;
}

void timer::deinit()
{
        delete time;
        time = NULL;
}
