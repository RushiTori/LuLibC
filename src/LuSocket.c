#include "LuSocket.h"

#define IsSocketValid(fd) (((SocketType)(fd)) != LU_SOCKET_INVALID_SOCKET)

static bool ErrorCausedByNonBlocking(void) {
	int errCode = 0;

#if defined(LINUX)
	errCode = errno;
	return ((errCode == EAGAIN) || (errCode == EWOULDBLOCK));
#endif	// Linux

#if defined(WINDOWS)
	errCode = WSAGetLastError();
	return (errCode == WSAEWOULDBLOCK);
#endif	// Windows
}

static void CloseSocket(SocketType socket) {
#if defined(LINUX)
	close(socket);
#endif	// Linux

#if defined(WINDOWS)
	closesocket(socket);
#endif	// Windows
}

LuSocket* LuSocketCreate() {
	int socketFD = socket(LU_SOCKET_DOMAIN, LU_SOCKET_TYPE, 0);
	if (!IsSocketValid(socketFD)) {
#if defined(LINUX)
		LogErrnoAndReturn(NULL);
#endif	// Linux

#if defined(WINDOWS)
		LogWSAErrorAndReturn(NULL);
#endif	// Windows
	}

	LuSocket* sock = calloc(1, sizeof(LuSocket));
	if (!sock) {
		CloseSocket(socketFD);
		LogErrnoAndReturn(NULL);
	}

	sock->source = (SocketInfo){
		.fd = socketFD,
		.addr = (struct sockaddr_in){0},
		.addrLen = sizeof(struct sockaddr_in),
	};

	for (size_t i = 0; i < LU_SOCKET_MAX_DESTINATIONS; i++) {
		sock->destinations[i].fd = -1;
	}

	return sock;
}

void LuSocketFree(LuSocket* socket) {
	CloseSocket(socket->source.fd);

	for (size_t i = 0; i < LU_SOCKET_MAX_DESTINATIONS; i++) {
		int fd = socket->destinations[i].fd;
		if (!IsSocketValid(fd)) continue;

		CloseSocket(fd);
	}

	free(socket);
}

bool LuSocketIsDestinationValid(LuSocket* socket, size_t dest) {
	if (dest >= LU_SOCKET_MAX_DESTINATIONS) return false;
	if (!IsSocketValid(socket->destinations[dest].fd)) return false;

	return true;
}

size_t LuSocketGetDestinationsCount(const LuSocket* socket) {
	size_t count = 0;

	for (size_t i = 0; i < LU_SOCKET_MAX_DESTINATIONS; i++) {
		int fd = socket->destinations[i].fd;
		if (!IsSocketValid(fd)) continue;

		count++;
	}

	return count;
}

static bool SetSocketNonBlocking(SocketType socket) {
	int errCode = 0;

#if defined(LINUX)
	errCode = fcntl(socket, F_SETFL, O_NONBLOCK);
	if (errCode < 0) LogErrnoAndReturn(false);
#endif	// Linux

#if defined(WINDOWS)
	u_long mode = 1;  // 1 to enable non-blocking socket
	errCode = ioctlsocket(socket, FIONBIO, &mode);
	if (errCode == SOCKET_ERROR) LogWSAErrorAndReturn(false);
#endif	// Windows

	return true;
}

bool LuSocketSetNonBlocking(LuSocket* socket) { return SetSocketNonBlocking(socket->source.fd); }

bool LuSocketBind(LuSocket* socket, uint16_t port) {
	struct sockaddr_in addr = (struct sockaddr_in){
		.sin_family = LU_SOCKET_FAMILY,
		.sin_port = htons(port),
		.sin_addr = (struct in_addr){.s_addr = LU_SOCKET_BIND_ADDR},
		.sin_zero = {0},
	};

	int bindErr = bind(socket->source.fd, (struct sockaddr*)(&addr), sizeof(addr));

#if defined(LINUX)
	if (bindErr < 0) LogErrnoAndReturn(false);
#endif	// Linux

#if defined(WINDOWS)
	if (bindErr == SOCKET_ERROR) LogWSAErrorAndReturn(false);
#endif	// Windows

	return true;
}

bool LuSocketSetOptions(LuSocket* socket, int level, int name, const void* val, size_t len) {
	int errCode = setsockopt(socket->source.fd, level, name, val, len);

#if defined(LINUX)
	if (errCode < 0) LogErrnoAndReturn(false);
#endif	// Linux

#if defined(WINDOWS)
	if (errCode == SOCKET_ERROR) LogWSAErrorAndReturn(false);
#endif	// Windows

	return true;
}

static ssize_t LuSocketGetFirstAvailableDestination(const LuSocket* socket) {
	for (size_t i = 0; i < LU_SOCKET_MAX_DESTINATIONS; i++) {
		int fd = socket->destinations[i].fd;
		if (!IsSocketValid(fd)) return i;
	}

	return -1;
}

bool LuSocketCanAcceptConnections(const LuSocket* socket) { return LuSocketGetFirstAvailableDestination(socket) >= 0; }

int LuSocketListenForConnections(LuSocket* socket, size_t n) {
	size_t listenCount = LU_SOCKET_MAX_DESTINATIONS - LuSocketGetDestinationsCount(socket);
	if (n < listenCount) listenCount = n;

	int listenErr = listen(socket->source.fd, listenCount);

#if defined(LINUX)
	if (listenErr < 0) LogErrnoAndReturn(false);
#endif	// Linux

#if defined(WINDOWS)
	if (listenErr == SOCKET_ERROR) LogWSAErrorAndReturn(false);
#endif	// Windows

	return listenCount;
}

bool LuSocketAcceptConnection(LuSocket* socket) {
	if (!LuSocketCanAcceptConnections(socket)) return false;

	struct sockaddr_in addr = {0};

#if defined(LINUX)
	socklen_t addrLen = sizeof(addr);
#endif	// Linux

#if defined(WINDOWS)
	int addrLen = sizeof(addr);
#endif	// Windows

	SocketType socketFD = accept(socket->source.fd, (struct sockaddr*)(&addr), &addrLen);
	if (!IsSocketValid(socketFD)) {
#if defined(LINUX)
		if (!ErrorCausedByNonBlocking()) LogErrnoAndReturn(false);
#endif	// Linux

#if defined(WINDOWS)
		if (!ErrorCausedByNonBlocking()) LogWSAErrorAndReturn(false);
#endif	// Windows

		return true;
	}

	SetSocketNonBlocking(socketFD);

	if (!LuSocketCanAcceptConnections(socket)) {
		CloseSocket(socketFD);
		return false;
	}

	ssize_t idx = LuSocketGetFirstAvailableDestination(socket);

	socket->destinations[idx] = (SocketInfo){
		.fd = socketFD,
		.addr = addr,
		.addrLen = addrLen,
	};

	return true;
}

bool LuSocketConnectTo(LuSocket* socket, const char* ipv4, uint16_t port) {
	struct sockaddr_in peerAddr = {0};

	peerAddr.sin_family = AF_INET;
	peerAddr.sin_port = htons(port);

#if defined(LINUX)
	int ptonErrCode = inet_pton(AF_INET, ipv4, &(peerAddr.sin_addr));
	if (ptonErrCode == 0) LogStringAndReturn("Invalid ipv4 address !", false);
#endif	// Linux

#if defined(WINDOWS)
	uint32_t netAddr = inet_addr(ipv4);
	if (netAddr == INADDR_NONE) LogStringAndReturn("Invalid ipv4 address !", false);
	peerAddr.sin_addr.S_un.S_addr = netAddr;
#endif	// Windows

	int connectErr = connect(socket->source.fd, (struct sockaddr*)(&peerAddr), sizeof(peerAddr));

#if defined(LINUX)
	if (connectErr < 0 && !ErrorCausedByNonBlocking()) LogErrnoAndReturn(false);
#endif	// Linux

#if defined(WINDOWS)
	if (connectErr == SOCKET_ERROR && !ErrorCausedByNonBlocking()) LogWSAErrorAndReturn(false);
#endif	// Windows

	return true;
}

static SocketType LuSocketGetDestFD(const LuSocket* socket, ssize_t dest) {
	SocketType destFD = -1;

	if (dest == LU_SOCKET_IO_SELF) {
		destFD = socket->source.fd;
	} else if (dest >= 0 && dest < LU_SOCKET_MAX_DESTINATIONS) {
		destFD = socket->destinations[dest].fd;
	}

	if (!IsSocketValid(destFD)) {
		errno = EBADF;
		LogErrnoAndReturn(-1);
	}

	return destFD;
}

ssize_t LuSocketCanReceive(const LuSocket* socket, ssize_t dest) {
	SocketType destFD = LuSocketGetDestFD(socket, dest);
	if (!IsSocketValid(destFD)) return -1;

#if defined(LINUX)
	int count = 0;
	int errCode = ioctl(destFD, FIONREAD, &count);
	if (errCode < 0) LogErrnoAndReturn(-1);
#endif	// Linux

#if defined(WINDOWS)
	u_long count = 0;
	int errCode = ioctlsocket(destFD, FIONREAD, &count);
	if (errCode < 0) LogWSAErrorAndReturn(-1);
#endif	// Windows

	return count;
}

static void LuSocketRemoveDestination(LuSocket* socket, size_t dest) {
	SocketType destFD = LuSocketGetDestFD(socket, dest);
	CloseSocket(destFD);
	socket->destinations[dest].fd = -1;
}

ssize_t LuSocketReceive(LuSocket* socket, ssize_t dest, void* buffer, size_t n, int flags) {
	SocketType destFD = LuSocketGetDestFD(socket, dest);
	if (!IsSocketValid(destFD)) return -1;

	ssize_t len = recv(destFD, buffer, n, flags);

#if defined(LINUX)
	if ((len < 0) && !ErrorCausedByNonBlocking()) {
		LogErrnoAndReturn(-1);
	} else if (len == 0) {
		if (dest != LU_SOCKET_IO_SELF) LuSocketRemoveDestination(socket, dest);
	}
#endif	// Linux

#if defined(WINDOWS)
	if ((len == SOCKET_ERROR) && !ErrorCausedByNonBlocking()) {
		LogWSAErrorAndReturn(-1);
	} else if (len == 0) {
		if (dest != LU_SOCKET_IO_SELF) LuSocketRemoveDestination(socket, dest);
	}
#endif	// Windows

	return len;
}

bool LuSocketProbe(LuSocket* socket, size_t dest) {
	char probe = 0;
	ssize_t len = LuSocketReceive(socket, dest, &probe, sizeof(probe), MSG_PEEK);
	return (len != 0);
}

ssize_t LuSocketSend(LuSocket* socket, ssize_t dest, const void* buffer, size_t n, int flags) {
	SocketType destFD = LuSocketGetDestFD(socket, dest);
	if (!IsSocketValid(destFD)) return -1;

	ssize_t len = send(destFD, buffer, n, flags);

#if defined(LINUX)
	if ((len < 0) && !ErrorCausedByNonBlocking()) {
		LogErrnoAndReturn(-1);
	} else if (len == 0) {
		if (dest != LU_SOCKET_IO_SELF) LuSocketRemoveDestination(socket, dest);
	}
#endif	// Linux

#if defined(WINDOWS)
	if ((len == SOCKET_ERROR) && !ErrorCausedByNonBlocking()) {
		LogWSAErrorAndReturn(-1);
	} else if (len == 0) {
		if (dest != LU_SOCKET_IO_SELF) LuSocketRemoveDestination(socket, dest);
	}
#endif	// Windows

	return len;
}

bool LuSocketBroadcast(LuSocket* socket, const void* buffer, size_t n, int flags) {
	bool success = true;

	for (size_t i = 0; i < LU_SOCKET_MAX_DESTINATIONS; i++) {
		int fd = socket->destinations[i].fd;
		if (!IsSocketValid(fd)) continue;

		if (LuSocketSend(socket, i, buffer, n, flags) < 0) success = false;
	}

	return success;
}

bool LuSocketBroadcastExcept(LuSocket* socket, size_t exception, const void* buffer, size_t n, int flags) {
	if (exception >= LU_SOCKET_MAX_DESTINATIONS) return LuSocketBroadcast(socket, buffer, n, flags);
	bool success = true;

	for (size_t i = 0; i < LU_SOCKET_MAX_DESTINATIONS; i++) {
		if (i == exception) continue;

		int fd = socket->destinations[i].fd;
		if (!IsSocketValid(fd)) continue;

		if (LuSocketSend(socket, i, buffer, n, flags) < 0) success = false;
	}

	return success;
}
