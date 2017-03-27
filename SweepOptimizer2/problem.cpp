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

	std::ifstream ifs(file_name);
	// 盤面サイズを読み込む
	ifs >> size_x_ >> size_y_;
	// 盤面を読み込みつつ、各メンバ変数に代入していく
	floor_dirty_.set_zero();
	for (size_t y = 0; y < size_y_; ++y) {
		for (size_t x = 0; x < size_x_; ++x) {
			size_t point = y * size_x_ + x;
			size_t temp;
			ifs >> temp;
			switch (static_cast<FloorObject>(temp)) {
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
