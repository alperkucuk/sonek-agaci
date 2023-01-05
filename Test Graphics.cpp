#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <math.h>
#define MAX_CHAR 256


int dugumSayisi=25;
int arananIndexler[5];
int arananIndexlerinElemanSayisi=0;
int arananIndexleriTut=0;
int arananIndex=30;
int dallanmaNumarasiTut=0;
int dallanmaNumarasiTut_i;



//SUFFÝX DÝZSÝ OLUSTURMA KODLARI////////////////////////////////////////////////////////////////////////////////////////////////
struct sonekAgaciDugumu {
	struct sonekAgaciDugumu *children[MAX_CHAR];
	struct sonekAgaciDugumu *sonekBaglantisi;//pointer sonek baðlantýsý ile dugumler arasi baglanti saglanir

	/*(start, end) interval, düðümün üst düðümüne baðlandýðý kenarý belirtir. 
	Her kenar, bir ebeveyn ve bir çocuk olmak üzere iki düðümü birbirine baðlar
	ve belirli bir kenarýn (baþlangýç, bitiþ) aralýðý alt düðümde dallanmaNumarasiTutnir. 
	Diyelim ki (5, 8) indeksli bir kenarla birbirine baðlanan iki düðüm A ve B var, 
	o zaman bu indeksler (5, 8)    B düðümünde dallanmaNumarasiTutnacak. */
	int start;
	int *end;

	//yaprak düðümleri için, kökten yapraða giden yolun sonek dizi dallanmaNumarasiTutr
	int sonekDizisi;
};
//////////////////////////////////////////////////////////////////

	int dugumNumarasi[50];
	int dugumNumarasisayac=0 ;
	
	
	typedef struct sonekAgaciDugumu Node;
	char index[50][50];
	int nodeSayisi=0;//Dugum Sayisi
	char okunan[100];  //Grilen string
	Node *root = NULL; //root dugumunu yani ilk nodu tutan pointer
	
	/*yeniDugumSonu, yeni oluþturulan dahili düðüme iþaret edecek 
		ve sonekBaglantisi nin ayarlanmasýný bekleyecek, 
		bu da ayný aþamanýn bir sonraki uzantýsýnda yeni bir sonek baðlantýsý (kök dýþýnda) alabilecek. 
		yeniDugumSonu, yeni oluþturulan dahili düðüm varsa 
		son ekini aldýðýnda, ayný fazýn bir sonraki uzantýsýnda 
		oluþturulan yeni dahili düðüme sýfýrlandýðýnda NULL olarak ayarlanýr. */
	
	Node *yeniDugumSonu = NULL;
	Node *aktifDugum = NULL;
	int count=0;
	
	//aktifKenar, girdi dizesi, karakter dizini olarak temsil edilir (karakterin kendisi deðil)	
	int aktifKenar = -1;
	int aktifUzunluk = 0;
	
	// kalanSonekSayisi tekrarDizisi aðaça daha kaç ek eklenecek
	int kalanSonekSayisi = 0;
	int yaprakSonu = -1;
	int *rootSonu = NULL;
	int *bolunmeSonu = NULL;
	int size = -1; //inpu string uzunlugu

Node *yeniDugum(int start, int *end)
{
	count++;
	Node *node =(Node*) malloc(sizeof(Node));
	int i;
	for (i = 0; i < MAX_CHAR; i++)
		node->children[i] = NULL;

	//Kök düðüm için, sonekBaglantisi NULL olarak ayarlanýr
	//Dahili düðümler için sonekBaglantisi root olarak ayarlanmalýdýr
	//cukent uzantýsýnda varsayýlan olarak ve sonraki uzantýda deðiþebilir
	node->sonekBaglantisi = root;
	node->start = start;
	node->end = end;

	//sonekDizisi varsayýlan olarak -1'e ayarlanýr ve gerçek sonek dizini
	//daha sonraki aþamasýnýn sonunda yapraklar için ayarlanýr
	node->sonekDizisi = -1;
	return node;
}





int kenarUzunluguFonk(Node *n) {
	return *(n->end) - (n->start) + 1;
}




int asagiGit(Node *gecerliDugum)
{
	//Atla/Sayým Trick'i (Trick 1) kullanarak aþaðý inmek (APCFWD) için activePoint deðiþikliði. 
	//aktifUzunluk cukent kenar uzunluðundan büyükse, sonraki dahili düðümü aktifDugum olarak ayarlayýn 
	//ve aktifKenar ve aktifUzunluk'u ayný activePoint
	if (aktifUzunluk >= kenarUzunluguFonk(gecerliDugum))
	{
		aktifKenar = (int)okunan[aktifKenar+kenarUzunluguFonk(gecerliDugum)]-(int)' ';
		aktifUzunluk -= kenarUzunluguFonk(gecerliDugum);
		aktifDugum = gecerliDugum;
		return 1;
	}
	return 0;
}



void sonekAgaciniGenislet(int konum)
{
	//Uzatma Kuralý 1, bu, o ana kadar aðaçta oluþturulan tüm yapraklarýn uzatýlmasýyla ilgilenir
	yaprakSonu = konum;

	//Aðaçta henüz eklenmemiþ eklerin listesine yeni bir ekin eklendiðini belirten kalanSonekSayýsýný artýr	
	kalanSonekSayisi++;

	// yeni bir aþamaya baþlarken yeniDugumSonu'yu NULL olarak ayarlayýn, 
	//cukent aþamasýnda soneki baðlantý sýfýrlamasýný bekleyen dahili bir düðüm olmadýðýný gösterir
	yeniDugumSonu = NULL;

	//Tüm son ekleri (henüz eklenmemiþ) aðaçta tek tek ekleyin
	while(kalanSonekSayisi > 0) {

		if (aktifUzunluk == 0) {
			//APCFALZ
			aktifKenar = (int)okunan[konum]-(int)' ';
		}
		//aktifDugum'dan aktifkenar ile baþlayan çýkýþ yok
		if (aktifDugum->children[aktifKenar] == NULL)
		{
			//Uzatma Kuralý 2 (Yeni bir yaprakDugum kenarý oluþturulur)
			aktifDugum->children[aktifKenar] = yeniDugum(konum, &yaprakSonu);

			//Yukarýdaki satýrda var olan bir düðümden (cukent aktifDugum) baþlayarak yeni bir yaprakDugum kenarý oluþturulur 
			//ve eðer son ek baðlantýsýnýn sýfýrlanmasýný bekleyen herhangi bir dahili düðüm varsa, o son dahili düðümden son ek baðlantýsýný cukent aktifDugum'a yönlendirin. 
			//Ardýndan, yeniDugumSonu'yu NULL olarak ayarlayýn, bu, son ek baðlantýsýnýn sýfýrlanmasýný bekleyen baþka bir düðüm olmadýðýný gösterir.
			if (yeniDugumSonu != NULL)
			{
				yeniDugumSonu->sonekBaglantisi = aktifDugum;
				yeniDugumSonu = NULL;
			}
		}
		// aktifDugum'dan aktifkenar ile baþlayan bir çýkýþ var
		else
		{
			//aktifkenar ile baþlayarak kenar sonunda bir sonraki düðümü al
			Node *next = aktifDugum->children[aktifKenar];
			if (asagiGit(next))
			{
				//Bir sonraki düðümden baþla (yeni aktifDugum)
				continue;
			}
			//Uzantý Kuralý 3 (iþlenmekte olan cukent karakteri zaten kenarda)
			if (okunan[next->start + aktifUzunluk] == okunan[konum])
			{
				//Yeni oluþturulan bir düðüm, son ek baðlantýsýnýn kurulmasýný bekliyorsa, o bekleyen düðümün son ek baðlantýsýný cukent aktif düðüme ayarlayýn.
				if(yeniDugumSonu != NULL && aktifDugum != root)
				{
					yeniDugumSonu->sonekBaglantisi = aktifDugum;
					yeniDugumSonu = NULL;
				}

				//APCFER3
				aktifUzunluk++;
				//Bu aþamada ekrarDizisi iþlemeyi durdurun ve bir sonraki aþamaya geçin
				break;
			}

			//ActivePoint, geçilen kenarýn ortasýndayken ve cukent karakter iþlenmekteyken kenarda olmadýðýnda burada olacaðýz (aðaçtan fatkrarDizisi alýyoruz). 
			//Bu durumda, yeni bir dahili düðüm ve bu yeni düðümden çýkan yeni bir yaprakDugum kenarý ekliyoruz. 
			//Bu, yeni bir yaprakDugum kenarýnýn ve yeni bir dahili düðümün oluþturulduðu Uzantý Kuralý 2'dir.
			bolunmeSonu = (int*) malloc(sizeof(int));
			*bolunmeSonu = next->start + aktifUzunluk - 1;

			//Yeni dahili düðüm
			Node *bol = yeniDugum(next->start, bolunmeSonu);
			aktifDugum->children[aktifKenar] = bol;

			//Yeni dahili düðümden çýkan yeni yaprakDugum
			bol->children[(int)okunan[konum]-(int)' '] =
									yeniDugum(konum, &yaprakSonu);
			next->start += aktifUzunluk;
			bol->children[aktifKenar] = next;

			//Burada yeni bir dahili düðümümüz var. 
			//Ayný fazýn son uzantýlarýnda oluþturulmuþ ve hala son ek baðlantý sýfýrlamasýný bekleyen bir dahili düðüm varsa, þimdi yapýn.
			if (yeniDugumSonu != NULL)
			{
			//yeniDugumSonu'dan sonekBaglantisi, yeni oluþturulan dahili düðümü iþaret ediyor
				yeniDugumSonu->sonekBaglantisi = bol;
			}

		//Cukent'in yeni oluþturulan dahili düðümünü, son ek baðlantý sýfýrlamasýný (þu anda köke iþaret ediyor) beklemesini saðlayýn. 
		//Ayný fazýn bir sonraki uzantýsýnda (mevcut veya yeni oluþturulmuþ) baþka bir dahili düðüme rastlarsak, 
		//o noktada yeni bir yaprakDugum kenarý eklendiðinde (yani Uzatma Kuralý 2 uygulandýðýnda, 
		//ayný fazýn sonraki uzantýlarýndan herhangi biri olduðunda), sonekBaglantisi bu düðüm, o iç düðüme iþaret edecektir.
			yeniDugumSonu = bol;
		}

		//Aðaçta bir son ek eklendi, henüz eklenmemiþ son eklerin sayýsýný azaltýn.
		kalanSonekSayisi--;
		if (aktifDugum == root && aktifUzunluk > 0) //APCFER2C1
		{
			aktifUzunluk--;
			aktifKenar = (int)okunan[konum -kalanSonekSayisi+1]-(int)' ';
		}
		else if (aktifDugum != root)
		{
			aktifDugum = aktifDugum->sonekBaglantisi;
		}
	}
}

void yazdir(int i, int j)
{
	int k;
	int tCouner=0;
	char dallanmaNumarasiTut[j];
	for (k=i; k<=j; k++){
	
		dallanmaNumarasiTut[tCouner]=okunan[k] ;
		tCouner++;
		printf("%c", okunan[k]);
	}
	dallanmaNumarasiTut[tCouner]='\0';
	strcpy(index[nodeSayisi],dallanmaNumarasiTut);
	nodeSayisi++;	
		
}

//sonek aðacýný, sonek indeksini ayarlamakla birlikte olacak þekilde yazdýr
//Dolayýsýyla aðaç DFS þeklinde yazdýrýlacak
//Her kenar, sonek dizini ile birlikte yazdýrýlacaktýr
void SonekDizisiniDegistirByDFS(Node *n, int labelHeight)
{
	if (n == NULL) return;

	if (n->start != -1) //root dugum degilse
	{
		yazdir(n->start, *(n->end)); //üst düðümden geçerli düðüme giden kenardaki etiketi yazdýr
	}
	
	
	int yaprakDugum = 1;
	int i;
	for (i = 0; i < MAX_CHAR; i++)
	{
		if (n->children[i] != NULL)
		{
			if (yaprakDugum == 1 && n->start != -1){
			
				printf("\t(%d)\n", n->sonekDizisi);
				dugumNumarasi[dugumNumarasisayac]=n->sonekDizisi ;
		        dugumNumarasisayac++;
		        }

			//Geçerli düðüm, ondan çýkan kenarlarý olduðu için bir yaprak dugum deðildir.
			yaprakDugum = 0;
			SonekDizisiniDegistirByDFS(n->children[i], labelHeight + kenarUzunluguFonk(n->children[i]));
		}
	}
	
	if (yaprakDugum == 1)
	{
		n->sonekDizisi = size - labelHeight;
		printf("\t(%d)\n", n->sonekDizisi);
		dugumNumarasi[dugumNumarasisayac]=n->sonekDizisi ;
		dugumNumarasisayac++;
	}
}

void sonekAgaciniCozBypostOrder(Node *n)
{
	if (n == NULL)
		return;
	int i;
	for (i = 0; i < MAX_CHAR; i++)
	{
		if (n->children[i] != NULL)
		{
			sonekAgaciniCozBypostOrder(n->children[i]);
		}
	}
	if (n->sonekDizisi == -1)
		free(n->end);
	free(n);
}

//Sonek aðacýný oluþturun ve kenar etiketlerini bunlarla birlikte yazýn
//sonek Dizisi. yaprakDugum kenarlarý için sonekDizisi >= 0 olacaktýr ve
//yaprak olmayanlar içinDugum kenarlarý -1 olacaktýr.

void SonekAgaciOlustur()
{
	size = strlen(okunan);
	int i;
	rootSonu = (int*) malloc(sizeof(int));
	*rootSonu = - 1;

	//Kök, baþlangýç ??ve bitiþ indeksleri -1 olan özel bir düðümdür, çünkü bir kenarýn köke geldiði yerden ebeveyni yoktur
	root = yeniDugum(-1, rootSonu);

	aktifDugum = root; //Ýlk aktifDugum root olacak
	for (i=0; i<size; i++)
		sonekAgaciniGenislet(i);
		
	int labelHeight = 0;
	SonekDizisiniDegistirByDFS(root, labelHeight);

	//Dinamik olarak ayrýlmýþ belleði boþaltýn
	sonekAgaciniCozBypostOrder(root);
}



//SUFFÝX DÝZSÝ OLUSTURMA KODLARI SONU////////////////////////////////////////////////////////////////////////////////////////////////




//-------------------------------------------------------------------------------------------------------------------------------------------------//
//Grafigi Cizme Kodlari*****************************************************************************
void rootuCiz(int x, int y){
	char str[30]="root";
	setcolor(YELLOW);//ROOT rengini degistir
	circle(x, y, 20);
	outtextxy(x - 2, y - 3, str);
}


void UstAgaciCiz(int x,int y,int array,char *stringDizi)
{
	if(dugumSayisi==array && dugumSayisi!=25)
	{
		setcolor(RED);
		circle(x, y, 20);
		outtextxy(x - 2, y - 3, stringDizi);
		dallanmaNumarasiTut++;
	}
	else if(arananIndex==array && arananIndex!=30)
	{
		setcolor(RED);
		circle(x, y, 20);
		outtextxy(x - 2, y - 3, stringDizi);
		dallanmaNumarasiTut++;
	}
	else if(arananIndexlerinElemanSayisi!=0 && arananIndexler[arananIndexleriTut]==array && dallanmaNumarasiTut==dallanmaNumarasiTut_i)
	{
		setcolor(RED);
		circle(x, y, 20);
		outtextxy(x - 2, y - 3, stringDizi);
		arananIndexleriTut++;
		dallanmaNumarasiTut++;
		
	}
	else 
	{
		circle(x, y, 20);
		outtextxy(x - 2, y - 3, stringDizi);
		dallanmaNumarasiTut++;
	}
}


void OrtaAgaciCiz(int x,int y,int array,char *stringDizi)
{	
	if(dugumSayisi==array && dugumSayisi!=25)
	{
		setcolor(RED);
		circle(x, y, 20);
		outtextxy(x - 2, y - 3, stringDizi);
		dallanmaNumarasiTut++;
	}
	else if(arananIndex==array && arananIndex!=30)
	{
		setcolor(RED);
		circle(x, y, 20);
		outtextxy(x - 2, y - 3, stringDizi);
		dallanmaNumarasiTut++;
	}
	else if(arananIndexlerinElemanSayisi!=0 && arananIndexler[arananIndexleriTut]==array)
	{
		setcolor(RED);
		circle(x, y, 20);
		outtextxy(x - 2, y - 3, stringDizi);
		arananIndexleriTut++;
		dallanmaNumarasiTut++;
		
	}
	else 
	{
		circle(x, y, 20);
		outtextxy(x - 2, y - 3, stringDizi);
		dallanmaNumarasiTut++;
	}
}

//Grafigi Cizme Kodlari Sonu*****************************************************************************
//------------------------------------------------------------------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GrafikteGoster_TekrarAdedi(int tekrariSay){
	setcolor(BROWN);
	char tekrarDizisi[17]="Tekrar adedi: " ;//grafikte en cok tekrar edenin alt katar adedi ifade etmek icin
	tekrarDizisi[14]=' ';
	tekrarDizisi[15]=tekrariSay+'0';//tekrar miktarini sayisal olarak grafikte gostemek icin
	tekrarDizisi[16]='\0';
	outtextxy(100, 400, tekrarDizisi);	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////









void pKatari_VarMi(char *p){//xa ve a yi bulamýyor. Tekrar sayisini bulmada sorun var
	//DOSYAYI OKUMA BOLUMU
	FILE *file ;
	char katar[50] ;
	if((file=fopen(p,"r"))==NULL){
		printf("Dosya acilamadi...\n");
	}
	
	fscanf(file, "%s",katar);
	fclose(file);
	//DOSYAYI OKUMA BOLUMU SONU
	
	
	int i,j=0,sayac=0;
	strcpy(okunan, katar); //katari okuanan kopyalar bu sayede sonek ogaci olusturulur.
	
	SonekAgaciOlustur();
	
	//aranmak istenen alt katarin kullanicidan alindigi bolum
	char altKatarMi[50];
	printf("aramak istediginiz altKatar'i girin :");
	scanf("%s",altKatarMi);
	//aranmak istenen alt katarin kullanicidan alindigi bolum sonu
	
	int x=300;
	int y=100;
	int counter2=0;
	
	
	int dallanmaSayisi[10];//elemanlari tutmak icin gecici dizi olustur
	for(i=0;i<10;i++)
	{
		dallanmaSayisi[i]=0;// gecicinin ilk 10 elemani 0 yapýlýr.
	}
	
	for(i=0;i<5;i++)
	{
		arananIndexler[i]=50;//aranan indexlerin ilk 5 i 50 yapilir.
	}

	//ARAMAYI BURADA YAPIYOR///////////////////////////////////////////////////////////////////////////////
/////	kok dugumse arama yapmaz.
			int no=0;
			int girilenAltKatarUzunlugu=strlen(altKatarMi);
			int sonektekiAltKatarUzunlugu=0,r=0;
			int tekrariSay=0;
		for(i=0;i<nodeSayisi;i++)
		{
		if(dugumNumarasi[i]==-1) //dugumden ayrilan yapraklar varsa
		{
			if(index[i][no]==altKatarMi[no])
			{
				no++;
				i--;
				sonektekiAltKatarUzunlugu++;
			}
			
			else
			{
				if(sonektekiAltKatarUzunlugu!=0)
				{
					for(j=0;j<dallanmaSayisi[i];j++)
					{
						if(index[i+j+1][r]==altKatarMi[no] &&  no<girilenAltKatarUzunlugu)
						{
							r++;
							no++;
							j--;
							sonektekiAltKatarUzunlugu++;
						}
						
						else if(girilenAltKatarUzunlugu==sonektekiAltKatarUzunlugu)
						{
							arananIndexler[0]=dugumNumarasi[i] ;//kullanicinin aradigi alt katarin ilk harfi dallari olan bir kokse o koku tutar.
							arananIndexler[1]=dugumNumarasi[i+j+1];//burda da i+j+1 ile aranan nodun degeri atanir.
							dallanmaNumarasiTut_i=i;
							break;
						}			
					}
					
					if (girilenAltKatarUzunlugu==sonektekiAltKatarUzunlugu)
						{
							exit(1);
						}
						else
						{
							sonektekiAltKatarUzunlugu=0; 
							no=0 ;
						}
				}
				else
				{
					no=0;
					i=i+dallanmaSayisi[i];
				}
				
				
			}
		}
		else{		//orta dugumleri gez
			if(dugumNumarasi[i]!=-1) //kok dugum degise
			{
				if(index[i][no]==altKatarMi[no])//kullanicinin girdigi katarin ilk harfi bizim alt katarin ilk harfine esit mi kontrolü
				{
					if(girilenAltKatarUzunlugu==sonektekiAltKatarUzunlugu)//girilenAltKatarUzunlugu sonektekiAltKatarUzunlugu'na(ilk harfi uyasan alt katarin uzunluguna) 
					{																									//esit olursa
					printf("Aranan index bulundu %s\n",index[i]);
					arananIndex=dugumNumarasi[i];//arananIndex o dugumun numarasina esitlenir. Bu sekilde 30 dan farklý olur.
					break ;
					}
					no++;
					i--;
					sonektekiAltKatarUzunlugu++;
				}			 
				else
				{
					no=0 ;
					sonektekiAltKatarUzunlugu=0;
				}
			}
		
		}
	}
	
	
	
	
	//aranan katarin sonekteki tekrar sayisini bulmak icin
//********************************************************************************************
		if(dugumNumarasi[i]==-1)
		{
					tekrariSay=2;
		}
		else {
				tekrariSay=1;
		}
		
//********************************************************************************************




////Grafikte Gosterme Kodlarý///////////////////////////////////////////////////////////////
	//aranan katar sonek icinde yok ise			
	if(sonektekiAltKatarUzunlugu!=girilenAltKatarUzunlugu)
	{
		int gd = DETECT, gm;
		initgraph(&gd, &gm, "None");
		setcolor(LIGHTRED);
	    line(150, 150, 450, 150); 
	    line(150, 200, 450, 200); 
	    outtextxy(160, 165, " Aranan Alt Karar Bulunamadi !!!");
		getch();//Ekraný acik tutmak icin
		closegraph();
	}
	
	//aranan katar sonek icinde var ise
	else if(sonektekiAltKatarUzunlugu==girilenAltKatarUzunlugu)
	{
		if(arananIndex==30)
		{
			for(i=0;i<5;i++)
			{
				if(arananIndexler[i]!=50)
				{
					arananIndexlerinElemanSayisi++ ;
				}
			}
		}
		
		
		
				
		int gd = DETECT, gm;
		initgraph(&gd, &gm, "None");
		
		int x=300 ;
		int y=100 ;
		int counter2=0,n=0,m=0;
		int temp[10];
		dallanmaNumarasiTut=0;
		char dallanmaNumarasiTutmaDizisi[10][10];
		
		for(i=0;i<nodeSayisi;i++)
		{
			m=0;
			if(dugumNumarasi[i]==-1) //dugumden ayrilan yapraklar varsa
			{
				strcpy(dallanmaNumarasiTutmaDizisi[dallanmaNumarasiTut],index[i+1]);
				dallanmaNumarasiTut++;
				strcpy(dallanmaNumarasiTutmaDizisi[dallanmaNumarasiTut],index[i+2]);
				//printf("dallanmaNumarasiTutma dizisi %s",dallanmaNumarasiTutmaDizisi[dallanmaNumarasiTut]);
				dallanmaNumarasiTut++;
				if(i+2<=6)
				{
					
					for(j=i+3;j<i+5;j++)
					{
						
						for(int k=0;k<2;k++)
						{
							if(strcmp(dallanmaNumarasiTutmaDizisi[k],index[j])==0)
							{
								temp[counter2]=dallanmaNumarasiTut ;
								m=1 ;
							}
							else 
							{
								n=1 ;
							}
						
						}
						if(m==0)
						{
							dallanmaNumarasiTut++ ;
							temp[counter2]=dallanmaNumarasiTut ;
						}
					}
					counter2++;
					dallanmaNumarasiTut=0;
				}
				else {
					temp[counter2]=dallanmaNumarasiTut;
				}
			}
		}
		
		counter2=0;
		int counter3=1;
		int counter4=1 ;
		for(i=0;i<nodeSayisi+1;i++)
		{
			
			char str2[20][20];
			if(i==0)//dongunun basindaysa rootu ekle
			{
			rootuCiz(x,y);
			}
			else
			{
				if(dugumNumarasi[i-1]==-1)
				{
					setcolor(YELLOW);
					UstAgaciCiz( (1.5*x- 5*y/(i) ), (y + 50) ,dugumNumarasi[i-1]*80,index[i-1]);
					line(x, y, ( 1.5*x- 5*y/(i) ), y + 50);
					int xSay=1.5*x-5*y/(i);//(1.4*x- 4*y/(i) ) UstAgaciCiz, line VE xSay BU DEGERLER ESÝT OLAMLI 
											   // CUNKU ROOTUN ALTINDAKI DUGUMLERÝ VE BU DUGUMLERE BAGLI CIGILERI BURDAN AYARLIYORUZ
					int nodeSayisi=y+50 ;
					if(counter2!=-1)
					{
						for(j=0;j<temp[counter2];j++)
						{
							setcolor(LIGHTCYAN);
							if(j==0)
							{
								OrtaAgaciCiz(((xSay-nodeSayisi)*counter3/2),y+100+(counter3*20),dugumNumarasi[i+j],index[i+j]);
								line(xSay, nodeSayisi, ((xSay-nodeSayisi)*counter3/2), y+100+(counter3*20)) ;
								counter3++;
							}
							else
							{
								OrtaAgaciCiz(((xSay+nodeSayisi)*(counter3))/4,y+100,dugumNumarasi[i+j],index[i+j]);
								line(xSay, nodeSayisi, ((xSay+nodeSayisi)*counter3)/4, y+100) ;
								counter3++;
							}
						}
						i=i+temp[counter2];
						counter2++;
					}
				}
				else
				{
					setcolor(YELLOW);
					OrtaAgaciCiz(((x-y)*counter4/2),y+200+(counter4*20),dugumNumarasi[i-1],index[i-1]);
					line(x, y, ((x-y)*counter4/2), y+200+(counter4*20)) ;
					counter4++;
				}
				GrafikteGoster_TekrarAdedi(tekrariSay);//Tekrar adedini gosteren fonksiyon
			}
		}
	
	getch();

	closegraph();
	}
	////Grafikte Gosterme Kodlarý Sonu///////////////////////////////////////////////////////////////
}



void enUzunAltKatar(char *p)
{
	FILE *file ;
	char katar[50] ;
	if((file=fopen(p,"r"))==NULL){
		printf("Dosya acilamadi...\n");
		exit(1);
	}
	
	fscanf(file, "%s",katar);//DOSYADAKÝ SONEKÝ OKU
	fclose(file);
	
    int i,j=0,sayac=0;
	strcpy(okunan, katar); //SONEKÝ okunana kopyala
	
	SonekAgaciOlustur();//sonek agacini almaya ve grafikte gostermeye yarar
	
	int enUzun[nodeSayisi];//nodlarin uzunlugunu tutmak icin olusturulan dizi
	for(i=0;i<nodeSayisi;i++)
	{
		enUzun[i]=strlen(index[i]);//nodlarin uzunlugu tek tek enUzun dizisine atilir.
	}
	//nodlarin uzunluklari kýyaslanir ve en uzun olani bulunur
	for(i=0;i<nodeSayisi;i++)
	{
		int gecici;
        for(j=0;j<nodeSayisi;j++)
		{
            if(enUzun[j]<=enUzun[i])
			{
                gecici=enUzun[i];  
                enUzun[i]=enUzun[j];
                enUzun[j]=gecici;
            }
        }
    }

    //TEKRARÝ SAYAN KÝSÝM--------------------------------------------------------------
	int tekrariSay=0,dallanmaNumarasiTut ;
    for(i=0;i<nodeSayisi;i++)
	{
		if(strlen(index[i])==enUzun[0])//Katarin uzunlugu enUzuna esitse (enUzunun ilk indeksi en buyuk strlen e sahiptir)
		{
			dallanmaNumarasiTut=dugumNumarasi[i];
			for(j=0;j<nodeSayisi;j++)
			{
				if(strcmp(index[i],index[j])==0)
				{
					tekrariSay++ ;
				}
			}
			break;
		}
	}
	//printf("En uzun alt katar %d numarali dugumde %d kez tekrar etmektedir.",dallanmaNumarasiTut,tekrariSay);//TEKRAR SAYISINI YAZDIRMA
	dugumSayisi=dallanmaNumarasiTut ;
 //TEKRARÝ SAYAN KÝSÝM--------------------------------------------------------------
	



//Grafik cizdirme bolumu ------------------------------------------------------------------------------------//

	int gd = DETECT, gm;
	initgraph(&gd, &gm, "None");
	
	int x=300 ;
	int y=100 ;
	int counter2=0,n=0,m=0;
	int temp[10];
	dallanmaNumarasiTut=0;
	char dallanmaNumarasiTutmaDizisi[10][10];
	for(i=0;i<nodeSayisi;i++)
	{
		m=0;
		if(dugumNumarasi[i]==-1)//dugumden ayrilan yapraklar varsa
		{
			strcpy(dallanmaNumarasiTutmaDizisi[dallanmaNumarasiTut],index[i+1]);//bir dugumun 2 yapragi oldugu icin burada ilk yapragi
			dallanmaNumarasiTut++;
			strcpy(dallanmaNumarasiTutmaDizisi[dallanmaNumarasiTut],index[i+2]);// buraya 2.yapragi tutuyoruz
			dallanmaNumarasiTut++;
			if(i+2<=6)//hata vermesini onlemek icin
			{
				for(j=i+3;j<i+5;j++)
				{
					for(int k=0;k<2;k++)
					{
						if(strcmp(dallanmaNumarasiTutmaDizisi[k],index[j])==0)
						{
							temp[counter2]=dallanmaNumarasiTut ;//dallanmaNumarasiTut i den sonra kactanesinin dallanma oldugunu tutar.
							m=1 ;
						}
						else 
						{
							n=1 ;
						}
					
					}
					if(m==0)
					{
						dallanmaNumarasiTut++ ;
						temp[counter2]=dallanmaNumarasiTut ;
					}
				}
				counter2++;
				dallanmaNumarasiTut=0;
				
			}
			else {
				temp[counter2]=dallanmaNumarasiTut ;
			}
			
		}
	
	}



	counter2=0;
	int counter3=1;
	int counter4=1 ;
	for(i=0;i<nodeSayisi+1;i++)
	{
		
		char str2[20][20];
		if(i==0)//dongunun basindaysa rootu ekle
		{
		rootuCiz(x,y);
		}
		else
		{
			if(dugumNumarasi[i-1]==-1)
			{
				setcolor(YELLOW);
				//counter2++;
				
				UstAgaciCiz( (1.5*x- 5*y/(i) ), (y + 50) ,dugumNumarasi[i-1]*80,index[i-1]);
				line(x, y, ( 1.5*x- 5*y/(i) ), y + 50);
				int xSay=1.5*x-5*y/(i);//(1.4*x- 4*y/(i) ) UstAgaciCiz, line VE xSay BU DEGERLER ESÝT OLAMLI 
										   // CUNKU ROOTUN ALTINDAKI DUGUMLERÝ VE BU DUGUMLERE BAGLI CIGILERI BURDAN AYARLIYORUZZ
				int nodeSayisi=y+50 ;
				if(counter2!=-1)
				{
					for(j=0;j<temp[counter2];j++)
					{
						setcolor(LIGHTCYAN);
						if(j==0)
						{
							OrtaAgaciCiz(((xSay-nodeSayisi)*counter3/2),y+100+(counter3*20),dugumNumarasi[i+j],index[i+j]);
							line(xSay, nodeSayisi, ((xSay-nodeSayisi)*counter3/2), y+100+(counter3*20)) ;
							counter3++;
						}
						else
						{
							OrtaAgaciCiz(((xSay+nodeSayisi)*(counter3))/4,y+100,dugumNumarasi[i+j],index[i+j]);
							line(xSay, nodeSayisi, ((xSay+nodeSayisi)*counter3)/4, y+100) ;
							counter3++;
						}
						
						
					}
					i=i+temp[counter2];
					counter2++;
				}
				
			}
			else
			{
				setcolor(YELLOW);
				OrtaAgaciCiz(((x-y)*counter4/2),y+200+(counter4*20),dugumNumarasi[i-1],index[i-1]);
				line(x, y, ((x-y)*counter4/2), y+200+(counter4*20)) ;
				counter4++;
			}
		}
	}
	
	GrafikteGoster_TekrarAdedi(tekrariSay);//Tekrar adedini gosteren fonksiyon
	
	getch();

	closegraph();
    
	
}








void enFazlaTekrar(char *p)
{
	//dosyayi okuma bolumu
	FILE *file ;
	char katar[50] ;
	if((file=fopen(p,"r"))==NULL){
		printf("Dosya acilamadi...\n");
		exit(1);
	}

	fscanf(file, "%s",katar);
	fclose(file);
	//dosyayi okuma bolumu sonu
	
	int i,j=0,sayac=0;
	strcpy(okunan, katar); // 
	
	SonekAgaciOlustur();
	//adet sayarak en cok tekrar edeni bulup, ilk nodun kacinci node oldugunu bulmaya yarar.////////////////////////////////////////
	int adetSay[nodeSayisi];
	for(i=0;i<nodeSayisi;i++)
	{
		adetSay[i]=0;
	}
	
	for(i=0;i<nodeSayisi;i++)
	{
		for(j=0;j<nodeSayisi;j++)
		{
			if(strcmp(index[i],index[j])==0)
			{
				adetSay[i]=adetSay[i]+1 ;
			}
		}
	}
	//adetSay daki degerleri adetSay2 ye kopyalar
	int adetSay2[nodeSayisi];
	for (i=0;i<nodeSayisi;i++){
		adetSay2[i]=adetSay[i];
	}
	//adetSay2yi i buyukten kucuge sýraldýk ki 0 inci elemani en buyuk elemani tutsun
	int temp1;
	for(i=0;i<nodeSayisi;i++)
	{
        for(j=0;j<nodeSayisi;j++)
		{
            if(adetSay2[j]<=adetSay2[i])
			{
                temp1=adetSay2[i];  
                adetSay2[i]=adetSay2[j];
                adetSay2[j]=temp1;
            }
        }
    }
    
    int tekrariSay=0,dallanmaNumarasiTut ;
    for(i=0;i<nodeSayisi;i++)
	{
		if(adetSay[i]==adetSay2[0])//en cok tekrar edene geldiginde
		{
			dallanmaNumarasiTut=dugumNumarasi[i];//dallanmaNumarasini en cok tekrar eden katarin ilkine esitler
			tekrariSay=adetSay2[0];//en buyuk eleman en cok tekrar eden degeri verir.
			break;
		}
	}
	dugumSayisi=dallanmaNumarasiTut ;
	//adet sayarak en cok tekrar edeni bulup, ilk nodun kacinci node oldugunu bulmaya yarar.////////////////////////////////////////
	
	
	
	
	//grafigi cizdirme bolumu----------------------------------------------------------------
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "None");
	
	int x=300 ;
	int y=100 ;
	int counter2=0;
	dallanmaNumarasiTut=0;
	int n=0,m=0;
	int temp[10];
	char dallanmaNumarasiTutmaDizisi[10][10];
	
	
	for(i=0;i<nodeSayisi;i++)
	{
		m=0;
		if(dugumNumarasi[i]==-1)//dugumden ayrilan yapraklar varsa
		{
			strcpy(dallanmaNumarasiTutmaDizisi[dallanmaNumarasiTut],index[i+1]);
			dallanmaNumarasiTut++;
			strcpy(dallanmaNumarasiTutmaDizisi[dallanmaNumarasiTut],index[i+2]);
			dallanmaNumarasiTut++;
			if(i+2<=6)
			{
				
				for(j=i+3;j<i+5;j++)
				{
					
					for(int k=0;k<2;k++)
					{
						if(strcmp(dallanmaNumarasiTutmaDizisi[k],index[j])==0)
						{
							temp[counter2]=dallanmaNumarasiTut ;
							m=1 ;
						}
						else 
						{
							n=1 ;
						}
					
					}
					if(m==0)
					{
						dallanmaNumarasiTut++ ;
						temp[counter2]=dallanmaNumarasiTut ;
					}
				}
				counter2++;
				dallanmaNumarasiTut=0;
				
			}
			else {
				temp[counter2]=dallanmaNumarasiTut ;
			}
			
		}
	
	}
	

	counter2=0;
	int counter3=1;
	int counter4=1 ;
	
	for(i=0;i<nodeSayisi+1;i++)
	{
		
		char str2[20][20];
		if(i==0)//dongunun basindaysa rootu ekle
		{
		rootuCiz(x,y);
		}
		else
		{
			if(dugumNumarasi[i-1]==-1)
			{
				setcolor(YELLOW);	
				UstAgaciCiz( (1.5*x- 5*y/(i) ), (y + 50) ,dugumNumarasi[i-1]*80,index[i-1]);
				line(x, y, ( 1.5*x- 5*y/(i) ), y + 50);
				int xSay=1.5*x-5*y/(i);//(1.4*x- 4*y/(i) ) UstAgaciCiz, line VE xSay BU DEGERLER ESÝT OLAMLI 
										   // CUNKU ROOTUN ALTINDAKI DUGUMLERÝ VE BU DUGUMLERE BAGLI CIGILERI BURDAN AYARLIYORUZ
				int nodeSayisi=y+50 ;
				if(counter2!=-1)
				{
					for(j=0;j<temp[counter2];j++)
					{
						setcolor(LIGHTCYAN);
						if(j==0)
						{
							OrtaAgaciCiz(((xSay-nodeSayisi)*counter3/2),y+100+(counter3*20),dugumNumarasi[i+j],index[i+j]);
							line(xSay, nodeSayisi, ((xSay-nodeSayisi)*counter3/2), y+100+(counter3*20)) ;
							counter3++;
						}
						else
						{
							OrtaAgaciCiz(((xSay+nodeSayisi)*(counter3))/4,y+100,dugumNumarasi[i+j],index[i+j]);
							line(xSay, nodeSayisi, ((xSay+nodeSayisi)*counter3)/4, y+100) ;
							counter3++;
						}
						
						
					}
					i=i+temp[counter2];
					counter2++;
				}
				
			}
			else
			{
				setcolor(YELLOW);
				OrtaAgaciCiz(((x-y)*counter4/2),y+200+(counter4*20),dugumNumarasi[i-1],index[i-1]);
				line(x, y, ((x-y)*counter4/2), y+200+(counter4*20)) ;
				counter4++;
			}
		}
	}
	
	GrafikteGoster_TekrarAdedi(tekrariSay);//Tekrar adedini gosteren fonksiyon
	
	getch();

	closegraph();
		
}





//SONEK AGACINI ÇZÝDÝRME FONKSÝYONU/////////////////////////////////////////////////////////////
void sonekMi(char *p){
	//DOSYA ACMA VE OKUMA KODU+++++++++++++++++++++++++++++++++++++++
	FILE *file;
	char katar[50];
	if((file=fopen(p,"r"))==NULL){
		printf("Dosya acilamadi...\n");
		exit(1);
	}
	fscanf(file, "%s",katar);

	fclose(file);
	//DOSYA ACMA VE OKUMA KODU SONU+++++++++++++++++++++++++++++++++++
	
	int i,j=0,sayac=0;
	strcpy(okunan, katar); //Dosyadan okunan katari okunan dizisine kopyala
	
	SonekAgaciOlustur();
	
	int katarBoyutu=strlen(katar);
	char onek[katarBoyutu][katarBoyutu];
	
	for(i=0;i<katarBoyutu;i++){
		if(i==katarBoyutu-1){
			strcpy(onek[i],katar);
		}
		else{
		memset(onek[i], '\0', sizeof(onek[i]));
		strncpy(onek[i],katar,i+1);
		}
	}
	char sonek[katarBoyutu+1][katarBoyutu+1] ;
	char sonek2[katarBoyutu+1][katarBoyutu+1] ;
	
	for(i=0;i<katarBoyutu;i++){
		for(j=0;j<katarBoyutu;j++){
			sonek[i][j-i]=katar[j] ;
			
			if(i==0){
				if(j==katarBoyutu-1){
				sonek[i][katarBoyutu]='\0';
				}
			}
			else if(i!=0){
				if(j==katarBoyutu-1){
				sonek[i][katarBoyutu-i]='\0';
				}
			}
			
		}
		strcpy(sonek2[i],sonek[i]);
	}
	
	
	sayac=0 ;
	for(i=0;i<katarBoyutu;i++){
		for(j=0;j<katarBoyutu;j++){
			if(strcmp(sonek2[i], onek[j])==0){
				sayac++ ;
			}	
		}
	}
	
	if(sayac>=3){//sayac 3ten buyuk veya esitse sonek dizisi yoktur.
		int gd = DETECT, gm;
		initgraph(&gd, &gm, "None");
		setcolor(LIGHTRED);
	    line(150, 150, 450, 150); 
	    line(150, 200, 450, 200); 
	    outtextxy(160, 165, " Boyle Bir Sonek Dizisi Yoktur !!!");
		getch();//Ekraný acik tutmak icin
		closegraph();
	}
	else
	{
		int gd = DETECT, gm;
		initgraph(&gd, &gm, "None");
		
		int x=300 ;
		int y=100,j ;
		int counter2=0,n=0,m=0;
		int temp[10];
		
		int dallanmaNumarasiTut=0;
		char dallanmaNumarasiTutmaDizisi[10][10];
		for(i=0;i<nodeSayisi;i++)
		{
			m=0;
			if(dugumNumarasi[i]==-1)//dugumden ayrilan yapraklar varsa
			{
				strcpy(dallanmaNumarasiTutmaDizisi[dallanmaNumarasiTut],index[i+1]);
			//	printf("dallanmaNumarasiTutmaDizisi : %s i :%d\n",dallanmaNumarasiTutmaDizisi[dallanmaNumarasiTut],i);
				dallanmaNumarasiTut++;
				strcpy(dallanmaNumarasiTutmaDizisi[dallanmaNumarasiTut],index[i+2]);
			//	printf("dallanmaNumarasiTutmaDizisi : %s i :%d\n",dallanmaNumarasiTutmaDizisi[dallanmaNumarasiTut],i);
				dallanmaNumarasiTut++;
				if(i+2<=6)
				{
					for(j=i+3;j<i+5;j++)
					{
						
						for(int k=0;k<2;k++)
						{
							if(strcmp(dallanmaNumarasiTutmaDizisi[k],index[j])==0)
							{
								temp[counter2]=dallanmaNumarasiTut ;
								m=1 ;
							}
							else 
							{
								n=1 ;
							}
						
						}
						if(m==0)
						{
							dallanmaNumarasiTut++ ;
							temp[counter2]=dallanmaNumarasiTut ;
						}
					}
				//	printf("temp %d\n",temp[counter2]);
					counter2++;
					dallanmaNumarasiTut=0;
				}
				else {
					temp[counter2]=dallanmaNumarasiTut ;
				}
				
			}
		
		}
	/*
		for(i=0;i<dallanmaNumarasiTut;i++)
		{
			printf("%d\n",temp[i]);
		}
		*/
		counter2=0;
		int counter3=1;
		int counter4=1 ;
		for(i=0;i<nodeSayisi+1;i++)
		{
			
			char str2[20][20];
			if(i==0)//dongunun basindaysa rootu ekle
			{
			rootuCiz(x,y);
			}
			else
			{
				if(dugumNumarasi[i-1]==-1)// ROOTUN ALTINDAKI DUGUMLER
				{
					setcolor(YELLOW);//ROOTUN ALTINDAKI DUGUMLER COLOR
					//counter2++;
					
					UstAgaciCiz( (1.5*x- 5*y/(i) ), (y + 50) ,dugumNumarasi[i-1]*80,index[i-1]);
					line(x, y, ( 1.5*x- 5*y/(i) ), y + 50);//rota dogrudan baglý dallanmasi olan dugumlerin cizgileri
					int xSay=1.5*x-5*y/(i);//(1.4*x- 4*y/(i) ) UstAgaciCiz, line VE xSay BU DEGERLER ESÝT OLAMLI 
										   // CUNKU ROOTUN ALTINDAKI DUGUMLERÝ VE BU DUGUMLERE BAGLI CIGILERI BURDAN AYARLIYORUZ
					int nodeSayisi=y+50 ;				
					
					if(counter2!=-1)
					{
						for(j=0;j<temp[counter2];j++)
						{
							setcolor(LIGHTCYAN);
							if(j==0)
							{
								OrtaAgaciCiz(((xSay-nodeSayisi)*counter3/2),( y+100+(counter3*20) ) ,dugumNumarasi[i+j],index[i+j]);
								line(xSay, nodeSayisi, ((xSay-nodeSayisi)*counter3/2), ( y+100+(counter3*20) )  ) ;
								counter3++;
							}
							else
							{
								OrtaAgaciCiz(((xSay+nodeSayisi)*(counter3))/4,y+100,dugumNumarasi[i+j],index[i+j]);
								line(xSay, nodeSayisi, ((xSay+nodeSayisi)*counter3)/4, y+100) ;
								counter3++;
							}
							
							
						}
						i=i+temp[counter2];
						counter2++;
					}
					
				}
				else
				{
					setcolor(YELLOW);
					OrtaAgaciCiz(((x-y)*counter4/2),y+200+(counter4*20), dugumNumarasi[i-1],index[i-1]);
					line(x, y, ((x-y)*counter4/2), y+200+(counter4*20)) ;//rota dogrudan baglý dallanmasi olmayan dugumlerin cizgileri
					counter4++;
				}
			}
		}
		getch();

	closegraph();
	}
	
}









int main(int argc, char *argv[]) {
	printf("S katarinin okunacagi dosyanin adini giriniz:\n");
	char dosyaAdi[30] ;
	scanf("%s",&dosyaAdi);
	char * file ;
	file=dosyaAdi ;
	
	while(1){
		
	printf("\t1. S katari icin sonek agaci olusturulabilir mi?\n\n\
	2. Sonek agaci olusturulan bir s katari icinde p katari geciyor mu,\n\tGeciyorsa ilk bulundugu yerin baslangic pozisyonu nedir, \n\tKac kez tekrar etmektedir?\n\n\
	3. Sonek agaci olusturulan bir s katari icinde tekrar eden en uzun altkatar nedir, \n\tKac kez tekrar etmektedir?\n\n\
	4. Sonek agaci olusturulan bir s katari icinde en cok tekrar eden altkatar nedir, \n\tKac kez tekrar etmektedir?\n\n\
	5. Exit\n\n");

	printf("\tLutfen bir islem numarasi girin...\n");
	fflush(stdin);
	
	int n;
	scanf("%d",&n);
	
		switch(n) 
		 { 
		 case 1:  sonekMi(file);
		 break; 
		
		 case 2: pKatari_VarMi(file); 
		 break; 
		 
		 case 3: enUzunAltKatar(file);
		 break;
		  
		 
		 case 4: enFazlaTekrar(file);  
		 break; 
		 
		 case 5: exit(0); 
		 break;
		 
		 default: printf("Gecersiz secim\n"); 
		 } 
		return 0;
	}	
}
