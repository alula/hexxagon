# Projekt PJC - gra Hexxagon

./gen_resources.sh regeneruje plik src/gui/sprites.bmp.h ktory po prostu zawiera teksture sprites.bmp. 
Plik w wyslanym projekcie juz wygenerowany wiec nie trzeba tego robic.

Projekt sklada sie z 2 programow:
- hexxagon_cli.exe - konsolowa wersja gry
- hexxagon_gui.exe - wersja graficzna gry

Projekt zostal napisany w C++20, z uzyciem biblioteki SDL2.

Przetestowano na nastepujacych systemach/kompilatorach:

- Windows 11 + Microsoft Visual Studio 2022 (Nie gwarantuje ze bedzie dzialac na MinGW!)
- Debian GNU/Linux 11 + GCC 12
- Debian GNU/Linux 11 + Clang 15