/*******************************************************************
* like EventDispatcher.It can be as a mutation of EventDispatcher
********************************************************************/
#ifndef _INVOKEACTION_H_
#define _INVOKEACTION_H_

#include "QpFishClientDefine.h"
#include <vector>
using namespace std;

template<typename Function1>
class CInvokeAction;

template <typename C,typename M>
class CInvokeAction<void (C::*)(const M&)> {
private:
  typedef bool (M::*Has_msg)() const ;
  typedef void (C::*InvokeMsg)(const M&); 
  struct Entrance {
    Has_msg has_msg;
    InvokeMsg invoke;
  };
public:
  CInvokeAction(){};
  virtual ~CInvokeAction(){};
  bool Register(Has_msg hasmsg,InvokeMsg invoke){
    if(!hasmsg || !invoke)
      return false;
    Entrance ent;
    ent.has_msg = hasmsg;
    ent.invoke = invoke;
    m_Entrances.push_back(ent);
    return true;
  };
  bool Invoke(const M* pM,C *pC){
    if(!pM || !pC)
      return false;
    if(m_Entrances.size() == 0)
      return false;
    for(size_t i = 0; i< m_Entrances.size(); i++){
      if((pM->*(m_Entrances[i].has_msg))()){
        (pC->*(m_Entrances[i].invoke))(*pM);
        return true;
      }
    }
    return false;
  };
private:
  vector<Entrance> m_Entrances;
};

template <typename C,typename M>
class CInvokeAction<bool (C::*)(M *pobj,const INT64& currentTime,const hgeInputEvent& hgeEvent)> {
private:
  typedef bool (C::*action)(M *pobj,const INT64& currentTime,const hgeInputEvent& hgeEvent); 
  struct Entrance {
    int eventtype;
    int eventkey;
    action compareFun;
    action invokeFun;
    bool usecompare_fun;
    Entrance():eventtype(0)
      ,eventkey(0)
      ,compareFun(nullptr)
      ,invokeFun(nullptr)
      ,usecompare_fun(false){};
    };
public:
  CInvokeAction(){};
  virtual ~CInvokeAction(){};
  bool Register(action compare,action invoke){
    if(!compare || !invoke)
      return false;
    Entrance ent;
    ent.compareFun = compare;
    ent.invokeFun = invoke;
    ent.usecompare_fun = true;
    m_Entrances.push_back(ent);
    return true;
  };
  bool Register(int evetype,int evekey,action invoke){
    if(!invoke)
      return false;
    Entrance ent;
    ent.eventtype = evetype;
    ent.eventkey = evekey;
    ent.usecompare_fun = false;
    ent.invokeFun = invoke;
    m_Entrances.push_back(ent);
    return true;
  };

  bool Invoke(M* pM,C *pC,const INT64& currentTime,const hgeInputEvent& hgeEvent){
    if(!pC||m_Entrances.size() == 0)
      return false;
    for(size_t i = 0; i< m_Entrances.size(); i++){
      if(m_Entrances[i].usecompare_fun
        && (m_Entrances[i].compareFun)
        && (pC->*(m_Entrances[i].compareFun))(pM,currentTime,hgeEvent)) 
	  return (pC->*(m_Entrances[i].invokeFun))(pM,currentTime,hgeEvent);
      else if(!(m_Entrances[i].usecompare_fun) 
        && (m_Entrances[i].eventtype == hgeEvent.type)
        &&(m_Entrances[i].eventkey == hgeEvent.key))
          return (pC->*(m_Entrances[i].invokeFun))(pM,currentTime,hgeEvent);
    }
    return false;
  };
private:
  vector<Entrance> m_Entrances;
};

#endif
