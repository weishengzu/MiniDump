#include "MiniDump.h"

namespace fs = std::filesystem;

MiniDump::MiniDump() {}

MiniDump::~MiniDump() {}

void MiniDump::EnableAutoDump(bool bEnable) {
  if (bEnable) {
    SetUnhandledExceptionFilter(
        (LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
  }
}

LONG MiniDump::ApplicationCrashHandler(EXCEPTION_POINTERS *pException) {
  /*if (IsDebuggerPresent())
  {
          return EXCEPTION_CONTINUE_SEARCH;
  }*/

  TCHAR szDumpFile[MAX_PATH] = {0};
  TCHAR szMsg[MAX_PATH] = {0};
  SYSTEMTIME stTime = {0};
  auto dumpPath = GetminiDumpDir().c_str();
  auto nextMinidumpId = GetCurrentDateTime().c_str();
  // 构建dump文件路径;
  TSprintf(szDumpFile, "%s\\%s.dmp", dumpPath, nextMinidumpId);
  // 创建dump文件;
  CreateDumpFile(szDumpFile, pException);

  // 弹出一个错误对话框或者提示上传， 并退出程序;
  TSprintf(szMsg,
           _T("I'm so sorry, but the program crashed.\r\ndump file : %s"),
           szDumpFile);
  FatalAppExit(-1, szMsg);

  return EXCEPTION_EXECUTE_HANDLER;
}

void MiniDump::CreateDumpFile(LPCSTR strPath, EXCEPTION_POINTERS *pException) {
  // 创建Dump文件;
  HANDLE hDumpFile = CreateFile(strPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL, NULL);
  // Dump信息;
  MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
  dumpInfo.ExceptionPointers = pException;
  dumpInfo.ThreadId = GetCurrentThreadId();
  dumpInfo.ClientPointers = TRUE;

  // 写入Dump文件内容;
  MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile,
                    MiniDumpNormal, &dumpInfo, NULL, NULL);
  CloseHandle(hDumpFile);
}

std::string MiniDump::GetCurrentDateTime() {
  std::time_t timep;
  std::time(&timep);
  char tmp[64];
  strftime(tmp, sizeof(tmp), "%Y-%m-%d %H-%M-%S", std::localtime(&timep));
  return tmp;
}

std::string MiniDump::GetminiDumpDir() {
  std::string dump_dir = fs::current_path().string().append("/minidump");
  fs::path dump_path_dir(dump_dir);
  try {
    if (!fs::exists(dump_path_dir)) {
      fs::create_directories(dump_path_dir);
    }
  } catch (const fs::filesystem_error &ex) {
    std::cout << "path create failed: " << ex.what()
              << " path is: " << dump_path_dir << std::endl;
  }
  return dump_dir;
}