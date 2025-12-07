#include <iostream>
#include <map>
#include <tuple>

using aracBilgileriTipi = std::map<int, std::tuple<int, int, int, int, int, bool>>; // <aracNo, <geldigiYon, gidecegiYon, bulunduguYon, isigaUzaklik, hiz, isiktaMi>>
using aracVerisiTipi = std::tuple<int, int, int, int, int, bool>;

int main()
{
    simulasyon(simulasyonSuresiAl());
    return 0;
}

auto simulasyon(const int simulasyonSuresi) -> int
{
    aracBilgileriTipi aracBilgileri;
    int eklenenAracSayisi = 0;

    bool secim = true;
    for (int gecenSure = 0; gecenSure < simulasyonSuresi; gecenSure++)
    {
        while (true)
        {
            std::cout << gecenSure << ". saniyede yeni arac eklemek ister misiniz? (1 veya 0): ";
            std::cin >> secim;
            if (secim)
            {
                aracBilgileri.insert({eklenenAracSayisi + 1, yeniAracGirisiAl(gecenSure)});
            }
        }
        
    }
}

auto simulasyonSuresiAl() -> int
{
    bool gecerliMi = false;
    int sure;
    do
    {
        std::cout << "Simulasyon suresini saniye cinsinden giriniz: ";
        std::cin >> sure;
        // Kontrol eklenebilir
        gecerliMi = true;
    } while (gecerliMi);
    return sure;
}

auto yeniAracGirisiAl(const int gecenSure) -> aracVerisiTipi
{
    int geldigiYon, gidecegiYon, hiz;
    std::cout << "Aracin geldigi yonu, gidecegi yonu ve hizini sirayla giriniz:" << std::endl;
    std::cin >> geldigiYon >> gidecegiYon >> hiz;
    return {geldigiYon, gidecegiYon, geldigiYon, 100, false};
}

void araclariIlerlet (aracBilgileriTipi& aracBilgileri)
{
    for (auto& [aracNo, veri] : aracBilgileri)
    {
        std::get<2>(veri) -= std::get<3>(veri);
        if (aracKirmizidaMi(kuyrukUzunluklari(aracBilgileri), veri))
        {
            std::get<2>(veri) = 0;
            std::get<5>(veri) = true;
        }
    }
}

auto aracKirmizidaMi(const std::map<int, int> kuyrukUzunluklari, const aracVerisiTipi aracVerisi) -> bool
{
    for (auto [yon, uzunluk] : kuyrukUzunluklari)
    {
        if (yon == std::get<4>(aracVerisi))
        {
            if (uzunluk >= std::get<2>(aracVerisi))
                return true;
            else
                return false;
        }
    }
    return false;
}

auto kuyrukUzunluklari(const aracBilgileriTipi aracBilgileri) -> std::map<int, int>
{
    const int aracUzunlugu = 3; // Ortalama arac uzunlugu bu degisken degistirilerek ayarlanacaktir.
    std::map<int, int> kuyruklar;
    for (auto [aracNo, veri] : aracBilgileri)
    {
        if (std::get<2>(veri) <= kuyruklar.at(std::get<4>(veri)))
        {
            kuyruklar.at(std::get<4>(veri))++;
        }
    }
    return kuyruklar;
}

/*
    Yeni arac ekleme eklendi, map olusturuldu. Sonraki adim: araclari ilerletme fonksiyonlari.
*/
