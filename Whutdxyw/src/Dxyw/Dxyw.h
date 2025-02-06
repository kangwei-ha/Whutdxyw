#pragma once

#include <iostream>

#include "../common/HttpClient/HttpClient.h"

namespace dxyw {
	class Student
	{
	public:
		Student();
		~Student();

		int SetInfo(int, std::string, std::string);
		void Register(std::function<int(int, int, CString content)> Callback);
		int DoAll();
		int Refresh();
		void SubNum() {
			m_num--;
		}

		void GetInfo(Student* newstu) {
			newstu->SetInfo(m_num, m_xuehao, m_mima);
			newstu->Register(InfoCallback);
		}

	private:
		int m_num;			//在listctrl中的序号，从0开始
		std::string m_xuehao;
		std::string m_mima;

		kHttpsClient::HttpsClient* m_httpClient;
		std::string m_sessionId;
		std::string m_viewState;

		std::wstring m_wName;
		std::wstring m_wOnlineTime;
		std::wstring m_wExamNum;

		std::function<int(int, int, CString content)> InfoCallback;			//拿到信息之后就修改列表值

		int GetSessionId();
		int PostAccount();
		int GetStudentInfo();		//获取学生信息
		int Study();
	};
}