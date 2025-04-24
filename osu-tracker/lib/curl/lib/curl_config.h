#ifndef HEADER_CURL_CONFIG_H
#define HEADER_CURL_CONFIG_H

/* Fundamental headers available */
#define HAVE_ASSERT_H 1
#define HAVE_ERRNO_H 1
#define HAVE_FCNTL_H 1
#define HAVE_MALLOC_H 1
#define HAVE_NETINET_IN_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_UNISTD_H 1

/* Functions */
#define HAVE_MEMCPY 1
#define HAVE_MEMMOVE 1
#define HAVE_STRDUP 1
#define HAVE_STRSTR 1
#define HAVE_SOCKET 1
#define HAVE_RECV 1
#define HAVE_SEND 1
#define HAVE_SELECT 1
#define HAVE_POLL 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_GMTIME_R 1
#define HAVE_TIME_H 1
#define HAVE_SYS_SELECT_H 1

/* Data types */
#define HAVE_STRUCT_TIMEVAL 1
#define HAVE_STRUCT_SOCKADDR_STORAGE 1
#define HAVE_SOCKLEN_T 1
#define HAVE_U_INT32_T 1

/* Network features */
#define ENABLE_IPV6 1
#define USE_THREADS_POSIX 1

/* Protocols */
#define USE_HTTP 1
#define CURL_DISABLE_FTP 1
#define CURL_DISABLE_LDAP 1
#define CURL_DISABLE_TFTP 1
#define CURL_DISABLE_TELNET 1
#define CURL_DISABLE_DICT 1
#define CURL_DISABLE_FILE 1
#define CURL_DISABLE_GOPHER 1
#define CURL_DISABLE_MQTT 1
#define CURL_DISABLE_IMAP 1
#define CURL_DISABLE_POP3 1
#define CURL_DISABLE_SMTP 1
#define CURL_DISABLE_RTSP 1

/* Zlib compression */
#define HAVE_ZLIB_H 1
#define HAVE_LIBZ 1
#define USE_ZLIB 1

/* General config */
#define CURL_STATICLIB 1
#define BUILDING_LIBCURL 1

/* Curl features */
#define CURL_DISABLE_COOKIES 0
#define CURL_DISABLE_PROXY 0
#define CURL_DISABLE_VERBOSE_STRINGS 0

#endif /* HEADER_CURL_CONFIG_H */
