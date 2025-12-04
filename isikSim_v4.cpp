#include <iostream>
#include <map>

const int yonSayisi = 4;

auto yesilSureleriGecerlikKontrolu(const std::map<int, int>& yesilSureleri) -> bool;
auto simulasyon(const std::map<int, int>& yesilSureleri) -> int;

class trafikIsigi
{
    std::map<int, int> yesilSureleri = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};
    public:

    void yesilSureleriAl()
    {
        for (int i = 0; i < yonSayisi; ++i)
        {
            std::cout << i + 1 <<  ". yon yesil yanma suresini giriniz: ";
            std::cin >> yesilSureleri[i];
        }
    }

    auto yesilSureleriniGoster() -> std::map<int, int>
    {
        return yesilSureleri;
    }
}

int main()
{
    std::cout << "----- TRAFIK ISIGI SIMULASYON PROGRAMI -----" << std::endl;
    trafikIsigi.yesilSureleriAl();
    std::cout << "Simulasyon basladi." << std::endl;
    int ortBeklemeSuresi = simulasyon(trafikIsigi.yesilSureleriniGoster());
    if (ortBeklemeSuresi == -1)
    {
        std::cout << "Hata 1: Yesil sureleri gecersiz." << std::endl;
        return 0;
    }
    std::cout << "Ortalama bekleme suresi: " << ortBeklemeSuresi << std::endl;
    return 0;
}

auto simulasyon(const std::map<int, int>& yesilSureleri) -> int // ortBeklemeSuresi
{
    int beklemeSuresi = 0, ortBeklemeSuresi = 0;
    if (!yesilSureleriGecerlikKontrolu(yesilSureleri)) return -1;
}

auto yesilSureleriGecerlikKontrolu(const std::map<int, int>& yesilSureleri) -> bool
{
    for (auto yon : yesilSureleri)
    {
        if (yesilSureleri < 0)
        {
            return false;
        }
    }
    return true;
}
