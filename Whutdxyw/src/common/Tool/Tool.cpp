#include "Tool.h"

/*多字符转换为宽字符 --- ANSI -to- Unicode*/
wchar_t* ANSIToUnicode(const char* str)
{
	size_t textlen;
	wchar_t* result;
	textlen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	result = (wchar_t*)malloc((textlen + 1) * sizeof(wchar_t));
	if (nullptr != (result = (wchar_t*)malloc((textlen + 1) * sizeof(wchar_t))))
	{
		memset(result, 0, (textlen + 1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)result, textlen);
		return result;
	}
	return 0;
}

/*宽字符转换为多字符 --- Unicode -to- ANSI*/
char* UnicodeToANSI(const wchar_t* str)
{
	char* result;
	size_t textlen;
	textlen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	result = (char*)malloc((textlen + 1) * sizeof(char));
	if (nullptr != result)
	{
		memset(result, 0, sizeof(char) * (textlen + 1));
		WideCharToMultiByte(CP_ACP, 0, str, -1, result, textlen, NULL, NULL);
		return result;
	}
	return 0;
}

/*UTF8转换为宽字符 --- UTF8 -to- Unicode */
wchar_t* UTF8ToUnicode(const char* str)
{
	size_t textlen;
	wchar_t* result;
	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t*)malloc((textlen + 1) * sizeof(wchar_t));
	if (nullptr != result)
	{
		memset(result, 0, (textlen + 1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
		return result;
	}
	return 0;
}

/*宽字符转换为UTF8 --- Unicode -to- UTF8 */
char* UnicodeToUTF8(const wchar_t* str)
{
	char* result;
	size_t textlen;
	textlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	result = (char*)malloc((textlen + 1) * sizeof(char));
	if (nullptr != result)
	{
		memset(result, 0, sizeof(char) * (textlen + 1));
		WideCharToMultiByte(CP_UTF8, 0, str, -1, result, textlen, NULL, NULL);
		return result;
	}
	return 0;
}

/*多字符转换为UTF8 --- Unicode -to- UTF8 */
char* ANSIToUTF8(const char* str)
{
	return UnicodeToUTF8(ANSIToUnicode(str));
}

/*UTF8转换为多字符 --- UTF8 -to- ANSI */
char* UTF8ToANSI(const char* str)
{
	return UnicodeToANSI(UTF8ToUnicode(str));
}

std::vector<std::string> GetChildStr(std::string& text, std::string start, std::string end) {
	// 构建正则表达式模式，注意这里我们假设 start 和 end 不包含需要转义的特殊字符
	std::string pattern = start + "[^" + end + "]*" + end;
	std::regex re(pattern);
	std::smatch match;
	std::vector<std::string> result;

	// 使用 std::sregex_iterator 查找所有匹配项
	auto words_begin = std::sregex_iterator(text.begin(), text.end(), re);
	auto words_end = std::sregex_iterator();

	for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
		match = *i;
		result.push_back(match.str());
	}

	return result;
}

std::string WCharToChar(CString cstr)
{
	CStringA stra(cstr.GetBuffer(0));
	cstr.ReleaseBuffer();

	std::string imgpath = stra.GetBuffer(0);
	stra.ReleaseBuffer();
	return imgpath;
}

/*
 * 通用显示错误信息函数
 */
void ShowErrorMsg(int msgType, const char* pszFormat, ...)
{
	va_list args;
	va_start(args, pszFormat);
	char szInfo[512] = { 0 };
	vsprintf_s(szInfo, 512, pszFormat, args);
	va_end(args);

	char errorMsg[512] = { 0 };

	if (msgType == ERRORMSG) {
		sprintf_s(errorMsg, "[ERROR] %s", szInfo);
	}
	else if (msgType == INFOMSG) {
		sprintf_s(errorMsg, "[INFO] %s", szInfo);
	}

	WCHAR wszClassName[512];
	memset(wszClassName, 0, sizeof(wszClassName));
	MultiByteToWideChar(CP_ACP, 0, errorMsg, strlen(errorMsg) + 1, wszClassName, sizeof(wszClassName) / sizeof(wszClassName[0]));

	if (msgType == ERRORMSG) {
		MessageBox(NULL, wszClassName, L"tips", MB_ICONWARNING | MB_OK);
	}
	else if (msgType == INFOMSG) {
		MessageBox(NULL, wszClassName, L"tips", MB_ICONINFORMATION | MB_OK);
	}

}

std::string urlEncode(const std::string& str) {
	std::stringstream encodedStr;
	encodedStr << std::hex;  // 将输出设置为十六进制格式

	for (char ch : str) {
		if (std::isalnum(ch) || ch == '_' || ch == '=' || ch == '&' || ch == '%') {
			encodedStr << ch;  // 字母和数字保持原样
		}
		else {
			encodedStr << '%' << std::toupper(static_cast<int>(ch));  // 非字母和数字字符转换为%XX的形式
		}
	}

	return encodedStr.str();
}

std::wstring UTF8string(std::string strTemp)
{
	// 将 UTF-8 字符串转换为 wstring 类型
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::wstring wide = conv.from_bytes(strTemp);

	// 输出转换后的 Unicode 字符串
	std::wcout << wide << std::endl;

	return wide;

}