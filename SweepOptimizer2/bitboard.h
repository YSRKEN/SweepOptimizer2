#pragma once
#include <iostream>
#include <immintrin.h>
#include <cstdint>

using std::cout;
using std::endl;

extern const size_t MAX_BOARD_SIZE;
class BitBoard;
extern BitBoard bit[];

// �r�b�g���Z�p�Ƀe�[�u������������
void initialize() noexcept;

// �r�b�g�{�[�h��\������N���X
class BitBoard {
	__m128i data_;
public:
	// ����̃r�b�g�̈ʒu�𗧂�����(1�ɂ���)
	void set_bit(const size_t index) noexcept;
	// ����̃r�b�g�̈ʒu��Q������(0�ɂ���)
	void unset_bit(const size_t index) noexcept;
	// ����̃r�b�g�̈ʒu�𔽓]������
	void reverse_bit(const size_t index) noexcept;
	// ����̃r�b�g�ɂ���l��ǂݍ���
	// �r�b�g�������Ă����true�A���Ȃ����false��Ԃ�
	bool get_bit(const size_t index) const noexcept;
	// �S�Ẵr�b�g���Q�Ă����true�A�����łȂ����false��Ԃ�
	bool is_zero() const noexcept;
	// 0�ɏ���������
	void set_zero() noexcept;
	// __m128i�ւ̃L���X�g
	operator __m128i() const noexcept {return data_;}
	// �R���X�g���N�^
	BitBoard() {}
	BitBoard(const __m128i bb) {
		data_ = bb;
	}
	// �o��(�f�o�b�O�p)
	void put(const size_t width, const size_t height) const noexcept;
};
