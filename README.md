## Kastil Terkutuk - RPG

**Kastil Terkutuk** adalah sebuah game RPG 2D yang dikembangkan menggunakan **C++** dan library **SFML**. Game ini mengusung genre eksplorasi dungeon dengan sistem pertarungan *turn-based* (bergiliran) bergaya klasik.

## Fitur Utama
* **Eksplorasi Dunia (Tile-based movement):** Jelajahi dua peta utama, yaitu Kastil dan Dungeon.
* **Sistem Party:** Mainkan dua karakter (Reyn dan Mira) yang dapat diganti kapan saja saat eksplorasi.
* **Sistem Pertarungan Turn-Based (Random Encounter):** Hadapi 24 jenis monster yang berbeda di dalam dungeon dengan menggunakan perintah Serang (Attack), Skill (membutuhkan MP), atau Lari (Flee).
* **Sistem Leveling:** Kalahkan monster untuk mendapatkan EXP, tingkatkan Level, Maksimum HP, dan status Serangan (ATK).
* **Interaksi NPC & Dialog:** Sistem dialog sederhana untuk berinteraksi dengan NPC di dalam map.
* **Rintangan Lingkungan:** Hindari jebakan (lubang mematikan) atau karaktermu akan terlempar kembali ke awal!
* **Debug Mode:** Fitur bawaan untuk menampilkan *hitbox* dari tembok, portal, dan jebakan (berguna untuk *development*).

## Karakter
1. **Reyn** (HP: 120, MP: 30, ATK: 22) - Petarung fisik.
2. **Mira** (HP: 80, MP: 80, ATK: 14) - Pengguna sihir (Skill).

## Kontrol Permainan
### Eksplorasi:
* **Tombol Panah (Up/Down/Left/Right):** Bergerak.
* **Tab:** Mengganti karakter aktif (Reyn <-> Mira).
* **Enter / Spasi:** Berinteraksi dengan NPC (Membaca dialog) atau melanjutkan dialog.
* **F1:** Mengaktifkan/menonaktifkan Debug Mode (menampilkan kotak *collision* merah untuk dinding, hijau untuk portal, biru untuk jebakan).
* **Esc:** Keluar dari permainan.

### Pertarungan (Battle):
* **[1]** Serang (Attack biasa)
* **[2]** Skill (Konsumsi 10 MP, menghasilkan kerusakan lebih besar)
* **[3]** Lari (Kabur dari pertarungan)

## Persyaratan Sistem (Prerequisites)
Untuk melakukan kompilasi (*build*) kode sumber permainan, pastikan sistem Anda memiliki:
* **C++ Compiler** yang mendukung standar **C++17**.
* **CMake** (minimum versi 3.16).
* **SFML** (Simple and Fast Multimedia Library) versi **2.6** (Modul Graphics, Window, dan System wajib dipasang).

## Link Youtube
https://youtu.be/YUOKwTWtZgk
