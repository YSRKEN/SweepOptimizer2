#pragma once
#include <iostream>
#include <immintrin.h>
#include <cstdint>

using std::cout;
using std::endl;

extern const size_t MAX_BOARD_SIZE;
class BitBoard;
extern BitBoard bit[];

// ビット演算用にテーブルを準備する
void initialize() noexcept;

// ビットボードを表現するクラス
class BitBoard {
	__m128i data_;
public:
	// 特定のビットの位置を立たせる(1にする)
	void set_bit(const size_t index) noexcept;
	// 特定のビットの位置を寝かせる(0にする)
	void unset_bit(const size_t index) noexcept;
	// 特定のビットの位置を反転させる
	void reverse_bit(const size_t index) noexcept;
	// 特定のビットにある値を読み込む
	// ビットが立っていればtrue、いなければfalseを返す
	bool get_bit(const size_t index) const noexcept;
	// 全てのビットが寝ていればtrue、そうでなければfalseを返す
	bool is_zero() const noexcept;
	// 0に初期化する
	void set_zero() noexcept;
	// __m128iへのキャスト
	operator __m128i() const noexcept {return data_;}
	// コンストラクタ
	BitBoard() {}
	BitBoard(const __m128i bb) {
		data_ = bb;
	}
	// 出力(デバッグ用)
	void put(const size_t width, const size_t height) const noexcept;
};
