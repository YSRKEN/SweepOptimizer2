#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "bitboard.cpp"

using std::cout;
using std::endl;
using std::vector;

// 清掃員の種類(男の子・女の子・ロボット)
enum class StaffType {Boy, Girl, Robot};
// 清掃員に残ったタスク(リンゴ・ビン捨て)があるならNonFree, ないならFree
enum class StaffTask { Free, NonFree };

class Problem {
	// 盤面サイズ
	size_t size_x_;
	size_t size_y_;
	// ある箇所に対して、次に進める箇所の一覧
	vector<vector<size_t>> point_next_;
	// 清掃員の位置
	vector<size_t> point_staff_;
	// 清掃員の最大歩数
	vector<size_t> walk_count_;
	// 清掃員の種類
	vector<StaffType> staff_type_;
	// 清掃員の残タスクフラグ
	// 女の子ならリンゴ・ロボットならビンを持つとNonFreeになる
	vector<StaffTask> staff_task_;

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
	Problem(const char file_name[]) {
		// 読み込み用定数
		enum class FloorObject { Dirty, Clean, Boy, Girl, Robot, Pool, Apple, Bottle, Dust, Recycle, Block };

		std::ifstream ifs(file_name);
		// 盤面サイズを読み込む
		ifs >> size_x_ >> size_y_;
		// 盤面を読み込みつつ、各メンバ変数に代入していく
		for (size_t y = 0; y < size_y_; ++y) {
			for (size_t x = 0; x < size_x_; ++x) {
				size_t temp;
				ifs >> temp;
				switch (static_cast<FloorObject>(temp)) {
				case FloorObject::Dirty:
					break;
				case FloorObject::Clean:
					break;
				case FloorObject::Boy:
					break;
				case FloorObject::Girl:
					break;
				case FloorObject::Robot:
					break;
				case FloorObject::Pool:
					break;
				case FloorObject::Apple:
					break;
				case FloorObject::Bottle:
					break;
				case FloorObject::Dust:
					break;
				case FloorObject::Recycle:
					break;
				case FloorObject::Block:
					break;
				default:
					throw "問題ファイルを読み込めませんでした.";
					break;
				}
			}
		}
		cout << file_name << endl;
	}
};
