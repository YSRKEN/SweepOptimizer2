#include "bitboard.h"
#include "problem.h"

// 清掃員の種類(男の子・女の子・ロボット)
enum class StaffType {Boy, Girl, Robot};
// 清掃員に残ったタスク(リンゴ・ビン捨て)があるならNonFree, ないならFree
enum class StaffTask { Free, NonFree };

// コンストラクタ
Problem::Problem(const char file_name[]) {
	// 読み込み用定数
	enum class FloorObject { Dirty, Clean, Boy, Girl, Robot, Pool, Apple, Bottle, Dust, Recycle, Block };
	// ファイルを読み込む
	std::ifstream ifs(file_name);
	// 盤面サイズを読み込む
	ifs >> size_x_ >> size_y_;
	// 各メンバ変数を初期化
	floor_dirty_.set_zero();
	floor_pool_.set_zero();
	floor_apple_.set_zero();
	floor_bottle_.set_zero();
	floor_around_dust_.set_zero();
	floor_around_recycle_.set_zero();
	// テンポラリ配列に盤面を読み込む
	vector<FloorObject> temp_floor(size_x_ * size_y_);
	for (size_t y = 0; y < size_y_; ++y) {
		for (size_t x = 0; x < size_x_; ++x) {
			size_t temp;
			ifs >> temp;
			size_t point = y * size_x_ + x;
			temp_floor[point] = static_cast<FloorObject>(temp);
		}
	}
	// テンポラリ配列を解析し、各メンバ変数に代入していく
	for (size_t y = 0; y < size_y_; ++y) {
		for (size_t x = 0; x < size_x_; ++x) {
			size_t point = y * size_x_ + x;
			switch (temp_floor[point]) {
			case FloorObject::Dirty:
				floor_dirty_.set_bit(point);
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
