#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <tuple>
#include <map>
#include <algorithm>

#include "kutuphane.cpp"

void simulasyon();
auto yeniAracGirisiAl() -> std::pair<int, std::pair<int, int>>;
auto yesilOlanYon (const int gecenZaman) -> int;

const std::vector<std::pair<int, int>> hareketler = {{1, 2}, {1, 3}, {1, 4}, {2, 1}, {2, 3}, {2, 4}, {3, 1}, {3, 2}, {3, 4}, {4, 1}, {4, 2}, {4, 3}};

int main()
{
    simulasyon();
    return 0;
}

void simulasyon()
{
    int simulasyonSuresi;
    int eklenenAracSayisi = 0;
    bool aracEklemeSecimi = false;
    std::pair<int, std::pair<int, int>> aracBilgileri;
    std::map<int, std::tuple<int, int, int, int>> aracKonumlari; // <aracNo, <geldigiYon, gidecegiYon, konum, hiz>>
    std::map<int, int> araclarinBulunduguYon; // <aracNo, yon>

    std::cout << "Simulasyon suresini saniye cinsinden giriniz: ";
    std::cin >> simulasyonSuresi;

    for (int i = 0; i < simulasyonSuresi; ++i)
    {
        std::cout << "\n" << i << ". saniye -> YESIL YANAN YON: " << yesilOlanYon(i) << std::endl;

        while (true)
        { 
            std::cout << "Yeni arac eklemek ister misiniz (1 veya 0)? ";
            std::cin >> aracEklemeSecimi;

            if (aracEklemeSecimi)
            {
                aracBilgileri = yeniAracGirisiAl();
                aracKonumlari.insert({eklenenAracSayisi, {
                    aracBilgileri.second.first, // geldigi yon
                    aracBilgileri.second.second, // gidecegi yon
                    100, // baslangic konumu
                    aracBilgileri.first // hiz
                }});
                araclarinBulunduguYon.insert({eklenenAracSayisi, aracBilgileri.second.first});
                eklenenAracSayisi++;
                continue;
            }
            else
                break;
        }

        std::cout << "1 sn zaman gecti." << std::endl;

        for (auto& [aracNo, veri] : aracKonumlari)
        {
            if (araclarinBulunduguYon.at(aracNo) == std::get<0>(veri))
            {
                std::get<2>(veri) -= std::get<3>(veri); // konumu azalt
                if (std::get<2>(veri) < 0)
                    std::get<2>(veri) = 0;
            }
            else if (araclarinBulunduguYon.at(aracNo) == std::get<1>(veri))
            {
                std::get<2>(veri) += std::get<3>(veri); // konumu artir
                if (std::get<2>(veri) > 100)
                {
                    aracKonumlari.erase(aracNo);
                    araclarinBulunduguYon.erase(aracNo);
                    std::cout << aracNo << ". arac simulasyonu terk etti." << std::endl;
                    continue;
                }

            }


            // Arac ışığa ulaştıysa ve kendi yönü yeşilse
            if (std::get<2>(veri) == 0 && std::get<0>(veri) == yesilOlanYon(i))
            {
                araclarinBulunduguYon[aracNo] = std::get<1>(veri);
            }

            std::cout << aracNo << ". arac -> "
            << "Trafik isigina uzaklik: " << std::get<2>(veri)
            << " | Geldigi yon: " << std::get<0>(veri)
            << " | Gidecegi yon: " << std::get<1>(veri)
            << " | Bulundugu yon: " << araclarinBulunduguYon.at(aracNo)
            << " | Hiz: " << std::get<3>(veri)
            << std::endl;
        }
    }
}

auto yeniAracGirisiAl() -> std::pair<int, std::pair<int, int>>
{
    int gelinenYon, gidilenYon;
    int hiz;
    bool gecerli;

    while (true)
    {
        std::cout << "Yeni arac eklemek icin geldigi ve gidecegi yonu giriniz: " << std::endl;
        std::cin >> gelinenYon >> gidilenYon;

        if (std::find(hareketler.begin(), hareketler.end(), std::make_pair(gelinenYon, gidilenYon)) != hareketler.end())
        {
            std::cout << "Yon girisi basarili." << std::endl;
            break;
        }
        else 
        {
            std::cout << "Yon girisi gecersiz. Lutfen tekrar deneyiniz." << std::endl;
            continue;
        }
    }

    std::cout << "Aracin hizini m/s cinsinden giriniz: ";
    std::cin >> hiz;

    return {hiz, {gelinenYon, gidilenYon}};
}

auto yesilOlanYon (const int gecenZaman) -> int
{
    const int ilkYesilYananYon = 1;
    const int yesilSuresi1 = 30;
    const int yesilSuresi2 = 10;
    const int yesilSuresi3 = 30;
    const int yesilSuresi4 = 10;
    const int donguSuresi = yesilSuresi1 + yesilSuresi2 + yesilSuresi3 + yesilSuresi4;

    const int sadelesmisSure = gecenZaman % donguSuresi;

    if (sadelesmisSure < yesilSuresi1)
        return 1;
    if (sadelesmisSure < yesilSuresi1 + yesilSuresi2)
        return 2;
    if (sadelesmisSure < yesilSuresi1 + yesilSuresi2 + yesilSuresi3)
        return 3;
    if (sadelesmisSure < yesilSuresi1 + yesilSuresi2 + yesilSuresi3 + yesilSuresi4)
        return 4;
    return 0;
}
