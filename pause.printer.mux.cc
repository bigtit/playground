#include <windows.h>

#include <vector>
#include <string>
#include <iostream>
#include <thread>

struct jjob {
  DWORD id;
  DWORD stat;
};

struct printer {
  HANDLE hdl;
  std::string name;
};

// enumerate all printers in host and add their name to ls
auto enum_printers2() {
  DWORD nd, ret;
  std::vector<struct printer> pls;
  EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 1, NULL, 0, &nd, &ret);
  LPBYTE buf = new BYTE[nd];
  EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 1, buf, nd, &nd, &ret);

  PPRINTER_INFO_1 pi = (PPRINTER_INFO_1)buf;
  for (DWORD i = 0; i < ret; ++i) {
    pls.push_back({ (HANDLE)0, pi->pName });
    ++pi;
  }
  return std::move(pls);
}

void mux_thread(const std::vector<struct printer>& pls) {
  for (auto& p : pls) std::cout << "hello: " << p.name << std::endl;
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
  ///////////////////////////
  // std::vector<HANDLE> nots;
  auto ps = pls.size();
  auto nots = new HANDLE[ps];
  for (size_t i = 0; i < ps; ++i) {
    // nots.push_back(FindFirstPrinterChangeNotification(p.hdl, PRINTER_CHANGE_JOB, 0, &pno));
    nots[i] = FindFirstPrinterChangeNotification(pls[i].hdl, PRINTER_CHANGE_JOB, 0, &pno);
  }
  // std::vector<jjob> jj;

  for (;;) {
    DWORD reason, jid, jstat = 0;
    std::vector<jjob> jj;
    // WaitForSingleObjectEx(notify, INFINITE, TRUE);
    DWORD idx = WaitForMultipleObjectsEx(ps, nots, FALSE, INFINITE, TRUE);
    HANDLE notify = nots[idx];
    FindNextPrinterChangeNotification(notify, &reason, &pno, (void**)&pni);
    // std::cout << "pni->Count: " << pni->Count << std::endl;
    // std::cout << "reason: " << reason;
    if (pni) {
      for (DWORD i = 0; i < pni->Count; ++i) {
        if (pni->aData[i].Type != JOB_NOTIFY_TYPE) continue;
        jid = pni->aData[i].Id;
        if (pni->aData[i].Field == JOB_NOTIFY_FIELD_STATUS) {
          jstat = pni->aData[i].NotifyData.adwData[0];
          jj.push_back({ jid, jstat });
        }
      }
    }

    if (reason & PRINTER_CHANGE_ADD_JOB) {
      std::cout << pls[idx].name << ": addjob\n";
      SetJob(pls[idx].hdl, jid, 0, NULL, JOB_CONTROL_PAUSE);
      // dealing with job adding
    }
    if (reason & PRINTER_CHANGE_DELETE_JOB) {
      std::cout << pls[idx].name << ": deletejob\n";
    }
    if (pni && reason & PRINTER_CHANGE_SET_JOB) {
      for (const auto& j : jj) {
        // std::cout << "jid = " << j.id << ", jstat = " << j.stat << ", deleted = " << j.deleted << std::endl;
        if (j.stat == JOB_STATUS_PAUSED);
        // when printing, set it to JOB_CONTROL_PAUSE cannot pause the job
        // but we can delete it directly
        if ((j.stat & JOB_STATUS_PRINTING || j.stat == 0)) {
          // too late to pause when printing
          // std::cout << "Forbidden operation\n";
          if (SetJob(pls[idx].hdl, j.id, 0, NULL, JOB_CONTROL_DELETE)) {
            std::cout << "deleted jobid = " << j.id << std::endl;
            MessageBox(0, "forbidden operation, delete current job", "error", 0);
            // dealing with job deletion
          }
        }
      }
    }
  }
}

int main() {
  auto pls = enum_printers2();
  for (auto& p : pls) OpenPrinter(const_cast<LPSTR>(p.name.c_str()), &p.hdl, NULL);
  auto t = std::thread(mux_thread, pls);
  t.join();
  for (const auto& p : pls) ClosePrinter(p.hdl);
  return 0;
}
