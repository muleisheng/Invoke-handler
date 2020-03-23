/*******************************************************************
* a Template is as MFC message. It can replace complicated 
* switch case or if else (if) logic blocks.
********************************************************************/
#ifndef _EVENTDISPATCHER_H_
#define _EVENTDISPATCHER_H_

#include <vector>
using namespace std;

template<typename C>
class CEventDispatcher {
 private:
  typedef void (C::*FunWithNoPar)();
  typedef void (C::*FunWith1Para)(void *p1);
  typedef void (C::*FunWith2Paras)(void *p1, void *p2);
  enum FunType{
    FunType_UnKnown,
    FunType_NoParam,
    FunType_With1Para,
    FunType_With2Para,
  };
  union FunEntry{
    FunWith2Paras   pFunWith2Paras;
    FunWith1Para    pFunWith1Para;
    FunWithNoPar    pFunWithNoPar;
  };
    
  struct ActiveEntry{
    int nAction;
    int nCtrlID;
    FunType funType;
    FunEntry funEntry;
  };
    
 public:
  CEventDispatcher(C *pObj):m_pObj(pObj){};
  virtual ~CEventDispatcher(){};
 public:
  template<typename Fun>
  void RegisterAction(const int &ctrlid, const int &action,Fun funEntry){
    typedef int ERROR_NoFun[-1];
  }
  template<>
  void RegisterAction<FunWithNoPar>(const int &ctrlid, const int &action, FunWithNoPar funEntry){
    ActiveEntry activeEntry;
    activeEntry.nAction = action;
    activeEntry.nCtrlID = ctrlid;
    if(typeid(FunWithNoPar) == typeid(funEntry)){
      activeEntry.funEntry.pFunWithNoPar = funEntry;
      activeEntry.funType = FunType_NoParam;
    }
    activeEntries.push_back(activeEntry);
  }
  template<>
  void RegisterAction<FunWith1Para>(const int &ctrlid, const int &action, FunWith1Para funEntry){
    ActiveEntry activeEntry;
    activeEntry.nAction = action;
    activeEntry.nCtrlID = ctrlid;
    if(typeid(FunWith1Para) == typeid(funEntry)){
      activeEntry.funEntry.pFunWith1Para = funEntry;
      activeEntry.funType = FunType_With1Para;
    }
    activeEntries.push_back(activeEntry);
  }
  template<>
  void RegisterAction<FunWith2Paras>(const int &ctrlid, const int &action, FunWith2Paras funEntry){
    ActiveEntry activeEntry;
    activeEntry.nAction = action;
    activeEntry.nCtrlID = ctrlid;
    if(typeid(FunWith2Paras) == typeid(funEntry)){
      activeEntry.funEntry.pFunWith2Paras = funEntry;
      activeEntry.funType = FunType_With2Para;
    }
    activeEntries.push_back(activeEntry);
  }
  bool dispatch(int ctrlid,int action,...){
    if(!m_pObj)
      return false;
    for(size_t i = 0; i< activeEntries.size(); i++){
      if(activeEntries[i].nCtrlID == ctrlid 
        && activeEntries[i].nAction == action){
        if(activeEntries[i].funType == FunType_NoParam){
          (m_pObj->*(activeEntries[i].funEntry.pFunWithNoPar))();
          return true;
        } else if(activeEntries[i].funType == FunType_With1Para){
          va_list   argptr;
          va_start(argptr, action);
          void *p1 = va_arg(argptr, void*);
          va_end(argptr);
          (m_pObj->*(activeEntries[i].funEntry.pFunWith1Para))(p1);
          return true;
        } else if(activeEntries[i].funType == FunType_With2Para){
          va_list   argptr;
          va_start(argptr, action);
          void *p1 = va_arg(argptr, void*);
          void *p2 = va_arg(argptr, void*);
          va_end(argptr);
          (m_pObj->*(activeEntries[i].funEntry.pFunWith2Paras))(p1,p2);
          return true;
        } else
          return false;
        break;
      }
    }
    return false;
  };
 protected:
  vector<ActiveEntry> activeEntries;
  C *m_pObj;
};
#endif
