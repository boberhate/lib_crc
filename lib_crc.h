#ifndef LIB_CRC_H
#define LIB_CRC_H

/* как я хочу использовать эту библиотеку
 *
 * Генерация таблицы происходит на этапе компиляции,
 * поэтому не надо будет тратить время на ее вычисление
 *
 * // список параметров шаблона
 * // < Width - размер слова (ширина),
 * //   Poly - полином,
 * //   Init - инициализирующее значение,
 * //   RefIn - флаг, определяющий порядок бит для начала вычислений:
 *              false - со старшего бита,
 *              true - с младшего,
 * //   RefOut - флаг, определяющий необходимость инвертации порядка битов регистрац при входе на XOR,
 * //   XorOut - число, с которым складывается по модулю 2 результат
 *               (если отсутствует, то по умолчанию NULL, если NULL, то игнорируется)>
 *
 */

#include <iostream>


namespace crc {

// реверс байт в числе
template<typename T>
constexpr T reflect(T val, int len = 0) {
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
			if constexpr (refIn) {
				table[i] = reflect(i, 8) << (sizeof (W)-1)*8;
			} else {
				table[i] = i << (sizeof (W)-1)*8;
			}
			for(auto j = 0; j < 8; j++) {
				table[i] = (table[i] << 1)
						   ^ (table[i] & (1 << (sizeof(W)*8-1)) ? poly : 0);
			}
			if constexpr (refIn) {
				table[i] = reflect(table[i]);
			}
		}
	}
	W operator[] (int index) const {
		return table[index];
	}

	W checksum(const void *data) const {
		const auto buffer = static_cast<const std::uint8_t*>(data);

		auto res = W(init);
		for(auto i = 0; buffer[i]; i++) {
			if constexpr (refOut) {
				res = (res >> 8) ^ table[(res & 0x0ff) ^ buffer[i]];
			} else {
				res = (res << 8) ^ table[(res >> (sizeof(W)-1)*8) ^ buffer[i]];
			}
		}
		return res ^ xorOut;
	}

	std::uint8_t *encode(const void *data, int len=0) const {
		const auto buffer = static_cast<const std::uint8_t*>(data);

		if(len == 0) {
			while(buffer[len++]);
		}

		auto sum = checksum(data);
		std::uint8_t *msg = (std::uint8_t*)malloc(len + sizeof(W) + 1);

		// message body
		for(auto i = 0; i < len; i++) {
			msg[i] = buffer[i];
		}
		// crc body
		std::uint8_t *symbol_sum = (std::uint8_t*)malloc(sizeof(W));
		for(auto i = 0; i < sizeof(W); i++) {
			msg[len + i] = ((const std::uint8_t*)&sum)[i];
			symbol_sum[i] = ((const std::uint8_t*)&sum)[i];
		}

		return msg;
	}

};

using crc_8			= crc<std::uint8_t, 0x07, 0x0, false, false, 0x0>;
using crc_8_cdma	= crc<std::uint8_t, 0x9b, 0xff, false, false, 0x0>;
using crc_8_darc	= crc<std::uint8_t, 0x39, 0x0, true, true, 0x0>;
using crc_16_ccitt	= crc<std::uint16_t, 0x1021, 0xffff, false, false, 0x0>;
using crc_16_arc	= crc<std::uint16_t, 0x8005, 0x0, true, true, 0x0>;
using crc_32		= crc<std::uint32_t, 0x04C11DB7, 0xFFFFFFFF, true, true, 0xFFFFFFFF>;
using crc_32_bzip2	= crc<std::uint32_t, 0x04C11DB7, 0xFFFFFFFF, false, false, 0xFFFFFFFF>;
using crc_32_posix	= crc<std::uint32_t, 0x04C11DB7, 0x0, false, false, 0xFFFFFFFF>;

} // namespace crc

#endif // LIB_CRC_H
