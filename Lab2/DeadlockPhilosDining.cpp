/*
#include <windows.h>
#include <cstdio>
#include <ctime>
#include <cstdlib>
using namespace std;

const unsigned pCnt = 5;	//哲学家数量
const unsigned sleepTime = 800;		//每次输出后的休眠时间

bool goon = true;	//用于退出的标志
int pS[pCnt];	//每个哲学家的筷子(0:思考,1:一只筷子,2:进餐)
HANDLE s[pCnt];	//每根筷子对应的信号量

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
		pS[0], pS[1], pS[2], pS[3], pS[4]);
	Sleep(sleepTime);
}

DWORD WINAPI philosopher(LPVOID lpPara) {
	int i = int(lpPara);
	while (goon) {
		auto step = rand() % 400 + 100;
		Sleep(step);
		WaitForSingleObject(s[i], INFINITE);	//等待左侧筷子可用
		beforeDining(i, i);
		WaitForSingleObject(s[(i + pCnt - 1) % pCnt], INFINITE);	//等待右侧筷子可用
		beforeDining(i, (i + pCnt - 1) % pCnt);
		dining(i);
		ReleaseSemaphore(s[(i + pCnt - 1) % pCnt], 1, NULL);	//放下右侧筷子
		afterDining(i, (i + pCnt - 1) % pCnt);
		ReleaseSemaphore(s[i], 1, NULL);	//放下左侧筷子
		afterDining(i, i);
	}
	return 0;
}


int main() {
	srand(unsigned(time(nullptr)));
	for (int i = 0; i < pCnt; ++i) {
		s[i] = CreateSemaphore(NULL, 1, 1, NULL);
	}
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