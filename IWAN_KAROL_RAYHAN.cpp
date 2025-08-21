#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Fungsi garis
void garis(char simbol, int jumlah) {
    for (int i = 0; i < jumlah; i++) printf("%c", simbol);
    printf("\n");
}

// Fungsi tampilkan papan
void tampilkan_papan(char papan[3][3]) {
    system("cls");
    printf("\n\t\t\t\tTIC-TAC-TOE\n");
    garis('-', 80);
    printf("\n__________ __________ __________");
    printf("\n          |          |");
    for (int i = 0; i < 3; i++) {
        printf("\n    %c     |    %c     |    %c\n", papan[i][0], papan[i][1], papan[i][2]);
        for (int j = 0; j < 3; j++) {
            printf("__________|");
        }
        if (i != 2) {
            printf("\n          |          |");
            printf("\n          |          |");
        }
    }
    printf("\n");
}

// Fungsi cek menang
int cek_menang(char simbol, char papan[3][3]) {
    for (int i = 0; i < 3; i++) {
        if ((papan[i][0] == simbol && papan[i][1] == simbol && papan[i][2] == simbol) ||
            (papan[0][i] == simbol && papan[1][i] == simbol && papan[2][i] == simbol))
            return 1;
    }
    if ((papan[0][0] == simbol && papan[1][1] == simbol && papan[2][2] == simbol) ||
        (papan[0][2] == simbol && papan[1][1] == simbol && papan[2][0] == simbol))
        return 1;
    return 0;
}

// Bot mudah
void bot_mudah(char papan[3][3], char simbol_bot) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (papan[i][j] != 'X' && papan[i][j] != 'O') {
                papan[i][j] = simbol_bot;
                return;
            }
}

// Bot medium
void bot_medium(char papan[3][3], char simbol_bot, char simbol_pemain) {
    // 1. Coba menang
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (papan[i][j] != 'X' && papan[i][j] != 'O') {
                char simpan = papan[i][j];
                papan[i][j] = simbol_bot;
                if (cek_menang(simbol_bot, papan)) return;
                papan[i][j] = simpan;
            }

    // 2. Blokir pemain
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (papan[i][j] != 'X' && papan[i][j] != 'O') {
                char simpan = papan[i][j];
                papan[i][j] = simbol_pemain;
                if (cek_menang(simbol_pemain, papan)) {
                    papan[i][j] = simbol_bot;
                    return;
                }
                papan[i][j] = simpan;
            }

    // 3. Pilih langkah acak (first available)
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (papan[i][j] != 'X' && papan[i][j] != 'O') {
                papan[i][j] = simbol_bot;
                return;
            }
}

// Tambahan: Fungsi cek seri
int cek_seri(char papan[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (papan[i][j] != 'X' && papan[i][j] != 'O')
                return 0;
    return 1;
}

// Fungsi brute force untuk bot hard
int brute_force(char papan[3][3], char simbol_bot, char simbol_pemain, int isBotTurn) {
    if (cek_menang(simbol_bot, papan)) return 1;      // bot menang
    if (cek_menang(simbol_pemain, papan)) return -1;  // pemain menang
    if (cek_seri(papan)) return 0;                   // seri

    if (isBotTurn) {
        int hasil = -2;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (papan[i][j] != 'X' && papan[i][j] != 'O') {
                    char simpan = papan[i][j];
                    papan[i][j] = simbol_bot;
                    int nilai = brute_force(papan, simbol_bot, simbol_pemain, 0);
                    papan[i][j] = simpan;
                    if (nilai == 1) return 1;
                    if (nilai > hasil) hasil = nilai;
                }
            }
        }
        return hasil;
    } else {
        int hasil = 2;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (papan[i][j] != 'X' && papan[i][j] != 'O') {
                    char simpan = papan[i][j];
                    papan[i][j] = simbol_pemain;
                    int nilai = brute_force(papan, simbol_bot, simbol_pemain, 1);
                    papan[i][j] = simpan;
                    if (nilai == -1) return -1;
                    if (nilai < hasil) hasil = nilai;
                }
            }
        }
        return hasil;
    }
}

void bot_hard(char papan[3][3], char simbol_bot, char simbol_pemain) {
    int bestRow = -1;
    int bestCol = -1;
    int bestScore = -2;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (papan[i][j] != 'X' && papan[i][j] != 'O') {
                char simpan = papan[i][j];
                papan[i][j] = simbol_bot;
                int score = brute_force(papan, simbol_bot, simbol_pemain, 0);
                papan[i][j] = simpan;

                if (score > bestScore) {
                    bestScore = score;
                    bestRow = i;
                    bestCol = j;
                }
            }
        }
    }

    if (bestRow != -1 && bestCol != -1) {
        papan[bestRow][bestCol] = simbol_bot;
    }
}

// Fungsi input pemain
void input_pemain(char papan[3][3], char nama[20], char simbol_pemain, int tingkat_bot) {
    char pilihan, simbol_bot;
    int giliran, sukses = 0;
    int menang_pemain = 0, menang_bot = 0;

    simbol_bot = (simbol_pemain == 'X' || simbol_pemain == 'x') ? 'O' : 'X';

    for (giliran = 1; giliran <= 9; giliran++) {
        sukses = 0;
        if (giliran % 2 != 0) {
            printf("\n%s, masukkan pilihanmu: ", nama);
            pilihan = getch();
            printf("%c", pilihan);

            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    if (pilihan == papan[i][j]) {
                        papan[i][j] = simbol_pemain;
                        sukses = 1;
                        menang_pemain = cek_menang(simbol_pemain, papan);
                    }

            if (!sukses) {
                printf("\nNiat main gaa??.");
                giliran--;
                continue;
            }
        } else {
            printf("\nBOT mikir dulu ya...");
            _sleep(400);

            if (tingkat_bot == 1)
                bot_mudah(papan, simbol_bot);
            else if (tingkat_bot == 2)
                bot_medium(papan, simbol_bot, simbol_pemain);
            else
                bot_hard(papan, simbol_bot, simbol_pemain);

            menang_bot = cek_menang(simbol_bot, papan);
        }

        tampilkan_papan(papan);

        if (menang_pemain) {
            garis('-', 70);
            printf("%s MENANG!\n", nama);
            garis('-', 70);
            return;
        }
        if (menang_bot) {
            garis('-', 70);
            printf("BOT MENANG!\n");
            garis('-', 70);
            return;
        }
    }

    printf("\nPermainan seri. Tidak ada pemenang.");
}

// Tutorial BOT vs BOT (tetap sama)
void tutorial_bot_vs_bot() {
    char papan[3][3] = {
        {'1','2','3'},
        {'4','5','6'},
        {'7','8','9'}
    };

    char simbol_X = 'X';
    char simbol_O = 'O';
    int giliran, menang_X = 0, menang_O = 0;

    system("cls");
    printf("\nTUTORIAL: BOT X vs BOT O\n");
    printf("----------------------------------------\n");

    tampilkan_papan(papan);
    printf("\nGame akan dimulai dalam 3 detik...\n");
    _sleep(1000);
    printf("\nGame akan dimulai dalam 2 detik...\n");
    _sleep(1000);
    printf("\nGame akan dimulai dalam 1 detik...\n");
    _sleep(1000);

    for (giliran = 1; giliran <= 9; giliran++) {
        system("cls");
        printf("\nTUTORIAL: BOT X vs BOT O\n");
        printf("----------------------------------------\n");
        tampilkan_papan(papan);

        printf("\nGiliran ke-%d: ", giliran);
        printf("BOT %c sedang berpikir...\n", (giliran % 2 != 0) ? simbol_X : simbol_O);
        _sleep(1500);

        if (giliran % 2 != 0) {
            bot_medium(papan, simbol_X, simbol_O);
        } else {
            bot_medium(papan, simbol_O, simbol_X);
        }

        system("cls");
        printf("\nTUTORIAL: BOT X vs BOT O\n");
        printf("----------------------------------------\n");
        tampilkan_papan(papan);

        menang_X = cek_menang(simbol_X, papan);
        menang_O = cek_menang(simbol_O, papan);

        if (menang_X) {
            printf("\nBOT X MENANG!\n");
            break;
        }
        if (menang_O) {
            printf("\nBOT O MENANG!\n");
            break;
        }

        printf("\nTekan tombol apa saja untuk lanjut ke giliran berikutnya...");
        getch();
    }

    if (!menang_X && !menang_O) {
        printf("\nPermainan seri. Tidak ada pemenang.\n");
    }

    printf("\nTekan tombol apa saja untuk kembali ke menu utama...");
    getch();
}

// Main program
int main() {
    int menu_pilihan, tingkat_bot;
    char nama_pemain[20], simbol;
    char ulangi;

    do {
        system("cls");
        printf("\n\t\t\t\tTIC-TAC-TOE\n");
        garis('-', 80);

        printf("\nMenu:\n");
        printf("1. Mulai Game\n");
        printf("2. Tutorial\n");
        printf("3. Keluar\n");
        printf("Pilihan (1-3): ");
        scanf("%d", &menu_pilihan);

        if (menu_pilihan == 1) {
            do {
                system("cls");
                char papan[3][3] = {
                    {'1','2','3'},
                    {'4','5','6'},
                    {'7','8','9'}
                };

                fflush(stdin);
                printf("\nPilih tingkat kesulitan bot:\n");
                printf("1. EASY\n");
                printf("2. MEDIUM\n");
                printf("3. HARD\n");
                printf("Pilihan (1-3): ");
                scanf("%d", &tingkat_bot);

                if (tingkat_bot < 1 || tingkat_bot > 3) {
                    printf("\nPilihan tidak valid.");
                    getch();
                    break;
                }

                fflush(stdin);
                printf("\nMasukkan nama pemain: ");
                gets(nama_pemain);

                do {
                    printf("\nPilih simbol kamu (X atau O): ");
                    scanf(" %c", &simbol);
                } while (!(simbol == 'X' || simbol == 'O' || simbol == 'x' || simbol == 'o'));

                tampilkan_papan(papan);
                input_pemain(papan, nama_pemain, simbol, tingkat_bot);

                printf("\n\nIngin bermain lagi? (Y/N): ");
                scanf(" %c", &ulangi);
            } while (ulangi == 'Y' || ulangi == 'y');
        }
        else if (menu_pilihan == 2) {
            tutorial_bot_vs_bot();
        }
        else if (menu_pilihan == 3) {
            printf("\nTerima kasih telah bermain. Sampai jumpa!\n");
            break;
        }
        else {
            printf("\nPilihan tidak valid!");
            getch();
        }
    } while (menu_pilihan != 3);

    return 0;
}

