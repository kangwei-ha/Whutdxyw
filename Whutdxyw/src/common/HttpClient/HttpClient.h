#pragma once

#pragma comment(lib, "ws2_32.lib")
#pragma warning (disable: 4996)

#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <chrono>
#include <random>
#include <regex>

#include "../Tool/Tool.h"

#define BUFFERSIZE	1024		//缓冲区大小
#define PORT_HTTPS	443			//https的默认端口
#define PORT_HTTP	80			//http的默认端口
#define PORT_USE	PORT_HTTP

namespace kHttpsClient {
	class HttpsClient
	{
	public:
		HttpsClient() {
		};

		~HttpsClient() {
			clear();
		}

		virtual int Init(const std::string& url) = 0;
		virtual int SendRequest(const std::string& sHeader, const std::string& sData, std::string& sRecv) = 0;

		void clear() {
			if (m_cSocket != -1) {
				closesocket(m_cSocket);
				m_cSocket = -1;
			}

			WSACleanup();
		}

	protected:
		std::string m_url;
		SOCKET m_cSocket = -1;
	};

	class WindowsHttpsClient : public HttpsClient
	{
	public:
		WindowsHttpsClient();
		~WindowsHttpsClient();

		int Init(const std::string& url);
		int SendRequest(const std::string& sHeader, const std::string& sData, std::string& sRecv);
	};

	HttpsClient* CreateHttpsClient();
}