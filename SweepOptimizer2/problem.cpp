#include "bitboard.h"
#include "problem.h"

// 清掃員の種類(男の子・女の子・ロボット)
enum class StaffType { Boy, Girl, Robot, Size };
const size_t StaffTypeSize = static_cast<size_t>(StaffType::Size);
// 清掃員に残ったタスク(リンゴ・ビン捨て)があるならNonFree, ないならFree
enum class StaffTask { Free, NonFree };

const size_t MAX_COST = 10000;

// コンストラクタ
Problem::Problem(const char file_name[]) {
	// 読み込み用定数
	enum class FloorObject { Dirty, Clean, Boy, Girl, Robot, Pool, Apple, Bottle, Dust, Recycle, Block };
	// ファイルを読み込む
	std::ifstream ifs(file_name);
	// 盤面サイズを読み込む
	ifs >> size_x_ >> size_y_;
	size_all_ = size_x_ * size_y_;
	// 各メンバ変数を初期化
	point_next_.resize(size_all_);
	floor_dirty_.set_zero();
	floor_pool_.set_zero();
	floor_apple_.set_zero();
	floor_bottle_.set_zero();
	floor_around_dust_.set_zero();
	floor_around_recycle_.set_zero();
	// テンポラリ配列に盤面を読み込む
	vector<FloorObject> temp_floor(size_all_);
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
	prev_point_staff_.resize(StaffTypeSize);
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
				prev_point_staff_[static_cast<size_t>(StaffType::Boy)].push_back(-1);
				break;
			case FloorObject::Girl:
				// 拭く必要がない場所だが、清掃員(女の子)を記録する必要がある
				point_staff_[static_cast<size_t>(StaffType::Girl)].push_back(point);
				prev_point_staff_[static_cast<size_t>(StaffType::Girl)].push_back(-1);
				break;
			case FloorObject::Robot:
				// 拭く必要がない場所だが、清掃員(ロボット)を記録する必要がある
				point_staff_[static_cast<size_t>(StaffType::Robot)].push_back(point);
				prev_point_staff_[static_cast<size_t>(StaffType::Robot)].push_back(-1);
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
	first_point_staff_ = point_staff_;
	// 事前に最小移動歩数を計算しておく(ワーシャル・フロイド法)
	min_cost_.resize(size_all_);
	for (size_t k = 0; k < size_all_; ++k) {
		min_cost_[k].resize(size_all_, MAX_COST);
	}
	for (size_t y = 0; y < size_y_; ++y) {
		for (size_t x = 0; x < size_x_; ++x) {
			size_t point = y * size_x_ + x;
			if (can_move_area(point)) {
				min_cost_[point][point] = 0;
				for (size_t next_point : point_next_[point]) {
					min_cost_[point][next_point] = 1;
					min_cost_[next_point][point] = 1;
				}
			}
		}
	}
	for (size_t k = 0; k < size_all_; ++k) {
		for (size_t i = 0; i < size_all_; ++i) {
			for (size_t j = 0; j < size_all_; ++j) {
				min_cost_[i][j] = std::min(min_cost_[i][j], min_cost_[i][k] + min_cost_[k][j]);
			}
		}
	}
	// 清掃員の最大歩数を記録する
	walk_count_.resize(StaffTypeSize);
	staff_task_.resize(StaffTypeSize);
	staff_root_.resize(StaffTypeSize);
	max_walk_count_ = 0;
	for (size_t ti = 0; ti < StaffTypeSize; ++ti) {
		size_t staff_size;
		ifs >> staff_size;
		if (point_staff_[ti].size() != staff_size)
			throw "問題ファイルを読み込めませんでした.";
		walk_count_[ti].resize(staff_size);
		staff_task_[ti].resize(staff_size);
		staff_root_[ti].resize(staff_size);
		for (size_t si = 0; si < staff_size; ++si) {
			size_t walk_count;
			ifs >> walk_count;
			walk_count_[ti][si] = walk_count;
			max_walk_count_ = std::max(walk_count, max_walk_count_);
			staff_task_[ti][si] = StaffTask::Free;
		}
	}
	walk_staff_list_.resize(max_walk_count_ + 2);
	for (size_t ti = 0; ti < StaffTypeSize; ++ti) {
		for (size_t si = 0; si < walk_count_[ti].size(); ++si) {
			for (size_t wi = 0; wi <= walk_count_[ti][si]; ++wi) {
				walk_staff_list_[wi].push_back(std::pair<size_t, size_t>(ti, si));
			}
		}
	}
	// かち合う可能性がある清掃員の一覧を作成する
	{
		// 清掃員一覧を作成
		vector<std::pair<size_t, size_t>> pair_list;
		for (size_t ti = 0; ti < StaffTypeSize; ++ti) {
			for (size_t si = 0; si < point_staff_[ti].size(); ++si) {
				pair_list.push_back(std::pair<size_t, size_t>(ti, si));
			}
		}
		// かち合う可能性がある一覧を作成
		for (size_t si1 = 0; si1 < pair_list.size() - 1; ++si1) {
			size_t point1 = point_staff_[pair_list[si1].first][pair_list[si1].second];
			size_t hash1 = ((point1 % size_x_) + (point1 / size_x_)) % 2;
			for (size_t si2 = si1 + 1; si2 < pair_list.size(); ++si2) {
				size_t point2 = point_staff_[pair_list[si2].first][pair_list[si2].second];
				size_t hash2 = ((point2 % size_x_) + (point2 / size_x_)) % 2;
				if (hash1 == hash2) {
					combo_staff_list_.push_back(std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>(pair_list[si1], pair_list[si2]));
				}
			}
		}
	}
	// マスクを初期化する
	initialize_mask(size_x_, size_y_);
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
bool Problem::solve(bool combo_flg){
	if (combo_flg) {
		return solve_with_combo_impl(max_walk_count_, walk_staff_list_[1].size() - 1);
	}
	else {

	}
	return solve_impl(max_walk_count_, walk_staff_list_[1].size() - 1);
}

// 問題が解けていればtrue
bool Problem::is_solved() const noexcept {
	if (!floor_dirty_.is_zero()) return false;
	if (!floor_pool_.is_zero()) return false;
	if (!floor_apple_.is_zero()) return false;
	if (!floor_bottle_.is_zero()) return false;
	for (size_t ti = 0; ti < StaffTypeSize; ++ti) {
		for (size_t si = 0; si < point_staff_[ti].size(); ++si) {
			if (staff_task_[ti][si] == StaffTask::NonFree) return false;
		}
	}
	return true;
}

bool Problem::solve_impl(const size_t depth, const int step) {
	// depth == 0 なら、正解盤面になっているかの判定を行う
	if (depth == 0) {
		return is_solved();
	}
	// step == -1 なら、depthを1つ増やす
	if (step == -1) {
		// 事前枝刈り
		const auto &staff_all = walk_staff_list_[max_walk_count_ - depth + 1];
		for (size_t point = 0; point < size_all_; ++point) {
			if (floor_dirty_.get_bit(point)) {
				bool clean_flg = false;
				const bool pool_flg = floor_pool_.get_bit(point);
				const bool apple_flg = floor_apple_.get_bit(point);
				const bool bottle_flg = floor_bottle_.get_bit(point);
				for (const auto &staff : staff_all) {
					size_t staff_point = point_staff_[staff.first][staff.second];
					if (min_cost_[point][staff_point] > walk_count_[staff.first][staff.second]
					|| pool_flg && staff.first != static_cast<size_t>(StaffType::Boy)
					|| apple_flg && staff.first != static_cast<size_t>(StaffType::Girl)
					|| bottle_flg && staff.first != static_cast<size_t>(StaffType::Robot)) {
						continue;
					}
					clean_flg = true;
					break;
				}
				if (!clean_flg)
					return false;
			}
		}
		// 枝刈りできなかったので探索を進める
		size_t depth_ = depth - 1;
		return solve_impl(depth_, walk_staff_list_[max_walk_count_ - depth_ + 1].size() - 1);
	}
	// それ以外なら、探索を進める
	const auto &staff = walk_staff_list_[max_walk_count_ - depth + 1][step];
	// 現在の状態を保存する
	size_t now_point = point_staff_[staff.first][staff.second];
	size_t now_prev_point = prev_point_staff_[staff.first][staff.second];
	StaffTask now_task = staff_task_[staff.first][staff.second];
	BitBoard now_floor_dirty_ = floor_dirty_;
	BitBoard now_floor_pool_ = floor_pool_;
	BitBoard now_floor_apple_ = floor_apple_;
	BitBoard now_floor_bottle_ = floor_bottle_;
	// 最大4方向に移動
	--walk_count_[staff.first][staff.second];
	for (size_t next_point : point_next_[now_point]) {
		if (next_point == now_prev_point)
			continue;
		// 1歩進める
		point_staff_[staff.first][staff.second] = next_point;
		prev_point_staff_[staff.first][staff.second] = now_point;
		floor_dirty_.unset_bit(next_point);
		switch (static_cast<StaffType>(staff.first)) {
		case StaffType::Boy:
			// 1歩進める
			floor_pool_.unset_bit(next_point);
			// 再帰を1段進める
			if (solve_impl(depth, step - 1)) {
				staff_root_[staff.first][staff.second].push_back(next_point);
				return true;
			}
			// 1歩戻す
			floor_pool_ = now_floor_pool_;
			break;
		case StaffType::Girl:
			// 1歩進める
			if (floor_apple_.get_bit(next_point))
				staff_task_[staff.first][staff.second] = StaffTask::NonFree;
			if (floor_around_dust_.get_bit(next_point))
				staff_task_[staff.first][staff.second] = StaffTask::Free;
			floor_apple_.unset_bit(next_point);
			// 再帰を1段進める
			if (solve_impl(depth, step - 1)) {
				staff_root_[staff.first][staff.second].push_back(next_point);
				return true;
			}
			// 1歩戻す
			staff_task_[staff.first][staff.second] = now_task;
			floor_apple_ = now_floor_apple_;
			break;
		case StaffType::Robot:
			// 1歩進める
			if (floor_bottle_.get_bit(next_point))
				staff_task_[staff.first][staff.second] = StaffTask::NonFree;
			if (floor_around_recycle_.get_bit(next_point))
				staff_task_[staff.first][staff.second] = StaffTask::Free;
			floor_bottle_.unset_bit(next_point);
			// 再帰を1段進める
			if (solve_impl(depth, step - 1)) {
				staff_root_[staff.first][staff.second].push_back(next_point);
				return true;
			}
			// 1歩戻す
			staff_task_[staff.first][staff.second] = now_task;
			floor_bottle_ = now_floor_bottle_;
			break;
		}
		// 1歩戻す
		floor_dirty_ = now_floor_dirty_;
	}
	point_staff_[staff.first][staff.second] = now_point;
	prev_point_staff_[staff.first][staff.second] = now_prev_point;
	staff_task_[staff.first][staff.second] = now_task;
	++walk_count_[staff.first][staff.second];
	return false;
}

bool Problem::solve_with_combo_impl(const size_t depth, const int step) {
	// depth == 0 なら、正解盤面になっているかの判定を行う
	if (depth == 0) {
		return is_solved();
	}
	// step == -1 なら、depthを1つ増やす
	if (step == -1) {
		// コンボ判定
		for (const auto &staff_pair : combo_staff_list_) {
			const auto staff1 = staff_pair.first;
			const auto staff2 = staff_pair.second;
			const auto combo_point = point_staff_[staff1.first][staff1.second];
			if (combo_point == point_staff_[staff2.first][staff2.second]) {
				floor_dirty_.unset_mask(combo_point);
			}
		}
		// 事前枝刈り
		const auto &staff_all = walk_staff_list_[max_walk_count_ - depth + 1];
		for (size_t point = 0; point < size_all_; ++point) {
			if (floor_dirty_.get_bit(point)) {
				bool clean_flg = false;
				const bool pool_flg = floor_pool_.get_bit(point);
				const bool apple_flg = floor_apple_.get_bit(point);
				const bool bottle_flg = floor_bottle_.get_bit(point);
				for (const auto &staff : staff_all) {
					size_t staff_point = point_staff_[staff.first][staff.second];
					if (min_cost_[point][staff_point] > walk_count_[staff.first][staff.second] + 2
						|| pool_flg && staff.first != static_cast<size_t>(StaffType::Boy)
						|| apple_flg && staff.first != static_cast<size_t>(StaffType::Girl)
						|| bottle_flg && staff.first != static_cast<size_t>(StaffType::Robot)) {
						continue;
					}
					clean_flg = true;
					break;
				}
				if (!clean_flg)
					return false;
			}
		}
		// 枝刈りできなかったので探索を進める
		size_t depth_ = depth - 1;
		return solve_with_combo_impl(depth_, walk_staff_list_[max_walk_count_ - depth_ + 1].size() - 1);
	}
	// それ以外なら、探索を進める
	const auto &staff = walk_staff_list_[max_walk_count_ - depth + 1][step];
	// 現在の状態を保存する
	size_t now_point = point_staff_[staff.first][staff.second];
	size_t now_prev_point = prev_point_staff_[staff.first][staff.second];
	StaffTask now_task = staff_task_[staff.first][staff.second];
	BitBoard now_floor_dirty_ = floor_dirty_;
	BitBoard now_floor_pool_ = floor_pool_;
	BitBoard now_floor_apple_ = floor_apple_;
	BitBoard now_floor_bottle_ = floor_bottle_;
	// 最大4方向に移動
	--walk_count_[staff.first][staff.second];
	for (size_t next_point : point_next_[now_point]) {
		if (next_point == now_prev_point)
			continue;
		// 1歩進める
		point_staff_[staff.first][staff.second] = next_point;
		prev_point_staff_[staff.first][staff.second] = now_point;
		floor_dirty_.unset_bit(next_point);
		switch (static_cast<StaffType>(staff.first)) {
		case StaffType::Boy:
			// 1歩進める
			floor_pool_.unset_bit(next_point);
			// 再帰を1段進める
			if (solve_with_combo_impl(depth, step - 1)) {
				staff_root_[staff.first][staff.second].push_back(next_point);
				return true;
			}
			// 1歩戻す
			floor_pool_ = now_floor_pool_;
			break;
		case StaffType::Girl:
			// 1歩進める
			if (floor_apple_.get_bit(next_point))
				staff_task_[staff.first][staff.second] = StaffTask::NonFree;
			if (floor_around_dust_.get_bit(next_point))
				staff_task_[staff.first][staff.second] = StaffTask::Free;
			floor_apple_.unset_bit(next_point);
			// 再帰を1段進める
			if (solve_with_combo_impl(depth, step - 1)) {
				staff_root_[staff.first][staff.second].push_back(next_point);
				return true;
			}
			// 1歩戻す
			staff_task_[staff.first][staff.second] = now_task;
			floor_apple_ = now_floor_apple_;
			break;
		case StaffType::Robot:
			// 1歩進める
			if (floor_bottle_.get_bit(next_point))
				staff_task_[staff.first][staff.second] = StaffTask::NonFree;
			if (floor_around_recycle_.get_bit(next_point))
				staff_task_[staff.first][staff.second] = StaffTask::Free;
			floor_bottle_.unset_bit(next_point);
			// 再帰を1段進める
			if (solve_with_combo_impl(depth, step - 1)) {
				staff_root_[staff.first][staff.second].push_back(next_point);
				return true;
			}
			// 1歩戻す
			staff_task_[staff.first][staff.second] = now_task;
			floor_bottle_ = now_floor_bottle_;
			break;
		}
		// 1歩戻す
		floor_dirty_ = now_floor_dirty_;
	}
	point_staff_[staff.first][staff.second] = now_point;
	prev_point_staff_[staff.first][staff.second] = now_prev_point;
	staff_task_[staff.first][staff.second] = now_task;
	++walk_count_[staff.first][staff.second];
	return false;
}

// 解を表示する
void Problem::show_answer() const {
	for (size_t ti = 0; ti < StaffTypeSize; ++ti) {
		for (size_t si = 0; si < point_staff_[ti].size(); ++si) {
			auto list = staff_root_[ti][si];
			list.push_back(first_point_staff_[ti][si]);
			cout << "種類：" << (ti == 0 ? "男の子　" : ti == 1 ? "女の子　" : "ロボット");
			size_t sx = first_point_staff_[ti][si] % size_x_;
			size_t sy = first_point_staff_[ti][si] / size_x_;
			cout << "(" << sx << "," << sy << ")　";
			for (int k = list.size() - 1; k >= 0; --k){
				cout << list[k];
				if (k >= 1) {
					if (list[k] + size_x_ == list[k - 1]) {
						cout << "(↓)";
					}
					else if (list[k] == list[k - 1] + size_x_) {
						cout << "(↑)";
					}
					else if (list[k] + 1 == list[k - 1]) {
						cout << "(→)";
					}
					else if (list[k] == list[k - 1] + 1) {
						cout << "(←)";
					}
					if (k != 0)
						cout << "⇒";
				}
			}
			cout << endl;
		}
	}
}
