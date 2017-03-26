#include <cstdint>
#include <iostream>
#include <immintrin.h>

using std::cout;
using std::endl;

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
			temp[0] = 1 << i;
		}
		else {
			temp[1] = 1 << (i - 64);
		}
		// _mm_loadu_si128でそれをテーブルに読み込む
		bit[i] = _mm_loadu_si128(reinterpret_cast<__m128i*>(temp));
	}
}

// ビットボードを表現するクラス
class BitBoard {
	__m128i data_;
public:
	// 特定のビットの位置を立たせる(1にする)
	void set_bit(const size_t index) noexcept{
		data_ = _mm_or_si128(data_, bit[index]);
	}
	// 特定のビットにある値を読み込む
	// ビットが立っていればtrue、いなければfalseを返す
	bool get_bit(const size_t index) const noexcept {
		return (_mm_testz_si128(data_, bit[index]) == 0);
	}
};

