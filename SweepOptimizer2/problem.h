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
	// �ՖʃT�C�Y
	size_t size_x_;
	size_t size_y_;
	size_t size_all_;
	// ����ӏ��ɑ΂��āA���ɐi�߂�ӏ��̈ꗗ
	vector<vector<size_t>> point_next_;
	// ���|���̈ʒu
	vector<vector<size_t>> point_staff_;
	vector<vector<size_t>> first_point_staff_;
	// ���|����1�O�̈ʒu
	vector<vector<size_t>> prev_point_staff_;
	// ���|���̍ő����
	vector<vector<size_t>> walk_count_;
	// ���|���̎c�^�X�N�t���O
	// ���̎q�Ȃ烊���S�E���{�b�g�Ȃ�r��������NonFree�ɂȂ�
	vector<vector<StaffTask>> staff_task_;
	// ���|���̓�������ɂ����郁���o�[�̃C���f�b�N�X
	// first����ށAsecond�����̎�ނɂ����鐴�|���̔ԍ�
	vector<vector<std::pair<size_t, size_t>>> walk_staff_list_;
	// ���|���̍ő����
	size_t max_walk_count_;
	// �������[�g�ɂ����鐴�|���̈ړ��o�H
	vector<vector<vector<size_t>>> staff_root_;
	// �ŏ��ړ��R�X�g
	vector<vector<size_t>> min_cost_;
	// �ŏ��ړ��R�X�g(�r�b�g�{�[�h��)
	// min_cost_bb_[�X�^�[�g][�ړ��}�X��]
	// 1�`�ړ��}�X���܂Ői�񂾍ۂɓ��B�\�Ȉʒu�̃r�b�g�������Ă���
	vector<vector<BitBoard>> min_cost_bb_;
	vector<vector<BitBoard>> min_cost_bb_combo_;
	// ���������\�������鐴�|���ꗗ
	vector<std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>> combo_staff_list_;

	// �@���K�v������ӏ���1�A����ȊO��0�𗧂Ă�
	// (�܂��@���Ă��Ȃ��ӏ�������Δ�0�A�S�Đ@���Ă����0)
	BitBoard floor_dirty_;
	// �����܂肪���݂���ӏ���1�A����ȊO��0�𗧂Ă�
	BitBoard floor_pool_;
	// �����S�����݂���ӏ���1�A����ȊO��0�𗧂Ă�
	BitBoard floor_apple_;
	// �r�������݂���ӏ���1�A����ȊO��0�𗧂Ă�
	BitBoard floor_bottle_;
	// �S�~���ɗאڂ���ӏ���1�A����ȊO��0�𗧂Ă�
	BitBoard floor_around_dust_;
	// ���T�C�N�����ɗאڂ���ӏ���1�A����ȊO��0�𗧂Ă�
	BitBoard floor_around_recycle_;

	// �\���o�[
	// depth�c�c
	// �ċA�[�x�B�ŏI�l�ɂȂ����ۂɐ���ՖʂɂȂ��Ă��邩�̔��肪�s����
	// �����l���ő�l(walk_staff_list_.size()�Ɠ�����)
	// �ŏI�l��0
	// step�c�c
	// �c�����o�[���B�ŏI�l�ɂȂ����ۂɍċA�[�x��1����
	// �����l���ő�l(walk_staff_list_[max_walk_count_ - depth].size() - 1�Ɠ�����)
	// �ŏI�l��-1
	bool solve_impl(const size_t depth, const int step);
	// �\���o�[(�R���{�l��)
	bool solve_with_combo_impl(const size_t depth, const int step);

	// ��肪�����Ă����true
	bool is_solved() const noexcept;
public:
	// �R���X�g���N�^
	Problem(const char file_name[]);
	// ���e��\������
	void put() const noexcept;
	// ��������
	bool solve(const bool combo_flg);
	// ����\������
	void show_answer() const;
};
