#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Arac {
    int gelisZamani;
};

class Yon {
public:
    int id; 
    double gelisOrani; 
    double yesilSuresi;
    std::queue<Arac> kuyruk;
    std::vector<int> beklemeler; 

    Yon(int id_, double oran, double yesil) : id(id_), gelisOrani(oran), yesilSuresi(yesil) {}

    void aracGelisKontrol(int simZaman) {
        double olasilik = gelisOrani / 60.0;
        double r = static_cast<double>(std::rand()) / RAND_MAX;

        if (r < olasilik) {
            Arac a{simZaman};
            kuyruk.push(a);
        }
    }

    void aracGecir(int simZaman, int yesilBitis, int gecisSuresi = 2) {
        while (!kuyruk.empty() && simZaman < yesilBitis) {
            Arac a = kuyruk.front();
            kuyruk.pop();

            int bekleme = simZaman - a.gelisZamani;
            beklemeler.push_back(bekleme);

            simZaman += gecisSuresi;
        }
    }
};

class TrafikIsigiSim {
public:
    std::vector<Yon> yonler;
    int cevrimSayisi;

    TrafikIsigiSim(int adet) : cevrimSayisi(adet) {}

    void yonEkle(int id, double oran, double yesil) {
        yonler.emplace_back(id, oran, yesil);
    }

    void calistir() {
        int simZaman = 0;

        for (int c = 0; c < cevrimSayisi; c++) {
            for (auto &y : yonler) {

                int yesilBas = simZaman;
                int yesilBit = simZaman + y.yesilSuresi;

                for (int t = yesilBas; t < yesilBit; t++) {
                    for (auto &y2 : yonler) {
                        y2.aracGelisKontrol(t);
                    }
                }

                y.aracGecir(yesilBas, yesilBit);

                simZaman = yesilBit;
            }
        }
    }

    void rapor() {
        std::cout << std::endl << "--- Simulasyon Sonuclari ---" << std::endl;

        for (auto &y : yonler) {
            double toplam = 0.0;
            for (int b : y.beklemeler) toplam += b;

            double ort = y.beklemeler.empty() ? 0.0 : toplam / y.beklemeler.size();

            std::cout << "Yon " << y.id
                      << " - Ortalama Bekleme: " << ort
                      << " saniye (" << y.beklemeler.size()
                      << " arac)" << std::endl;
        }
    }
};

int main() {
    std::srand(std::time(nullptr));

    int yonSayisi, donguSayisi;
    std::cout << "Kavsakta birlesen yon sayisi: ";
    std::cin >> yonSayisi;
    std::cout << "Simulasyon kac dongu olarak uygulansin?";
    std::cin >> donguSayisi;

    TrafikIsigiSim sim(donguSayisi);

    std::cout << "Her yon icin dakikadaki arac sayisini ve yesil sureyi giriniz." << std::endl;

    for (int i = 1; i <= yonSayisi; i++) {
        double oran, yesil;

        std::cout << std::endl << i << ". yon - dakikada gelen arac sayisi: ";
        std::cin >> oran;

        std::cout << i << ". yon - yesil sure (saniye): ";
        std::cin >> yesil;

        sim.yonEkle(i, oran, yesil);
    }

    sim.calistir();
    sim.rapor();

    system("pause");
    return 0;
}
