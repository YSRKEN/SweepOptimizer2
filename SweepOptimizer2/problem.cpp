#include <iostream>

using std::cout;
using std::endl;

class Problem {
public:
	// コンストラクタ
	Problem(const char file_name[]) {
		cout << file_name << endl;
	}

	// 中身を表示する
	void put() const noexcept{
		cout << "【問題ファイル】" << endl;
	}
};
