
#include <windows.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

const unsigned short BufLen = 10;	//缓冲区长度
const unsigned short ProducerCnt = 2;	//生产者个数
const unsigned short ConsumerCnt = 3;	//消费者个数
const unsigned sleepTime = 1000;	//每次输出后的休眠时间

int produnctId1 = 1000, produnctId2 = 2000;	//2个生产者的生产产品起始号
int consumeId;	//消费的产品好
int buf[BufLen];	//缓冲区
int pIdx = 0, cIdx = 0;	//分别记录生产的序号和消费的序号
bool goon = true;	//用于退出的标志
HANDLE sFull, sEmpty;	//信号量
CRITICAL_SECTION cs;	//临界区

void produce(int no);
void consume();
DWORD WINAPI producer(LPVOID lpPara);
DWORD WINAPI consumer(LPVOID lpPara);

//生产者
DWORD WINAPI producer(LPVOID lpPara) {
	int no = int(lpPara);	//生产者编号
	while (goon) {
		auto step = rand() % 900 + 100;
		WaitForSingleObject(sEmpty, INFINITE);	//要求缓冲区有有空位
		//INFINITE:对象被触发信号后函数才会返回
		EnterCriticalSection(&cs);		//互斥使用缓冲区	
		Sleep(step);
		produce(no);
		LeaveCriticalSection(&cs);
		ReleaseSemaphore(sFull, 1, NULL);	//缓冲区数据个数+1
	}
	return 0;
}

//消费者
DWORD WINAPI consumer(LPVOID lpPara) {
	while (goon) {
		auto step = rand() % 900 + 100;
		WaitForSingleObject(sFull, INFINITE);
		EnterCriticalSection(&cs);
		Sleep(step);
		consume();
		LeaveCriticalSection(&cs);
		ReleaseSemaphore(sEmpty, 1, NULL);
	}
	return 0;
}

//生产
void produce(int no) {
	auto produnctId = no == 0 ? produnctId1++ : produnctId2++;
	cout << "生产产品: " << ++produnctId << endl;
	cout << "将产品放入缓冲区\n";
	buf[pIdx] = produnctId;
	cout << "缓冲区状态:";
	for (int i = 0; i < BufLen; ++i) {
		if (buf[i] != 0) cout << " (" << i + 1 << ')' << buf[i];
		else cout << " (" << i + 1 << ')' << "NULL";
		if (i == pIdx) cout << "<-生产";
	}
	cout << endl << endl;
	pIdx = (pIdx + 1) % BufLen;
	Sleep(sleepTime);
}

//消费
void consume() {
	cout << "从缓冲区取出产品\n";
	consumeId = buf[cIdx];
	cout << "缓冲区状态:";
	for (int i = 0; i < BufLen; ++i) {
		if (buf[i] != 0) cout << " (" << i + 1 << ')' << buf[i];
		else cout << " (" << i + 1 << ')' << "NULL";
		if (i == cIdx) cout << "<-消费";
	}
	cout << endl;
	buf[cIdx] = 0;
	cIdx = (cIdx + 1) % BufLen;
	cout << "消费产品: " << consumeId << endl << endl;
	Sleep(sleepTime);
}

int main() {
	srand(unsigned(time(nullptr)));
	//创建信号量和临界区
	sFull = CreateSemaphore(NULL, 0, BufLen, NULL);		//缓冲区数据个数
	sEmpty = CreateSemaphore(NULL, BufLen, BufLen, NULL);	//缓冲区空位个数
	//sMutex = CreateSemaphore(NULL, 1, 1, NULL);
	InitializeCriticalSection(&cs);
	//创建生产者线程
	HANDLE hThread[ProducerCnt+ConsumerCnt];
	DWORD producers[ProducerCnt], consumers[ConsumerCnt];
	for (int i = 0; i < ProducerCnt; ++i) {
		hThread[i] = CreateThread(NULL, 0, producer, LPVOID(i), 0, &producers[i]);
		if (!hThread[i]) return -1;
	}
	//创建消费者线程
	for (int i = 0; i < ConsumerCnt; ++i) {
		hThread[ProducerCnt + i] 
			= CreateThread(NULL, 0, consumer, NULL, 0, &consumers[i]);
		if (!hThread[ProducerCnt + i]) return -1;
	}
	//输入任意字符终止
	while (goon) {
		if (getchar()) goon = false;
	}
	return 0;
}

/**/