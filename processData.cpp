/*
 * =========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 1 - Data structures and Algorithms - Spring 2018
 * =========================================================================================
 */
#include "requestLib.h"
#include "dbLib.h"

 /// Initialize and Finalize any global data that you use in the program
#define     cmdList_N  27

string *List;
char requestCode[4];
char requestID[16];
char result[40];

struct node {
	char	*ID;
	double	xStop, yStop, xStopLast, yStopLast, xFirst, yFirst, xLast, yLast;
	double	length, averageLength, averageSpeed;
	time_t	stampFirst, stampLast;
	unsigned int		totalTime, totalStopTime, maxStop;
	unsigned int		tmp_timeStop;
	size_t  count;


	node(VRecord data) {
		count = 1;
		ID = new char;
		strcpy(ID, data.id);
		xFirst = xLast = data.x;
		yFirst = yLast = data.y;
		xStopLast = yStopLast = 0;
		stampFirst = stampLast = data.timestamp;
		length = averageLength = 0;
		totalTime = totalStopTime = maxStop = tmp_timeStop = 0;
	}

	~node() {
		xFirst = yFirst = xLast = yLast = length = count = 0;
		stampFirst = stampLast = 0;
		delete ID;
		totalTime = totalStopTime = maxStop = tmp_timeStop = 0;
	}

	void distanceCal(double inp_x, double inp_y, time_t inp_stamp) {
		int tmp_time = inp_stamp - stampLast;
		stampLast = inp_stamp;
		do {
			totalTime = totalTime + tmp_time;
		} while (inp_x != xLast || inp_y != yLast);
		double tmp_length = distanceVR(yLast, xLast, inp_y, inp_x);
		length = length + tmp_length;
		count++;
		averageLength = length / (count - 1);
		double calStop;
		calStop = xStop + yStop;
		if ((tmp_length <= 0.005 && calStop == 0) || distanceVR(yStop, xStop, inp_y, inp_x) <= 0.005 &&calStop != 0) {
			if (calStop == 0) {
				xStop = xLast;
				yStop = yLast;
			}
			tmp_timeStop = 0;
		}
		else if (distanceVR(yStop, xStop, inp_y, inp_x) > 0.005 &&calStop != 0) {
			xStopLast = xStop;
			yStopLast = yStop;
			xStop = yStop = 0;
			totalStopTime = totalStopTime + tmp_timeStop;
			if (maxStop < tmp_timeStop)
				maxStop = tmp_timeStop;
			tmp_timeStop = 0;
		}
		averageSpeed = length / (totalTime - totalStopTime);
		xLast = inp_x;
		yLast = inp_y;
	}
};

char* id;
L1List<node> *pList;
L1Item<node> *pItem;

double Devices_totalLength;
unsigned int DeviceRecord;

bool isList = false;
unsigned int count;

bool findfirst(node& f, void* ID) {
	if (strcmp(f.ID, (char*)ID) == 0)
		return true;
	else return false;
}
void CreateList(L1Item<VRecord>*& list) {
	pItem = pList->traverse1(&findfirst, list->data.id);
	if (pItem != NULL)
		pItem->data.distanceCal(list->data.x, list->data.y, list->data.timestamp);
	else
		pList->insertHead(*(new node(list->data)));
}
bool Empty() { 
	return pItem = NULL; 
}

void MST(L1Item<node>*& list) {
	if (Empty() || pItem->data.maxStop < list->data.maxStop)
		pItem = list;
}
void MRV(L1Item<node>*& list) {
	if (Empty() || pItem->data.count < list->data.count)
		pItem = list;
}
void LRV(L1Item<node>*& list) {
	if (Empty() || pItem->data.count > list->data.count)
		pItem = list;
}
void LPV(L1Item<node>*& list) {
	if (Empty() || pItem->data.length < list->data.length)
		pItem = list;
}
void SPV(L1Item<node>*& list) {
	if (Empty() || pItem->data.length > list->data.length)
		pItem = list;
}
void CAS(L1Item<node>*& list) {
	if (list->data.length = 0)
		return;
	Devices_totalLength - Devices_totalLength + list->data.length;
	DeviceRecord = DeviceRecord + list->data.count - 1;
}
void MTV(L1Item<node>*& list) {
	double pItemSub, listSub;
	pItemSub = pItem->data.totalTime - pItem->data.totalStopTime;
	listSub = list->data.totalTime - list->data.totalStopTime;
	if (Empty() || pItemSub < listSub)
		pItem = list;
}
void MVV(L1Item<node>*& list) {
	if (Empty() || pItem->data.averageSpeed < list->data.averageSpeed)
		pItem = list;
}
void CNS(L1Item<node>*& list) {
	if (list->data.count == 1) {
		::count++;
	}
}
L1List<size_t> *pRVR;
L1List<size_t> * pItemRVR;
void RVR(VRecord& list, void* param, size_t &position) {
	
	if (strcmp(list.id, (char*)param) == 0)
		pRVR->insertHead(position);
}


bool initVGlobalData(void** pGData) {
	// TODO: allocate global data if you think it is necessary.
	/// pGData contains the address of a pointer. You should allocate data for the global data
	/// and then assign its address to *pGData
	*pGData = new string[27]{"CNR" ,"VFF" ,"VFL","VFY","VFX","VFT","VFS","VLX" ,"VLY","VLT" ,"VLS","VCL" ,"VMT","VCR","VMS","VAS" , "MRV" ,"LRV","CNS" ,"MST","CNV","MVV" ,"CAS" ,"LPV","SPV","MTV" ,"RVR" };
	return true;
}
void releaseVGlobalData(void* pGData) {
	// TODO: release the data if you finish using it
	List = (string*)pGData;
	delete[] List;
}

bool processRequest(VRequest& request, L1List<VRecord>& recList, void* pGData) {
	// TODO: Your code comes here

	/// NOTE: The output of the request will be printed on one line
	/// end by the end-line '\n' character.
	unsigned int i = 0;
	List = (string*)pGData;
	strncpy(requestCode, request.code, 3);
	if (request.code[3] != '\0')
		strncpy(requestID, request.code + 3, 16);
	
	for (i = 0; i < cmdList_N && !(requestCode == List[i]); i++);
	if (!isList) {
		pList = new L1List<node>();
		recList.traverse(&CreateList);
		isList = true;
	}

	if (i == 0) {
		cout << "CNR: " << recList.getSize() << "\n";
	}
	else if (i == 1) {
		cout << "VFF: ";
		printVRecord(recList.operator[](0));
	}
	else if (i == 2) {
		cout << "VFL: " << pList->operator[](0).ID << "\n";
	}
	else if (i == 3) {
		pItem = pList->traverse1(&findfirst, requestID);
		cout << "VFY" << requestID << ": " << pItem->data.yFirst << "\n";
	}
	else if (i == 4) {
		pItem = pList->traverse1(&findfirst, requestID);
		cout << "VFX" << requestID << ": " << pItem->data.xFirst << "\n";
	}
	else if (i == 5) {
		pItem = pList->traverse1(&findfirst, requestID);
		strPrintTime(result, pItem->data.stampFirst);
		cout << "VFT" << requestID << ": " << result << "\n";
	}
	else if (i == 6) {
		pItem = pList->traverse1(&findfirst, requestID);
		cout << "VFS" << requestID << ": (" << pItem->data.xFirst << " " << pItem->data.yFirst << ")\n";
	}
	else if (i == 7) {
		pItem = pList->traverse1(&findfirst, requestID);
		cout << "VLX" << requestID << ": " << pItem->data.xLast << "\n";
	}
	else if (i == 8) {
		pItem = pList->traverse1(&findfirst, requestID);
		cout << "VLY" << requestID << ": " << pItem->data.yLast << "\n";
	}
	else if (i == 9) {
		pItem = pList->traverse1(&findfirst, requestID);
		strPrintTime(result, pItem->data.stampLast);
		cout << "VLT" << requestID << ": " << result << "\n";
	}
	else if (i == 10) {
		pItem = pList->traverse1(&findfirst, requestID);
		cout << "VCL" << requestID << ": " << pItem->data.length << "\n";
	}
	else if (i == 11) {
		pItem = pList->traverse1(&findfirst, requestID);
		double pItemSub;
		pItemSub = pItem->data.totalTime - pItem->data.totalStopTime;
		cout << "VMT" << requestID << ": " << pItemSub << "\n";
	}
	else if (i == 12) {
		pItem = pList->traverse1(&findfirst, requestID);
		cout << "VCR" << requestID << ": " << pItem->data.count << "\n";
	}
	else if (i == 13) {
		pItem = pList->traverse1(&findfirst, requestID);
		cout << "VLS" << requestID << ": (" << pItem->data.xStopLast << " " << pItem->data.yStopLast << ")\n";
	}
	else if (i == 14) {
		pItem = pList->traverse1(&findfirst, requestID);
		cout << "VMS" << requestID << ": " << pItem->data.maxStop << "\n";
	}
	else if (i == 15) {
		pItem = pList->traverse1(&findfirst, requestID);
		cout << "VAS" << requestID << ": " << pItem->data.averageLength << "\n";
	}
	else if (i == 16) {
		pList->traverse(&MST);
		cout << "MST: " << pItem->data.maxStop << " " << pItem->data.ID << "\n";
	}
	else if (i == 17) {
		cout << "CNV: " << pList->getSize() << "\n";
	}
	else if (i == 18) {
		pList->traverse(&MRV);
		cout << "MRV: " << pItem->data.ID << "\n";
	}
	else if (i == 19) {
		pList->traverse(&LRV);
		cout << "LRV: " << pItem->data.ID << "\n";
	}
	else if (i == 20) {
		pList->traverse(&MTV);
		cout << "MTV: " << pItem->data.ID << "\n";
	}
	else if (i == 21) {
		pList->traverse(&MVV);
		cout << "MVV: " << pItem->data.ID << "\n";
	}
	else if (i == 22) {
		::count = 0;
		pList->traverse(&CNS);
		cout << "CNS: " << ::count << "\n";
	}
	else if (i == 23) {
		pList->traverse(&CAS);
		double deviceCal;
		deviceCal = Devices_totalLength / DeviceRecord;
		cout << "CAS: " << deviceCal << "\n";
	}
	else if (i == 24) {
		pList->traverse(&LPV);
		cout << "LPV: " << pItem->data.ID << "\n";
	}
	else if (i == 25) {
		pList->traverse(&SPV);
		cout << "SPV: " << pItem->data.ID << "\n";
	}
	else if (i == 26) {
		pRVR = new L1List<size_t>();
		recList.traverse(&RVR, requestID);
		cout << "RVR: ";
		if (pRVR->isEmpty()) {
			cout << "Fail \n";
		}
		if (pRVR->isEmpty() != NULL) {
			recList.remove((int)pRVR->operator[](0));
			pRVR->removeHead();
		}
		cout << "Sucess \n";
	}
	else return true;

}

