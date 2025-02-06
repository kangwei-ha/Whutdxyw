#include "HttpClient.h"

namespace kHttpsClient {
	WindowsHttpsClient::WindowsHttpsClient() {

	}

	WindowsHttpsClient::~WindowsHttpsClient() {
		clear();
	}

	int WindowsHttpsClient::Init(const std::string& url){
		m_url = url;

		//初始化dll
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			ShowErrorMsg(-1, "HTTPClient::Init : Failed to initialize Winsock\n");
			clear();
			return 0;
		}

		//获取Host的IP地址等信息
		ADDRINFOT aiHints;
		ZeroMemory(&aiHints, sizeof(ADDRINFOT));
		aiHints.ai_family = AF_INET;
		aiHints.ai_flags = AI_PASSIVE;
		aiHints.ai_protocol = 0;
		aiHints.ai_socktype = SOCK_STREAM;
		std::wstring wstrHost(url.begin(), url.end());
		PADDRINFOT paiResult;
		GetAddrInfo(wstrHost.c_str(), NULL, &aiHints, &paiResult);

		//创建套接字
		m_cSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_cSocket == INVALID_SOCKET) {
			ShowErrorMsg(-1, "HTTPClient::Init : Failed to create socket\n");
			clear();
			return 0;
		}

		SOCKADDR_IN sinHost{};
		sinHost.sin_addr = ((LPSOCKADDR_IN)paiResult->ai_addr)->sin_addr;
		sinHost.sin_family = AF_INET;
		sinHost.sin_port = htons(PORT_USE);

		if (connect(m_cSocket, (SOCKADDR*)&sinHost, sizeof(sinHost)) == SOCKET_ERROR) {
			ShowErrorMsg(-1, "HTTPClient::Init : Failed to connect to server\n");
			clear();
			return 0;
		}

		return 1;
	}

	int WindowsHttpsClient::SendRequest(const std::string& sHeader, const std::string& sData, std::string& sRecv) {
		std::string head_data = sHeader + sData;

		int iErrorWrite = send(m_cSocket, head_data.c_str(), head_data.size(), 0);		//通过SSL链接发送数据
		if (iErrorWrite < 0)
		{
			ShowErrorMsg(-1, "HTTPClient::SendRequest : Failed send\n");
			return 0;
		}

		char msg[BUFFERSIZE + 1];
		int totalrecv = 0;

		int ContentLength = 0;
		int HeadLength = 0;
		while (true)
		{
			int iErrorRead = recv(m_cSocket, msg, BUFFERSIZE, 0);
			if (iErrorRead < 0) {
				//ShowErrorMsg(-1, "HTTPClient::SendRequest : Failed recv\n");
				break;
			}

			sRecv.append(msg, iErrorRead);
			totalrecv += iErrorRead;

			if (!ContentLength) {			//如果不知道contentlength
				std::vector<std::string> match1 = GetChildStr(sRecv, std::string("Content-Length:"), std::string("\r\n"));
				if (match1.size() > 0) {
					ContentLength = atoi(match1[0].substr(15, match1[0].size() - 16).c_str());
				}
			}

			if (!HeadLength) {			//如果不知道headlength
				size_t headerEndPos = sRecv.find("\r\n\r\n");
				if (headerEndPos != std::string::npos) {
					HeadLength = headerEndPos + 4;
				}
			}

			if (totalrecv == HeadLength + ContentLength)
				break;
		}

		return sRecv.size();
	}

	HttpsClient* CreateHttpsClient() {
#ifdef _WIN32
		return new WindowsHttpsClient();
#else
		return new LinuxTcpClient();
#endif
	}
}