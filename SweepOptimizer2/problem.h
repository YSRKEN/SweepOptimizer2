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
	// ����ӏ��ɑ΂��āA���ɐi�߂�ӏ��̈ꗗ
	vector<vector<size_t>> point_next_;
	// ���|���̈ʒu
	vector<vector<size_t>> point_staff_;
	// ���|���̍ő����
	vector<vector<size_t>> walk_count_;
	// ���|���̎c�^�X�N�t���O
	// ���̎q�Ȃ烊���S�E���{�b�g�Ȃ�r��������NonFree�ɂȂ�
	vector<vector<StaffTask>> staff_task_;
	// ���|���̓�������ɂ����郁���o�[�̃C���f�b�N�X
	vector<vector<std::pair<size_t, size_t>>> walk_staff_list_;

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
public:
	// �R���X�g���N�^
	Problem(const char file_name[]);
	// ���e��\������
	void put() const noexcept;
	// ��������
	void solve();
};
