#include "bitboard.h"
#include "problem.h"

// 清掃員の種類(男の子・女の子・ロボット)
enum class StaffType { Boy, Girl, Robot, Size };
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
	point_next_.resize(size_x_ * size_y_);
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
	const auto can_move_area = [&temp_floor](const size_t point) {
		return (temp_floor[point] != FloorObject::Dust
			&& temp_floor[point] != FloorObject::Recycle
			&& temp_floor[point] != FloorObject::Block);
	};
	point_staff_.resize(static_cast<size_t>(StaffType::Size));
	for (size_t y = 0; y < size_y_; ++y) {
		for (size_t x = 0; x < size_x_; ++x) {
			size_t point = y * size_x_ + x;
			// 各BitBoard・vectorへの代入
			switch (temp_floor[point]) {
			case FloorObject::Dirty:
				// 汚れた床は拭く必要がある箇所
				floor_dirty_.set_bit(point);
				break;
			case FloorObject::Clean:
			case FloorObject::Boy:
				// 拭く必要がない場所だが、清掃員(男の子)を記録する必要がある
				point_staff_[static_cast<size_t>(StaffType::Boy)].push_back(point);
				break;
			case FloorObject::Girl:
				// 拭く必要がない場所だが、清掃員(女の子)を記録する必要がある
				point_staff_[static_cast<size_t>(StaffType::Girl)].push_back(point);
				break;
			case FloorObject::Robot:
				// 拭く必要がない場所だが、清掃員(ロボット)を記録する必要がある
				point_staff_[static_cast<size_t>(StaffType::Robot)].push_back(point);
				break;
			case FloorObject::Pool:
				// 拭く必要があり、かつ水たまりがある
				floor_dirty_.set_bit(point);
				floor_pool_.set_bit(point);
				break;
			case FloorObject::Apple:
				// 拭く必要があり、かつリンゴがある
				floor_dirty_.set_bit(point);
				floor_apple_.set_bit(point);
				break;
			case FloorObject::Bottle:
				// 拭く必要があり、かつビンがある
				floor_dirty_.set_bit(point);
				floor_bottle_.set_bit(point);
				break;
			case FloorObject::Dust:
				// 周囲のマスにゴミ箱の位置を知らせる
				if (x != 0)
					floor_around_dust_.set_bit(point - 1);
				if (y != 0)
					floor_around_dust_.set_bit(point - size_x_);
				if (x != size_x_ - 1)
					floor_around_dust_.set_bit(point + 1);
				if (y != size_y_ - 1)
					floor_around_dust_.set_bit(point + size_x_);
				break;
			case FloorObject::Recycle:
				// 周囲のマスにリサイクル箱の位置を知らせる
				if (x != 0)
					floor_around_recycle_.set_bit(point - 1);
				if (y != 0)
					floor_around_recycle_.set_bit(point - size_x_);
				if (x != size_x_ - 1)
					floor_around_recycle_.set_bit(point + 1);
				if (y != size_y_ - 1)
					floor_around_recycle_.set_bit(point + size_x_);
				break;
				break;
			case FloorObject::Block:
				break;
			default:
				throw "問題ファイルを読み込めませんでした.";
				break;
			}
			// point_next_への代入
			if (can_move_area(point)) {
				if (x != 0 && can_move_area(point - 1))
					point_next_[point].push_back(point - 1);
				if (y != 0 && can_move_area(point - size_x_))
					point_next_[point].push_back(point - size_x_);
				if (x != size_x_ - 1 && can_move_area(point + 1))
					point_next_[point].push_back(point + 1);
				if (y != size_y_ - 1 && can_move_area(point + size_x_))
					point_next_[point].push_back(point + size_x_);
			}
		}
	}
	// 従業員の最大歩数を記録する
	

/*	floor_dirty_.put(size_x_, size_y_);
	floor_pool_.put(size_x_, size_y_);
	floor_apple_.put(size_x_, size_y_);
	floor_bottle_.put(size_x_, size_y_);
	floor_around_dust_.put(size_x_, size_y_);
	floor_around_recycle_.put(size_x_, size_y_);*/
	cout << file_name << endl;
}
