#include "bitboard.h"

const size_t MAX_BOARD_SIZE{ 81 };

// ビット演算用に用意するテーブル
// bit[x]は、xビット目のみ立っている値となる
__m128i bit[MAX_BOARD_SIZE];

// ビット演算用にテーブルを準備する
void initialize() noexcept{
	// ビット演算用にテーブルを初期化
	for (uint64_t i = 0; i < MAX_BOARD_SIZE; ++i) {
		// 64bit型変数を用いたテンポラリな配列を作成・初期化
		uint64_t temp[2]{};
		if (i < 64) {
			temp[0] = 1llu << i;
		}
		else {
			temp[1] = 1llu << (i - 64);
		}
		// _mm_loadu_si128でそれをテーブルに読み込む
		bit[i] = _mm_loadu_si128(reinterpret_cast<__m128i*>(temp));
	}
}

// 特定のビットの位置を立たせる(1にする)
void BitBoard::set_bit(const size_t index) noexcept {
	data_ = _mm_or_si128(data_, bit[index]);
}
// 特定のビットの位置を寝かせる(0にする)
void BitBoard::unset_bit(const size_t index) noexcept {
	data_ = _mm_andnot_si128(bit[index], data_);
}
// 特定のビットの位置を反転させる
void BitBoard::reverse_bit(const size_t index) noexcept {
	data_ = _mm_xor_si128(data_, bit[index]);
}
// 特定のビットにある値を読み込む
// ビットが立っていればtrue、いなければfalseを返す
bool BitBoard::get_bit(const size_t index) const noexcept {
	return (_mm_testz_si128(data_, bit[index]) == 0);
}
// 0に初期化する
void BitBoard::set_zero() noexcept {
	data_ = _mm_setzero_si128();
}

void BitBoard::put(const size_t width, const size_t height) const noexcept {
	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < width; ++x) {
			size_t index = y * width + x;
			cout << (get_bit(index) ? "■" : "・");
		}
		cout << endl;
	}
	cout << endl;
}
