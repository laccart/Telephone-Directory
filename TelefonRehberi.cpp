#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct _TelDef {		//Telefon bilgilerini tutan yapý
	char adi[30];
	char soyadi[30];
	char tel_num[12];
	struct _TelDef *sonraki;	//Listedeki bir sonraki yapýyý gösteren iþaretçi
} *_baslangic;		//Listenin baþlangýç adresini tutar
typedef struct _TelDef TelDef;

void kisiOku(TelDef *dugum, FILE *dosya)	//Dosyadan kiþi bilgisi okur
{
	fgets(dugum->adi, 30, dosya);	// Satýrý okur ve ilk parametrede gösterilen yere yazar
	fgets(dugum->soyadi, 30, dosya);
	fgets(dugum->tel_num, 12, dosya);
	dugum->adi[strlen(dugum->adi) - 1] = '\0';	//Beklenmedik karakter dönüþümü
	dugum->soyadi[strlen(dugum->soyadi) - 1] = '\0';
	dugum->tel_num[strlen(dugum->tel_num) - 1] = '\0';
}

void defteriOku()	//Dosyayý okur, her kiþi için bir düðüm oluþturup bilgileri
{					//saklar ve düðümleri birbirine baðlar
	TelDef *dugum;
	FILE *dosya = fopen("telefon defteri.txt", "r");
	if(!dosya)
		return;
	
	if(fgetc(dosya) == '#') {	//Dosyada # iþareti ile baþlayan bölüm bir kiþi bilgisidir
		_baslangic = dugum = (TelDef *) malloc(sizeof(TelDef));	/* Ýlk kiþi için
		yer ayrýlýr ve baþlangýç adresi olarak kaydedilir.*/
		
		kisiOku(dugum, dosya);
	}
	else {
		fclose(dosya);	// Dosya boþ ise kapat ve çýk.
		return;
	}
		
	while(fgetc(dosya) == '#') {	/*Devam eden kiþiler içinde yer ayrýlýr ve
		dugumlere kaydedilir*/
		
		dugum->sonraki = (TelDef *) malloc(sizeof(TelDef));
		dugum = dugum->sonraki;
		kisiOku(dugum, dosya);
	}
	dugum->sonraki = NULL;
	fclose(dosya);
}

bool yeniKisiEkle(TelDef *kisi)	//Eklenecek yeni kiþi için düðüm oluþturur ve
{								//hem belleðe hem de dosyaya yazar.
	TelDef *dugum = _baslangic;
	FILE *dosya = fopen("telefon defteri.txt", "a");
	if(!dosya)
		return false;
	
	fprintf(dosya, "#%s\n", kisi->adi);
	fprintf(dosya, "%s\n", kisi->soyadi);
	fprintf(dosya, "%s\n", kisi->tel_num);
	fclose(dosya);
	
	if(dugum) {
		while(dugum->sonraki)	//Listenin sonuna gelene kadar devam et
			dugum = dugum->sonraki;
		dugum->sonraki = (TelDef *) malloc(sizeof(TelDef));
		dugum = dugum->sonraki;
	}
	else	//Liste boþ ise ilk düðüm
		_baslangic = dugum = (TelDef *) malloc(sizeof(TelDef));
	dugum->sonraki = NULL;
	
	strcpy(dugum->adi, kisi->adi);
	strcpy(dugum->soyadi, kisi->soyadi);
	strcpy(dugum->tel_num, kisi->tel_num);
	return true;
}

char* numarayiBul(char adi[], char soyadi[])	//Ad ve soyada bakarak numarayý bulur
{
	TelDef *dugum = _baslangic;
	while(dugum) {
		if(!strcmp(adi, dugum->adi) && !strcmp(soyadi, dugum->soyadi))
			return dugum->tel_num;
		dugum = dugum->sonraki;
	}
	
	return NULL;
}

void dokumAl()	//Bütün listeyi yazdýrýr
{
	TelDef *dugum = _baslangic;
	if(!dugum)
		printf("\nListe bos!\n");
		
	while(dugum) {
		printf("%s\n%s\n%s\n\n", dugum->adi, dugum->soyadi, dugum->tel_num);
		dugum = dugum->sonraki;
	}
}

int main()
{
	char ch, *numara;
	TelDef kisi;
	
	defteriOku();	//Bunu mutlaka çaðýrýyoruz
	do {
		fflush(stdin);
		printf("\n(Y)eni kisi ekle:\n(N)umara bul:\nDoku(m) al\n(C)ik:\n");
		ch = getchar();
		
		if(ch == 'y') {
			fflush(stdin);
			printf("Eklenecek kisi\nAdi: ");
			gets(kisi.adi);
			printf("Soyadi: ");
			gets(kisi.soyadi);
			printf("Telefon numarasi: ");
			gets(kisi.tel_num);
			
			if(yeniKisiEkle(&kisi))
				printf("\nKisi eklendi\n");
			else
				printf("\nEklenemedi!\n");
		}
		
		if(ch == 'n') {
			fflush(stdin);
			printf("Bulunacak kisi\nAdi: ");
			gets(kisi.adi);
			printf("Soyadi: ");
			gets(kisi.soyadi);
			
			numara = numarayiBul(kisi.adi, kisi.soyadi);
			if(numara)
				printf("\nNumara: %s\n", numara);
			else
				printf("\nKisi bulunamadi!\n");
		}
		
		if(ch == 'm')
			dokumAl();
	} while(ch != 'c');

	return 0;
}
