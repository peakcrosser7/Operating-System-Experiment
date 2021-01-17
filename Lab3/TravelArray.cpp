
#include <iostream>
#include <ctime>
using namespace std;

const int times = 1;	//倍数
const int row = 2048 * times;	//行数
const int col = 1024 * times;	//列数
int myArray[row][col];	//大数组

void travel1();
void travel2();

int main() {
	auto s = clock();
	travel2();
	auto e = clock();
	cout << "Cost " << e - s << "ms\n";	//遍历耗时
	return 0;
}

//先行后列遍历数组
void travel1() {
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			myArray[i][j] = 0;
		}
	}
}
//先列后行遍历数组
void travel2() {
	for (int i = 0; i < col; ++i) {
		for (int j = 0; j < row; ++j) {
			myArray[j][i] = 0;
		}
	}
}


/**/