#include <QCoreApplication>
#include <iostream>
#include "lib_crc.h"

using namespace crc;


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	std::cout << std::hex << "CRC metaprogramming" << std::endl;

	std::string check1 = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

	auto crc8 = crc_8();
	auto crc8_cdma = crc_8_cdma();
	auto crc8_darc = crc_8_darc();
	auto crc16_arc = crc_16_arc();
	auto crc16_ccitt = crc_16_ccitt();
	auto crc32 = crc_32();
	auto crc32_bzip2 = crc_32_bzip2();
	auto crc32_posix = crc_32_posix();

	std::cout << std::endl
			  << "crc_8\t\t0x8005\t\t0x0\t\ttrue\ttrue\t0x0"
			  << std::endl << (int)crc8.checksum(check1.c_str()) << std::endl;
	std::cout << std::endl
			  << "crc_8_cdma\t0x8005\t\t0x0\t\ttrue\ttrue\t0x0"
			  << std::endl << (int)crc8_cdma.checksum(check1.c_str()) << std::endl;
	std::cout << std::endl
			  << "crc_8_darc\t0x8005\t\t0x0\t\ttrue\ttrue\t0x0"
			  << std::endl << (int)crc8_darc.checksum(check1.c_str()) << std::endl;
	std::cout << std::endl
			  << "crc_16_arc\t0x8005\t\t0x0\t\ttrue\ttrue\t0x0"
			  << std::endl << crc16_arc.checksum(check1.c_str()) << std::endl;
	std::cout << std::endl
			  << "crc_16_ccitt\t0x1021\t\t0xffff\t\tfalse\tfalse\t0xffff\n"
			  << crc16_ccitt.checksum(check1.c_str()) << std::endl;
	std::cout << std::endl
			  << "crc_32\t\t0x04C11DB7\t0xFFFFFFFF\ttrue\ttrue\t0xFFFFFFFF\n"
			  << crc32.checksum(check1.c_str()) << std::endl;
	std::cout << std::endl
			  << "crc_32_bzip2\t0x04C11DB7\t0xFFFFFFFF\tfalse\tfalse\t0xFFFFFFFF\n"
			  << crc32_bzip2.checksum(check1.c_str()) << std::endl;
	std::cout << std::endl
			  << "crc_32_bzip2\t0x04C11DB7\t0xFFFFFFFF\tfalse\tfalse\t0xFFFFFFFF\n"
			  << crc32_posix.checksum(check1.c_str()) << std::endl;


	char c[] = {'a', 'b', 'c', 0x0};
	std::string s = "abc";

	std::cout << crc16_ccitt.checksum(&c[0]) << std::endl;
	std::cout << crc16_ccitt.checksum(s.c_str()) << std::endl;


	return a.exec();
}
