// TinyTimer.h - Tiny timer library for Arduino
// Copyright 2016 abouvier <abouvier@student.42.fr>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef TINYTIMER_H
#define TINYTIMER_H

#include <Arduino.h>

template <unsigned long (*now)() = millis>
class TinyTimer
{
public:
	typedef unsigned long time_t;
	typedef void (*callback_t)();

private:
	callback_t m_callback;
	time_t m_delay;
	size_t m_repeat;
	time_t m_now;

public:
	TinyTimer() : TinyTimer(nullptr)
	{
	}

	TinyTimer(callback_t callback) : TinyTimer(callback, 0)
	{
	}

	TinyTimer(callback_t callback, time_t delay) : TinyTimer(callback, delay, 0)
	{
	}

	TinyTimer(callback_t callback, time_t delay, size_t repeat) : m_callback(callback), m_delay(delay), m_repeat(repeat)
	{
		restart();
	}

	void init(callback_t callback)
	{
		init(callback, 0);
	}

	void init(callback_t callback, time_t delay)
	{
		init(callback, delay, 0);
	}

	void init(callback_t callback, time_t delay, size_t repeat)
	{
		m_callback = callback;
		m_delay = delay;
		m_repeat = repeat;
		restart();
	}

	template <size_t size>
	static void update(TinyTimer (&timers)[size])
	{
		for (auto &timer : timers)
			update(timer);
	}

	static void update(TinyTimer &timer)
	{
		if (timer.m_callback)
		{
			if (now() - timer.m_now >= timer.m_delay)
			{
				timer.m_callback();
				if (timer.m_repeat && !--timer.m_repeat)
					timer.m_callback = nullptr;
				timer.m_now += timer.m_delay;
			}
		}
	}

	void restart()
	{
		m_now = now();
	}
};

#endif
