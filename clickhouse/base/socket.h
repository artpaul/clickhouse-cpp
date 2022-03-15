#pragma once

#include "input.h"
#include "output.h"
#include "platform.h"

#include <cstddef>
#include <string>
#include <optional>

#if defined(_win_)
#   pragma comment(lib, "Ws2_32.lib")

#   include <winsock2.h>
#   include <ws2tcpip.h>
#else
#   include <arpa/inet.h>
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <poll.h>

#   if !defined(SOCKET)
#       define SOCKET int
#   endif
#endif

struct addrinfo;
struct timeval;

namespace clickhouse {

/**
 *
 */
class NetworkAddress {
public:
    explicit NetworkAddress(const std::string& host,
                            const std::string& port = "0");
    ~NetworkAddress();

    const struct addrinfo* Info() const;

private:
    struct addrinfo* info_;
};

class SocketTimeoutParams {
public:
    explicit SocketTimeoutParams(
        unsigned int connection_socket_recv_timeout_sec,
        unsigned int connection_socket_recv_timeout_usec,
        unsigned int connection_socket_send_timeout_sec,
        unsigned int connection_socket_send_timeout_usec
    );

    const struct timeval& GetRecvTimeout();
    const struct timeval& GetSendTimeout();

private:
    const struct timeval recv_timeout_;
    const struct timeval send_timeout_;
};


class SocketHolder {
public:
    SocketHolder();
    SocketHolder(SOCKET s);
    SocketHolder(SocketHolder&& other) noexcept;

    ~SocketHolder();

    void Close() noexcept;

    bool Closed() const noexcept;

    /// @params idle the time (in seconds) the connection needs to remain
    ///         idle before TCP starts sending keepalive probes.
    /// @params intvl the time (in seconds) between individual keepalive probes.
    /// @params cnt the maximum number of keepalive probes TCP should send
    ///         before dropping the connection.
    void SetTcpKeepAlive(int idle, int intvl, int cnt) noexcept;

    SocketHolder& operator = (SocketHolder&& other) noexcept;

    operator SOCKET () const noexcept;

private:
    SocketHolder(const SocketHolder&) = delete;
    SocketHolder& operator = (const SocketHolder&) = delete;

    SOCKET handle_;
};


/**
 *
 */
class SocketInput : public InputStream {
public:
    explicit SocketInput(SOCKET s);
    ~SocketInput();

protected:
    size_t DoRead(void* buf, size_t len) override;

private:
    SOCKET s_;
};

class SocketOutput : public OutputStream {
public:
    explicit SocketOutput(SOCKET s);
    ~SocketOutput();

protected:
    void DoWrite(const void* data, size_t len) override;

private:
    SOCKET s_;
};

static struct NetworkInitializer {
    NetworkInitializer();
} gNetworkInitializer;

///
SOCKET SocketConnect(const NetworkAddress& addr, const std::optional<SocketTimeoutParams>& socket_timeout_params);

ssize_t Poll(struct pollfd* fds, int nfds, int timeout) noexcept;

}
