#include "Dxyw.h"

namespace dxyw {
	Student::Student() {
		m_httpClient = kHttpsClient::CreateHttpsClient();
		m_httpClient->Init("59.69.102.9");
	}

	Student::~Student() {

	}

	int Student::SetInfo(int num, std::string xuehao, std::string mima) {
		m_num = num;
		m_xuehao = xuehao;
		m_mima = mima;

		return 1;
	}

	void Student::Register(std::function<int(int, int, CString content)> Callback) {
		InfoCallback = Callback;
	}

	int Student::DoAll() {
		if (!GetSessionId()) {
			ShowErrorMsg(-1, "Student::DoAll : Failed to GetSessionId!\n请检查您是否正确连接校园网\n");
			return 0;
		}
		if (!PostAccount()) {
			ShowErrorMsg(-1, "Student::DoAll : Failed to PostAccount!\n请检查您的学号密码是否正确输入\n");
			return 0;
		}
		if (!GetStudentInfo()) {
			ShowErrorMsg(-1, "Student::DoAll : Failed to GetStudentInfo!\n请检查您是否正确连接校园网\n");
			return 0;
		}

		if (!Study()) {
			ShowErrorMsg(-1, "Student::DoAll : Failed to Study!\n请检查您是否正确连接校园网\n");
			return 0;
		}

		return 1;
	}

	int Student::Refresh() {
		if (!GetStudentInfo())
			return 0;

		if (!Study())
			return 0;

		return 1;
	}

	int Student::GetSessionId() {
		std::string header = "GET /zgyw/ HTTP/1.1\r\n"
			"Host: 59.69.102.9\r\n"
			"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n"
			"Accept-Encoding: gzip, deflate\r\n"
			"Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n"
			"Connection: keep-alive\r\n"
			"Upgrade-Insecure-Requests: 1\r\n"
			"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0\r\n"
			"\r\n";

		std::string srecv;
		m_httpClient->SendRequest(header, std::string(""), srecv);

		std::regex pattern("ASP.NET_SessionId=(.*?);");
		std::regex pattern_viewstate("name=\"__VIEWSTATE\" id=\"__VIEWSTATE\" value=\"(.*?)==\" />");

		std::smatch match;
		std::smatch match_viewstate;

		if (std::regex_search(srecv, match, pattern) && std::regex_search(srecv, match_viewstate, pattern_viewstate)) {
			m_sessionId.append(match[1].str());
			m_viewState.append(match_viewstate[1].str());
			char tempchar[7] = { 0x25, 0x33, 0x44, 0x25, 0x33, 0x44, 0x00 };
			m_viewState += tempchar;
			return true;
		}
		else {
			ShowErrorMsg(-1, "Student::GetSessionId : Failed to GetSessionId\n");
			return 0;
		}
		return 1;
	}

	int Student::PostAccount() {
		std::string postData = "__VIEWSTATE=" + m_viewState + "&"
			"ctl00$ContentPlaceHolder1$name=" + m_xuehao + "&"
			"ctl00$ContentPlaceHolder1$pwd=" + m_mima + "&ctl00$ContentPlaceHolder1$login=";

		postData = urlEncode(postData);
		char tempchar[19] = { 0x25, 0x45, 0x37, 0x25, 0x39, 0x39, 0x25, 0x42, 0x42,
				0x25, 0x45, 0x35, 0x25, 0x42, 0x44, 0x25, 0x39, 0x35, 0x00 };
		postData += tempchar;

		std::string postHeader = "POST /zgyw/index.aspx HTTP/1.1\r\n"
			"Host: 59.69.102.9\r\n"
			"Connection: keep-alive\r\n"
			"Content-Length: " + std::to_string(postData.size()) + "\r\n"
			"Cache-Control: max-age=0\r\n"
			"Upgrade-Insecure-Requests: 1\r\n"
			"Origin: http://59.69.102.9\r\n"
			"Content-Type: application/x-www-form-urlencoded\r\n"
			"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0\r\n"
			"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n"
			"Referer: http://59.69.102.9/zgyw/index.aspx\r\n"
			"Accept-Encoding: gzip, deflate\r\n"
			"Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n"
			"Cookie: ASP.NET_SessionId=" + m_sessionId + "\r\n"
			"\r\n";

		std::string srecv;
		m_httpClient->SendRequest(postHeader, postData, srecv);

		std::regex pattern("Location: /zgyw/(.*?)\r\n");
		std::smatch match;
		if (std::regex_search(srecv, match, pattern) && !strcmp(match[1].str().c_str(), "index.aspx")) {
			return 1;
		}
		return 0;
	}

	int Student::GetStudentInfo() {
		std::string header = "GET /zgyw/index.aspx HTTP/1.1\r\n"
			"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n"
			"Accept-Encoding: gzip, deflate\r\n"
			"Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n"
			"Cache-Control: max-age=0\r\n"
			"Connection: keep-alive\r\n"
			"Cookie: ASP.NET_SessionId=" + m_sessionId + "\r\n"
			"Host: 59.69.102.9\r\n"
			"Referer: http://59.69.102.9/zgyw/index.aspx\r\n"
			"Upgrade-Insecure-Requests: 1\r\n"
			"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0\r\n"
			"\r\n";

		std::string srecv;
		m_httpClient->SendRequest(header, std::string(""), srecv);

		std::string studentInfo;
		std::regex pattern_studentInfo("<div style=\" padding:15px;\">([\\s\\S]*?)</div>");
		std::smatch match_studentInfo;
		if (std::regex_search(srecv, match_studentInfo, pattern_studentInfo)) {
			studentInfo = match_studentInfo[1].str();

			std::regex pattern_name("<a href=\'onlineExam/user/usercenter.aspx\'>([\\s\\S]*?)</a>");
			std::regex pattern_onlineTime("<span id=\"ctl00_ContentPlaceHolder1_lblonlineTime\" style=\"color:Red;\">([\\s\\S]*?)</span>");
			std::regex pattern_examNum("<span id=\"ctl00_ContentPlaceHolder1_lblscore\" style=\"color:Red;\">([\\s\\S]*?)</span>");

			std::smatch match_name;
			std::smatch match_onlineTime;
			std::smatch match_examNum;

			if (std::regex_search(studentInfo, match_name, pattern_name) &&
				std::regex_search(studentInfo, match_onlineTime, pattern_onlineTime) &&
				std::regex_search(studentInfo, match_examNum, pattern_examNum))
			{
				m_wName = UTF8string(match_name[1].str());
				m_wOnlineTime = UTF8string(match_onlineTime[1].str());
				size_t pos = 0;
				std::wstring subwstr = L"&nbsp;";
				while ((pos = m_wOnlineTime.find(subwstr, pos)) != std::wstring::npos) {
					m_wOnlineTime.erase(pos, subwstr.length());
				}
				m_wExamNum = UTF8string(match_examNum[1].str());

				if (InfoCallback != nullptr) {
					InfoCallback(m_num, 2, m_wName.c_str());
					InfoCallback(m_num, 3, m_wOnlineTime.c_str());
					InfoCallback(m_num, 4, m_wExamNum.c_str());
					InfoCallback(m_num, 5, L"正在学习");
				}

				return true;
			}

		}
		m_wName = L"";
		m_wOnlineTime = L"";
		m_wExamNum = L"";

		return false;
	}

	int Student::Study() {
		std::string header = "GET /zgyw/study/LearningContent.aspx?type=1&id=1&learningid=3092 HTTP/1.1\r\n"
			"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n"
			"Accept-Encoding: gzip, deflate\r\n"
			"Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n"
			"Connection: keep-alive\r\n"
			"Cookie: ASP.NET_SessionId=" + m_sessionId + "\r\n"
			"Host: 59.69.102.9\r\n"
			"Referer: http://59.69.102.9/zgyw/study/LearningList.aspx?type=1&id=1\r\n"
			"Upgrade-Insecure-Requests: 1\r\n"
			"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0\r\n"
			"\r\n";

		std::string srecv;
		m_httpClient->SendRequest(header, std::string(""), srecv);

		if (srecv.size() < 20000)
			return false;

		return true;
	}
}