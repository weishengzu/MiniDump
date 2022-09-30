/*
 * @Author: shengzu@wei
 * @Date: 2022-09-30 16:17:22
 * @LastEditTime: 2022-09-30 17:56:28
 * @LastEditors: shengzu@wei
 * @Description:
 */
#pragma once

#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <windows.h>

#include <ctime>
#include <filesystem>
#include <iostream>
#include <string>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

#ifdef UNICODE
#define TSprintf wsprintf
#else
#define TSprintf sprintf
#endif

class MiniDump {
 private:
  MiniDump();
  ~MiniDump();

 public:
  // 程序崩溃时是否启动自动生成dump文件;
  // 只需要在main函数开始处调用该函数即可;
  static void EnableAutoDump(bool bEnable = true);

 private:
  static LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException);

  static void CreateDumpFile(LPCSTR lpstrDumpFilePathName,
                             EXCEPTION_POINTERS *pException);

  static std::string GetCurrentDateTime();

  static std::string GetminiDumpDir();
};