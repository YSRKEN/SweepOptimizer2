#pragma once
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include "bitboard.h"

using std::cout;
using std::endl;
using std::vector;

enum class StaffType;
enum class StaffTask;

class Problem{
	// 盤面サイズ
	size_t size_x_;
	size_t size_y_;
	// ある箇所に対して、次に進める箇所の一覧
	vector<vector<size_t>> point_next_;
	// 清掃員の位置
	vector<vector<size_t>> point_staff_;
	// 清掃員の最大歩数
	vector<vector<size_t>> walk_count_;
	// 清掃員の残タスクフラグ
	// 女の子ならリンゴ・ロボットならビンを持つとNonFreeになる
	vector<vector<StaffTask>> staff_task_;
	// 清掃員の特定歩数におけるメンバーのインデックス
	vector<vector<std::pair<size_t, size_t>>> walk_staff_list_;

	// 拭く必要がある箇所は1、それ以外は0を立てる
	// (まだ拭いていない箇所があれば非0、全て拭いていれば0)
	BitBoard floor_dirty_;
	// 水たまりが存在する箇所は1、それ以外は0を立てる
	BitBoard floor_pool_;
	// リンゴが存在する箇所は1、それ以外は0を立てる
	BitBoard floor_apple_;
	// ビンが存在する箇所は1、それ以外は0を立てる
	BitBoard floor_bottle_;
	// ゴミ箱に隣接する箇所は1、それ以外は0を立てる
	BitBoard floor_around_dust_;
	// リサイクル箱に隣接する箇所は1、それ以外は0を立てる
	BitBoard floor_around_recycle_;
public:
	// コンストラクタ
	Problem(const char file_name[]);
	// 内容を表示する
	void put() const noexcept;
	// 問題を解く
	void solve();
};
