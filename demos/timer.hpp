#ifndef TIMER_H
#define TIMER_H

struct timer {
    unsigned frame_number;
    unsigned last_frame_timestamp;
    unsigned last_frame_duration;
    unsigned long last_frame_clockstamp;
    unsigned long last_frame_clock_ticks;
    bool is_paused;
    double average_frame_duration;
    float fps;

    static timer& get();
    static void update();
    static void init();
    static void deinit();
    static unsigned get_time();
    static unsigned long get_clock();

private:
    timer() {}
    timer(const timer &) {}
    timer& operator=(const timer &);
};

#endif
