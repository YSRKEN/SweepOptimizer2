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
	size_t size_all_;
	// ある箇所に対して、次に進める箇所の一覧
	vector<vector<size_t>> point_next_;
	// 清掃員の位置
	vector<vector<size_t>> point_staff_;
	vector<vector<size_t>> first_point_staff_;
	// 清掃員の1つ前の位置
	vector<vector<size_t>> prev_point_staff_;
	// 清掃員の最大歩数
	vector<vector<size_t>> walk_count_;
	// 清掃員の残タスクフラグ
	// 女の子ならリンゴ・ロボットならビンを持つとNonFreeになる
	vector<vector<StaffTask>> staff_task_;
	// 清掃員の特定歩数におけるメンバーのインデックス
	// firstが種類、secondがその種類における清掃員の番号
	vector<vector<std::pair<size_t, size_t>>> walk_staff_list_;
	// 清掃員の最大歩数
	size_t max_walk_count_;
	// 正解ルートにおける清掃員の移動経路
	vector<vector<vector<size_t>>> staff_root_;
	// 最小移動コスト
	vector<vector<size_t>> min_cost_;
	// 最小移動コスト(ビットボード版)
	// min_cost_bb_[スタート][移動マス数]
	// 1～移動マス数まで進んだ際に到達可能な位置のビットが立っている
	vector<vector<BitBoard>> min_cost_bb_;
	vector<vector<BitBoard>> min_cost_bb_combo_;
	// かち合う可能性がある清掃員一覧
	vector<std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>> combo_staff_list_;

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

	// ソルバー
	// depth……
	// 再帰深度。最終値になった際に正解盤面になっているかの判定が行われる
	// 初期値→最大値(walk_staff_list_.size()と等しい)
	// 最終値→0
	// step……
	// 残メンバー数。最終値になった際に再帰深度が1減る
	// 初期値→最大値(walk_staff_list_[max_walk_count_ - depth].size() - 1と等しい)
	// 最終値→-1
	bool solve_impl(const size_t depth, const int step);
	// ソルバー(コンボ考慮)
	bool solve_with_combo_impl(const size_t depth, const int step);

	// 問題が解けていればtrue
	bool is_solved() const noexcept;
public:
	// コンストラクタ
	Problem(const char file_name[]);
	// 内容を表示する
	void put() const noexcept;
	// 問題を解く
	bool solve(const bool combo_flg);
	// 解を表示する
	void show_answer() const;
};
