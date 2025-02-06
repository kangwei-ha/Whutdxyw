
// WhutdxywDlg.h: 头文件
//

#pragma once
#include "src/Dxyw/Dxyw.h"
#include <thread>
#include <mutex>

#define AutoRefreshTime		1000*60*1		//1分钟自动刷新一次

// CWhutdxywDlg 对话框
class CWhutdxywDlg : public CDialogEx
{
// 构造
public:
	CWhutdxywDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WHUTDXYW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonDeleterow();

	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editXuehao;
	CEdit m_editMima;

	CListCtrl m_userList;

	std::mutex m_enterVec;
	std::vector<dxyw::Student*> m_studyStudents;
private:
	int Init();
	int SetValue(int r, int c, CString content);
	int AutoRefresh();
};
