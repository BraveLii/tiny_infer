/* Copyright (c) 2020 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once
#include <stdlib.h>
#include <iostream>

// A Standard Timer implementation for debugging
class Timer {
 public:
  // a timer class for profiling
  // Reset() will be called during initialization
  // all timing variables will be set 0 in Reset()
  Timer() { Start(); }
  // void Reset();
  void Start();
  // void Pause();
  // Resume will get current system time
  // void Resume();
  int Count();
  // return elapsed time in us
  double ElapsedUS();
  // return elapsed time in ms
  double ElapsedMS();
  // return elapsed time in sec
  double ElapsedSec();
  int64_t TimeStampUS();

 private:
  struct timeval _start;
  struct timeval _now;
  int _count;
  int64_t _elapsed;
  bool _paused;

  // get us difference between start and now
  int64_t Tickus();
};

#define TIMER_INIT    \
  Timer macro_timer;
#define TIMER_RESTART \
  macro_timer.Start();
#define TIMER_MS      \
  macro_timer.ElapsedMS()
#define TIMER_MS_LOG(MSG) \
  std::cout << MSG << " cost " << macro_timer.ElapsedMS() << " ms." << std::endl;

#ifndef NDEBUG
#define DTIMER_INIT    TIMER_INIT
#define DTIMER_RESTART TIMER_RESTART
#define DTIMER_MS      TIMER_MS
#define DTIMER_MS_LOG(MSG) TIMER_MS_LOG(MSG)
#else
#define DTIMER_INIT  
#define DTIMER_RESTART 
#define DTIMER_MS 
#define DTIMER_MS_LOG(MSG) 
#endif
