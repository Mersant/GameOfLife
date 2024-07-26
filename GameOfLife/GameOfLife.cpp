#include <iostream>
#include <bitset>
#include <bit>
#include <cstdint>
#include <format>

inline uint16_t generateMask(uint16_t mask, uint8_t col) {
    mask <<= (14 - col);
    if (col == 15) {
        mask |= 0x8000U;
    } else if (col == 0) {
		mask |= 1U;
	}
    return mask;
}

void PrintMap(uint16_t gameMap[16]) {
	for (int i = 0; i < 16; i++) {
		std::bitset<16> row(gameMap[i]);
		for (int i = 15; i > 0; i--) {
			std::cout << (row[i] ? " \033[31mX " : " \033[32m. ");
		}
		std::cout << std::endl;
	}
}

void Generation(uint16_t gameMap[16]) {
    uint16_t newMap[16] = { 0 };
    for (int row = 0; row < 16; row++) {
        uint16_t aboveRow = row == 0 ? gameMap[15] : gameMap[row - 1];
        uint16_t currentRow = gameMap[row];
        uint16_t belowRow = row == 15 ? gameMap[0] : gameMap[row + 1];
        for (int col = 0; col < 15; col++) {
            uint16_t yMask = generateMask(0b111, col);
            // Exclude the current cell
            uint16_t xMask = generateMask(0b101, col);

            uint8_t neighbors = std::popcount(uint16_t(aboveRow & yMask)) +
                                std::popcount(uint16_t(belowRow & yMask)) +
                                std::popcount(uint16_t(currentRow & xMask));
         
            bool isAlive = (currentRow << col) & 0x8000U;
            if ((isAlive && (neighbors == 2 || neighbors == 3)) || (!isAlive && neighbors == 3)) {
                newMap[row] |= (0x8000U >> col);
            }
        }
    }
    // Copy the new map back to the original
    std::copy(std::begin(newMap), std::end(newMap), gameMap);
}

int main() {
    uint16_t map[16] = { 0, 0b0010000000000000, 0b0001000000000000, 0b0011100000000000 };
    while (true) {
        PrintMap(map);
        Generation(map);
        std::cout << "\nPress Enter for next generation...";
        std::cin.get();
        std::cout << "\033[2J\033[1;1H";
    }
    return 0;
}