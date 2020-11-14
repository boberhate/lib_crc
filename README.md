# lib_crc 
## Вычисление CRC сумм с помощью таблиц, сгенерированных во время компиляции
### Описание
Для использования необходимой спецификации алгоритма необходимо вызвать шаблон класса `crc` с нужными параметрами:
- `typename W` - ширина слова, то есть размер контрольной суммы
- `W poly` - полином
- `W init` - инициализация числа crc (обычно `0xFF` или `0x0`)
- `bool refIn` - реверс бит на входе при инициализации таблицы
- `bool refOut` - реверс бит во время вычисления контрольной суммы
- `W xorOut` - число, с которым выполняется XOR (исключающее ИЛИ) на выходе

**Например, crc32/MPEG-2**
`crc<std::uint32_t, 0x04C11DB7, 0xFFFFFFFF, false, false, 0x0>`

### Пример использования
```c++
using crc32_mpeg2 = crc::crc<std::uint32_t, 0x04C11DB7, 0xFFFFFFFF, false, false, 0x0>;

...

std::string some_data = "I need your clothes, your boots, and your motocycle";
auto crc_sum = crc32_mpeg2().checksum(some_data.c_str());       // 0xa9cd0f0d

```

### Доделать
- [ ] кодирование строки (добавление к концу строки crc суммы)
- [ ] проверка целостности строки (достать crc сумму и сравнить строку с ней)
- [ ] проверка целостности данных (по входным данным и входной crc сумме проверить на наличие ошибок)