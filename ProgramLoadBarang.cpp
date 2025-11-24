#include <iostream>
#include <vector>
#include <iomanip>
#include <stdexcept>
using namespace std;

// TEMPLATE CLASS (untuk wadah fleksibel menyimpan data)
template <typename T>
class Container {
private:
    vector<T> data;
public:
    void add(T item) { data.push_back(item); }
    T& get(int index) { return data.at(index); }
    int size() { return data.size(); }

    // Operator overloading (+= untuk menambah elemen)
    void operator+=(T item) {
        data.push_back(item);
    }
};


// BASE CLASS : ITEM
// Menggunakan polymorphism (virtual function)
class Item {
protected:
    string nama;
    float berat;

public:
    Item(string n, float b) : nama(n), berat(b) {}
    virtual ~Item() {}

    // POLYMORPHISM (method virtual)
    virtual float hitungNilai() const = 0;

    string getNama() const { return nama; }
    float getBerat() const { return berat; }
};


// DERIVED CLASS : BARANG

class Barang : public Item {
private:
    float hargaPerKg;

public:
    Barang(string n, float b, float h)
        : Item(n, b), hargaPerKg(h) {}

    float hitungNilai() const override {
        return berat * hargaPerKg;
    }

    void tampil() const {
        cout << "Nama Barang : " << nama << endl;
        cout << "Berat       : " << berat << " kg\n";
        cout << "Harga/Kg    : Rp " << hargaPerKg << endl;
        cout << "Nilai Total : Rp " << hitungNilai() << endl;
        cout << "-------------------------------------" << endl;
    }

    // Friend function (mengakses private)
    friend void tampilHarga(const Barang &b);
};

// Friend function (mengakses private di Barang)
void tampilHarga(const Barang &b) {
    cout << "Harga per Kg barang " << b.nama << " adalah Rp " << b.hargaPerKg << endl;
}


// BASE CLASS : PERSON

class Person {
protected:
    string nama;
public:
    Person(string n) : nama(n) {}
    string getNama() const { return nama; }
};


// DERIVED CLASS : LOADER

class Loader : public Person {
private:
    float biayaPerKg;

public:
    Loader(string n, float biaya)
        : Person(n), biayaPerKg(biaya) {}

    float hitungBiaya(float totalBerat) const {
        return totalBerat * biayaPerKg;
    }

    void tampil() const {
        cout << "Loader: " << nama
             << " | Biaya Per Kg: Rp " << biayaPerKg << endl;
    }
};


// COMPOSITION : Kendaraan memiliki barang (HAS-A)

class Kendaraan {
private:
    string jenis;
    float kapasitasMaks;
    float kapasitasTerpakai = 0;
    vector<Barang> daftarBarang;

public:
    Kendaraan(string j, float k)
        : jenis(j), kapasitasMaks(k) {}

    bool muatBarang(const Barang &b) {
        if (kapasitasTerpakai + b.getBerat() <= kapasitasMaks) {
            daftarBarang.push_back(b);
            kapasitasTerpakai += b.getBerat();
            return true;
        }
        return false;
    }

    float getTotalBerat() const { return kapasitasTerpakai; }

    float hitungTotalNilai() const {
        float total = 0;
        for (auto &b : daftarBarang)
            total += b.hitungNilai();
        return total;
    }

    float efisiensi() const {
        return (kapasitasTerpakai / kapasitasMaks) * 100;
    }

    void tampilBarang() const {
        cout << endl;
        cout << "\t\tBARANG DI KENDARAAN"<< endl;
        for (auto &b : daftarBarang)
            b.tampil();
    }
};


// CLASS TRANSAKSI LOADING
// Menerapkan exception handling
class TransaksiLoading {
private:
    Loader loader;
    Kendaraan kendaraan;

public:
    TransaksiLoading(Loader lo, Kendaraan ken)
        : loader(lo), kendaraan(ken) {}

    void proses(Container<Barang> &listBarang) {
        cout << endl;
        cout << "\t\tPROSES LOADING "<< endl;

        for (int i = 0; i < listBarang.size(); i++) {
            try {
                Barang &b = listBarang.get(i);

                if (!kendaraan.muatBarang(b))
                    throw runtime_error("Kapasitas penuh! Barang tidak dapat dimuat.");

                cout << b.getNama() << " berhasil dimuat." << endl;
            }
            catch (exception &e) {
                cout << "ERROR: " << e.what() << endl;
            }
        }
    }

    void ringkasan() {
        kendaraan.tampilBarang();

        float totalBerat = kendaraan.getTotalBerat();
        float totalNilai = kendaraan.hitungTotalNilai();
        float biayaLoader = loader.hitungBiaya(totalBerat);
        float keuntungan = totalNilai - biayaLoader;

        cout << fixed << setprecision(2);
        cout << endl;
        cout << "RINGKASAN EKONOMI" << endl;
        cout << "Total Berat Barang : " << totalBerat << " kg\n";
        cout << "Total Nilai Barang : Rp " << totalNilai << endl;
        cout << "Biaya Loading      : Rp " << biayaLoader << endl;
        cout << "Efisiensi Kapasitas: " << kendaraan.efisiensi() << "%\n";
        cout << "---------------------------------\n";
        cout << "KEUNTUNGAN         : Rp " << keuntungan << endl;
    }
};


// MAIN PROGRAM

int main() {

    Container<Barang> daftarBarang;
    int jumlah;
    cout << endl;
    cout << "Masukkan jumlah barang: ";
    cin >> jumlah;

    for (int i = 0; i < jumlah; i++) {
        string nama;
        float berat, harga;

        cout << endl;
        cout << "Barang ke-" << i + 1 << endl;
        cout << "Nama barang  : ";
        cin >> nama;
        cout << "Berat (kg)   : ";
        cin >> berat;
        cout << "Harga/kg     : ";
        cin >> harga;

        // operator overloading (+=)
        daftarBarang += Barang(nama, berat, harga);
    }

    Loader loader("Budi", 5000);
    Kendaraan truk("Truk Ekspedisi", 500);

    TransaksiLoading transaksi(loader, truk);
    transaksi.proses(daftarBarang);
    transaksi.ringkasan();

    return 0;
}
