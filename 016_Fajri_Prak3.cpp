#include <iostream>
#include <fstream>
using namespace std;

struct Customer;
struct Service;
int tutup = 0;
bool finish = false;


struct Customer
{
    string nama, gender, telepon, alamat, umur, kosong;
    Customer *next;
    Customer *prev;
    Service *headServ;
};

struct Service
{
    string tipeMobil, merekMobil, kendala, namaMontir, tanggal, kosong, noAntri;
    string doneServ;
    Customer *dataCust;
    Service *allnext;
    Service *next;
    Service *allprev;
    Service *prev;
};

struct Mechanic
{
    string namaMontir, kosong;
    Mechanic *next;
};

Customer *headCust = NULL;
Customer *tailCust = NULL;
Service *servHistory = NULL;
Service *tailHistory = NULL;
Mechanic *headMech = NULL;
Mechanic *tailMech = NULL;
Service *undoStack = NULL;
string nama, namaLog;

void updateServ() // Function untuk menambahkan data service ke file Service3.txt
{
    ofstream fileServ("Service3.txt", ios::trunc);
    Service* serv = servHistory;
    while(serv != NULL)
    {
        if (serv -> dataCust != NULL) 
        {
            fileServ << serv -> dataCust -> nama << endl
                     << serv -> noAntri << endl
                     << serv -> tipeMobil << endl
                     << serv -> merekMobil << endl
                     << serv -> kendala << endl
                     << serv -> namaMontir << endl
                     << serv -> tanggal << endl
                     << serv -> doneServ << endl
                     << serv -> kosong << endl;
        }
        serv = serv -> allnext;
    }
    fileServ.close();
}

void custHistory() // Function untuk menampilkan riwayat service dengan STACK
{
    system("cls");
    cout << "====== Riwayat Service Anda ======" << endl;
    cout << "====== Services ======" << endl;
    Service *serv = servHistory;
    bool found = false;
    Service *stackRiwayat[100];
    int top = -1;
    while (serv != NULL)
    {
        if (serv -> dataCust != NULL && serv -> dataCust -> nama == namaLog)
        {
            found = true;
            top++;
            stackRiwayat[top] = serv;
        }
        serv = serv -> allnext; 
    }
    while (top >= 0) 
    {
        Service *cetak = stackRiwayat[top]; 
        top--; 
        if (cetak -> doneServ == "1")
        {
            cout << "Tipe Mobil: " << cetak -> tipeMobil << endl
                 << "Merek Mobil: " << cetak -> merekMobil << endl
                 << "Kendala: " << cetak -> kendala << endl
                 << "Nama Montir: " << cetak -> namaMontir << endl
                 << "Tanggal Masuk: " << cetak -> tanggal << endl
                 << "Nama Customer: " << cetak -> dataCust -> nama << endl
                 << "Nomor Telepon Customer: " << cetak -> dataCust -> telepon << endl
                 << "-----------------------------" << endl;
        }    
    } 
    if (!found) cout << "Riwayat tidak ditemukan." << endl;
    system("pause");
}

void customerBaru() // Function untuk menambahkan data customer baru
{
    system("cls");
    Service *servBaru = new Service();
    cin.ignore();
    cout << "====== Register Customer Baru ======" << endl;
    cout << "Nama Customer: ";
    getline(cin, nama);
    Customer *that = headCust;
    while (that != NULL)
    {
        if (that -> nama == nama)
            break;
        that = that -> next;
    }
    if (that == NULL)
    {
        Customer *custBaru = new Customer();
        custBaru -> nama = nama;
        cout << "Nomor Telepon: ";
        getline(cin, custBaru -> telepon);
        cout << "Alamat: ";
        getline(cin, custBaru -> alamat);
        cout << "Umur: ";
        getline(cin, custBaru -> umur);
        cout << "Gender: ";
        getline(cin, custBaru -> gender);
        if (headCust == NULL)
        {
            custBaru -> next = NULL;
            custBaru -> prev = NULL;
            custBaru -> headServ = NULL;
            headCust = tailCust = custBaru;
        }
        else
        {
            custBaru -> prev = tailCust;
            tailCust -> next = custBaru;
            custBaru -> headServ = NULL;
            tailCust = custBaru;
        }
        custBaru -> headServ = servBaru;
        servBaru -> dataCust = custBaru;
        ofstream fileCust("Customer3.txt", ios::app);
        fileCust << nama << endl
                 << custBaru -> telepon << endl
                 << custBaru -> alamat << endl
                 << custBaru -> umur << endl
                 << custBaru -> gender << endl << endl;
        fileCust.close();
        cout << endl << "Pelanggan baru telah terdaftar" << endl;
        system("pause");
    }
    else
    {
        servBaru -> next = that -> headServ;
        that -> headServ = servBaru;
        servBaru -> dataCust = that;
        cout << endl << "Pelanggan sudah terdaftar sebelumnya" << endl;
        system("pause");
    }
}

void serviceBaru() // Function untuk total data (service & customer) baru
{
    system("cls");
    Service *servBaru = new Service();
    cout << "====== Register Service Baru ======" << endl;
    string namaAkhir;
    if (namaLog == "adminacces8008")
    {
        cout << "Nama Customer: ";
        getline(cin, nama);
        namaAkhir = nama;
    }
    else
    {
        nama = namaLog;
        namaAkhir = "anda";
    }

    cout << "Tipe Mobil: ";
    cin >> ws;
    getline(cin, servBaru -> tipeMobil);
    cout << "Merek Mobil: ";
    getline(cin, servBaru -> merekMobil);
    cout << "Kendala: ";
    getline(cin, servBaru -> kendala);

    Customer *that = headCust;
    while (that != NULL)
    {
        if (that -> nama == nama)
            break;
        that = that -> next;
    }
    if (that != NULL)
    {
        Service *cekServ = that -> headServ;
        while (cekServ != NULL)
        {
            if (cekServ -> tipeMobil == servBaru -> tipeMobil && 
                cekServ -> merekMobil == servBaru -> merekMobil && 
                cekServ -> doneServ == "0")
            {
                cout << endl << "Mobil ini sudah ada di bengkel, anda ingin menambahkan kendala baru ke service tersebut? (Y/N): ";
                string konfirm;
                cin >> konfirm;

                if (konfirm == "Y" || konfirm == "y")
                {
                    cekServ -> kendala = cekServ -> kendala + " + " + servBaru -> kendala;
                    servBaru -> kendala = cekServ -> kendala;
                    updateServ(); 
                    cout << endl << "*Kendala sudah di update*" << endl;
                    delete servBaru;
                    system("pause");
                    return; 
                }
                else
                {
                    break;
                }
            }
            cekServ = cekServ -> next;
        }
    }    
    cout << "Tanggal Masuk Bengkel: ";
    getline(cin, servBaru -> tanggal);
    if (headMech == NULL)
    {
        cout << "Belum ada montir di bengkel! Silahkan tambahkan montir baru!" << endl;
        delete servBaru;
        system("pause");
        return;
    }
    cout << "Pilih Montir: " << endl;
    Mechanic *mech = headMech;
    int jumlah = 1;
    while (mech != NULL)
    {
        cout << jumlah << "." << mech -> namaMontir << endl;
        mech = mech -> next;
        jumlah++;
    }
    int pilMech, target;
    cout << "Pilihan: ";
    cin >> pilMech;
    cin.ignore();
    target = pilMech - 1;
    mech = headMech;
    for (int i = 0; i < target && mech != NULL; i++)
    {
        mech = mech -> next;
    }

    if (mech == NULL && pilMech < 1)
    {
        cout << "Pilihan tidak valid" << endl;
        delete servBaru;
        system("pause");
        return;
    }
    servBaru -> namaMontir = mech -> namaMontir;
    int antri = 1;
    Service *hitungAntri = servHistory;
    while (hitungAntri != NULL)
    {
        if (hitungAntri -> doneServ == "0")
        {
            antri++;
        }
        hitungAntri = hitungAntri -> allnext;
    }
    servBaru -> noAntri = to_string(antri);
    if (that == NULL)
    {
        Customer *custBaru = new Customer();
        custBaru -> nama = nama;
        cout << "Nomor Telepon: ";
        getline(cin, custBaru -> telepon);
        cout << "Alamat: ";
        getline(cin, custBaru -> alamat);
        cout << "Umur: ";
        getline(cin, custBaru -> umur);
        cout << "Gender: ";
        getline(cin, custBaru -> gender);
        
        if (headCust == NULL)
        {
            custBaru -> next = NULL;
            custBaru -> prev = NULL;
            custBaru -> headServ = NULL;
            headCust = tailCust = custBaru;
        }
        else
        {
            custBaru -> prev = tailCust;
            tailCust -> next = custBaru;
            custBaru -> headServ = NULL;
            tailCust = custBaru;
        }
        custBaru -> headServ = servBaru;
        servBaru -> dataCust = custBaru;
        ofstream fileCust("Customer3.txt", ios::app);
        fileCust << nama << endl
                 << custBaru -> telepon << endl
                 << custBaru -> alamat << endl
                 << custBaru -> umur << endl
                 << custBaru -> gender << endl << endl;
        fileCust.close();
        cout << endl << "*Pelanggan baru telah terdaftar dan service berhasil dicatat, nomor antrian " << namaAkhir << " adalah: " << servBaru -> noAntri << "*" << endl;
        system("pause");
    }
    else
    {
        servBaru -> next = that -> headServ;
        that -> headServ = servBaru;
        servBaru -> dataCust = that;

        cout << endl << "*Service berhasil dicatat, nomor antrian " << namaAkhir << " adalah: " << servBaru -> noAntri << "*" << endl;
        system("pause");
    }
    servBaru -> doneServ = "0";
    servBaru -> allnext = NULL;
    if (servHistory == NULL) 
    {
        servHistory = servBaru;
        tailHistory = servBaru;
    } 
    else 
    {
        tailHistory -> allnext = servBaru;
        servBaru -> allprev = tailHistory;
        tailHistory = servBaru;
    }
    updateServ();
}

void finishJob() // Function untuk menuntaskan service salah satu montir
{
    system("cls");
    string montirAktif[20]; 
    int jumlahMontir = 0;
    Service *serv = servHistory;
    while(serv != NULL)
    {
        if (serv -> doneServ == "0")
        {
            bool ada = false;
            for (int i = 0; i < jumlahMontir; i++)
            {
                if (montirAktif[i] == serv -> namaMontir)
                {
                    ada = true;
                    break;
                }
            }
            if(!ada)
            {
                montirAktif[jumlahMontir] = serv -> namaMontir;
                jumlahMontir++;
            }
        }
        serv = serv -> allnext;
    }
    if(jumlahMontir == 0)
    {
        cout << "====== Finish Job ======" << endl
             << "Tidak ada antrian servis yang pending saat ini." << endl;
        system("pause");
        return;
    }
    cout << "====== Finish Job ======" << endl
         << "Pilih Montir: " << endl;

    for (int i = 0; i < jumlahMontir; i++)
    {
        cout << i + 1 << ". " << montirAktif[i] << endl;
    }
    cout << "Pilihan: ";
    int pil;
    cin >> pil;
    if (pil < 1 || pil > jumlahMontir) 
    {
        cout << "Pilihan tidak valid!" << endl;
        system("pause");
        return;
    }
    string targetMontir = montirAktif[pil - 1];
    Service *servN = servHistory;
    bool adaIn = false;
    while (servN != NULL)
    {
        if (servN -> namaMontir == targetMontir && servN -> doneServ == "0")
        {
            adaIn = true;
            cout << endl << "====== Service ======" << endl
                 << "Tipe Mobil: " << servN -> tipeMobil << endl
                 << "Merek Mobil: " << servN -> merekMobil << endl
                 << "Kendala: " << servN -> kendala << endl
                 << "Nama Montir: " << servN -> namaMontir << endl
                 << "Tanggal Masuk: " << servN -> tanggal << endl
                 << "Nama Customer: " << servN -> dataCust -> nama << endl
                 << "Nomor Telepon Customer: " << servN -> dataCust -> telepon << endl
                 << "-----------------------------" << endl;

            cout << "Apakah servis ini sudah selesai? (Y/N): ";
            string jawaban;
            cin >> jawaban;

            if (jawaban == "y" || jawaban == "Y")
            {
                servN -> doneServ = "1";
                Service *antrian = servHistory;
                int antri = 1;
                while (antrian != NULL)
                {
                    if (antrian -> doneServ == "0")
                    {
                        antrian -> noAntri = to_string(antri);
                        antri++;
                    }
                    else
                    {
                        antrian -> noAntri = "0";
                    }
                    antrian = antrian -> allnext;
                }                
                updateServ();
                cout << "Servis berhasil diselesaikan!" << endl;
            }
            else {
                cout << "Servis dibatalkan / belum diselesaikan." << endl;
            }
            break;
        }
        servN = servN -> allnext;
    }
    if (!adaIn) cout << endl << "Tidak ada antrian pekerjaan untuk montir " << targetMontir << "." << endl;
    system("pause");
}

void allServ() // Function untuk menampilkan semua service yang belum dituntaskan
{
    system("cls");
    Service *serv = servHistory;
    bool ada = false;
    cout << "====== All Services ======" << endl;
    while (serv != NULL)
    {
        if (serv -> doneServ == "0")
        {
            cout << "Tipe Mobil: " << serv -> tipeMobil << endl
                 << "Merek Mobil: " << serv -> merekMobil << endl
                 << "Kendala: " << serv -> kendala << endl
                 << "Nama Montir: " << serv -> namaMontir << endl
                 << "Nama Customer: " << serv -> dataCust -> nama << endl
                 << "Nomor Telepon Customer: " << serv -> dataCust -> telepon << endl;
            serv -> doneServ;
            cout << "-----------------------------" << endl;  
            ada = true;
        }
        serv = serv -> allnext;
    }
    if(!ada)
    {
        cout << "Tidak service saat ini" << endl;
        cout << "-----------------------------" << endl;        
    }
    system("pause");
}

void queueServ() // Function untuk menampilkan antrian service
{
    system("cls");
    Service *serv = servHistory;
    bool antri = false;
    string antrianUser = "";
    
    cout << "====== All Services ======" << endl;
    
    while (serv != NULL)
    {
        if(serv -> doneServ == "0")
        {
            cout << "-----------------------" << endl
                 << "Model Mobil: " << serv -> tipeMobil << endl
                 << "Merek Mobil: " << serv -> merekMobil << endl
                 << "Kendala: " << serv -> kendala << endl
                 << "Montir: " << serv -> namaMontir << endl
                 << "Nama Pelanggan: " << serv -> dataCust -> nama << endl
                 << "No Telp Pelanggan: " << serv -> dataCust -> telepon << endl
                 << "-----------------------" << endl;
            antri = true;

            if (serv -> dataCust -> nama == namaLog) {
                antrianUser += "Servis " + serv -> merekMobil + " Anda berada di antrian ke-" + serv -> noAntri + "\n";
            }
        }
        serv = serv -> allnext;
    }
    if (!antri)
    {
        cout << "Tidak ada antrian service saat ini" << endl;
        cout << "-----------------------" << endl;
    }
    else 
    {
        if (antrianUser != "") {
            cout << antrianUser;
        }
    }
    system("pause");
}

void batalServ() // Fitur Membatalkan Booking
{
    system("cls");
    cout << "====== Cancel Service ======" << endl;
    Service *serv = servHistory;
    Service *pilihanBatal[20];
    int jumlah = 0;
    string urutan[] = {"Pertama", "Kedua", "Ketiga", "Keempat", "Kelima", "Keenam", "Ketujuh", "Kedelapan", "Kesembilan", "Kesepuluh"};
    while (serv != NULL)
    {
        if (serv -> doneServ == "0" && serv -> dataCust -> nama == namaLog)
        {
            pilihanBatal[jumlah] = serv; 
            
            cout << jumlah + 1 << ". Servis ";
            if (jumlah < 10)
            {
                cout << urutan[jumlah] << ":" << endl;
            }
            else
            {
                cout << jumlah + 1 << ":" << endl;
            }
            cout << "Model Mobil: " << serv -> tipeMobil << endl;
            cout << "Merek Mobil: " << serv -> merekMobil << endl;
            cout << "Kendala: " << serv -> kendala << endl;
            cout << "Montir: " << serv -> namaMontir << endl;
            jumlah++;
        }
        serv = serv -> allnext;
    }
    if (jumlah == 0)
    {
        cout << "Anda tidak memiliki antrian service yang aktif." << endl;
        system("pause");
        return;
    }
    cout << "Pilih Mobil yang ingin dibatalkan: ";
    int pil;
    cin >> pil;
    
    if (pil >= 1 && pil <= jumlah)
    {
        Service *target = pilihanBatal[pil - 1];
        if (target != NULL)
        {
            if (target -> allprev != NULL)
            {
                target -> allprev -> allnext = target -> allnext;
            }
            else
            {
                servHistory = target -> allnext;
            }
                
            if (target -> allnext != NULL)
            {
                target -> allnext -> allprev = target -> allprev;
            }
            else
            {
                tailHistory = target -> allprev;
            }
        }
        target -> allnext = undoStack;
        undoStack = target;
        int antriUlang = 1;
        Service *recalc = servHistory;
        while (recalc != NULL)
        {
            if (recalc -> doneServ == "0")
            {
                recalc -> noAntri = to_string(antriUlang);
                antriUlang++;
            }
            recalc = recalc -> allnext;
        }
        updateServ();
        cout << "*Servis " << target -> merekMobil << " telah dibatalkan*" << endl;
    }
    else
    {
        cout << "Pilihan tidak valid!" << endl;
    }
    system("pause");
}

void undoBatal() // Fitur Undo Pembatalan
{
    system("cls");
    cout << "====== Booking Kembali Service ======" << endl;
    if (undoStack == NULL)
    {
        cout << "Tidak ada data pembatalan yang bisa di-undo saat ini." << endl;
        system("pause");
        return;
    }
    Service *target = undoStack; 
    cout << "Model Mobil: " << target -> tipeMobil << endl;
    cout << "Merek Mobil: " << target -> merekMobil << endl;
    cout << "Kendala: " << target -> kendala << endl;
    cout << "Montir: " << target -> namaMontir << endl;
    cout << "Apakah anda ingin membooking kembali servis ini? (Y/N): ";
    string jawab;
    cin >> ws;
    cin >> jawab;
    
    if (jawab == "y" || jawab == "Y")
    {
        undoStack = undoStack -> allnext; 
        cout << "Apakah ingin di reschedule? input (-) jika tidak" << endl;
        cout << "Tanggal Lama: " << target -> tanggal << endl;
        cout << "Tanggal Baru: ";
        string tanggal;
        getline(cin >> ws, tanggal);
        if (tanggal != "-")
        {
            target -> tanggal = tanggal;
        }
        target -> allnext = NULL;
        if (servHistory == NULL)
        {
            servHistory = target;
            tailHistory = target;
            target -> allprev = NULL;
        }
        else
        {
            target -> allprev = tailHistory;
            tailHistory -> allnext = target;
            tailHistory = target;
        }
        int antrian = 1;
        Service *antri = servHistory;
        while (antri != NULL)
        {
            if (antri -> doneServ == "0")
            {
                antri -> noAntri = to_string(antrian);
                antrian++;
            }
            antri = antri -> allnext;
        }
        updateServ();
        cout << "*Service " << target -> merekMobil << " telah dibooking kembali*" << endl;
    }
    else
    {
        cout << "Undo dibatalkan." << endl;
    }
    system("pause");
}

void newMechanic()
{
    system("cls");
    string namaMon;
    cout << "====== New Mechanic ======" << endl
         << "Masukkan nama montir baru" << endl
         << "Nama: ";
    getline (cin, namaMon);

    Mechanic *mech = headMech;
    while(mech != NULL)
    {
        bool ada = false;
        if (mech -> namaMontir == namaMon)
        {
            ada = true;
            break;
        }
        mech = mech -> next; 
    }
    if (mech == NULL)
    {
        Mechanic *mechBaru = new Mechanic;
        mechBaru -> namaMontir = namaMon;
        mechBaru -> kosong = "";
        mechBaru -> next = NULL;        

        if (headMech == NULL)
        {
            headMech = tailMech = mechBaru;
        }
        else
        {
            tailMech -> next = mechBaru;
            tailMech = mechBaru;
        }
        ofstream fileMech("Mechanic3.txt", ios::app);
        fileMech << namaMon << endl << endl;
        fileMech.close();
        cout << "Montir" << namaMon << "berhasil didaftarkan!" << endl;
        system("pause");
    }
    else
    {
        cout << namaMon << "sudah terdaftar sebagai montir" << endl;
        system("pause");
        return;
    }
}

void mechanicHistory() // Function untuk menampilkan riwayat montir dengan STACK
{
    system("cls");
    string montirAktif[20];
    int jumlahMontir = 0;
    Service *serv = servHistory;
    
    // Mencari daftar montir yang aktif/punya kerjaan
    while(serv != NULL)
    {
        bool ada = false;
        for (int i = 0; i < jumlahMontir; i++)
        {
            if (montirAktif[i] == serv -> namaMontir)
            {
                ada = true;
                break;
            }
        }
        if(!ada && serv -> namaMontir != "") 
        {
            montirAktif[jumlahMontir] = serv -> namaMontir;
            jumlahMontir++;
        }
        serv = serv -> allnext;
    }
    
    if (jumlahMontir == 0) 
    {
        cout << "====== Mechanic's Job History ======" << endl
             << "Belum ada riwayat kerja satupun di bengkel ini." << endl;
        system("pause");
        return;
    }
    
    cout << "====== Mechanic's Job History ======" << endl
         << "Pilih Menu: " << endl << endl;
    for (int i = 0; i < jumlahMontir; i++) 
    {
        cout << i + 1 << ". " << montirAktif[i] << endl;
    }
    cout << "Pilihan: ";
    int pil;
    cin >> pil;

    if (pil >= 1 && pil <= jumlahMontir) 
    {
        string targetMontir = montirAktif[pil - 1];
        cout << endl << "====== " << targetMontir << "'s Job History ======" << endl;

        bool ada = false;
        Service* stackMontir[100];
        int top = -1;
        
        serv = servHistory; 

        while (serv != NULL)
        {
            if (serv -> namaMontir == targetMontir)
            {
                ada = true;
                top++;
                stackMontir[top] = serv;
            }
            serv = serv -> allnext;
        }

        if (!ada) 
        {
            cout << "Tidak ada pekerjaan." << endl;
        }
        else
        {

            while (top >= 0)
            {
                Service *serv = stackMontir[top];
                top--;

                cout << "Tipe Mobil: " << serv -> tipeMobil << endl
                     << "Merek Mobil: " << serv -> merekMobil << endl
                     << "Kendala: " << serv -> kendala << endl
                     << "Tanggal Masuk: " << serv -> tanggal << endl
                     << "Nama Customer: " << serv -> dataCust -> nama << endl 
                     << "-----------------------------" << endl;
            }
        }
        system("pause");
    }
    else if (pil == jumlahMontir + 1)
    {
        return;
    }
    else 
    {
        cout << "Pilihan tidak valid!" << endl;
        system("pause");
    }
}

void serviceMenu() // Function untuk menampilkan UI dari Service
{  
    int pil;
    while (pil != 5)
    {
        system("cls");
        cout << "====== Welcome to Lognuts, admin ======" << endl
             << "Pilih Menu: " << endl << endl
             << "1. Semua Service Singkat" << endl
             << "2. Service Baru" << endl
             << "3. Selesaikan Pekerjaan" << endl
             << "4. Riwayat Kerja" << endl 
             << "5. Keluar" << endl << endl
             << "Pilihan: ";
        cin >> pil;
        cin.ignore();
        if(pil == 1)
        {
            allServ();
        }
        else if(pil == 2)
        {
            serviceBaru();
        }
        else if(pil == 3)
        {
            finishJob();
        }
        else if(pil == 4)
        {
            mechanicHistory();
        }
        else if(pil == 5)
        {
            return;
        } 
    }
}

void adminMenu() // Function untuk menampilkan UI dari Admin
{
    int pil;
    while (pil != 4)
    {
        system("cls");
        cout << "====== Welcome to Lognuts, Admin ======" << endl
             << "Pilih Menu: " << endl << endl
             << "1. Service" << endl
             << "2. Pelanggan Baru" << endl 
             << "3. Montir Baru" << endl
             << "4. Keluar" << endl << endl
             << "Pilihan: ";
        cin >> pil;
        cin.ignore();
        if (pil == 1)
        {
            serviceMenu();
        }
        else if (pil == 2)
        {
            customerBaru();
        }
        else if (pil == 3)
        {
            newMechanic();
        }
        else if (pil == 4)
        {
            namaLog = "";
            finish = false;
        }
        else
        {
            cout << "Pilihan tidak valid!" << endl;
            system("pause");
        }
        
    }
}

void Menu() // Function untuk menampilkan UI dari Menu
{
    string nomor;
    while (!finish)
    {
        system("cls");
        cout << "====== Welcome to Suby's Garage ======" << endl
             << "Masukkan Nama dan Nomor Telepon" << endl
             << "Nama: ";
        getline(cin >> ws, namaLog);
        if (namaLog == "adminacces8008")
        {
            adminMenu();
            return;
        }
        cout << "Nomor Telepon: ";
        getline(cin, nomor);

        Customer *that = headCust;

        while (that != NULL)
        {
            if (that -> nama == namaLog)
            {
                if (that -> telepon == nomor)
                {
                    finish = true;
                    break;
                }
                else
                {
                    cout << "Nomor telepon anda salah!" << endl;
                    system("pause");
                    return;
                }
            }
            that = that -> next;
        }
        if (that == NULL)
        {
            if (headCust == NULL)
            {
                cout << "Belum ada customer yang terdaftar pada database Lognuts!" << endl;
                system("pause");
            }
            else
            {
                string konfir;
                while(konfir != "n" || konfir != "N")
                {
                    cout << "Customer belum terdaftar!" << endl
                         << "Ingin Register? (Y/N): ";
                    cin >> konfir;
                    if (konfir == "Y" || konfir == "y")
                    {
                        customerBaru();
                        return;
                    }
                }
            };
        }
        else
        {
            continue;
        }
    }
    string pil;
    system("cls");
    cout << "====== Welcome to Lognuts, " << namaLog << "! ======" << endl
         << "Pilih Menu! " << endl << endl
         << "1. Antrian Service" << endl
         << "2. Booking Service" << endl
         << "3. Batalkan Service" << endl 
         << "4. Undo Pembatalan" << endl
         << "5. Riwayat Service" << endl 
         << "6. Keluar" << endl << endl
         << "Pilihan: ";
    cin >> pil;
    cin.ignore();
    if (pil == "1")
    {
        queueServ();
    }
    else if (pil == "2")
    {
        serviceBaru();
    }
    else if (pil == "3")
    {
        batalServ();
    }
    else if (pil == "4")
    {
        undoBatal();
    }
    else if (pil == "5")
    {
        custHistory();
    }
    else if (pil == "6")
    {
        cout << endl << "====== Thank you for visiting Suby's Garage! ======";
        tutup = 1;        
    }
    else if (pil == "adminacces8008")
    {
        adminMenu();
    }
}
int main() // And ofcourse, the main
{
    ifstream fileCust("Customer3.txt");
    string namaCust;
    while (getline(fileCust, namaCust)) 
    {
        if (namaCust == "") continue;
        Customer *custBaru = new Customer();
        custBaru -> nama = namaCust;       
        getline(fileCust, custBaru -> telepon);
        getline(fileCust, custBaru -> alamat);
        getline(fileCust, custBaru -> umur);
        getline(fileCust, custBaru -> gender);
        getline(fileCust, custBaru -> kosong);
        custBaru -> headServ = NULL;
        custBaru -> next = NULL;
        custBaru -> prev = NULL;
        if (headCust == NULL)
        {
            headCust = tailCust = custBaru;
        }
        else
        {
            tailCust -> next = custBaru;
            custBaru -> prev = tailCust;
            tailCust = custBaru;
        }
    }
    fileCust.close();
    ifstream fileServ("Service3.txt");
    string namaCustServ;
    while (getline(fileServ, namaCustServ)) 
    {
        if (namaCustServ == "") continue;
        Service *servBaru = new Service();
        getline(fileServ, servBaru -> noAntri);
        getline(fileServ, servBaru -> tipeMobil);
        getline(fileServ, servBaru -> merekMobil);
        getline(fileServ, servBaru -> kendala);
        getline(fileServ, servBaru -> namaMontir);
        getline(fileServ, servBaru -> tanggal);
        getline(fileServ, servBaru -> doneServ);
        getline(fileServ, servBaru -> kosong);        
        servBaru -> allnext = NULL;
        servBaru -> next = NULL;
        servBaru -> prev = NULL;
        Customer* that = headCust;
        while(that != NULL)
        {
            if (that -> nama == namaCustServ)
                break;
            that = that -> next;
        }
        if (that != NULL)
        {
            servBaru -> next = that -> headServ;
            that -> headServ = servBaru;
            servBaru -> dataCust = that;
        }
        if (servHistory == NULL)
        {
            servHistory = servBaru;
            tailHistory = servBaru; 
        }
        else
        {
            tailHistory -> allnext = servBaru;
            servBaru -> allprev = tailHistory;
            tailHistory = servBaru;
        }
    }
    fileServ.close();

    ifstream fileMech("Mechanic3.txt");
    string namaMon, kosong;
    while(getline(fileMech, namaMon))
    {
        if (namaMon == "") continue;
        Mechanic *mechBaru = new Mechanic;
        mechBaru -> namaMontir = namaMon;
        getline(fileMech, kosong);
        mechBaru -> next = NULL;
        if (headMech == NULL)
        {
            headMech = tailMech = mechBaru;
        }
        else
        {
            tailMech -> next = mechBaru;
            tailMech = mechBaru;
        }
    }
    fileMech.close();
    while (tutup != 1)
    {
        Menu();
    }
    return 0;
}