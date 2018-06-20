#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define BASLA start = clock()
#define BIT end = clock()
#define SURE printf("Islem Suresi: %.3f", (double)(end - start) / CLOCKS_PER_SEC)
#define MAX 1024
clock_t start, end;

//YAPILAR
struct ogr {
    int no;
    char* ad;
    char* soyad;
    int sira;
    int ogretim;
};

//DEGISKENLER
struct ogr ogrenci[MAX];
char* silinenler[MAX] = {};
int silmeSayisi[MAX] = {};
int siniflar[MAX];
int numaralandir[2] = {0, 0};
int kacOgrenci[2] = {0,0};
int silinenlerUzunluk = 0, kacEklendiToplam = 0, sinifSayisi;

//AYARLAR
int listele = 0;

//FONKSIYONLAR
void ogrOku();
void ogrParse(char* str);
struct ogr *ogrOlustur(int no, char* ad, char* soyad, int sira, int ogretim);
void ogrListele();
void ogrSilme();
void silinenEkle(char *isim);
void ogrSirala(int i);
void ogrNumaralandir();
void siniflariAl();
void yazdir();
void esitDagilim();
void enAzDagilim();

int main(){
    BASLA;

    ogrOku();
    ogrListele();

    ogrSilme();
    ogrListele();

    ogrSirala(0);
    ogrListele();

    ogrNumaralandir();
    ogrListele();

    ogrSirala(1);
    ogrListele();

    BIT;
    SURE;

    siniflariAl();

    esitDagilim();
    enAzDagilim();

    return 0;
}

void ogrOku(){
    FILE* file = fopen("dosya.txt", "r");
    char line[512];
    while(fgets(line, sizeof(line), file)){ogrParse(line);}
    fclose(file);
}

void ogrParse(char* str){
    int ekleme = 0, nth = 0, no, sira, ogretim;
    char* par;
    char* ad;
    char* soyad;
    par = strtok (str, " \t");
    while (par !=  NULL){
        if(nth == 0){
            if(par[0] >= '0' && par[0] <= '9'){no = (int)strtod(par, NULL);}
            else if(strcmp(par, "-") == 0){no = 0;}
            else{ekleme = 1; break;}
        }else if(nth == 1){ad = par;}
        else if(nth == 2){soyad = par;}
        else if(nth == 3){sira = (int)strtod(par, NULL);}
        else if(nth == 4){
            if(strcmp(par, "I\n") == 0 || strcmp(par, "I") == 0){ogretim = 1;}
            else if(strcmp(par, "II\n") == 0 || strcmp(par, "II") == 0){ogretim = 2;}
        }
        nth++;
        par = strtok (NULL, " \t");
    }

    if(!ekleme){
        ogrenci[kacEklendiToplam] = *ogrOlustur(no, ad, soyad, sira, ogretim);
        kacEklendiToplam++;
    }
}

struct ogr *ogrOlustur(int no, char* ad, char* soyad, int sira, int ogretim){
    struct ogr *p = malloc(sizeof(struct ogr));
    p->no = no;
    p->ad = strdup(ad);
    p->soyad = strdup(soyad);
    p->sira = sira;
    p->ogretim = ogretim;
    return p;
}

void ogrListele(){
    if(listele){
        printf("\nListeleniyor:\n");
        int i, b = 0;
        for(i = 0; i < kacEklendiToplam; i++){
            if(ogrenci[i].ogretim != 0){
                printf("%7d %15s %-15s %5d %5d\n", ogrenci[i].no, ogrenci[i].ad,
                       ogrenci[i].soyad, ogrenci[i].sira, ogrenci[i].ogretim);
                b++;
            }
        }
        printf("\n");
    }
}

void ogrSilme(){
    int i = 0, j = 0;
    for(i = 0; i < kacEklendiToplam; i++){
        for(j = 0; j < i; j++){
            if(strcmp(ogrenci[i].ad, ogrenci[j].ad) == 0 &&
               strcmp(ogrenci[i].soyad, ogrenci[j].soyad) == 0){
                //ad ve soyadi birlestir
                char *merger = malloc(256);
                strcpy(merger, ogrenci[j].ad);
                size_t length = strlen(merger);
                merger[length] = ' ';
                merger[length + 1] = '\0';
                strcpy(merger, strcat(merger, ogrenci[j].soyad));

                silinenEkle(merger);
                ogrenci[i].sira = ogrenci[j].sira;
                ogrenci[j].ad = "";
                ogrenci[j].soyad = "";
                ogrenci[j].no = 0;
                ogrenci[j].sira = 0;
                ogrenci[j].ogretim = 0;
            }
        }
    }
    printf("Birden fazla kayit olan ogrenciler silindi.\n");
    for(i = 0; i < silinenlerUzunluk; i++)
        printf("%s %d kere silindi.\n", silinenler[i], silmeSayisi[i]);
}

void silinenEkle(char *isim){
    if(silinenlerUzunluk == 0){
        silinenler[0] = isim;
        silmeSayisi[0] = 1;
        silinenlerUzunluk++;
    }else{
        int i;
        for(i = 0; i < silinenlerUzunluk; i++){
            if(strcmp(silinenler[i], isim) == 0){
                silmeSayisi[i]++;
                break;
            }else{
                if(i + 1 == silinenlerUzunluk){
                    silinenler[silinenlerUzunluk] = isim;
                    silmeSayisi[silinenlerUzunluk] = 1;
                    silinenlerUzunluk++;
                    break;
                }else{continue;}
            }
        }
    }
}

void ogrSirala(int i){
    struct ogr swapper;
    int n = kacEklendiToplam, c = 0, d = 0, islem;
    for (c = 0;  c < ( n - 1 );  c++){
        for (d = 0;  d < n - c - 1;  d++){
            islem = i == 1 ? ogrenci[d].no > ogrenci[d + 1].no : i == 0 ? ogrenci[d].sira > ogrenci[d + 1].sira : 0;
            if(islem){
                swapper = ogrenci[d + 1];
                ogrenci[d + 1] = ogrenci[d];
                ogrenci[d] = swapper;
            }
        }
    }
}

void ogrNumaralandir(){
    int i;
    for(i = 0; i < kacEklendiToplam; i++){
        if(ogrenci[i].ogretim != 0){
            kacOgrenci[ogrenci[i].ogretim - 1]++;
            if(ogrenci[i].no == 0){
                ogrenci[i].no = 1700000 + (ogrenci[i].ogretim*1000)
                        + ++numaralandir[ogrenci[i].ogretim - 1];
            }
        }
    }
}

void siniflariAl(){
    char* siniflarString = malloc(256);

    printf("\nSinif kapasitelerini giriniz:");
    fgets(siniflarString, 256, stdin);

    //temizle
    if (siniflarString[strlen (siniflarString) - 1] == '\n')
        siniflarString[strlen (siniflarString) - 1] = '\0';

    char* par;
    int i = 0, j;
    par = strtok(siniflarString, " ");
    while (par !=  NULL){
        siniflar[i] = (int)strtod(par, NULL);
        i++;
        par = strtok (NULL, " ");
    }
    sinifSayisi = i;
    printf("\nSiniflar: (%d tane)\n",sinifSayisi);
    for(j = 0; j < i; j++){
        printf("%d,", siniflar[j]);
    }
}

void yazdir(int *ogrenciler, int ogretim, int oku){
    int i,j,b=0,toplamSilinen=0;
    for(i=0;i<silinenlerUzunluk;i++){
        toplamSilinen += silmeSayisi[i];
    }
    int kactaKaldi = 0;
    for (i=0;i<sinifSayisi;i++){

        char birlesim[256] = "";
        snprintf(birlesim, sizeof birlesim, "sinif%dogr%d.txt", i+1, ogretim+1);
        FILE *f = fopen(birlesim, oku ? "w" : "a");
        if (f == NULL){printf("Error opening file!\n");exit(1);}

        if(oku){
            fprintf(f, "--------------     ESIT DAGILIM     --------------\n");
        }else{
            fprintf(f, "\n-------------- EN AZ SINIF DAGILIMI --------------\n");
        }


        b = kactaKaldi;
        for(j = kactaKaldi; j < kacEklendiToplam; j++){
            if(b==kactaKaldi+ogrenciler[i]){
                kactaKaldi = j;
                break;
            }
            if(ogrenci[j].ogretim == ogretim+1){
                fprintf(f, "%7d %15s %-15s %5d %5d\n", ogrenci[j].no, ogrenci[j].ad, ogrenci[j].soyad, ogrenci[j].sira, ogrenci[j].ogretim);
                //printf("%7d %15s %-15s %5d %5d\n", ogrenci[j].no, ogrenci[j].ad, ogrenci[j].soyad, ogrenci[j].sira, ogrenci[j].ogretim);
                b++;
            }
        }
        fclose(f);
    }
}

void esitDagilim(){
    int ogrMevcut[2][MAX];
    int i,j,kalan,kapasite=0;

    for(j=0;j<2;j++){
        kapasite = 0;
        kalan = kacOgrenci[j];
        printf("\n\n%d. Ogretim Esit Dagilimi (%d kisi):\n", j+1, kalan);
        for (i=0;i<sinifSayisi;i++){
            ogrMevcut[j][i] = 0;
            kapasite += siniflar[i];
        }
        if(kapasite<kalan){
            printf("Kapasite yetersiz! Acikta kalan ogrenci sayisi: %d", kalan-kapasite);
        }else{
            while(kalan!=0){
                for (i=0;i<sinifSayisi;i++){
                    if(kalan==0){
                        break;
                    }else{
                        if(ogrMevcut[j][i]<siniflar[i]){
                            ogrMevcut[j][i]++;
                            kalan--;
                        }
                    }
                }
            }
            /*
            for (i=0;i<sinifSayisi;i++){
                printf("%-3d : %3d \n", siniflar[i], ogrMevcut[j][i]);
            }
            */
            yazdir(ogrMevcut[j],j,1);
        }
    }
}

void enAzDagilim(){
    int ogrSiniflar[2][MAX];
    int ogrMevcut[2][MAX];
    int i,j,kalan,kapasite=0;
    int enBuyukIndex;
    for(j=0;j<2;j++){
        enBuyukIndex=0;
        kapasite = 0;
        kalan = kacOgrenci[j];
        printf("\n\n%d. Ogretim En Az Sinif Dagilimi (%d kisi):\n", j+1, kalan);
        for (i=0;i<sinifSayisi;i++){
            ogrSiniflar[j][i] = siniflar[i];
            ogrMevcut[j][i] = 0;
            kapasite += siniflar[i];
        }
        if(kapasite<kalan){
            printf("Kapasite yetersiz! Acikta kalan ogrenci sayisi: %d", kalan-kapasite);
        }else{
            while(kalan>0){
                enBuyukIndex = 0;
                for (i=0;i<sinifSayisi;i++){
                    if(ogrSiniflar[j][enBuyukIndex]<ogrSiniflar[j][i]){
                        enBuyukIndex = i;
                    }
                }
                if(kalan>=ogrSiniflar[j][enBuyukIndex]){
                    ogrMevcut[j][enBuyukIndex] = ogrSiniflar[j][enBuyukIndex];
                    kalan = kalan - ogrSiniflar[j][enBuyukIndex];
                    ogrSiniflar[j][enBuyukIndex] = 0;
                }else{
                    ogrMevcut[j][enBuyukIndex] = kalan;
                    kalan = 0;
                    ogrSiniflar[j][enBuyukIndex] = ogrSiniflar[j][enBuyukIndex] - kalan;
                }
            }
            /*
            for (i=0;i<sinifSayisi;i++){
                printf("%-3d : %3d \n", siniflar[i], ogrMevcut[j][i]);
            }
            */
            yazdir(ogrMevcut[j],j,0);
        }
    }
}
