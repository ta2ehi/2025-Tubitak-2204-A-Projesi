#include <iostream>
#include  <fstream>
#include      <map>
#include    <cmath>

class trafikIsigi
{
    private:
        std::map<int, int> yesilSureleri = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};
        std::map<int, int> ortalamaAracSayilari = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};
        std::map<int, int> fazBasinaAracSayilari = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};
        std::map<int, int> bekleyenAracSayilari = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};
        int katsayi;
        int simulasyonSuresi;

        void yesilOlanYonuGuncelle(const int gecenSure)
        {
            int sure = 0;
            for (auto [yon, yesilSuresi] : yesilSureleri)
            {
                sure += yesilSuresi;
                if (sure >= gecenSure) 
                {
                    yesilOlanYon = yon;
                    break;
                }
            }
        }
        void yesilSureleriAl (const int& katsayi)
        {
            for (auto [yon, aracSayisi] : ortalamaAracSayilari)
            {
                yesilSureleri.at(yon) = aracSayisi * katsayi;
            }
        }
        void fazBasinaAracSayisiHesapla()
        {
            for (auto [yon, aracSayisi] : ortalamaAracSayilari)
            {
                fazBasinaAracSayilari[yon] = ortalamaAracSayilari[yon] / 60 * yesilSureleri[yon]
            }
        } 
        void araclariEkle()
        {
            for (auto [yon, aracSayisi] : fazBasinaAracSayilari)
            {
                bekleyenAracSayilari[yon] += fazBasinaAracSayilari[yon];
            }
        }
    public:
        int yesilOlanYon;

        void verileriAl()
        {
            for (auto& [yon, aracSayisi] : ortalamaAracSayilari)
            {
                std::cout << yon << " yonundeki dakikada gelen ortalama arac sayisini giriniz: ";
                std::cin >> aracSayisi;
            }

            std::cout << "Yesil suresi / arac sayisi / dakika katsayisi giriniz: ";
            std::cin >> katsayi;

            std::cout << "Simulasyon suresi giriniz: ";
            std::cin >> simulasyonSuresi;

            std::cout << "Toplam bekleme suresi: " << simulasyon(simulasyonSuresi, ortalamaAracSayilari) << std::endl;
        }
        auto simulasyon() -> int
        {
            for (int i = 0; i < simulasyonSuresi; ++i)
            {
                yesilOlanYonuGuncelle(i);

            }
        }
        trafikIsigi(std::map<int, int> _ortalamaAracSayilari) : ortalamaAracSayilari(_ortalamaAracSayilari) {}
};

int main()
{
    std::map<int, int> ortalamaAracSayilari = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};
    int katsayi;
    int simulasyonSuresi;

    auto dosya = std::ofstream {"veriler_v3.txt"};
    
    std::cout << "Trafik isigi simulasyonu basladi." << std::endl;
    
    for (auto& [yon, aracSayisi] : ortalamaAracSayilari)
    {
        std::cout << yon << " yonundeki dakikada gelen ortalama arac sayisini giriniz: ";
        std::cin >> aracSayisi;
    }

    std::cout << "Yesil suresi / arac sayisi / dakika katsayisi giriniz: ";
    std::cin >> katsayi;

    std::cout << "Simulasyon suresi giriniz: ";
    std::cin >> simulasyonSuresi;

    std::cout << "Toplam bekleme suresi: " << simulasyon(simulasyonSuresi, ortalamaAracSayilari) << std::endl;
    return 0;    
}

auto simulasyon(const int simulasyonSuresi, const std::map<int, int> ortalamaAracSayilari) -> int
{
    trafikIsigi kavsak = trafikIsigi(ortalamaAracSayilari);
    kavsak.yesilSureleriAl(katsayi);
    kavsak.fazBasinaAracSayisiHesapla();


}
