/* lib/curl/curl_config.h */

#pragma once

#define HAVE_ASSERT_H       1
#define HAVE_MEMORY_H       1
#define HAVE_STDINT_H       1
#define HAVE_STDIO_H        1
#define HAVE_STDLIB_H       1
#define HAVE_STRING_H       1
#define HAVE_UNISTD_H       1
#define HAVE_SYS_SOCKET_H   1
#define HAVE_SYS_TIME_H     1

#define HAVE_SYS_TYPES_H    1
#define HAVE_TIME_H         1
#define TIME_WITH_SYS_TIME  1

#define SIZEOF_LONG         8
#define SIZEOF_CURL_OFF_T   8

#define CURL_PULL_SYS_TYPES_H
#define CURL_PULL_SYS_SOCKET_H
#define CURL_PULL_TIME_H

#define CURL_DISABLE_LDAP
#define CURL_DISABLE_POP3
#define CURL_DISABLE_SMTP
#define CURL_DISABLE_RTSP
#define CURL_DISABLE_TFTP
#define CURL_DISABLE_GOPHER
#define CURL_DISABLE_IMAP
#define CURL_DISABLE_NTLM
