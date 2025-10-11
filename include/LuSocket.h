#ifndef LU_SOCKET_H
#define LU_SOCKET_H

#include "LuUtils.h"

// net "lib"
#if defined(LINUX)
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#endif	// Linux

#if defined(WINDOWS)
#include <winsock2.h>
#endif	// Windows

#define LU_SOCKET_DOMAIN AF_INET
#define LU_SOCKET_TYPE SOCK_STREAM

#define LU_SOCKET_FAMILY AF_INET
#define LU_SOCKET_BIND_ADDR INADDR_ANY

#define LU_SOCKET_IO_SELF -1

#define LU_SOCKET_MAX_DESTINATIONS 255

typedef struct SocketInfo {
	SocketType fd;
	char padding[sizeof(int64_t) - sizeof(SocketType)];

	struct sockaddr_in addr;
	uint64_t addrLen;
} SocketInfo;

typedef struct LuSocket {
	SocketInfo source;

	SocketInfo destinations[LU_SOCKET_MAX_DESTINATIONS];
} LuSocket;

// Wrapper around "socket"
// Return: a pointer to a LuSocket that should be freed using LuSocketFree on success, or NULL on failure
LuSocket* LuSocketCreate(void);

// Wrapper around "close"
// Return: true for success, false for failure
void LuSocketFree(LuSocket* socket);

// Checks socket->destinations[dest] to see if it's a valid one
// Return: true if the destination is valid, false if not
bool LuSocketIsDestinationValid(LuSocket* socket, size_t dest);

// Return: the number of valid destinations in socket->destinations
size_t LuSocketGetDestinationsCount(const LuSocket* socket);

// Wrapper around "fcntl"
// Return: true for success, false for failure
bool LuSocketSetNonBlocking(LuSocket* socket);

// Wrapper around "bind"
// Return: true for success, false for failure
bool LuSocketBind(LuSocket* socket, uint16_t port);

// Wrapper around "setsockopt"
// Return: true for success, false for failure
bool LuSocketSetOptions(LuSocket* socket, int level, int name, const void* val, size_t len);

// Wrapper around "listen"
// Return: number of connections that are being listened to, or -1 for errors
int LuSocketListenForConnections(LuSocket* socket, size_t n);

// Checks all destinations in socket->destinations for an available one
// Return: true if a destination is free, false if there are no destination available
bool LuSocketCanAcceptConnections(const LuSocket* socket);

// Wrapper around "accept"
// Return: true for success, false for failure
bool LuSocketAcceptConnection(LuSocket* socket);

// Wrapper around "connect"
// Return: true for success, false for failure
bool LuSocketConnectTo(LuSocket* socket, const char* ipv4, uint16_t port);

// Wrapper around ioctl(fd, FIONREAD, ...)
// Return the number of bytes that can be received from "dest"
ssize_t LuSocketCanReceive(const LuSocket* socket, ssize_t dest);

// Wrapper around "recv"
// Return: the number of bytes received, or -1 in case of failure
ssize_t LuSocketReceive(LuSocket* socket, ssize_t dest, void* buffer, size_t n, int flags);

// Will call recv with MSG_PEEK and MSG_DONTWAIT to see if the destination is still reachable
// If the destination isn't reachable the destination will be freed to accept a future one
// Return: true if the destination is still reachable, false if the destination isn't reachable
bool LuSocketProbe(LuSocket* socket, size_t dest);

// Wrapper around "send" (will also call "LuSocketProbe")
// Return: the number of bytes sent, or -1 in case of failure
ssize_t LuSocketSend(LuSocket* socket, ssize_t dest, const void* buffer, size_t n, int flags);

// Loops over all destinations (except LU_SOCKET_IO_SELF), to send the buffer to all
// Return: true for success, false for failure
bool LuSocketBroadcast(LuSocket* socket, const void* buffer, size_t n, int flags);

// Loops over all destinations (except LU_SOCKET_IO_SELF), to send the buffer to all except "exception"
// Return: true for success, false for failure
bool LuSocketBroadcastExcept(LuSocket* socket, size_t exception, const void* buffer, size_t n, int flags);

#endif	// LU_SOCKET_H
