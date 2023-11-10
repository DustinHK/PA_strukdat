#include <iostream>  // Input-output stream
#include <string>    // Library untuk penggunaan string
#include <vector>    // Library untuk penggunaan vektor
#include <stack>     // Library untuk penggunaan stack
#include <iomanip>   // Library untuk manipulasi input-output, terutama untuk manipulasi angka (seperti presisi desimal)
#include <conio.h>   // Library khusus untuk beberapa fungsi pengontrolan konsol (biasanya digunakan di lingkungan Windows)
#include <windows.h> // Library untuk fungsi-fungsi khusus Windows, termasuk pengaturan kursor pada konsol
#include <limits>    // Library untuk batasan nilai numerik
#include <algorithm> // Library untuk penggunaan fungsi-fungsi algoritma seperti sorting dan searching
#include <sstream>   // Library untuk operasi pada string streams (seperti menggunakan stringstream)
#include <fstream>   // Library untuk operasi file eksternal, seperti membaca dan menulis file

using namespace std;

// Struktur data untuk simpul (node) dalam double linked list
int add_id = 0; // penghitung jumlah barang

// Struktur untuk merepresentasikan game dalam linked list
struct Game
{
    string name;
    Game *next;
};

// Struktur untuk merepresentasikan mata uang dalam game terkait
struct GameMoney
{
    string gameName; // Nama Game yang dihubungkan dengan Game Money ini
    string moneyName;
    int amount;
    double price;
    GameMoney *next;
};

Game *gamesHead = nullptr;          // Menyimpan head dari linked list game
GameMoney *gameMoneyHead = nullptr; // Menyimpan head dari linked list game money

// Struktur data untuk riwayat pembelian
struct PurchaseHistory
{
    string itemName;
    double itemPrice;
    string username;
    PurchaseHistory *next;
};

// Struktur data untuk node dalam linked list pengguna (user)
struct Node
{
    string username; // Nama pengguna (username)
    string password; // Kata sandi (password)
    int isAdmin;     // Variabel yang menandakan status admin (1 jika admin, 0 jika bukan)
    double saldo;    // Saldo pengguna (untuk transaksi, misalnya)
    Node *next;      // Pointer ke node pengguna selanjutnya
    Node *prev;      // Pointer ke node pengguna sebelumnya
};

// Struktur data untuk mewakili pembelian yang tertunda atau menunggu persetujuan
struct PendingPurchase
{
    string itemName;       // Nama barang atau item yang akan dibeli
    double itemPrice;      // Harga barang yang akan dibeli
    string username;       // Nama pengguna yang melakukan pembelian
    bool isApproved;       // Status persetujuan pembelian (true jika disetujui)
    PendingPurchase *next; // Pointer ke pembelian tertunda berikutnya
};

PendingPurchase *pendingPurchasesFront = nullptr; // Pointer ke depan (front) dari linked list pembelian tertunda
PendingPurchase *pendingPurchasesRear = nullptr;  // Pointer ke belakang (rear) dari linked list pembelian tertunda

bool cekhuruf(const string &str)
{
    // fungsi untuk mengecek apakah huruf itu true atau false
    // pada suatu kondisi tersebut
    for (char c : str)
    {
        if (!isalpha(c) && !isspace(c))
        {
            return false;
        }
    }
    return true;
}

// Menampilkan menu mata uang game berdasarkan daftar game yang tersedia
void gameMoneyMenus(GameMoney **head, Game *gameList);

// Menampilkan menu permainan yang tersedia
void showGameMenus(Game *head);

// Menampilkan menu mata uang game berdasarkan permainan yang dipilih
void showGameMoneyMenus(GameMoney *moneyList, const string &selectedGameName);

// Menampilkan menu mata uang game untuk admin
void showGameMoneyMenusAdmin(GameMoney *moneyList);

// Fungsi untuk menghitung jumlah node dalam linked list game
int lengthGame(Game *head)
{
    int jumlah = 0; // Variabel untuk menyimpan jumlah node
    while (head != NULL)
    {
        jumlah++;          // Menambahkan 1 ke jumlah setiap kali ada node
        head = head->next; // Memindahkan ke node selanjutnya
    }
    return jumlah; // Mengembalikan jumlah total node dalam linked list game
}

// Fungsi untuk menghitung jumlah node dalam linked list mata uang game
int lengthGameMoney(GameMoney *head)
{
    int jumlah = 0; // Variabel untuk menyimpan jumlah node
    while (head != NULL)
    {
        jumlah++;          // Menambahkan 1 ke jumlah setiap kali ada node
        head = head->next; // Memindahkan ke node selanjutnya
    }
    return jumlah; // Mengembalikan jumlah total node dalam linked list mata uang game
}

Node *Head = NULL; // Pointer ke kepala (head) dari linked list Node
Node *Tail = NULL; // Pointer ke ekor (tail) dari linked list Node

void clear(Node **head)
{                 // Berfungsi untk menghapus semua isi linked list
    *head = NULL; // Untuk refresh data
    return;
}

// Fungsi untuk membaca data dari file "data.csv" dan memasukkannya ke dalam linked list Node
void bacaData(Node **head, Node **tail)
{
    add_id = 0;   // Reset penghitung ID
    clear(&Head); // Menghapus semua data pada linked list

    string data;
    string usernames;
    string passwords;
    string isAdmins;
    string saldos;
    int isAdmin_i;
    double saldo_s;

    fstream file;
    file.open("data.csv", ios::in);

    if (file.is_open())
    {
        while (getline(file, data))
        {
            // Memeriksa jika baris data kosong atau memiliki karakter newline terakhir
            if (data.empty())
            {
                continue;
            }
            if (data[data.length() - 1] == '\n')
            {
                data.erase(data.length() - 1);
            }

            // Membaca baris data menggunakan stringstream
            stringstream ss(data);
            getline(ss, usernames, ',');
            getline(ss, passwords, ',');
            getline(ss, isAdmins, ',');
            getline(ss, saldos, ',');

            // Mengubah string ke tipe data yang sesuai
            stringstream saldo_string(saldos);
            stringstream isAdmins_string(isAdmins);
            isAdmins_string >> isAdmin_i;
            saldo_string >> saldo_s;

            // Membuat node baru
            Node *newNode = new Node;
            newNode->username = usernames;
            newNode->password = passwords;
            newNode->isAdmin = isAdmin_i;
            newNode->saldo = saldo_s;
            newNode->next = *head;
            newNode->prev = *tail;

            // Menambahkan node ke linked list
            if (*head == NULL)
            {
                *head = newNode;
                *tail = newNode;
            }
            else
            {
                (*tail)->next = newNode;
                *tail = newNode;
            }
            newNode->next = NULL;
            add_id++;
        }
        file.close();
    }
}

// Fungsi untuk menyimpan data ke dalam file "data.csv" dengan menulis ulang (overwrite)
void SimpanKeFile(Node *simpan)
{
    fstream file;
    file.open("data.csv", ios::out | ios::trunc); // Membuka file untuk menulis data, menimpa data sebelumnya
    while (simpan != NULL)
    {
        file << simpan->username << ","; // Menulis username
        file << simpan->password << ","; // Menulis password
        file << simpan->isAdmin << ",";  // Menulis status admin
        file << simpan->saldo << ",";    // Menulis saldo
        simpan = simpan->next;           // Pindah ke node selanjutnya
        file << "\n";                    // Baris baru untuk setiap entri pengguna
    }
    file.close(); // Menutup file setelah selesai menyimpan data
}

// NAMA GAME DATABASE
Game *gameList = nullptr; // Variabel untuk menyimpan daftar nama game

// Fungsi untuk menghapus semua data game dari daftar
void clearGame(Game **gameList)
{
    *gameList = nullptr;
}

// Fungsi untuk membaca nama game dari file "games.csv"
void BacaGameDariFile(Game **gameList)
{
    clearGame(gameList); // Menghapus semua data game dari daftar

    fstream file;
    file.open("games.csv", ios::in);

    if (file.is_open())
    {
        string data, nama;
        while (getline(file, data))
        {
            // Memeriksa jika baris data kosong atau memiliki karakter newline terakhir
            if (data.empty())
            {
                continue;
            }
            if (data[data.length() - 1] == '\n')
            {
                data.erase(data.length() - 1);
            }

            // Memproses baris data
            stringstream ss(data);
            getline(ss, nama, ',');

            // Membuat node baru untuk nama game dan menambahkannya ke daftar game
            Game *newGame = new Game;
            newGame->name = nama;
            newGame->next = *gameList;
            *gameList = newGame;
        }
        file.close();
    }
}

// Fungsi untuk menyimpan daftar nama game ke dalam file "games.csv" dengan menulis ulang (overwrite)
void SimpanGameKeFile(Game *gameList)
{
    fstream file;
    file.open("games.csv", ios::out | ios::trunc); // Membuka file untuk menulis data, menimpa data sebelumnya
    while (gameList != nullptr)
    {
        file << gameList->name << ","; // Menulis nama game
        gameList = gameList->next;     // Pindah ke node nama game selanjutnya
        file << "\n";                  // Baris baru untuk setiap entri nama game
    }
    file.close(); // Menutup file setelah selesai menyimpan data
}

// ADD MONEY GAME
GameMoney *moneyList = nullptr;
void clearGameMoney(GameMoney **moneyList)
{
    *moneyList = nullptr; // Menghapus semua data game dari daftar
}
// Fungsi untuk membaca data mata uang game dari file "item.csv"
void BacaGameMoneyDariFile(GameMoney **moneyList)
{
    clearGameMoney(moneyList); // Menghapus semua data mata uang game dari daftar

    string data;
    string gameNames;
    string moneyNames;
    string amounts;
    string prices;
    int amount_a;
    double price_p;

    fstream file;
    file.open("item.csv", ios::in);

    if (file.is_open())
    {
        while (getline(file, data))
        {
            // Memeriksa jika baris data kosong
            if (data.empty())
            {
                continue;
            }

            // Memproses baris data
            stringstream ss(data);
            getline(ss, gameNames, ',');
            getline(ss, moneyNames, ',');
            getline(ss, amounts, ',');
            getline(ss, prices, ',');

            // Konversi string ke tipe data yang sesuai
            stringstream amount_string(amounts);
            stringstream price_string(prices);
            amount_string >> amount_a;
            price_string >> price_p;

            // Membuat node baru untuk mata uang game dan menambahkannya ke daftar mata uang game
            GameMoney *newMoney = new GameMoney;
            newMoney->gameName = gameNames;
            newMoney->moneyName = moneyNames;
            newMoney->amount = amount_a;
            newMoney->price = price_p;
            newMoney->next = *moneyList;
            *moneyList = newMoney;
        }
        file.close();
    }
}

// Fungsi untuk menambahkan Game Money
GameMoney *addGameMoney(Game *gameList)
{
    GameMoney *newGameMoney = new GameMoney;
    GameMoney *moneyList = nullptr;

    string gameName;
    string moneyName;
    string priceStr;
    string amountStr;
    int amount;
    double price;

    while (true)
    {
    balik_input_gamemoney:
        cout << "Masukkan Nama Game yang Ingin Ditambahkan Money: ";
        getline(cin >> ws, gameName);

        // Validasi apakah nama game sesuai dengan yang ada dalam gameList
        Game *currentGame = gameList;
        bool gameFound = false;

        while (currentGame != nullptr)
        {
            if (currentGame->name == gameName)
            {
                gameFound = true;
                break;
            }
            currentGame = currentGame->next;
        }

        if (gameFound)
        {
            break;
        }
        else
        {
            cout << "Game tidak ada. Silakan masukkan nama game yang valid." << endl;
            system("pause");
            goto balik_input_gamemoney;
        }
    }

    cout << "Masukkan Nama Money Game: ";
    getline(cin >> ws, moneyName);

    while (true)
    {
        cout << "Masukkan Jumlah Unit Money Game: ";
        cin >> amountStr;

        if (isdigit(amountStr[0]))
        {
            amount = stoi(amountStr);
            break;
        }
        else
        {
            cout << "Masukkan harus berupa angka." << endl;
        }
    }

    while (true)
    {
        cout << "Masukkan Harga Money Game: ";
        cin >> priceStr;

        if (isdigit(priceStr[0]))
        {
            price = stod(priceStr);
            break;
        }
        else
        {
            cout << "Masukkan harus berupa angka." << endl;
        }
    }

    newGameMoney->gameName = gameName;
    newGameMoney->moneyName = moneyName;
    newGameMoney->amount = amount;
    newGameMoney->price = price;

    return newGameMoney;
}


// Fungsi untuk menyimpan data mata uang game ke dalam file "item.csv" dengan menulis ulang (overwrite)
void SimpanGameMoneyKeFile(GameMoney *moneyList)
{
    fstream file;
    file.open("item.csv", ios::out | ios::app); // Mode 'app' untuk menambahkan ke file tanpa menghapus yang sudah ada

    // Jika file tidak dapat dibuka, keluarkan pesan kesalahan
    if (!file.is_open())
    {
        cout << "Gagal membuka file item.csv" << endl;
        return;
    }

    // Tulis data yang sudah ada
    while (moneyList != nullptr)
    {
        file << moneyList->gameName << ",";
        file << moneyList->moneyName << ",";
        file << moneyList->amount << ",";
        file << moneyList->price << ",";
        file << "\n";
        moneyList = moneyList->next;
    }

    // Tambahkan data baru
    GameMoney *newGameMoney = addGameMoney(gameList);
    file << newGameMoney->gameName << ",";
    file << newGameMoney->moneyName << ",";
    file << newGameMoney->amount << ",";
    file << newGameMoney->price << ",";
    file << "\n";

    file.close();
}

void SimpanGameMoneyKeFile1(GameMoney *moneyList)
{
    fstream file;
    file.open("item.csv", ios::out | ios::trunc); // Menyimpan data game ke file dengan cara menulis ulang atau overwrite atau menimpa
    while (moneyList != nullptr)
    {
        file << moneyList->gameName << ",";
        file << moneyList->moneyName << ",";
        file << moneyList->amount << ",";
        file << moneyList->price << ",";
        moneyList = moneyList->next;
        file << "\n";
    }
    file.close();
}

// Fungsi untuk menambahkan pengguna baru ke dalam linked list Node
void addUser(Node *&head, Node *&tail, const string &username, const string &password, bool isAdmin = false)
{
    Node *newUser = new Node;
    newUser->username = username;
    newUser->password = password;
    newUser->isAdmin = isAdmin;
    newUser->saldo = 0; // Menginisialisasi saldo ke 0
    newUser->next = nullptr;

    if (head == nullptr)
    {
        // Jika linked list kosong, pengguna baru menjadi head dan tail
        head = newUser;
        tail = newUser;
        newUser->prev = nullptr;
    }
    else
    {
        // Jika linked list tidak kosong, pengguna baru ditambahkan ke bagian paling belakang
        tail->next = newUser;
        newUser->prev = tail;
        tail = newUser;
    }
}

// Fungsi untuk mencari akun pengguna atau admin berdasarkan username
Node *findUser(Node *head, const string &username)
{
    Node *current = head;
    while (current != nullptr)
    {
        if (current->username == username)
        {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Fungsi untuk mendaftar akun pengguna
void registerUser(Node *&head, Node *&tail)
{
    string username, password;
    cout << "Masukkan username: ";
    cin >> username;

    if (findUser(head, username) != nullptr)
    {
        cout << "Username sudah digunakan. Pilih username lain." << endl;
        system("pause");
        return;
    }

    cout << "Masukkan password: ";
    cin >> password;

    addUser(head, tail, username, password, false);
    add_id++;
    SimpanKeFile(head);
    cout << "Akun pengguna berhasil terdaftar." << endl;
    system("pause");
}

// Fungsi untuk login sebagai pengguna atau admin
Node *login(Node *head)
{
    // Make sure the data has been read
    // bacaData(&head);
    string username, password;
    cout << "Masukkan username: ";
    getline(cin, username);

    Node *user = findUser(head, username);

    if (user != nullptr)
    {
        cout << "Masukkan password: ";
        getline(cin, password);

        if (user->password == password)
        {
            if (user->isAdmin == 1)
            {
                cout << "Login admin berhasil." << endl;
                system("pause");
                return user;
                // kembali ke menu admin
            }
            else
            {
                cout << "Login pengguna berhasil." << endl;
                system("pause");
                return user; // kembali ke menu user
            }
        }
        else
        {
            cout << "Password salah." << endl;
            system("pause");
        }
    }
    else
    {
        cout << "User tidak ditemukan." << endl;
        system("pause");
    }

    return nullptr; // Return ke nullptr jika login gagal
}

// Fungsi untuk menambahkan game ke dalam linked list
Game *addGame()
{
    Game *newGame = new Game;

    string gameName;

    cout << "Masukkan Nama Game: ";
    getline(cin >> ws, gameName);

    newGame->name = gameName;

    return newGame;
}



// Bagian Penambahan Game
void addgameFirst(Game **head)
{
    // Membuat game baru dan menambahkannya ke bagian depan linked list
    Game *newGame = addGame();                          // Fungsi addGame() digunakan untuk membuat game baru
    newGame->next = *head;                              // Menambah game baru ke bagian depan (head) dari linked list
    *head = newGame;                                    // Memperbarui head dengan game baru
    SimpanGameKeFile(*head);                            // Menyimpan perubahan ke file eksternal
    cout << "Data Berhasil Dimasukkan Di Awal" << endl; // Pesan sukses
}



// END ADD GAME SECTION

// ADD GAME MONEY SECTION

void addgameMoneyFirst(GameMoney **head, Game *gameList)
{
    SimpanGameMoneyKeFile(*head);                       // Menyimpan perubahan ke file eksternal
    cout << "Data Berhasil Dimasukkan Di Awal" << endl; // Pesan keberhasilan
}


// UBAH GAME SECTION
void updategame(Game **head, Game *gameList)
{

    if (*head == NULL)
    {
        cout << "Data Kosong, Isi dulu" << endl;
        system("pause");
        return;
    }

    showGameMenus(gameList);

    string ubah;

    cout << "Masukan nama game yang ingin di ubah : ";
    getline(cin >> ws, ubah);

    string update_game;

    Game *temp = (*head);

    while (temp != NULL)
    {
        if (temp->name == ubah)
        {
            while (true)
            {
                try
                {
                    // menggunakan try and catch untuk error handling
                    cout << "Masukan Nama Game Baru\t\t: ";
                    getline(cin >> ws, update_game);
                    // memanggil fungsi cekhuruf untuk mengecek
                    // apakah huruf itu true atau false dalam kondisi itu
                    if (cekhuruf(update_game))
                    {
                        temp->name = update_game;
                        cout << "Nama Game Baru --> " << temp->name << " updated!\n";
                        // temp = gameName;               
                        SimpanGameKeFile(temp);

                        break; // jika Genre valid, maka keluar dari perulangan
                    }
                    else
                    {
                        throw invalid_argument("Inputan mengandung karakter selain huruf");
                    }
                }
                catch (const invalid_argument &e)
                {
                    cerr << e.what() << endl;
                    cerr << "Inputan harus huruf!" << endl;
                    system("pause");
                }
            }

            return;
        }
        temp = temp->next;
    }

    cout << " nama game tidak ditemukan " << endl;
}
// Fungsi mengubah nama game pada database item

// END OF UBAH GAME SECTION
// UBAH GAME MONEY SECTION
void updategameMoney(GameMoney **head, Game *gameList)
{

    showGameMenus(gameList);

    string ubah;

    cout << "Masukan nama game yang ingin di ubah : ";
    getline(cin >> ws, ubah);

    string update_money, update_amount_string, update_price_string;
    int update_amount, update_price;

    GameMoney *temp = (*head);

    while (temp != NULL)
    {
        if (temp->gameName == ubah)
        {

            cout << "pilih yang ingin diperbarui\n";
            cout << "1. Nama Mata Uang Game\n2. Jumlah Unit Mata Uang\n3. Harga" << endl;
            cout << "pilih : ";
            cin.ignore();

            string choice;
            getline(cin, choice);

            if (choice == "1")
            {
                while (true)
                {
                    try
                    {
                        // menggunakan try and catch untuk error handling
                        cout << "Masukan Nama Mata Uang Game Baru\t\t: ";
                        getline(cin >> ws, update_money);
                        // memanggil fungsi cekhuruf untuk mengecek
                        // apakah huruf itu true atau false dalam kondisi itu
                        if (cekhuruf(update_money))
                        {
                            temp->moneyName = update_money;
                            cout << "Nama Mata Uang Baru --> " << temp->moneyName << " updated!\n";
                            break; // jika Genre valid, maka keluar dari perulangan
                        }
                        else
                        {
                            throw invalid_argument("Inputan mengandung karakter selain huruf");
                        }
                    }
                    catch (const invalid_argument &e)
                    {
                        cerr << e.what() << endl;
                        cerr << "Inputan harus huruf!" << endl;
                        system("pause");
                    }
                }

                return;
            }

            else if (choice == "2")
            {
                while (true)
                {
                    try
                    {
                        // menggunakan try and catch untuk error handling
                        cout << "Masukan Jumlah Unit Mata Uang    : ";
                        getline(cin >> ws, update_amount_string);
                        update_amount = stoi(update_amount_string);
                        temp->amount = update_amount;
                        cout << "Episode  " << temp->amount << " updated!\n";
                        break;
                    }
                    catch (invalid_argument &e)
                    {
                        cerr << "Inputan jumlah unit harus berupa angka. Mohon coba lagi." << endl;
                    }
                }
            }

            else if (choice == "3")
            {
                while (true)
                {
                    try
                    {
                        // menggunakan try and catch untuk error handling
                        cout << "Masukan Harga Baru Untuk Mata Uang   : ";
                        getline(cin >> ws, update_price_string);
                        update_price = stoi(update_price_string);
                        temp->price = update_price;
                        cout << "Episode  " << temp->price << " updated!\n";
                        break;
                    }
                    catch (invalid_argument &e)
                    {
                        cerr << "Inputan Harga Harus Berupa Angka." << endl;
                    }
                }
            }
        }
        SimpanGameMoneyKeFile1(temp);
        temp = temp->next;
    }

    cout << " nama game tidak ditemukan " << endl;
}
// END OF UBAH MONEY SECTION

// berfungsi untuk menghapus game


// DELETE GAME SECTION


void deleteMidGame(Game **head)
{
    Game *current, *before, *toDelete;

    if (*head == NULL)
    {
        cout << "Data Kosong, Isi dulu" << endl;
        system("pause");
        return;
    }

    int data_select;
    cout << "Pilih data yang ingin dihapus: ";
    while (!(cin >> data_select))
    {
        cout << "harus berupa angka. Silakan coba lagi: ";
        system("pause");
        cin.clear();
        cin.ignore();
        return;
    }

    if (data_select < 1 || data_select > lengthGame(*head))
    {
        cerr << "Data tidak ditemukan" << endl;
        system("pause");
        return;
    }

    int data = 1;
    current = *head;
    before = NULL;  // Initialize before to NULL

    while (data <= data_select)
    {
        if (data == data_select - 1)
        {
            before = current;
        }

        if (data == data_select)
        {
            toDelete = current;
        }

        current = current->next;
        data++;
    }

    if (before != NULL)
    {
        // If before is not NULL, update the next pointer of the node before the one to be deleted
        before->next = current;
    }
    else
    {
        // If before is NULL, update the head of the linked list
        *head = current;
    }

    delete toDelete;
    SimpanGameKeFile(*head);
    cout << "Data berhasil dihapus" << endl;
    system("pause");
}


// END OF DELETE SELECTION

// DELETE GAME Money SECTION

void deleteMidMoneyGame(GameMoney **head, Game *gameList)
{
     GameMoney *current, *before, *toDelete;

    if (*head == NULL)
    {
        cout << "Data Kosong, Isi dulu" << endl;
        system("pause");
        return;
    }

    int data_select;
    cout << "Pilih data yang ingin dihapus: ";
    while (!(cin >> data_select))
    {
        cout << "harus berupa angka. Silakan coba lagi: ";
        system("pause");
        cin.clear();
        cin.ignore();
        return;
    }

    if (data_select < 1 || data_select > lengthGameMoney(*head))
    {
        cerr << "Data tidak ditemukan" << endl;
        system("pause");
        return;
    }

    int data = 1;
    current = *head;
    before = NULL;  

    while (data <= data_select)
    {
        if (data == data_select - 1)
        {
            before = current;
        }

        if (data == data_select)
        {
            toDelete = current;
        }

        current = current->next;
        data++;
    }

    if (before != NULL)
    {

        before->next = current;
    }
    else
    {

        *head = current;
    }

    delete toDelete;
    SimpanGameMoneyKeFile(*head);
    cout << "Data berhasil dihapus" << endl;
    system("pause");
}



// END OF DELETE Money SELECTION

// Fungsi untuk menampilkan menu game dan mata uang game
void showGameAndMoneyMenus(Game *head, GameMoney *moneyList)
{
    Game *currentGame = head;
    int gameCount = 1;

    cout << "=============================================\n";
    cout << "|              Daftar Game                    |\n";
    cout << "=============================================\n";

    while (currentGame != nullptr)
    {
        cout << "| " << gameCount << "."
             << "| " << currentGame->name << endl;
        cout << "---------------------------------------------\n";

        // Menampilkan daftar mata uang game untuk game saat ini
        showGameMoneyMenus(moneyList, currentGame->name);

        currentGame = currentGame->next;
        gameCount++;
    }

    cout << "=============================================\n";
}

// berfungsi untuk memilih game yang ada
Game *chooseGame(Game *head, int gameChoice, const string &gameName)
{
    Game *current = head;
    int currentGame = 1;

    while (current != nullptr)
    {
        if (current->name == gameName || currentGame == gameChoice)
        {
            return current;
        }
        current = current->next;
        currentGame++;
    }

    return nullptr;
}

// Fungsi untuk memilih game money
GameMoney *chooseGameMoney(GameMoney *head, int gameChoice)
{
    GameMoney *current = head;
    int currentGame = 1;

    while (current != nullptr)
    {
        if (currentGame == gameChoice)
        {
            return current;
        }
        current = current->next;
        currentGame++;
    }

    return nullptr;
}

// Fungsi untuk top up saldo
void topUpBalance(Node *user)
{
    string amount_string;
    LONGLONG amount;

balik_input_balance:
    try
    {
        cout << "Masukkan jumlah saldo yang ingin di-top up (Ketik 0 Jika Ingin Keluar): ";
        getline(cin >> ws, amount_string);
        if (amount_string == "0" || amount_string == "o")
        {
            return;
        }
        // Periksa apakah seluruh karakter dalam amount_string adalah digit
        if (!all_of(amount_string.begin(), amount_string.end(), ::isdigit))
        {
            throw invalid_argument("Inputan harus berupa angka");
        }

        amount = stoll(amount_string);
    }
    catch (invalid_argument &e)
    {
        cout << "Inputan harus berupa angka" << endl;
        system("pause");
        goto balik_input_balance;
    }

    if (amount <= 0)
    {
        cout << "Jumlah saldo tidak valid. Harap masukkan jumlah yang positif." << endl;
        system("pause");
        return;
    }

    // Maksimum saldo yang diperbolehkan
    LONGLONG maxbalance = 10000000;

    if ((user->saldo + amount) > maxbalance)
    {
        cout << "Top Up Saldo tidak dapat melebihi Rp. 10,000,000" << endl;
        system("pause");
        return;
    }

    user->saldo += amount;
    cout << "Top-up saldo berhasil. Saldo Anda sekarang: Rp. " << fixed << setprecision(2) << user->saldo << endl;
    system("pause");
}

// Fungsi untuk melihat saldo
void viewBalance(Node *user)
{
    cout << "Saldo Anda saat ini: Rp. " << fixed << setprecision(2) << user->saldo << endl;
    system("pause");
}

void displayPurchaseHistory(Node *currentUser, PurchaseHistory *purchaseHistory, PendingPurchase *pendingPurchases)
{
    if (purchaseHistory == nullptr)
    {
        cout << "Belum ada riwayat pembelian." << endl;
        system("pause");
    }
    else
    {
        cout << "Riwayat Pembelian " << currentUser->username << ":\n";
        stack<PurchaseHistory *> purchaseStack; // Gunakan stack untuk menyimpan riwayat pembelian

        // Langkah 1: Menambahkan pembelian dari purchaseHistory ke dalam stack
        PurchaseHistory *current = purchaseHistory;
        while (current != nullptr)
        {
            if (current->username == currentUser->username)
            {
                purchaseStack.push(current);
            }
            current = current->next;
        }

        // Langkah 2: Menampilkan riwayat pembelian
        if (purchaseStack.empty())
        {
            cout << "Belum ada riwayat pembelian untuk pengguna ini." << endl;
        }
        else
        {
            int purchaseNumber = 1;

            stack<PurchaseHistory *> tempStack;
            while (!purchaseStack.empty())
            {
                tempStack.push(purchaseStack.top());
                purchaseStack.pop();
            }

            while (!tempStack.empty())
            {
                PurchaseHistory *purchase = tempStack.top();

                // Periksa apakah ada entri yang sesuai di PendingPurchase
                PendingPurchase *pendingPurchase = pendingPurchases;
                bool isPending = false;
                while (pendingPurchase != nullptr)
                {
                    if (pendingPurchase->username == currentUser->username &&
                        pendingPurchase->itemName == purchase->itemName &&
                        pendingPurchase->itemPrice == purchase->itemPrice)
                    {
                        isPending = true;
                        break;
                    }
                    pendingPurchase = pendingPurchase->next;
                }

                cout << purchaseNumber << ". " << purchase->itemName << " - Rp. " << fixed << setprecision(2) << purchase->itemPrice;

                if (isPending)
                {
                    cout << " (Menunggu Persetujuan)";
                }
                else
                {
                    cout << " (Disetujui)";
                }

                cout << endl;
                tempStack.pop();
                purchaseNumber++;
            }
        }

        system("pause");
    }
}


// persetujuan admin section (QUEUE)

void enqueuePurchase(PendingPurchase *&front, PendingPurchase *&rear, const string &itemName, double itemPrice, const string &username)
{
    PendingPurchase *newPurchase = new PendingPurchase;
    newPurchase->itemName = itemName;
    newPurchase->itemPrice = itemPrice;
    newPurchase->username = username;
    newPurchase->isApproved = false;
    newPurchase->next = nullptr;

    if (front == nullptr)
    {
        front = rear = newPurchase;
    }
    else
    {
        rear->next = newPurchase;
        rear = newPurchase;
    }
}

// Menyesuaikan fungsi displayPendingPurchases
void displayPendingPurchases(PendingPurchase *front)
{
    PendingPurchase *current = front;
    while (current != nullptr)
    {
        cout << "Username: " << current->username << ", Item: " << current->itemName << ", Price: " << current->itemPrice << ", Status: " << (current->isApproved ? "Approved" : "Pending") << endl;
        current = current->next;
    }
}

void processPendingPurchases(PendingPurchase *&front)
{
    PendingPurchase *current = front;
    while (current != nullptr)
    {
        // Membersihkan layar dan menampilkan detail pembelian yang belum diverifikasi
        system("cls");
        cout << "Pembelian yang perlu diverifikasi:\n";
        displayPendingPurchases(front);

        string decision;
        cout << "Setujui (Y) / Tolak (N) pembelian ini? (Masukkan Y/N) (Ketik 0 Jika Ingin Keluar): ";
        getline(cin >> ws, decision);

        PendingPurchase *next = current->next;

        // Memproses keputusan admin mengenai pembelian
        if (decision == "Y" || decision == "y")
        {
            current->isApproved = true;
            cout << "Pembelian " << current->itemName << " disetujui." << endl;
        }
        else if (decision == "N" || decision == "n")
        {
            cout << "Pembelian " << current->itemName << " ditolak." << endl;
        }
        else if (decision == "0" || decision == "o")
        {
            return; // Menghentikan proses jika user memilih keluar
        }
        else
        {
            cout << "Pilihan tidak valid." << endl;
        }

        // Menghapus entri yang diproses dari antrian
        delete current;
        current = next;
    }

    front = nullptr; // Kosongkan antrian setelah diproses
}

// Fungsi untuk menambahkan pembelian yang perlu diverifikasi (pending) ke dalam antrian
void enqueuePendingPurchase(const string &itemName, double itemPrice, const string &username)
{
    // Membuat entri pembelian baru
    PendingPurchase *newPurchase = new PendingPurchase;
    newPurchase->itemName = itemName;
    newPurchase->itemPrice = itemPrice;
    newPurchase->username = username;
    newPurchase->isApproved = false;
    newPurchase->next = nullptr;

    // Memeriksa apakah antrian pembelian masih kosong
    if (pendingPurchasesFront == nullptr)
    {
        // Jika antrian kosong, tambahkan pembelian sebagai elemen pertama dan terakhir
        pendingPurchasesFront = pendingPurchasesRear = newPurchase;
    }
    else
    {
        // Jika antrian sudah ada elemen, tambahkan pembelian baru di akhir antrian
        pendingPurchasesRear->next = newPurchase;
        pendingPurchasesRear = newPurchase;
    }
}

// Fungsi untuk menggabungkan dua linked list yang terurut secara menaik
Game *SortedMerge(Game *a, Game *b);

// Fungsi untuk menggabungkan dua linked list yang terurut secara menurun
Game *SortedMergeDesc(Game *a, Game *b);

// Fungsi rekursif untuk mengurutkan linked list secara menurun
void MergeSortDesc(Game **headRef);

// Fungsi untuk membagi linked list menjadi dua bagian
void FrontBackSplit(Game *source, Game **frontRef, Game **backRef);

/* sorts the linked list by changing next pointers (not data) */
void MergeSort(Game **headRef)
{
    Game *head = *headRef;
    Game *a;
    Game *b;
    /* Base case -- length 0 or 1 */
    if ((head == NULL) || (head->next == NULL))
    {
        return;
    }
    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit(head, &a, &b);
    /* Recursively sort the sublists */
    MergeSort(&a);
    MergeSort(&b);
    /* answer = merge the two sorted lists together */
    *headRef = SortedMerge(a, b);
}

Game *SortedMerge(Game *a, Game *b)
{
    Game *result = NULL;
    /* Base cases */
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);
    /* Pick either a or b, and recur */
    if (a->name <= b->name)
    {

        result = a;
        result->next = SortedMerge(a->next, b);
    }
    else
    {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return (result);
}

// Fungsi untuk mengurutkan linked list secara menurun
void MergeSortDesc(Game **headRef)
{
    Game *head = *headRef;
    Game *a;
    Game *b;

    // Kasus Dasar: jika linked list kosong atau hanya memiliki satu elemen
    if (head == NULL || head->next == NULL)
    {
        return;
    }

    // Memisahkan linked list menjadi dua bagian
    FrontBackSplit(head, &a, &b);

    // Mengurutkan kedua bagian terpisah secara rekursif
    MergeSortDesc(&a);
    MergeSortDesc(&b);

    // Menggabungkan dua bagian yang sudah diurutkan secara terurut
    *headRef = SortedMergeDesc(a, b);
}

Game *SortedMergeDesc(Game *a, Game *b)
{
    Game *result = NULL;

    // Jika salah satu linked list kosong, kembalikan yang lain
    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    // Menggabungkan linked list a dan b secara terurut secara menurun
    if (a->name >= b->name)
    {
        result = a;
        result->next = SortedMergeDesc(a->next, b);
    }
    else
    {
        result = b;
        result->next = SortedMergeDesc(a, b->next);
    }
    return result;
}

void FrontBackSplit(Game *source, Game **frontRef, Game **backRef)
{
    Game *fast;
    Game *slow;
    slow = source;
    fast = source->next;

    // Memisahkan linked list menjadi dua bagian
    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    // frontRef menunjuk ke awal linked list
    // backRef menunjuk ke pertengahan linked list
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL; // Memutuskan hubungan antara dua bagian
}

// END OF SORTING GAME SECTION

// SORTING MONEY GAME SECTION

//// QUICK SORT GameMoney ASCENDING
struct GameMoney *getTail(struct GameMoney *cur)
{
    while (cur != NULL && cur->next != NULL)
    {
        cur = cur->next;
    }
    return cur;
}

struct GameMoney *partition(struct GameMoney *head, struct GameMoney *end, struct GameMoney **newHead, struct GameMoney **newEnd)
{
    struct GameMoney *pivot = end;
    struct GameMoney *prev = NULL, *cur = head, *tail = pivot;
    while (cur != pivot)
    {
        if (cur->amount < pivot->amount)
        {
            if ((*newHead) == NULL)
            {
                (*newHead) = cur;
            }
            prev = cur;
            cur = cur->next;
        }
        else
        {
            if (prev)
            {
                prev->next = cur->next;
            }
            struct GameMoney *tmp = cur->next;
            cur->next = NULL;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }
    if ((*newHead) == NULL)
    {
        (*newHead) = pivot;
    }
    (*newEnd) = tail;
    return pivot;
}
struct GameMoney *quickSortRecur(struct GameMoney *head, struct GameMoney *end)
{
    if (!head || head == end)
    {
        return head;
    }
    GameMoney *newHead = NULL, *newEnd = NULL;
    struct GameMoney *pivot = partition(head, end, &newHead, &newEnd);

    if (newHead != pivot)
    {
        struct GameMoney *tmp = newHead;
        while (tmp->next != pivot)
        {
            tmp = tmp->next;
        }
        tmp->next = NULL;
        newHead = quickSortRecur(newHead, tmp);
        tmp = getTail(newHead);
        tmp->next = pivot;
    }
    pivot->next = quickSortRecur(pivot->next, newEnd);
    return newHead;
}

void quickSort(struct GameMoney **headRef)
{
    (*headRef) = quickSortRecur(*headRef, getTail(*headRef));
    return;
}
//// QUICK SORT END

// QUICK SORT GameMoney DESCENDING

GameMoney *getHead(GameMoney *cur)
{
    while (cur != NULL && cur->next != NULL)
    {
        cur = cur->next;
    }
    return cur;
}

GameMoney *partitionDesc(GameMoney *head, GameMoney *end, GameMoney **newHead, GameMoney **newEnd)
{
    GameMoney *pivot = end;
    GameMoney *prev = NULL, *cur = head, *tail = pivot;
    while (cur != pivot)
    {
        if (cur->moneyName > pivot->moneyName)
        {
            if ((*newHead) == NULL)
            {
                (*newHead) = cur;
            }
            prev = cur;
            cur = cur->next;
        }
        else
        {
            if (prev)
            {
                prev->next = cur->next;
            }
            GameMoney *tmp = cur->next;
            cur->next = NULL;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }
    if ((*newHead) == NULL)
    {
        (*newHead) = pivot;
    }
    (*newEnd) = tail;
    return pivot;
}

GameMoney *quickSortRecurDesc(GameMoney *head, GameMoney *end)
{
    if (!head || head == end)
    {
        return head;
    }
    GameMoney *newHead = NULL, *newEnd = NULL;
    GameMoney *pivot = partitionDesc(head, end, &newHead, &newEnd);

    if (newHead != pivot)
    {
        GameMoney *tmp = newHead;
        while (tmp->next != pivot)
        {
            tmp = tmp->next;
        }
        tmp->next = NULL;
        newHead = quickSortRecurDesc(newHead, tmp);
        tmp = getHead(newHead);
        tmp->next = pivot;
    }
    pivot->next = quickSortRecurDesc(pivot->next, newEnd);
    return newHead;
}

void quickSortDesc(GameMoney **headRef)
{
    (*headRef) = quickSortRecurDesc(*headRef, getHead(*headRef));
}

// END OF DESCENDING

// END OF SORTING MONEY GAME SECTION

// BOYER MORE SEARCH SECTION

const int NO_OF_CHARS = 256;
// Fungsi untuk mengisi array badChar[] untuk memberikan informasi
// terakhir kemunculan karakter yang tidak cocok
void badCharHeuristic(string str, int size, int badChar[NO_OF_CHARS])
{
    // Inisialisasi semua nilai di badChar sebagai -1
    for (int i = 0; i < NO_OF_CHARS; i++)
    {
        badChar[i] = -1;
    }
    // Isi nilai aktual dari badChar untuk karakter yang ada
    // dalam str
    for (int i = 0; i < size; i++)
    {
        badChar[(int)str[i]] = i;
    }
}
// Fungsi pencarian string dengan Boyer-Moore Bad Character Heuristic
void search(Game *head, string pat)
{
    int m = pat.size();
    Game *temp = head;

    if (lengthGame == 0)
    {
        cout << "Data belum ada." << endl;
        return;
    }

    bool dataFound = false;
    int dataPosition = 0; // Counter to keep track of the position of found data

    while (temp != NULL)
    {
        int n = temp->name.size();
        int badChar[NO_OF_CHARS];
        badCharHeuristic(pat, m, badChar);
        int s = 0;

        while (s <= (n - m))
        {
            int j = m - 1;
            while (j >= 0 && pat[j] == temp->name[s + j])
            {
                j--;
            }
            if (j < 0)
            {
                dataFound = true;
                cout << "|=======================================================|" << endl;
                cout << "|                    GAME DI TEMUKAN                    |" << endl;
                cout << "|=======================================================|" << endl;
                cout << "| Nama Game     : " << temp->name << endl;
                cout << "|=======================================================|" << endl;
                break;
            }
            else
            {
                s += max(1, j - badChar[temp->name[s + j]]);
            }
        }
        temp = temp->next;
    }

    if (!dataFound)
    {
        cout << "Data tidak ditemukan / belum ada " << endl;
        system("pause");
    }
}

// Fungsi untuk menampilkan menu game
void showGameMenus(Game *head)
{

    Game *current = head;
    int gameCount = 1;

    cout << "=============================================\n";
    cout << "|              Daftar Game                    |\n";
    cout << "=============================================\n";
    cout << "| No.|          Nama Game                     |\n";
    cout << "=============================================\n";
    cout << endl;

    while (current != nullptr)
    {
        cout << "| " << gameCount << "."
             << "| " << current->name << endl;
        cout << "---------------------------------------------\n";
        current = current->next;
        gameCount++;
    }

    MergeSort(&gameList);
    cout << "=============================================\n";
}

// Fungsi untuk menampilkan menu game money
void showGameMoneyMenus(GameMoney *moneyList, const string &selectedGameName)
{
    BacaGameMoneyDariFile(&moneyList);
    GameMoney *currentMoney = moneyList;
    int gameMoneyChoice = 1;
    currentMoney->gameName == selectedGameName;
    cout << "=============================================\n";
    cout << "|   Daftar Mata Uang Game " << selectedGameName << "|\n";
    cout << "=============================================\n";
    cout << "| No.|          PILIHAN                      |\n";
    cout << "=============================================\n";

    while (currentMoney != nullptr)
    {
        if (currentMoney->gameName == selectedGameName)
        {
            quickSort(&moneyList);
            cout << "| " << gameMoneyChoice << "."
                 << "| " << currentMoney->moneyName << ","
                 << " Jumlah " << currentMoney->moneyName
                 << " : " << currentMoney->amount << ","
                 << " Rp. " << currentMoney->price << endl;
            cout << "---------------------------------------------\n";
            gameMoneyChoice++;
        }
        currentMoney = currentMoney->next;
    }

    if (gameMoneyChoice == 1)
    {
        cout << "Tidak ada mata uang game yang tersedia untuk game ini." << std::endl;
    }

    cout << "=============================================\n";
}

// END OF BOYER MORE

// END OF BOYER MORE SEARCH SECTION

int main()
{
    BacaGameDariFile(&gameList);
    BacaGameMoneyDariFile(&moneyList);
    Node *userListHead = nullptr;
    Node *userListTail = nullptr;
    PurchaseHistory *purchaseHistory = nullptr;

    GameMoney *moneyList = nullptr;
    Game *currentGame = nullptr;
    GameMoney *currentMoney = nullptr;
    Game *headGame = nullptr; // Deklarasikan atau inisialisasikan linked list game
    
    bacaData(&userListHead, &userListTail);
    bacaData(&Head, &Tail); // Pastikan bahwa data telah dibaca

    // BacaGameDariFile(&headGame);

    Node *currentUser = nullptr;

    while (true)
    {
    balik_menu_awal:
        system("cls");
        cout << "============================================================" << endl;
        cout << "      _=====_            WELCOME TO          _=====_" << endl;
        cout << "     / _____ \\       DHK GAMING STORE       / _____ \\" << endl;
        cout << "    +.-'_____'-.---------------------------.-'_____'-.+" << endl;
        cout << "   /   |     |  '.         ©KMTT          .' |  _  |   \\" << endl;
        cout << "  / ___| /|\\ |___ \\         2017        /____| /_\\ |___ \\" << endl;
        cout << " / |      |      | ;  __           _   ; | _         _ | ;" << endl;
        cout << " | | <---   ---> | | |__|         |_:> | ||_|       (_)| |" << endl;
        cout << " | |___   |   ___| ;SELECT       START ; |___       ___| ;" << endl;
        cout << " |\\    | \\|/ |    /  _     ___      _   \\    | (X) |    /" << endl;
        cout << " | \\   |_____|  .','\" \"', |___|  ,'"
                "', '. \\   |_____| .' |"
             << endl;
        cout << " |  '-.______.-' /       \\ANALOG/       \\  '-.____.-'   |" << endl;
        cout << " |               |       |------|       |               |" << endl;
        cout << " |              /\\       /      \\       /\\              |" << endl;
        cout << " |             /  '.___.'        '.___.'  \\             |" << endl;
        cout << " |            /                            \\            |" << endl;
        cout << "  \\         /                               \\          /" << endl;
        cout << "   \\_______/                                 \\________/" << endl;
        cout << "============================================================" << endl;
        cout << endl;
        cout << "Selamat datang di Top Up Games\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Keluar\n";

        string choice_string;
        int choice;

        try
        {
            cout << "Pilihan: ";
            getline(cin >> ws, choice_string);

            if (choice_string == "1")
            {
                choice = 1;
            }
            else if (choice_string == "2")
            {
                choice = 2;
            }
            else if (choice_string == "3")
            {
                choice = 3;
            }
            else
            {
                cerr << "Pilihan tidak valid. Mohon coba lagi." << endl;
                system("pause");
                goto balik_menu_awal;
            }

            choice = stoi(choice_string);
        }
        catch (invalid_argument &e)
        {
            cerr << "Inputan Harus Angka. Mohon coba lagi." << endl;
            system("pause");
            goto balik_menu_awal;
        }

        switch (choice)
        {
        case 1:
            currentUser = login(userListHead);
            if (currentUser != nullptr)
            {
                if (!currentUser->isAdmin)
                {
                    while (true)
                    {
                    balik_menu_user:
                        system("cls");
                        cout << "====================================\n";
                        cout << "   Selamat Datang  " << currentUser->username << "\t\t    |\n";
                        cout << "====================================\n";
                        cout << "| No. | Pilihan                     |\n";
                        cout << "------------------------------------\n";
                        cout << "| 1.  | Pilihan Game                |\n";
                        cout << "| 2.  | Top Up Saldo                |\n";
                        cout << "| 3.  | Lihat Saldo                 |\n";
                        cout << "| 4.  | Riwayat Pembelian           |\n";
                        cout << "| 5.  | Logout                      |\n";
                        cout << "====================================\n";
                        try
                        {
                            cout << "Pilihan: ";
                            getline(cin >> ws, choice_string);

                            if (choice_string == "1")
                            {
                                choice = 1;
                            }
                            else if (choice_string == "2")
                            {
                                choice = 2;
                            }
                            else if (choice_string == "3")
                            {
                                choice = 3;
                            }
                            else if (choice_string == "4")
                            {
                                choice = 4;
                            }
                            else if (choice_string == "5")
                            {
                                choice = 5;
                            }
                            else
                            {
                                cerr << "Pilihan tidak valid. Mohon coba lagi." << endl;
                                system("pause");
                                goto balik_menu_user;
                            }

                            choice = stoi(choice_string);

                            if (choice < 1 || choice > 5)
                            {
                                throw invalid_argument("Pilihan tidak valid. Mohon coba lagi.");
                            }
                        }
                        catch (invalid_argument &e)
                        {
                            cerr << "Inputan Harus Angka. Mohon coba lagi." << endl;
                            system("pause");
                            goto balik_menu_user;
                        }
                        switch (choice)
                        {

                        case 1:
                        {
                            BacaGameMoneyDariFile(&moneyList);
                            int gameChoice;
                            string gameChoiceString;
                            string gameChoicename;
                            cout << "Menu Game:\n";

                        show_menu:
                            showGameMenus(gameList);

                        input_game:
                            cout << "Pilih game (masukkan nomor game) (ketik 'cari' / 'CARI' jika ingin mencari game) (ketik 0 jika ingin keluar) : ";
                            getline(cin >> ws, gameChoiceString);

                            if (gameChoiceString == "0")
                            {
                                goto balik_menu_user;
                            }
                            else if (gameChoiceString == "cari" || gameChoiceString == "CARI")
                            {
                                string cari;
                                string pilih;
                                system("cls");
                                cout << "Masukkan nama game yang ingin di cari : ";
                                getline(cin >> ws, cari);
                                search(gameList, cari);

                                cout << "ingin melakukan pembelian ? (Y/N)";
                                getline(cin >> ws, pilih);

                                if (pilih == "Y" || pilih == "y")
                                {
                                    cout << "Masukkan Nama Game Yang sudah anda cari Dan Yang Anda Inginkan (ketik 0 jika ingin keluar): ";
                                    getline(cin >> ws, gameChoicename);
                                    if (gameChoicename == "0" || gameChoicename == "0")
                                    {
                                        goto balik_menu_user;
                                    }else{
                                       cout << "pilihan tak valid" << endl;
                                       goto balik_menu_user;
                                    }
                                }else{
                                    cout << "pilihan tak valid" << endl;
                                    goto balik_menu_user;
                                }
                            }
                            else
                            {
                                try
                                {
                                    gameChoice = stoi(gameChoiceString);
                                }
                                catch (invalid_argument &e)
                                {
                                    cerr << "Inputan Harus Angka atau 'cari'. Mohon coba lagi." << endl;
                                    system("pause");
                                    goto input_game;
                                }
                            }

                            Game *selectedGame = chooseGame(gameList, gameChoice, gameChoicename);

                            if (selectedGame != nullptr)
                            {
                                // Mencari game money yang sesuai berdasarkan nama game
                                GameMoney *currentMoney = moneyList;
                                GameMoney *gameMoneyOptions = nullptr;

                                GameMoney *lastMoney = nullptr; 

                                while (currentMoney != nullptr)
                                {
                                    if (currentMoney->gameName == selectedGame->name)
                                    {
                                        // Tambahkan mata uang game ke linked list
                                        GameMoney *newMoney = new GameMoney;
                                        *newMoney = *currentMoney;
                                        newMoney->next = nullptr;

                                        if (lastMoney == nullptr)
                                        {
                                            gameMoneyOptions = lastMoney = newMoney;
                                        }
                                        else
                                        {
                                            lastMoney->next = newMoney;
                                            lastMoney = newMoney;
                                        }
                                    }
                                    currentMoney = currentMoney->next;
                                }

                                if (gameMoneyOptions == nullptr)
                                {
                                    cout << "Tidak ada mata uang game yang tersedia untuk game ini." << endl;
                                    system("pause");
                                }
                                else
                                {

                                    string selectedmoneychoice_string;
                                    int selectedMoneyChoice;

                                    showGameMoneyMenus(gameMoneyOptions, selectedGame->name);

                                input_mata_uang:
                                    try
                                    {
                                        cout << "Pilih mata uang game (masukkan nomor mata uang game): ";
                                        getline(cin >> ws, selectedmoneychoice_string);
                                        selectedMoneyChoice = stoi(selectedmoneychoice_string);
                                    }
                                    catch (invalid_argument &e)
                                    {
                                        cerr << "Inputan Harus Angka. Mohon coba lagi." << endl;
                                        system("pause");
                                        goto input_mata_uang;
                                    }

                                    GameMoney *selectedGameMoney = gameMoneyOptions;
                                    int moneyOptionIndex = 1;

                                    // Temukan mata uang game yang sesuai berdasarkan pilihan
                                    while (selectedGameMoney != nullptr && moneyOptionIndex < selectedMoneyChoice)
                                    {
                                        selectedGameMoney = selectedGameMoney->next;
                                        moneyOptionIndex++;
                                    }

                                    if (selectedGameMoney != nullptr)
                                    {
                                        // Lanjutkan dengan proses pembelian seperti yang Anda lakukan sebelumnya
                                        string pilihan;
                                        double gamePrice = selectedGameMoney->price;

                                        cout << "Nama Game: " << selectedGame->name << endl;
                                        cout << "Nama mata uang game: " << selectedGameMoney->moneyName << endl;
                                        cout << "Jumlah Unit " << selectedGameMoney->moneyName << " : " << selectedGameMoney->amount << endl;
                                        cout << "Harga Game: " << gamePrice << endl;

                                        system("pause");

                                    input_persetujuan:
                                        try
                                        {
                                            cout << "Anda akan membeli " << selectedGameMoney->moneyName << " seharga Rp. " << fixed << setprecision(2) << gamePrice << ". Lanjutkan pembelian? (Y/N): ";
                                            getline(cin >> ws, pilihan);
                                            if (pilihan != "Y" && pilihan != "y")
                                            {
                                                cout << "Pembelian anda gagal" << endl;
                                                goto show_menu;
                                            }
                                        }
                                        catch (invalid_argument &e)
                                        {
                                            cerr << "Inputan Yang Anda Masukkan Salah. Mohon coba lagi." << endl;
                                            system("pause");
                                            goto input_persetujuan;
                                        }

                                        system("pause");

                                        if (currentUser->saldo >= gamePrice)
                                        {
                                            currentUser->saldo -= gamePrice;
                                            // cout << "Pembelian game '" << selectedGame->name << "' berhasil. Saldo Anda sekarang: " << currentUser->saldo << endl;
                                            cout << "Pembelian game '" << selectedGame->name << " akan menunggu persetujuan dari admin ,silahkan tunggu terimakasih " << endl;

                                            // Tambahkan pembelian ke riwayat pembelian
                                            PurchaseHistory *newPurchase = new PurchaseHistory;
                                            newPurchase->itemName = selectedGame->name;
                                            newPurchase->itemPrice = gamePrice;
                                            newPurchase->username = currentUser->username; // Menyimpan username pengguna
                                            newPurchase->next = purchaseHistory;
                                            purchaseHistory = newPurchase;
                                            // Tambahkan pembelian ke antrian pendingPurchases
                                            enqueuePendingPurchase(selectedGame->name, gamePrice, currentUser->username);
                                            system("pause");
                                        }
                                        else
                                        {
                                            cout << "Saldo Anda tidak mencukupi untuk membeli game ini." << endl;
                                            system("pause");
                                        }
                                    }
                                    else
                                    {
                                        cout << "Pilihan mata uang game tidak valid." << endl;
                                        system("pause");
                                    }
                                }

                                // Hapus linked list setelah digunakan
                                while (gameMoneyOptions != nullptr)
                                {
                                    GameMoney *nextMoney = gameMoneyOptions->next;
                                    delete gameMoneyOptions;
                                    gameMoneyOptions = nextMoney;
                                }
                            }
                            else
                            {
                                cout << "Game tidak ditemukan." << endl;
                                system("pause");
                            }
                            break;
                        }
                        break;

                        case 2:
                            topUpBalance(currentUser);
                            break;
                        case 3:
                            viewBalance(currentUser);
                            break;
                        case 4:
                            displayPurchaseHistory(currentUser, purchaseHistory, pendingPurchasesFront);
                            break;
                        case 5:
                            currentUser = nullptr;
                            cout << "Logout berhasil." << endl;
                            system("pause");
                            break;
                        default:
                            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                            system("pause");
                        }

                        if (currentUser == nullptr)
                        {
                            break;
                        }
                    }
                }
                else
                { // Admin user
                    while (true)
                    {
                    balik_menu:
                        system("cls");
                        // SimpanGameMoneyKeFile1(*head);
                        cout << "=============================================\n";
                        cout << "|              Menu Admin                    |\n";
                        cout << "=============================================\n";
                        cout << "| No. | Pilihan                              |\n";
                        cout << "---------------------------------------------\n";
                        cout << "| 1.  | Tambahkan Game                       |\n";
                        cout << "| 2.  | Tambahkan Game Money                 |\n";
                        cout << "| 3.  | Ubah Game                            |\n";
                        cout << "| 4.  | Ubah Game Money                      |\n";
                        cout << "| 5.  | Hapus Game                           |\n";
                        cout << "| 6.  | Hapus Game Money                     |\n";
                        cout << "| 7.  | Lihat Data Game                      |\n";
                        cout << "| 8.  | Persetujuan Pembelian User           |\n";
                        cout << "| 9 . | Logout                               |\n";
                        cout << "=============================================\n";

                        try
                        {
                            cout << "Pilihan: ";
                            getline(cin >> ws, choice_string);

                            if (choice_string == "1")
                            {
                                choice = 1;
                            }
                            else if (choice_string == "2")
                            {
                                choice = 2;
                            }
                            else if (choice_string == "3")
                            {
                                choice = 3;
                            }
                            else if (choice_string == "4")
                            {
                                choice = 4;
                            }
                            else if (choice_string == "5")
                            {
                                choice = 5;
                            }
                            else if (choice_string == "6")
                            {
                                choice = 6;
                            }
                            else if (choice_string == "7")
                            {
                                choice = 7;
                            }
                            else if (choice_string == "8")
                            {
                                choice = 8;
                            }
                            else if (choice_string == "9")
                            {
                                choice = 9;
                            }
                            else
                            {
                                cerr << "Pilihan tidak valid. Mohon coba lagi." << endl;
                                system("pause");
                                goto balik_menu;
                            }

                            choice = stoi(choice_string);

                            if (choice < 1 || choice > 9)
                            {
                                throw invalid_argument("Pilihan tidak valid. Mohon coba lagi.");
                            }
                        }
                        catch (invalid_argument &e)
                        {
                            cerr << "Inputan Harus Angka. Mohon coba lagi." << endl;
                            system("pause");
                            goto balik_menu;
                        }

                        switch (choice)
                        {
                        case 1:
                        // tes
                            addgameFirst(&gameList);
                            // gameMenus(&gameList, lengthGame(gameList));
                            break;
                        case 2:
                        // tes
                            addgameMoneyFirst(&moneyList, gameList); 
                            // gameMoneyMenus(&moneyList, gameList);
                            break;
                        case 3:
                            updategame(&gameList, gameList);

                            // updategame(&gameList, gameList, moneyList);
                            break;
                        case 4:
                            updategameMoney(&moneyList, gameList);
                            break;
                        case 5:
                            deleteMidGame(&gameList);
                            // deletegameMenus(&gameList);
                            break;
                        case 6:
                            deleteMidMoneyGame(&moneyList, gameList);
                            // deletegameMoneyMenus(&moneyList, gameList);
                            break;
                        case 7:
                            showGameAndMoneyMenus(gameList, moneyList); 
                            system("pause");
                            break;
                        case 8:
                                // Menampilkan pembelian yang perlu diverifikasi
                            system("cls");
                            cout << "Pembelian yang perlu diverifikasi:\n";
                            displayPendingPurchases(pendingPurchasesFront);
                            system("pause");

                            // Memproses pembelian yang perlu diverifikasi
                            processPendingPurchases(pendingPurchasesFront);
                            system("pause");
                            break;
                        case 9:
                            currentUser = nullptr;
                            cout << "Logout berhasil." << endl;
                            system("pause");
                            break;
                        default:
                            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                            system("pause");
                        }

                        if (currentUser == nullptr)
                        {
                            break;
                        }
                    }
                }
            }
            break;

        case 2:
            if (!currentUser)
            {
                registerUser(userListHead, userListTail);
            }
            else
            {
                cout << "Anda harus logout terlebih dahulu sebelum mendaftar akun baru." << endl;
                system("pause");
            }
            break;

        case 3:
            cout << "Terima kasih telah menggunakan Top Up Games. Selamat tinggal!" << endl;
            system("pause");
            return 0;

        default:
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            system("pause");
        }
    }

    return 0;
}
