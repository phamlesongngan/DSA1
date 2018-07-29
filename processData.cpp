/*
* =========================================================================================
* Name        : processData.cpp
* Description : student code for Assignment 1 - Data structures and Algorithms - Spring 2018
* =========================================================================================
*/
#include "requestLib.h"
#include "dbLib.h"
#include <cmath>
#include <iomanip>

#define DBL_MAX 1.79769e+308
#define INT_MAX 2147483647
/// Initialize and Finalize any global data that you use in the program

bool isListCreated;
double longest;
time_t longeststop;
bool check = false;
double max;
char maxId[64];
char* nodeRec_x, nodeRec_y;

bool doubleCmp(double x, double y) {
	/*unsigned long long int a = (unsigned long long int)(x*(10e13));
	unsigned long long int b = (unsigned long long int)(y*(10e13));
	if(a <= b) return true;
	else return false;*/

	double diff = x - y;
	if (fabs(diff) < 1e-9) return true;
	else if (x < y)return true;
	else return false;
}

struct info {
	VRecord nodeFirst, nodeLast, nodeStop, nodeSecond;
	int size;
	double DistanceTraveled;
	time_t totalTime;
	time_t longestStopTime, tempTime, travelTime, stillTime;
	bool stop;
	char id[64];
	info(VRecord a) {
		strcpy(id, a.id);
		memcpy(&nodeFirst, &a, sizeof(a));
		memcpy(&nodeSecond, &a, sizeof(a));
		memcpy(&nodeLast, &a, sizeof(a));
		memcpy(&nodeStop, &a, sizeof(a));
		DistanceTraveled = totalTime = longestStopTime = tempTime = travelTime = stillTime = 0;
		stop = false;
		size = 1;
	}
	info(char _id[64]) {
		strcpy(id, _id);
		DistanceTraveled = totalTime = longestStopTime = 0;
		stop = false;
		size = 1;
	}
	info() {
		DistanceTraveled = totalTime = longestStopTime = 0;
		stop = false;
		size = 0; 
	}
	void update(VRecord a) {
		//memcpy(&nodePrevious, &nodeLast, sizeof(VRecord));

		size++;
		DistanceTraveled += distanceVR(nodeLast.y, nodeLast.x, a.y, a.x);
		totalTime = a.timestamp - nodeFirst.timestamp-nodeStop.timestamp;


		if (doubleCmp(distanceVR(nodeStop.y, nodeStop.x, a.y, a.x), 0.005)) {
			stillTime += a.timestamp - nodeLast.timestamp;
			tempTime = a.timestamp - nodeStop.timestamp;
			stop = true;
		}
		else {
			travelTime += a.timestamp - nodeLast.timestamp;
			tempTime = 0;
			memcpy(&nodeStop, &a, sizeof(VRecord));
		}
		if (tempTime > longestStopTime) {
			longestStopTime = tempTime;
		}
		memcpy(&nodeLast, &a, sizeof(VRecord));
	}

};
struct info;
int eqCmp(info &a, info &b) {
	int ret;
	ret = strcmp((a).id, (b).id);
	if (ret < 0)
		return -1;	//a < b
	else if (ret == 0)
		return 0;	//a = b
	else return 1;	//a > b
}
L1List<info>* List;

void LPV(info &a) { //thiet bi co hanh trinh dai nhat
	if (doubleCmp(longest, a.DistanceTraveled)) {
		longest = a.DistanceTraveled;
		strcpy(maxId, a.id);
	}
}
void SPV(info &a) { //thiet bi co hanh trinh ngan nhat
	if (doubleCmp(a.DistanceTraveled, longest)) {
		longest = a.DistanceTraveled;
		strcpy(maxId, a.id);
	}
}
void MST(info &a) {
	if (a.longestStopTime > longeststop) {
		longeststop = a.longestStopTime;
		strcpy(maxId, a.id);
	}
}
int num;
void CNS(info &a) {
	if (!a.stop) {
		::num++;
	}
}
int maxRec;
void MRV(info &a) { //thiet bi co so luong record nhieu nhat
	if (a.size > maxRec) {
		::maxRec = a.size;
		strcpy(maxId, a.id);
	}
}
void LRV(info &a) { //thiet bi co so luong record it nhat
	if (a.size <= maxRec) {
		::maxRec = a.size;
		strcpy(maxId, a.id);
	}
}
time_t longestTime;
void MTV(info &a) { //thiet bi co thoi gian di chuyen lau nhat
	if (a.nodeLast.timestamp - a.nodeFirst.timestamp > longestTime) {
		longestTime = a.nodeLast.timestamp - a.nodeFirst.timestamp;
		strcpy(maxId, a.id);
	}
}
double maxVel;
void MVV(info &a) { //thiet bi co van toc di chuyen tb nhanh nhat
	double v = a.DistanceTraveled / a.travelTime;
	maxVel = 0;
	if (a.travelTime == 0) {
		v = 0;
		strcpy(maxId, a.id);
	}
	else {
		if ((maxVel < v)) {
			maxVel = v;
			strcpy(maxId, a.id);
		}
	}
}
double dis;
int size;
void CAS(info &a) { //khoang cach trung binh khu thu thap du lieu tb
	dis += a.DistanceTraveled;
	//::size += a.size-1;
}

bool initVGlobalData(void** pGData) {

	return true;
}
void releaseVGlobalData(void* pGData) {
	// TODO: release the data if you finish using it
}
void print(info &a) {
	std::cout << a.id << "  " << a.longestStopTime << "\n";
}
void VFS(L1List<VRecord>& recList, char ID[16]) {
	L1Item<VRecord> *p = recList._pHead;
	L1Item<VRecord> *res = NULL;
	bool first = true;
	bool flagFind = false;
	VRecord temp;
	while (p) {
		if (strcmp(p->data.id, ID) == 0) {
			if (first) {
				temp = p->data;
				flagFind = true;
				first = false;
			}
			else {
				if (doubleCmp(distanceVR(p->data.y, p->data.x, temp.y, temp.x), 0.005)) {
					cout << "(" << temp.x << ", " << temp.y << ")" << endl;
					return;
				}
				else {
					temp = p->data;
				}
			}
		}
		p = p->pNext;
	}
	if (flagFind) cout << "non stop!" << endl;
	else cout << "not found!" << endl;
}

void VLS(L1List<VRecord>& recList, char ID[16]) {
	L1Item<VRecord> *p = recList._pHead;
	L1Item<VRecord> *res = NULL;
	bool first = true;
	bool flagFind = false;
	bool flagStop = false;
	VRecord temp;
	VRecord stopRecord;
	while (p) {
		if (strcmp(p->data.id, ID) == 0) {
			if (first) {
				temp = p->data;
				flagFind = true;
				first = false;
			}
			else {
				if (doubleCmp(distanceVR(p->data.y, p->data.x, temp.y, temp.x), 0.005)) {
					flagStop = true;
					stopRecord = temp;
				}
				else {
					temp = p->data;
				}
			}
		}
		p = p->pNext;
	}
	if (flagFind) {
		if (flagStop) {
			cout << "(" << stopRecord.x << ", " << stopRecord.y << ")" << endl;
		}
		else {
			cout << "non stop!" << endl;
		}
	}
	else cout << "not found!" << endl;
}
bool processRequest(VRequest& request, L1List<VRecord>& recList, void* pGData) {
	if (isListCreated == false) {
		List = new L1List<info>();
		VRecord currentRec;
		recList.initCur();
		VRecord temp;
		temp.timestamp = 0;
		temp.x = 0;
		temp.y = 0;
		while (recList.storeCurrent(currentRec)) {
			/*if(strcmp(currentRec.id, "51B13103") == 0 ){
			double a = distanceVR(temp.y, temp.x, currentRec.y, currentRec.x);
			std::cout << currentRec.id<<"	"<<currentRec.timestamp<<"	"<<currentRec.x<<"	"<<currentRec.y<<"	"<<currentRec.timestamp - temp.timestamp<<"	"<<a<<"\n";
			memcpy(&temp, &currentRec, sizeof(VRecord));
			}*/
			info tmp(currentRec);
			bool found;
			info *ret = List->_find(tmp, found, &eqCmp);
			if (found) ret->update(currentRec);
			else List->insertHead(tmp);
		}
		::isListCreated = true;
		//List->traverse(&print);
	}
	char ID[64];
	char rqCode[64];
	int j = 0;
	strncpy(rqCode, request.code, 3);
	rqCode[3] = '\0';
	info ret;
	char des[128];

	if (strlen(request.code) > 3) {
		strncpy(ID, request.code + 3, sizeof(request.code));
		info doit(ID);
		List->find(doit, &ret, &eqCmp);
	}
	
	if (strcmp(rqCode, "CNV") == 0 && strlen(request.code) ==3) {
		cout << "CNV: " << List->getSize() << "\n";
	}
	else if (strcmp(rqCode, "VFF") == 0 && strlen(request.code) == 3) {
		//cout << "VFF: " << recList.at(0).id << "\n";
		cout << "VFF: " << List->at(List->getSize()-1).id << "\n";
	}
	else if (strcmp(rqCode, "VFL") == 0 && strlen(request.code) == 3) {
		cout << "VFL: " << List->at(0).id << "\n";
	}
	else if (strcmp(rqCode, "VFY") == 0) {
		cout << "VFY" << ID << ": ";
		info VFY(ID);
		strcpy(ret.id, ID);
		if (List->find(VFY, &ret, &eqCmp))
			 cout << setprecision(6) << ret.nodeFirst.y << "\n";
		else
			cout << "not found!\n";
	}
	else if (strcmp(rqCode, "VFX") == 0) {
		cout << "VFX" << ID << ": ";
		info VFX(ID);
		strcpy(ret.id, ID);
		if (List->find(VFX, &ret, &eqCmp)) {
			cout << ret.nodeFirst.x << "\n";
		}
		else {
			cout << "not found!\n";
		}
	}
	else if (strcmp(rqCode, "VLY") == 0) {
		cout << "VLY" << ID << ": ";
		info VLY(ID);
		strcpy(ret.id, ID);
		if (List->find(VLY, &ret, &eqCmp))
			cout << ret.nodeLast.y << "\n";
		else
			cout << "not found!\n";
	}
	else if (strcmp(rqCode, "VLX") == 0) {
		cout << "VLX" << ID << ": ";
		info VLX(ID);
		strcpy(ret.id, ID);
		if (List->find(VLX, &ret, &eqCmp))
			cout << ret.nodeLast.x << "\n";
		else
			cout << "not found!\n";
	}
	else if (strcmp(rqCode, "VFT") == 0) {
		cout << "VFT" << ID << ": ";
		info VFT(ID);
		strcpy(ret.id, ID);
		if (List->find(VFT, &ret, &eqCmp)) {
			strPrintTime(des, ret.nodeFirst.timestamp);
			cout << des << "\n";
		}
		else
			cout << "not found!\n";
	}
	else if (strcmp(rqCode, "VLT") == 0) {
		cout << "VLT" << ID << ": ";
		info VLT(ID);
		strcpy(ret.id, ID);
		if (List->find(VLT, &ret, &eqCmp)) {
			strPrintTime(des, ret.nodeLast.timestamp);
			cout << des << "\n";
		}
		else
			cout << "not found!\n";
	}
	else if (strcmp(rqCode, "VCR") == 0) {
		if (ret.size == 0) {
			cout << "VCR" << ID << ": " << "not found!\n";
		}
		else {
			cout << "VCR" << ID << ": " << ret.size << "\n";
		}
	}
	else if (strcmp(rqCode, "VCL") == 0) {
		cout << "VCL" << ID << ": ";
		info VCL(ID);
		strcpy(ret.id, ID);
		if (List->find(VCL, &ret, &eqCmp))
			cout << ret.DistanceTraveled << "\n";
		else
			cout << "not found!\n";
		
	}
	else if (strcmp(rqCode, "VMT") == 0) {
		cout << "VMT" << ID << ": ";
		info VMT(ID);
		strcpy(ret.id, ID);
		if (List->find(VMT, &ret, &eqCmp))
			cout << ret.travelTime << "\n";
		else
			cout << "not found!\n";
	}
	else if (strcmp(rqCode, "VFS") == 0) {
		cout << "VFS" << ID << ": ";
		VFS(recList, ID);
	}
	else if (strcmp(rqCode, "VLS") == 0) {
		cout << "VLS" << ID << ": ";
		VLS(recList, ID);
	}
	else if (strcmp(rqCode, "VMS") == 0) {
		if (ret.stop)
			cout << "VMS" << ID << ": " << ret.longestStopTime << "\n";
		else cout << "VMS" << ID << ": non stop!\n";
	}
	else if (strcmp(rqCode, "VAS") == 0) {
		if (ret.size == 1 ) {
			cout << "VAS" << ID << ": " << 0 << "\n";
		}
		else if (ret.size == 0) {
			cout << "VAS" << ID << ": " << "not found!\n";
		}
		else {
			cout << "VAS" << ID << ": " << ret.DistanceTraveled / (ret.size - 1) * 1000 << " meter\n";
		}
	}
	else if (strcmp(rqCode, "MST") == 0 && strlen(request.code) == 3) {
		::longeststop = 0;
		List->traverse(&MST);
		cout << "MST" << ": " << longeststop << "s\n";
	}
	else if (strcmp(rqCode, "CNR") == 0 && strlen(request.code) == 3) {
		cout << "CNR: " << recList.getSize() << "\n";
	}
	else if (strcmp(rqCode, "MRV") == 0 && strlen(request.code) == 3) {
		::maxRec = 0;
		List->traverse(&MRV);
		cout << "MRV: " << maxId << "\n";
	}
	else if (strcmp(rqCode, "LRV") == 0 && strlen(request.code) == 3) {
		::maxRec = INT_MAX;
		List->traverse(&LRV);
		cout << "LRV: " << maxId << "\n";
	}
	else if (strcmp(rqCode, "MTV") == 0 && strlen(request.code) == 3) {
		::longestTime = 0;
		List->traverse(&MTV);
		cout << "MTV: " << maxId << "\n";
	}
	else if (strcmp(rqCode, "MVV") == 0 && strlen(request.code) == 3) {
		maxVel = 0;
		List->traverse(&MVV);
		cout << "MVV: " << maxId << "\n";
	}
	else if (strcmp(rqCode, "CNS") == 0 && strlen(request.code) == 3) {
		::num = 0;
		List->traverse(&CNS);
		cout << "CNS: " << ::num << "\n";
	}
	else if (strcmp(rqCode, "CAS") == 0 && strlen(request.code) == 3) {
		dis = 0;
		List->traverse(&CAS);
		cout << "CAS: " << dis / (recList.getSize() - List->getSize()) * 1000 << " meter\n";
	}
	else if (strcmp(rqCode, "LPV") == 0 && strlen(request.code) == 3) {
		longest = 0;
		List->traverse(&LPV);
		cout << "LPV: " << maxId << "\n";
	}
	else if (strcmp(rqCode, "SPV") == 0 && strlen(request.code) == 3) {
		longest = DBL_MAX;
		List->traverse(&SPV);
		cout << "SPV: " << maxId << "\n";
	}
	else if (strcmp(rqCode, "RVR") == 0) {
		cout << "RVR" << ID << ": ";
		info RVR(ID);
		strcpy(ret.id, ID);
		if (!List->find(RVR, &ret, &eqCmp)) {
			cout << "not found!\n";
		}
		else{
			List->_remove1(RVR, &eqCmp);
			cout << "success!\n";
		}
		while (strcmp(recList._pHead->data.id, ID) == 0) {
			recList._pHead = recList._pHead->pNext;
			recList._size--;
		}
		if (recList._pHead->pNext == NULL) return true;
		L1Item<VRecord> *p = recList._pHead, *pN = p->pNext;
		while (pN) {
			if (strcmp(pN->data.id, ID) == 0) {
				p->pNext = pN->pNext; 
				pN = pN->pNext;
				recList._size--;
			}
			else {
				p = pN;
				pN = pN->pNext;
			}
		}
	}
	else { 
		cout << request.code <<": " << "not found!\n";
	}
	return true;
}