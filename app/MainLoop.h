#pragma once

#include <chrono>

struct GLFWwindow;

void mainLoop(GLFWwindow* window, std::chrono::steady_clock::time_point startTime);