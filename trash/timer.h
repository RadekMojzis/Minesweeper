
#ifndef TIMER_INCLUDED
#define TIMER_INCLUDED

class Timer{
  private:
    int start_ticks;
    int paused_ticks;

    bool running;
  public:
    Timer();
    void start();
    void stop();
    void pause();

    int get_ticks();
    bool is_running();
};

#endif 
