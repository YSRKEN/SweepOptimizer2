#include "bitboard.h"
#include "problem.h"

// 清掃員の種類(男の子・女の子・ロボット)
enum class StaffType { Boy, Girl, Robot, Size };
const size_t StaffTypeSize = static_cast<size_t>(StaffType::Size);
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
	point_staff_.resize(StaffTypeSize);
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
	walk_count_.resize(StaffTypeSize);
	staff_task_.resize(StaffTypeSize);
	size_t max_walk_count = 0;
	for (size_t ti = 0; ti < StaffTypeSize; ++ti) {
		size_t staff_size;
		ifs >> staff_size;
		if (point_staff_[ti].size() != staff_size)
			throw "問題ファイルを読み込めませんでした.";
		walk_count_[ti].resize(staff_size);
		staff_task_[ti].resize(staff_size);
		for (size_t si = 0; si < staff_size; ++si) {
			size_t walk_count;
			ifs >> walk_count;
			walk_count_[ti][si] = walk_count;
			max_walk_count = std::max(walk_count, max_walk_count);
			staff_task_[ti][si] = StaffTask::Free;
		}
	}
	walk_staff_list_.resize(max_walk_count + 1);
	for (size_t ti = 0; ti < StaffTypeSize; ++ti) {
		for (size_t si = 0; si < walk_count_[ti].size(); ++si) {
			for (size_t wi = 0; wi <= walk_count_[ti][si]; ++wi) {
				walk_staff_list_[wi].push_back(std::pair<size_t, size_t>(ti, si));
			}
		}
	}
}

// 内容を表示する
void Problem::put() const noexcept {
	cout << "【内容表示】" << endl;
	cout << "盤面サイズ：" << size_x_ << "x" << size_y_ << endl;
	cout << "移動可能な場所：" << endl;
	for (size_t y = 0; y < size_y_; ++y) {
		for (size_t x = 0; x < size_x_; ++x) {
			size_t point = y * size_x_ + x;
			cout << (point_next_[point].size() != 0 ? "・" : "■");
		}
		cout << endl;
	}
	cout << endl;
	cout << "汚れた床：" << endl;
	floor_dirty_.put(size_x_, size_y_);
	cout << "水たまり：" << endl;
	floor_pool_.put(size_x_, size_y_);
	cout << "リンゴ：" << endl;
	floor_apple_.put(size_x_, size_y_);
	cout << "ビン：" << endl;
	floor_bottle_.put(size_x_, size_y_);
	cout << "ゴミ箱の周囲：" << endl;
	floor_around_dust_.put(size_x_, size_y_);
	cout << "リサイクル箱の周囲：" << endl;
	floor_around_recycle_.put(size_x_, size_y_);
	cout << "メンバーリスト：" << endl;
	for (size_t ti = 0; ti < StaffTypeSize; ++ti) {
		for (size_t si = 0; si < point_staff_[ti].size(); ++si) {
			size_t sx = point_staff_[ti][si] % size_x_;
			size_t sy = point_staff_[ti][si] / size_x_;
			cout << "種類：" << (ti == 0 ? "男の子　" : ti == 1 ? "女の子　" : "ロボット")
				<< "　位置：(" << sx << "," << sy <<")"
				<< "　歩数：" << walk_count_[ti][si] << endl;
		}
	}
	cout << endl;
	cout << "歩数による分類：" << endl;
	for (size_t wi = 0; wi < walk_staff_list_.size(); ++wi) {
		cout << "～" << wi << "歩";
		for (const auto &it : walk_staff_list_[wi]) {
			size_t sx = point_staff_[it.first][it.second] % size_x_;
			size_t sy = point_staff_[it.first][it.second] / size_x_;
			cout << "　種類：" << (it.first == 0 ? "男の子　" : it.first == 1 ? "女の子　" : "ロボット")
				<< "　位置：(" << sx << ", " << sy <<")";
		}
		cout << endl;
	}
	cout << endl;
}

// 問題を解く
void Problem::solve(){
	/* 各メンバーの初期位置は分かっているので、
	 * 歩数を1歩づつ進めながら判定を行う。
	 * 特定歩数において動ける清掃員の一覧はwalk_staff_list_から分かり、
	 * 各清掃員をpoint_next_を見ながらそれぞれ動かす。
	 * その際staff_task_・floor_dirty_・floor_pool_・floor_apple_・floor_bottle_は変化するし、
	 * それらの変数の値をチェックすれば、完成したかが分かる。
	 * (floor_around_dust_とfloor_around_recycle_は参照用)
	 * ん、size_x_・size_y_・point_staff_・walk_count_は不要？
	 */
}