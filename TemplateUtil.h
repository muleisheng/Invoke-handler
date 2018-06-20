#ifndef _TEMPLATEUTIL_H_
#define _TEMPLATEUTIL_H_

#include "QpFishClientDefine.h"
#include <vector>
using namespace std;

enum TU_RC{
	TU_RC_OK,
	TU_RC_FAILED,
	TU_RC_ALREADY_EXISTED,
};

template<typename T,typename C, typename A=vector<T>>
class CTemplateUtil{
public:
template<typename R, typename Z>
static bool Compare(const R &a,const Z &b){
	return a.compare(b);
};
template<>
static bool Compare<int,int>(const int &a,const int &b){
	return (a == b);
}
template<>
static bool Compare<unsigned long,unsigned long>(const unsigned long &a,const unsigned long &b){
	return (a == b);
}

template<>
static bool Compare<string, string>(const string &a,const string &b){
	return (a.compare(b) == 0)?true:false;
}

typedef bool (*pFuncWithReturnBool)(const T &a,const C &b);
typedef bool (*pFunc2WithReturnBool)(const T &a,const C &b,const C &c);
typedef void (*pFuncWithNoReturn)(T &a,const C &b);

static int GetFirstIndex(const A &vec,const T &data,pFuncWithReturnBool pCompare){
	int index = -1;
	A::const_iterator itor = vec.begin();
	int i = 0;
	while(itor != vec.end()){
		if(pCompare((*itor),data)){
			index = i;
			break;
		}
		itor++;
		i++;
	}
	return index;
}
static T *FindItem(A &vec,const C &data,pFuncWithReturnBool pCompare){
	T *p = NULL;
	A::iterator itor = vec.begin();
	while(itor != vec.end()){
		if(pCompare((*itor),data)){
			p = &(*itor);
			break;
		}
		itor++;
	}
	return p;
}

static T *FindItem(A &vec,const C &data1,const C &data2,pFunc2WithReturnBool pCompare){
	T *p = NULL;
	A::iterator itor = vec.begin();
	while(itor != vec.end()){
		if(pCompare((*itor),data1,data2)){
			p = &(*itor);
			break;
		}
		itor++;
	}
	return p;
}

static void RemoveItem(A &vec,const C &data,pFuncWithReturnBool pCompare){
	A::iterator itor = vec.begin();
	while(itor != vec.end()){
		if(pCompare((*itor),data)){
			vec.erase(itor);
			break;
		}
		itor++;
	}
};

static TU_RC PushItem(A &vec,const C &data, T item,pFuncWithReturnBool pCompare){
	TU_RC rc = TU_RC_OK;
	A::iterator itor = vec.begin();
	while(itor != vec.end()){
		if(pCompare((*itor),data)){
			rc = TU_RC_ALREADY_EXISTED;
			break;
		}
		itor++;
	}
	if(rc != TU_RC_ALREADY_EXISTED){
		vec.push_back(item);
		rc = TU_RC_OK;
	}
	return rc;
}
static void DoSomeThingOnItems(A &vec,const C &data,pFuncWithNoReturn pDoSomeThing){
	A::iterator itor = vec.begin();
	while(itor != vec.end()){
		pDoSomeThing(*itor,data);
		itor++;
	}
};

static void DoSomeThingOnItems(A &vec,const C &CompareDat,const C &DoSomeThingDat,pFuncWithReturnBool pCompare,pFuncWithNoReturn pDoSomeThing){
	A::iterator itor = vec.begin();
	while(itor != vec.end()){
		if(pCompare((*itor),CompareDat)){
			pDoSomeThing(*itor,DoSomeThingDat);
			break;
		}
		itor++;
	}
};
private:
	DISALLOW_COPY_AND_ASSIGN(CTemplateUtil);
};

template <typename T,typename C, size_t count>
class CTemplateArray
{
public:
	typedef bool (*pFuncWithReturnBool)(const T &a,const C &b);
	static const T *FindItem(const T arry[],const C &data,pFuncWithReturnBool pCompare){
		const T *p = NULL;
		for(size_t i = 0; i< count; i++){
			if(pCompare(arry[i],data)){
				p = &arry[i];
				break;
			}
		}
		return p;
	};
private:
	DISALLOW_COPY_AND_ASSIGN(CTemplateArray);
};

template<typename T>
void ClearVector(vector<T*> &vec){
	vector<T*>::iterator itor = vec.begin();
	while(itor != vec.end()){
		T* pTemp = *itor;
		if(pTemp)
			delete pTemp;
		pTemp = NULL;
		itor = vec.erase(itor);
	}
};
#endif