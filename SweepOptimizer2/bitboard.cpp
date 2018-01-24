#include "bitboard.h"

const size_t MAX_BOARD_SIZE{ 81 };

// ビット演算用に用意するテーブル
// bit[x]は、xビット目のみ立っている値となる
BitBoard bit[MAX_BOARD_SIZE];
// mask[x]は、xビット目とその周囲が立っている
BitBoard mask[MAX_BOARD_SIZE];

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

void initialize_mask(const size_t size_x, const size_t size_y) noexcept {
	// ビット演算用にマスクを初期化
	for (uint64_t y = 0; y < size_y; ++y) {
		for (uint64_t x = 0; x < size_x; ++x) {
			size_t i = y * size_x + x;
			mask[i] = bit[i];
			// 左上
			if (x >= 1 && y >= 1)
				mask[i] = _mm_or_si128(mask[i], bit[i - size_x - 1]);
			// 上
			if (y >= 1)
				mask[i] = _mm_or_si128(mask[i], bit[i - size_x]);
			// 右上
			if (x < size_x - 1 && y >= 1)
				mask[i] = _mm_or_si128(mask[i], bit[i - size_x + 1]);
			// 左
			if (x >= 1)
				mask[i] = _mm_or_si128(mask[i], bit[i - 1]);
			// 右
			if (x < size_x - 1)
				mask[i] = _mm_or_si128(mask[i], bit[i + 1]);
			// 左下
			if (x >= 1 && y < size_y - 1)
				mask[i] = _mm_or_si128(mask[i], bit[i + size_x - 1]);
			// 下
			if (y < size_y - 1)
				mask[i] = _mm_or_si128(mask[i], bit[i + size_x]);
			// 右下
			if (x < size_x - 1 && y < size_y - 1)
				mask[i] = _mm_or_si128(mask[i], bit[i + size_x + 1]);
		}
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
// 特定のマスクを適用する
void BitBoard::unset_mask(const size_t index) noexcept {
	data_ = _mm_andnot_si128(mask[index], data_);
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
// 全てのビットが寝ていればtrue、そうでなければfalseを返す
bool BitBoard::is_zero() const noexcept {
	return (_mm_testz_si128(data_, data_) == 1);
}
// 引数aのビットを内包していればtrue、そうでなければfalseを返す
bool BitBoard::has_bit(const BitBoard &a) const noexcept {
	const auto temp = _mm_and_si128(data_, a);
	const auto temp2 = _mm_xor_si128(temp, a);
	return (_mm_testz_si128(temp2, temp2) == 1);
}
// 0に初期化する
void BitBoard::set_zero() noexcept {
	data_ = _mm_setzero_si128();
}

// オペレータ
const BitBoard BitBoard::operator & (const BitBoard &a) const noexcept {
	return BitBoard(_mm_and_si128(data_, a));
}
const BitBoard BitBoard::operator | (const BitBoard &a) const noexcept {
	return BitBoard(_mm_or_si128(data_, a));
}
void BitBoard::operator |= (const BitBoard &a) noexcept {
	data_ = _mm_or_si128(data_, a);
}

// 出力(デバッグ用)
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
