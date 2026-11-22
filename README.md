# RPG (Kastil Terkutuk)

## Fungsi
Game 2D berbasis petak dengan mekanisme pertarungan bergiliran yang berfokus pada eksplorasi area kastil dan dungeon.

## Bahasa dan Stack
C++17, SFML 2.6, CMake 3.16

## Cara Build dan Menjalankan
Buat direktori build dan masuk ke dalamnya:
mkdir build && cd build

Konfigurasi proyek:
cmake ..

Kompilasi kode sumber:
cmake --build .

Jalankan executable game:
./game

## Fitur Utama
- Eksplorasi peta kastil dan dungeon menggunakan sistem pergerakan berbasis petak.
- Pergantian karakter aktif di dalam party antara Reyn dan Mira.
- Sistem pertarungan bergiliran dengan perintah serang biasa, skill berkonsumsi MP, dan lari.
- Sistem perolehan poin pengalaman, kenaikan level, serta peningkatan status HP dan serangan.
- Dialog interaktif dengan karakter non pemain.
- Rintangan lubang jebakan yang mengembalikan posisi pemain.
- Mode debug F1 untuk menampilkan kotak batas tabrakan objek, portal, dan jebakan.

## Status
Stabil

## Lisensi
MIT
