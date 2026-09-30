#!/bin/bash
set -e

# Компиляция кода
pio run

cd .pio/build/esp32dev

# Сборка в единый flash-образ
/opt/homebrew/opt/python@3.10/bin/python3.10 -m esptool \
  --chip esp32 merge-bin \
  -o flash-image.bin \
  --flash-mode dio \
  --flash-freq 40m \
  --flash-size 4MB \
  --pad-to-size 4MB \
  0x1000 bootloader.bin \
  0x8000 partitions.bin \
  0x10000 firmware.bin

# Активация окружения esp-idf
source "$HOME/esp/esp-idf/export.sh"

# Запуск эмулятора
qemu-system-xtensa -nographic -machine esp32 -drive file=flash-image.bin,if=mtd,format=raw