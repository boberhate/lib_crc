#ifndef LIB_CRC_H
#define LIB_CRC_H

/* как я хочу использовать эту библиотеку
 *
 * // список параметров шаблона
 * // < Width - размер слова (ширина),
 * //   Poly - полином,
 * //   Init - инициализирующее значение,
 * //   RefIn - флаг, определяющий порядок бит для начала вычислений^ false - со старшего бита, true - с младшего,
 * //   RefOut - флаг, определяющий необходимость инвертации порядка битов регистрац при входе на XOR,
 * //   XorOut - число, с которым складывается по модулю 2 результат (если отсутствует, то по умолчанию NULL, если NULL, то игнорируется)>
 *
 * Пример:
 * using crc32_posix = crc::crc<std::uint32, 0x04c11db7, 0x4C11DB7, 0xFFFFFFFF, false, false, 0xFFFF>;
 *
 * crc32_posix::crc(unsigned char* data);
 *
 * Генерация таблицы происходит на этапе компиляции, поэтому не надо будет тратить время на ее вычисление
 */

#include <iostream>


namespace crc {

// реверс байт в числе
template<typename T>
T reflect(T val, int len = 0) {
	auto result = T(0);
	char ch = len != 0 ? len : sizeof(T)*8;
	for(auto i = 0; i < ch; i++) {
		if(val & (1 << i)) {
			result |= 1 << ((ch - 1) - i);
		}
	}
	return result;
}

// объект, хранящий таблицу crc и выводящий crc-код
template<typename W, W poly, W init, bool refIn, bool refOut, W xorOut, int table_size = 256>
struct crc {
private:
	W table[256];

public:
	constexpr crc()
		: table()
	{
		for(auto i = 0; i < 256; i++) {
			//table[i] = (refIn ? reflect(i, 8) : i) << (sizeof (W)-1)*8;
			table[i] = i << (sizeof (W)-1)*8;
			for(auto j = 0; j < 8; j++) {
				table[i] = (table[i] << 1)
						   ^ (table[i] & (1 << (sizeof(W)*8-1)) ? poly : 0);
			}
			if(refIn)
				table[i] = reflect(table[i]);
		}
	}
	W operator[] (int index) const {
		return table[index];
	}

	W hash(const void *data) {
		const auto buffer = static_cast<const std::uint8_t*>(data);

		auto res = W(init);
		for(auto i = 0; buffer[i]; i++) {
			res = (res << 8)
				  ^ table[(res >> (sizeof(W)-1)*8) ^ buffer[i]];
		}
		return refOut ? (~res ^ xorOut) : (res ^ xorOut);
		//return res ^ xorOut;
	}
};

using crc_16_ccitt	= crc<std::uint16_t, 0x1021, 0xffff, false, false, 0x0>;
using crc_16_arc	= crc<std::uint16_t, 0x8005, 0x0, true, true, 0x0>;
using crc_32		= crc<std::uint32_t, 0x04C11DB7, 0xFFFFFFFF, true, true, 0xFFFFFFFF>;
using crc_32_bzip2	= crc<std::uint32_t, 0x04C11DB7, 0xFFFFFFFF, false, false, 0xFFFFFFFF>;
using crc_32_posix	= crc<std::uint32_t, 0x04C11DB7, 0x0, false, false, 0xFFFFFFFF>;

} // namespace crc














/*
namespace crc {

template<typename T>
T reflect(T val, int len = 0) {
	auto result = T(0);
	char ch = len != 0 ? len : sizeof(T)*8;
	for(int i = 0; i < ch; i++) {
		if(val & (1 << i)) {
			result |= 1 << ((ch - 1) - i);
		}
		//val >>= 1;
	}
	return result;
}

template<typename W, W poly, bool refIn>
W *initCrcTable() {
	W *t = new W[256];
	for(auto i = 0; i < 0xff; i++) { // 0xFF = 255
		t[i] = (refIn ? reflect(i, 8) : i) << (sizeof(W)-1)*8;
		for(auto j = 0; j < 8; j++) {
			t[i] = (t[i] << 1) ^ (t[i] & (1 << (sizeof(W)*8-1)) ? poly : 0);
		}
		if(refIn) t[i] = reflect(t[i]);
	}
	return t;
}

template<typename W, W poly, bool refIn>
struct s_crc_table {
private:
	W *_table;
public:
	constexpr s_crc_table() {
		_table = initCrcTable<W, poly, refIn>();
	}

	W operator[] (int index) {
		return _table[index];
	}
};

template<typename W, W Poly, W Init, bool RefIn, bool RefOut, W XorOut>
struct crc {
	crc() {
		std::cout << "crc  " << std::endl;
		std::cout << "    Poly  : " << std::hex << Poly << std::endl
				  << "    Init  : " << Init << std::endl
				  << "    RefIn : " << RefIn << std::endl
				  << "    RefOut: " << RefOut << std::endl
				  << "    XorOut: " << XorOut << std::endl
				  << std::dec
				  << "    sizeof: " << sizeof(W)*8
				  << std::hex << std::endl;
	}

	W hash(const void *data) {
		const auto buffer = static_cast<const std::uint8_t*>(data);

		auto crc = W(Init);
		for(int i = 0; buffer[i]; i++) {
/*			std::cout << std::hex << std::endl
//					  << "    crc : " << crc << std::endl
//					  << "    crc >> (sizeof(W)-1)*8) : " << (crc >> (sizeof(W)-1)*8) << std::endl
//					  << "    crc << (sizeof(W)-1)*8 : " << (crc << (sizeof(W)-1)*8) << std::endl
//					  << "    buffer[i]: " << buffer[i] << std::endl
//					  << "    (crc >> (sizeof(W)-1)*8)) ^ buffer[i] : " << ((crc >> (sizeof(W)-1)*8) ^ buffer[i]) << std::endl
//					  << "    table[(crc >> (sizeof(W)-1)*8) ^ buffer[i]] " << (table[(crc >> (sizeof(W)-1)*8) ^ buffer[i]]) << std::endl;
*
			crc = (crc << ((sizeof(W)-1)*8)) ^ table[(crc >> ((sizeof(W)-1)*8)) ^ buffer[i]];
		}

//		return RefOut ? (~crc ^ XorOut) : (crc ^ XorOut);
		return crc ^ XorOut;
	}
private:
	s_crc_table<W, Poly, RefIn> table;
};


using crc32_posix = crc<std::uint32_t, 0x04c11db7, 0x0, false, false, 0xFFFFFFFF>;
using crc32 = crc<std::uint32_t, 0x04c11db7, 0xFFFFFFFF, true, true, 0xffffffff>;

using crc16_ccitt_false = crc<std::uint16_t, 0x1021, 0xFFFF, false, false, 0x0>;
using crc16_aug_ccitt = crc<std::uint16_t, 0x1021, 0x1d0f, false, false, 0x0>;
using crc16_buypass = crc<std::uint16_t, 0x8005, 0x0, false, false, 0x0>;

using crc16_dnp = crc<std::uint16_t, 0x3D65, 0x0, true, true, 0xffff>;

using crc8 = crc<std::uint8_t, 0x07, 0x00, false, false, 0x0>;
using crc8_drac = crc<std::uint8_t, 0x39, 0x00, true, true, 0x0>;

} // namespace crc
*/
#endif // LIB_CRC_H
