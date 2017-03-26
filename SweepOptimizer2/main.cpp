﻿#include <cstdlib>
#include <iostream>
#include "problem.cpp"

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
		// 探索を行う

	}
	catch (char *str) {
		cout << "エラー：" << str << endl;
	}
}
