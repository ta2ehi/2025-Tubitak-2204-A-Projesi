#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>
#include <cmath>

auto modSecici() -> int;

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
    private:
    auto monopolyYesilSureleriniBelirle(const int& yonSayisi, const int& cevrimSuresi, const std::vector<Yon>& yonBilgileri) -> std::map<int, int>
    {
        int toplamKuyrukUzunlugu = 0;
        for (auto yonBilgisi : yonBilgileri)
        {
            toplamKuyrukUzunlugu += yonBilgisi.kuyruk.size();
        }
        std::map<int, double> yonOranlari;
        for (auto yonBilgisi : yonBilgileri)
        {
            yonOranlari[yonBilgisi.id] = static_cast<double>(yonBilgisi.kuyruk.size() / toplamKuyrukUzunlugu);
        }
        std::map<int, int> yesilSureleri;
        for (int i = 0; i < yonSayisi; ++i)
        {
            yesilSureleri[i + 1] = std::round(yonOranlari[i + 1] * cevrimSuresi);
        }
        return yesilSureleri;
    }

    public:
    std::vector<Yon> yonler;
    int cevrimSayisi;

    TrafikIsigiSim(int adet) : cevrimSayisi(adet) {}

    void yonEkle(int id, double oran, double yesil) 
    {
        yonler.emplace_back(id, oran, yesil);
    }

    void calistirStatik() 
    {
        int simZaman = 0;

        for (int c = 0; c < cevrimSayisi; ++c) 
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

    void calistirDinamik(const int& mod, const int& cevrimSuresi)
    {
        if (mod == 1) // Monopoly Algoritmasi
        {
            int simZaman = 0;

            for (int c = 0; c < cevrimSayisi; ++c)
            {
                std::map<int, int> yesilSureleri = monopolyYesilSureleriniBelirle(yonler.size(), cevrimSuresi, yonler);
                for (auto& y : yonler)
                {
                    y.yesilSuresi = yesilSureleri[y.id];
                }
                for (auto& y : yonler) 
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
    }

    void rapor() 
    {
        std::cout << std::endl << "--- Simulasyon Sonuclari ---" << std::endl;
        double toplamBekleme = 0.0;
        double toplamArac = 0.0;

        for (auto& y : yonler) 
        {
            double toplam = 0.0;
            double ortBekleme = 0.0;
            for (int b : y.beklemeler) 
            {
               toplam += b;
            }
            double ort = y.beklemeler.empty() ? 0.0 : toplam / y.beklemeler.size();
            toplamArac += y.beklemeler.size();
            toplamBekleme += toplam;

            std::cout << "Yon " << y.id << " - Ortalama Bekleme: " << ort << " saniye (" << y.beklemeler.size() << " arac)" << std::endl;
        }
        if (toplamArac == 0)
        {
            std::cout << "Simulasyona hic arac girisi olmadi." << std::endl;
            return;
        }
        std::cout << "Ortalama bekleme suresi (tum yonler): " << toplamBekleme / toplamArac << std::endl;
    }
};

class StatikAlgoritmalar
{
    std::map<int, int> yesilSureleri;
    std::map<int, int> ortalamaAracSayilari; 
    int algoritmaModu = -1;
    /*
        -1: Tanimsiz
        0: Yardim
        1: Kullanicidan alinan faz sureleri
        2: Round Robin (RR) Algoritmasi
        3: Ort. arac sayilarina gore dogru orantili faz sureleri, kullanicidan alinan yesilSuresi : aracSayisi katsayisi
    */
    public:
    
    StatikAlgoritmalar() {}

    void algoritmaAl(const int& yonSayisi)
    {
        int mod;
        std::cout << "Mod giriniz (Yardim icin 0): ";
        std::cin >> mod;

        for (int i = 0; i < yonSayisi; ++i)
        {
            yesilSureleri[i + 1] = 0;
            ortalamaAracSayilari[i + 1] = 0;
        }

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
        if (mod == 4)
        {

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
        std::cout << "2: Round Robin (RR) Algoritmasi" << std::endl;
        std::cout << "3: Ort. arac sayilarina gore dogru orantili faz sureleri, kullanicidan alinan yesilSuresi : aracSayisi katsayisi" << std::endl;
    }

};

class DinamikAlgoritmalar
{
    public:

    DinamikAlgoritmalar() {}
    std::map<int, int> ortalamaAracSayilari;
    int monopolyCevrimSuresi = 0;
    int alinanMod = 0;

    void algoritmaAl(const int& yonSayisi)
    {
        int mod = -1;
        std::cout << "Mod giriniz (0: Yardim): ";
        std::cin >> mod;

        for (int i = 0; i < yonSayisi; ++i)
        {
            ortalamaAracSayilari[i + 1] = 0;
        }
        
        if (mod <= 0) 
            return;
        if (mod == 0)
            yardim();
        else
            alinanMod = mod;
        if (mod == 1)
        {
            std::cout << "1 no.lu mod aktif." << std::endl;
            for (auto& [yon, aracSayisi] : ortalamaAracSayilari)
            {
                std::cout << yon << ". yonde dakikada gelen ortalama arac sayisini giriniz: ";
                std::cin >> aracSayisi;
            }
            std::cout << "Bir cevrimin ne kadar surecegini giriniz: ";
            std::cin >> monopolyCevrimSuresi;
        }
    }

    auto ortalamaAracSayilariniGoster() -> std::map<int, int>
    {
        return ortalamaAracSayilari;
    }

    void yardim()
    {
        std::cout << "-----           MODLAR           -----" << std::endl;
        std::cout << "1: Monopoly Algoritmasi" << std::endl;
    }
};

int main() 
{
    std::srand(std::time(nullptr));
    int secilenMod = modSecici();
    if (secilenMod == 0)
        return 0;
    if (secilenMod == 1)
    {
        int yonSayisi, donguSayisi;

        std::map<int, int> yesilSureleri;
        std::map<int, int> ortalamaAracSayilari;

        std::cout << "Kavsakta birlesen yon sayisi: ";
        std::cin >> yonSayisi;
        std::cout << "Simulasyon kac dongu olarak uygulansin?";
        std::cin >> donguSayisi;

        TrafikIsigiSim sim(donguSayisi);
        StatikAlgoritmalar algoritma;

        algoritma.algoritmaAl(yonSayisi);

        yesilSureleri = algoritma.yesilSureleriniGoster();
        ortalamaAracSayilari = algoritma.ortalamaAracSayilariniGoster();

        for (int i = 1; i <= yonSayisi; i++) 
        {
            sim.yonEkle(i, ortalamaAracSayilari[i], yesilSureleri[i]);
        }

        sim.calistirStatik();
        sim.rapor();
    }
    if (secilenMod == 2)
    {
        int yonSayisi, donguSayisi;

        std::map<int, int> ortalamaAracSayilari;

        std::cout << "Kavsakta birlesen yon sayisi: ";
        std::cin >> yonSayisi;
        std::cout << "Simulasyon kac dongu olarak uygulansin?";
        std::cin >> donguSayisi;

        TrafikIsigiSim sim(donguSayisi);
        DinamikAlgoritmalar algoritma;

        algoritma.algoritmaAl(yonSayisi);

        std::cout << "Algoritma alimi basarili." << std::endl;

        ortalamaAracSayilari = algoritma.ortalamaAracSayilariniGoster();

        for (int i = 1; i <= yonSayisi; i++) 
        {
            sim.yonEkle(i, ortalamaAracSayilari[i], 0);
        }

        std::cout << "Yonler eklendi." << std::endl;

        sim.calistirDinamik(algoritma.alinanMod, algoritma.monopolyCevrimSuresi);
        sim.rapor();
    }
    

    system("pause");
    return 0;
}

auto modSecici() -> int
{
    /*
        <0: Gecersiz
        0: Yardim
        1: Statik modlar
        2: Dinamik modlar
    */

    int mod;

    std::cout << "Mod giriniz (0: Yardim): ";
    std::cin >> mod;
    if (mod < 0)
        return 0;
    if (mod == 0)
    {
        std::cout << "Modlar: " << std::endl;
        std::cout << "1: Statik Modlar" << std::endl;
        std::cout << "2: Dinamik Modlar" << std::endl;
        return 0;
    }
    return mod;
}
