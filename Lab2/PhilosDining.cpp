
#include <windows.h>
#include <cstdio>
#include <ctime>
#include <cstdlib>
using namespace std;

const unsigned pCnt = 5;
const unsigned sleepTime = 800;

bool goon = true;
int pS[pCnt];
HANDLE s[pCnt];
unsigned diningCnt = 0;
CRITICAL_SECTION cs;

inline void beforeDining(int i, int k) {
	printf("哲学家%d 拿起%d号筷子 现有%d支筷子 ", i, k, ++pS[i]);
	if (pS[i] == 1) printf("不能进餐\n");
	else if (pS[i] == 2) printf("开始进餐\n");
}

inline void afterDining(int i, int k) {
	printf("哲学家%d 放下%d号筷子\n", i, k);
	--pS[i];
}

inline void dining(int i) {
	printf("哲学家%i 就餐\n", i);
	printf("[0号:%d] [1号:%d] [2号:%d] [3号:%d] [4号:%d]\n",
		pS[0],pS[1],pS[2],pS[3], pS[4]);
	Sleep(sleepTime);

}

DWORD WINAPI philosopher(LPVOID lpPara) {
	int i = int(lpPara);
	while (goon) {
		auto step = rand() % 400 + 100;
		Sleep(step);
		//判断当前拿筷子的人数, 若达到"总数-1"则不允许拿筷子
		if (diningCnt == pCnt - 1) continue;

		WaitForSingleObject(s[i], INFINITE);	//等待左侧筷子可用

		EnterCriticalSection(&cs);
		++diningCnt;	//拿筷子人数+1
		LeaveCriticalSection(&cs);

		beforeDining(i, i);
		WaitForSingleObject(s[(i + pCnt - 1) % pCnt], INFINITE);	//等待右侧筷子可用
		beforeDining(i, (i + pCnt - 1) % pCnt);
		dining(i);
		ReleaseSemaphore(s[(i + pCnt - 1) % pCnt], 1, NULL);	//放下右侧筷子
		afterDining(i, (i + pCnt - 1) % pCnt);
		ReleaseSemaphore(s[i], 1, NULL);	//放下左侧筷子
		afterDining(i, i);

		EnterCriticalSection(&cs);
		--diningCnt;	//拿筷子人数-1
		LeaveCriticalSection(&cs);
	}
	return 0;
}


int main() {
	srand(unsigned(time(nullptr)));
	for (int i = 0; i < pCnt; ++i) {
		s[i] = CreateSemaphore(NULL, 1, 1, NULL);
	}
	InitializeCriticalSection(&cs);
	HANDLE hThread[pCnt];
	DWORD phs[pCnt];
	for (int i = 0; i < pCnt; ++i) {
		hThread[i] = CreateThread(NULL, 0, philosopher, LPVOID(i), 0, &phs[i]);
		if (!hThread[i]) return -1;
	}
	while (goon) {
		if (getchar()) goon = false;
	}
	return 0;
}

/**/