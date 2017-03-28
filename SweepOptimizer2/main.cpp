#include <cstdlib>
#include <iostream>
#include <chrono>
#include "bitboard.h"
#include "problem.h"

// using宣言
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
	// コマンドライン引数処理
	if (argc < 2)
		return EXIT_FAILURE;
	// ビット演算用にテーブルを準備する
	initialize();
	// 問題を読み込み、計算を行う
	try {
		// ファイルを読み込む
		Problem problem(argv[1]);
		problem.put();
		// 探索を行う
		auto start_time = std::chrono::high_resolution_clock::now();
		problem.solve();
		auto end_time = std::chrono::high_resolution_clock::now();
		// 解を表示する
		problem.show_answer();
		auto duaration = std::chrono::duration_cast< std::chrono::milliseconds >(end_time - start_time).count();
		cout << duaration << "[ms]" << endl;
	}
	catch (char *str) {
		cout << "エラー：" << str << endl;
	}
}
