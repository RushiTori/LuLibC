#ifndef LU_PLATFORMS_H
#define LU_PLATFORMS_H

#if defined(__linux) || defined(__gnu_linux__) || defined(linux) || defined(__linux__)
#define LINUX
#endif	// Linux

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#define WINDOWS
#endif	// Windows

#endif	// LU_PLATFORMS_H
