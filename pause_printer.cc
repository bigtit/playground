#include <windows.h>

#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <deque>

// enumerate all printers in host and add their name to ls
void enum_printers(std::vector<std::string>& pnls) {
  DWORD nd, ret;
  EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 1, NULL, 0, &nd, &ret);
  LPBYTE buf = new BYTE[nd];
  EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 1, buf, nd, &nd, &ret);

  PPRINTER_INFO_1 pi = (PPRINTER_INFO_1)buf;
  for (DWORD i = 0; i < ret; ++i) {
    pnls.push_back(pi->pName);
    ++pi;
  }
}

class task_t {
};

class job_t {
  enum job_stat { initialized, paused, resumed, canceled };
  HANDLE _p; // printer
  HANDLE _id; // job id
  job_stat _stat; // job status
public:
  job_t() = delete;
  job_t(const job_t&) = delete;
  job_t& operator=(const job_t&) = delete;
};

void test_thread(HANDLE lp) {
  std::cout << "hello my name is: " << lp << std::endl;
  PPRINTER_NOTIFY_INFO pni = NULL;
  unsigned short fs = JOB_NOTIFY_FIELD_STATUS;
  PRINTER_NOTIFY_OPTIONS_TYPE pnot;
  PRINTER_NOTIFY_OPTIONS pno;
  pnot.Type = JOB_NOTIFY_TYPE;
  pnot.Count = 1;
  pnot.pFields = &fs;
  pno.Version = 2;
  pno.Flags = PRINTER_NOTIFY_OPTIONS_REFRESH;
  pno.Count = 1;
  pno.pTypes = &pnot;

  HANDLE notify = FindFirstPrinterChangeNotification(lp, PRINTER_CHANGE_JOB /*&(~PRINTER_CHANGE_SET_JOB)*/, 0, &pno);
  DWORD reason, jid, jstat = 0;

  for (;;) {
    WaitForSingleObjectEx(notify, INFINITE, TRUE);
    FindNextPrinterChangeNotification(notify, &reason, &pno, (void**)&pni);
    // std::cout << "pni->Count: " << pni->Count << std::endl;
    // std::cout << "reason: " << reason;
    if (pni) {
      for (DWORD i = 0; i < pni->Count; ++i) {
        if (pni->aData[i].Type != JOB_NOTIFY_TYPE) continue;
        jid = pni->aData[i].Id;
        if (pni->aData[i].Field == JOB_NOTIFY_FIELD_STATUS) {
          jstat = pni->aData[i].NotifyData.adwData[0];
        }
      }
    }
    if (reason & PRINTER_CHANGE_ADD_JOB) {
      std::cout << "addjob\n";
      SetJob(lp, jid, 0, NULL, JOB_CONTROL_PAUSE);
    }
    if (reason & PRINTER_CHANGE_SET_JOB) {
      if (jstat == JOB_STATUS_PAUSED);
      if (jstat & JOB_STATUS_PRINTING) SetJob(lp, jid, 0, NULL, JOB_CONTROL_DELETE);
    }
    if (reason & PRINTER_CHANGE_DELETE_JOB);
  }
}

int main() {
  std::vector<std::string> ps;
  std::vector<std::thread> ts;
  std::vector<HANDLE> lps;
  enum_printers(ps);

  for (auto& p : ps) {
    HANDLE lp;
    OpenPrinter(const_cast<LPSTR>(p.c_str()), &lp, NULL);
    lps.push_back(lp);
    ts.push_back(std::thread(test_thread, lp));
  }
  for (auto& t : ts) t.join();
  for (auto lp : lps) ClosePrinter(lp);
  return 0;
}
