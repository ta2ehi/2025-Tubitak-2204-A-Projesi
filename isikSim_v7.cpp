#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>

class Arac 
{
    public:
    int gelisZamani;
};

class Yon 
{
    public:
    int id; 
    double gelisOrani; 
    double yesilSuresi;
    std::queue<Arac> kuyruk;
    std::vector<int> beklemeler; 

    Yon(int id_, double oran, double yesil) : id(id_), gelisOrani(oran), yesilSuresi(yesil) {}

    void aracGelisKontrol(int simZaman) 
    {
        double olasilik = gelisOrani / 60.0;
        double r = static_cast<double>(std::rand()) / RAND_MAX;

        if (r < olasilik) 
        {
            Arac a{simZaman};
            kuyruk.push(a);
        }
    }

    void aracGecir(int simZaman, int yesilBitis, int gecisSuresi = 2) 
    {
        while (!kuyruk.empty() && simZaman < yesilBitis) 
        {
            Arac a = kuyruk.front();
            kuyruk.pop();

            int bekleme = simZaman - a.gelisZamani;
            beklemeler.push_back(bekleme);

            simZaman += gecisSuresi;
        }
    }
};

class TrafikIsigiSim 
{
    public:
    std::vector<Yon> yonler;
    int cevrimSayisi;

    TrafikIsigiSim(int adet) : cevrimSayisi(adet) {}

    void yonEkle(int id, double oran, double yesil) 
    {
        yonler.emplace_back(id, oran, yesil);
    }

    void calistir() {
        int simZaman = 0;

        for (int c = 0; c < cevrimSayisi; c++) 
        {
            for (auto &y : yonler) 
            {

                int yesilBas = simZaman;
                int yesilBit = simZaman + y.yesilSuresi;

                for (int t = yesilBas; t < yesilBit; t++) 
                {
                    for (auto &y2 : yonler) 
                    {
                        y2.aracGelisKontrol(t);
                    }
                }

                y.aracGecir(yesilBas, yesilBit);

                simZaman = yesilBit;
            }
        }
    }

    void rapor() 
    {
        std::cout << std::endl << "--- Simulasyon Sonuclari ---" << std::endl;

        for (auto &y : yonler) 
        {
            double toplam = 0.0;
            for (int b : y.beklemeler) toplam += b;

            double ort = y.beklemeler.empty() ? 0.0 : toplam / y.beklemeler.size();

            std::cout << "Yon " << y.id << " - Ortalama Bekleme: " << ort << " saniye (" << y.beklemeler.size() << " arac)" << std::endl;
        }
    }
};

class TrafikIsikAlgoritmasi
{
    std::map<int, int> yesilSureleri = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};
    std::map<int, int> ortalamaAracSayilari = {{1, 0}, {2, 0}, {3, 0}, {4, 0}}; 
    int algoritmaModu = 0;
    /*
        0: Tanimsiz
        1: Kullanicidan alinan faz sureleri
        2: Kullanicidan alinan esit faz sureleri 
        3: Ort. arac sayilarina gore dogru orantili faz sureleri, kullanicidan alinan yesilSuresi : aracSayisi katsayisi
        4: ...
    */
    public:
    
    TrafikIsikAlgoritmasi() {}

   void algoritmaAl(const int& yonSayisi)
    {
        int mod;
        std::cout << "Mod giriniz (Yardim icin 0): ";
        std::cin >> mod;

        if (mod < 0)
            return;
        if (mod == 0)
        {
            yardim();
            return;
        }
        if (mod == 1)
        {
            std::cout << "1 no.lu mod aktif.\n";
            for (int i = 1; i <= yonSayisi; ++i)
            {
                std::cout << i << ". yonde dakikada gelen ortalama arac sayisi: ";
                std::cin >> ortalamaAracSayilari[i];

                std::cout << i << ". yonde yesil isik suresi: ";
                std::cin >> yesilSureleri[i];
            }
            return;
        }

        if (mod == 2)
        {
            int yesilSure = 0;
            std::cout << "2 no.lu mod aktif.\n";

            for (int i = 1; i <= yonSayisi; ++i)
            {
                std::cout << i << ". yonde ortalama arac sayisi: ";
                std::cin >> ortalamaAracSayilari[i];
            }

            std::cout << "Yon basina sabit yesil isik suresi: ";
            std::cin >> yesilSure;

            for (auto& [yon, sure] : yesilSureleri)
                sure = yesilSure;

            return;
        }

        if (mod == 3)
        {
            int katsayi = 0;
            std::cout << "3 no.lu mod aktif.\n";

            for (int i = 1; i <= yonSayisi; ++i)
            {
                std::cout << i << ". yonde ortalama arac sayisi: ";
                std::cin >> ortalamaAracSayilari[i];
            }

            std::cout << "Yesil suresi / arac sayisi katsayisi: ";
            std::cin >> katsayi;

            for (auto& [yon, sure] : yesilSureleri)
                sure = katsayi * ortalamaAracSayilari[yon];

            return;
        }
    }

    auto ortalamaAracSayilariniGoster() -> std::map<int, int>
    {
        return ortalamaAracSayilari;
    }
    auto yesilSureleriniGoster() -> std::map<int, int>
    {
        return yesilSureleri;
    }

    void yardim()
    {
        std::cout << "                    -----                            MODLAR                            -----" << std::endl;
        std::cout << "1: Kullanicidan alinan faz sureleri" << std::endl;
        std::cout << "2: Kullanicidan alinan esit faz sureleri" << std::endl;
        std::cout << "3: Ort. arac sayilarina gore dogru orantili faz sureleri, kullanicidan alinan yesilSuresi : aracSayisi katsayisi" << std::endl;
    }

};

int main() 
{
    std::srand(std::time(nullptr));

    int yonSayisi, donguSayisi;

    std::map<int, int> yesilSureleri = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};
    std::map<int, int> ortalamaAracSayilari = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};

    std::cout << "Kavsakta birlesen yon sayisi: ";
    std::cin >> yonSayisi;
    std::cout << "Simulasyon kac dongu olarak uygulansin?";
    std::cin >> donguSayisi;

    TrafikIsigiSim sim(donguSayisi);
    TrafikIsikAlgoritmasi algoritma;

    algoritma.algoritmaAl(yonSayisi);

    yesilSureleri = algoritma.yesilSureleriniGoster();
    ortalamaAracSayilari = algoritma.ortalamaAracSayilariniGoster();

    for (int i = 1; i <= yonSayisi; i++) 
    {
        sim.yonEkle(i, ortalamaAracSayilari[i], yesilSureleri[i]);
    }

    sim.calistir();
    sim.rapor();

    system("pause");
    return 0;
}
