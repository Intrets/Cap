#pragma once

#define NOCOPY(T) T(const T&) = delete; T& operator=(const T&) = delete;
#define NOMOVE(T) T(T&&) = delete; T& operator=(T&&) = delete;
#define NOCOPYMOVE(T) NOCOPY(T) NOMOVE(T)