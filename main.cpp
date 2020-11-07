#include <QCoreApplication>
#include <iostream>
#include "lib_crc.h"

using namespace crc;


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	std::cout << "CRC на основе метапрограммирования" << std::endl;

	std::string check1 = "Hello World";

	for(int i = 0; i < 256; i++) {
		std::cout << std::dec << i << " : " << std::hex << crc_32()[i] << '\t';
		if(i % 4 == 0)
			std::cout << std::endl;
	}

	for(int i = 0; i < 256; i++) {
		std::cout << std::dec << i << " : " << std::hex << crc_16_arc()[i] << '\t';
		if(i % 4 == 0)
			std::cout << std::endl;
	}

	for(int i = 0; i < 256; i++) {
		std::cout << std::dec << i << " : " << std::hex << crc_16_ccitt()[i] << '\t';
		if(i % 4 == 0)
			std::cout << std::endl;
	}

	for(int i = 0; i < 256; i++) {
		std::cout << std::dec << i << " : " << std::hex << crc_32_posix()[i] << '\t';
		if(i % 4 == 0)
			std::cout << std::endl;
	}

	std::cout << std::endl
			  << "crc_16_arc\t0x8005\t\t0x0\t\ttrue\ttrue\t0x0"
			  << std::endl << crc_16_arc().hash(check1.c_str()) << std::endl;
	std::cout << std::endl
			  << "crc_16_ccitt\t0x1021\t\t0xffff\t\tfalse\tfalse\t0xffff\n"
			  << crc_16_ccitt().hash(check1.c_str()) << std::endl;
	std::cout << std::endl
			  << "crc_32\t\t0x04C11DB7\t0xFFFFFFFF\ttrue\ttrue\t0xFFFFFFFF\n"
			  << crc_32().hash(check1.c_str()) << std::endl;
	std::cout << std::endl
			  << "crc_32_bzip2\t0x04C11DB7\t0xFFFFFFFF\tfalse\tfalse\t0xFFFFFFFF\n"
			  << crc_32_bzip2().hash(check1.c_str()) << std::endl;
	std::cout << std::endl
			  << "crc_32_bzip2\t0x04C11DB7\t0xFFFFFFFF\tfalse\tfalse\t0xFFFFFFFF\n"
			  << crc_32_posix().hash(check1.c_str()) << std::endl;


	return a.exec();
}
