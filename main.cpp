/** @file main.cpp
	@brief Hlavni soubor programu.
*/
/**
	@author Daniel Blazek
	@version eShop 1.1
*/

/**@mainpage Daniel Blazek, BLA0172, cviceni (C/07 st 16.00)
	Aplikace eShop jako semestrali projekt do Programovani I
*/

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

using namespace std;

typedef struct shop /** @brief Zakladni struktura promennych pouzitych pro produkty eshopu*/
{
    long int id; /**< @brief promenna pro ulozeni ID produktu */
    char nazev[50]; /**< @brief promenna pro ulozeni nazvu produktu */
    char popis[250]; /**< @brief promenna pro ulozeni popisu produktu */
    long int cena; /**< @brief promenna pro ulozeni ceny produktu */
    long int pocet_kusu; /**< @brief promenna pro ulozeni poctu kusu zbozi, ktery je na sklade */
}shop;


void clear_screen(void) //Vycisteni obrazovky (funkcni jak na Win. tak na Linuxovem jadre)
{
    /** @brief Funkce pro vycisteni obrazovky (Clear Screen), funkcni jak na Win. tak na Linuxovem jadre.*/
    #ifdef linux
        system("clear");
    #else
        system("cls");
    #endif
}

void chyba(void) //Vypis chybove hlasky - pouze hlavicka
{
    /** @brief Funkce pro vypis chybove hlasky - pouze hlavicka.*/
    clear_screen();
    cout.width(50);
    cout << "***********************\n";
    cout.width(50);
    cout << "*      ! Chyba !      *\n";
    cout.width(51);
    cout << "***********************\n\n";
}

void konec_programu(void) //Ukonceni programu - pouze hlavicka
{
    /** @brief Funkce pro vypis o ukonceni programu - pouze hlavicka.*/
    clear_screen();
    cout.width(50);
    cout << "***********************\n";
    cout.width(50);
    cout << "*    Konec programu   *\n";
    cout.width(51);
    cout << "***********************\n\n";
}

void chyba_nepovolene_znaky(void) //Vypis chybove hlasky - po zadani nepovolenych znaku u jmen souboru
{
    /** @brief Funkce pro vypis chybove hlasky - po zadani nepovolenych znaku u jmen souboru.
        @brief Za nepovolene jsou povazovany: / : . * ? " < > |\
    */

    cout << "Zadal jste spatny nazev souboru.\n";
    cout << "Zadany nazev souboru nesmi obsahovat znaky: \"\\ / : . * ? \" < > |\"\n";
    cout << "\nPro opetovne zadani nazvu vystupniho souboru, stisknete Enter...";
    cin.ignore();
    cin.get();
}

void vypis_do_konzole(shop *pole_produktu, int velikost_pole) //Jednoduchy vypis produktu do konzole, vzdy nesetrizeny
{
    /** @brief Funkce pro jednoduchy vypis produktu do konzole, vzdy nesetrizeny, tak jak ho ma uzivatel v .csv souboru.
        @param pole_produktu je promenna struktur, ktere obsahuje produkty.
        @param velikost_pole je cislo, ktere udava pocet produktu.
    */

    clear_screen();
    cout.width(69);
        cout << "***********************************************************\n";
        cout.width(69);
        cout << "*                Vypis produktu do konzole                *\n";
        cout.width(70);
        cout << "***********************************************************\n\n";

    if(velikost_pole == NULL) //v pripade, ze soubor s produkty je prazdny nebo vubec nebyl nacten, vypis nasledujici chybove hlasky
    {
        cout << "Seznam produktu je prazdny.\n";
    }
    else
    {
        for(int i=0; i<velikost_pole; i++) //serazeni produktu podle id od nejmensi po nejvetsi
        {
            for(int j=0; j<i; j++)
            {
                if(pole_produktu[i].id < pole_produktu[j].id)
                {
                    long int temp_cena = pole_produktu[i].cena;
                    pole_produktu[i].cena = pole_produktu[j].cena;
                    pole_produktu[j].cena = temp_cena;

                    long int temp_id = pole_produktu[i].id;
                    pole_produktu[i].id = pole_produktu[j].id;
                    pole_produktu[j].id = temp_id;

                    long int temp_pocet_kusu = pole_produktu[i].pocet_kusu;
                    pole_produktu[i].pocet_kusu = pole_produktu[j].pocet_kusu;
                    pole_produktu[j].pocet_kusu = temp_pocet_kusu;

                    char temp_nazev[50];
                    strcpy(temp_nazev, pole_produktu[i].nazev);
                    strcpy(pole_produktu[i].nazev, pole_produktu[j].nazev);
                    strcpy(pole_produktu[j].nazev, temp_nazev);


                    char temp_popis[250];
                    strcpy(temp_popis, pole_produktu[i].popis);
                    strcpy(pole_produktu[i].popis, pole_produktu[j].popis);
                    strcpy(pole_produktu[j].popis, temp_popis);
                }
            }
        }

        for(int i=0; i<velikost_pole; i++)
        {
            cout << "ID: " << pole_produktu[i].id << endl
                 << "Nazev: " << pole_produktu[i].nazev << endl
                 << "Popis: " << pole_produktu[i].popis << endl
                 << "Cena: " << pole_produktu[i].cena << " Kc" << endl
                 << "Pocet kusu na sklade: " << pole_produktu[i].pocet_kusu << endl << endl;
        }
    }
}

void autor(void) //Autoruv prostor
{
    /** @brief Funkce pro autoruv prostor, jeho jmeno, osobni cislo a kontaktni email.*/
    clear_screen();
    cout.width(69);
    cout << "***********************************************************\n";
    cout.width(69);
    cout << "*                     Autor programu                      *\n";
    cout.width(70);
    cout << "***********************************************************\n\n";

    cout << "Jmeno: Daniel Blazek\n"
         << "Osobni cislo: bla0172\n"
         << "Email: daniel.blazek.st@vsb.cz\n";
}

shop *nacteni_produktu(shop *pole_produktu, int &velikost_pole) //Nacteni produktu ze souboru .csv, dle zadane struktury
{
    /** @brief Funkce pro nacteni produktu ze souboru .csv, do zadane struktury.
        @param pole_produktu je promenna struktur, ktere obsahuje produkty.
        @param velikost_pole je cislo, ktere udava pocet produktu.
        @return vraci strukturu: pole_produktu
    */
    string file_in;
    string proud;
    int o = 0;

    clear_screen();
    cout.width(69);
    cout << "***********************************************************\n";
    cout.width(69);
    cout << "*                Nacteni produktu ze souboru              *\n";
    cout.width(70);
    cout << "***********************************************************\n\n";

    cout << "Zadejte nazev vstupniho souboru ve formatu csv bez pripony: ";
    cin >> file_in;

    while(file_in.c_str()[o]!='\0') //Osetreni nazvu vstupniho souboru, v pripade chyby, volani funkci a navraceni zpet
	{
		if(!cin.good() || file_in.c_str()[o]=='.' || file_in.c_str()[o]=='/' || file_in.c_str()[o]=='\\'  || file_in.c_str()[o]==':' || file_in.c_str()[o]=='*' || file_in.c_str()[o]=='?' || file_in.c_str()[o]=='"' || file_in.c_str()[o]=='<' || file_in.c_str()[o]=='>' || file_in.c_str()[o]=='|')
		{
			chyba();
			chyba_nepovolene_znaky();
            return nacteni_produktu(pole_produktu, velikost_pole);
		}
		o++;
	}

    string pripona = ".csv";
    file_in = file_in + pripona; //Spojeni nazvu souboru s priponou, ktera je konstantne .csv

    // Nactení ze souboru a otevreni vstupniho souboru
    ifstream vstup_soubor;
    vstup_soubor.open(file_in.c_str(), ios::in);

    if(pole_produktu != NULL) //Osetreni pro opakovane nacitani ze souboru
	{
		free(pole_produktu);
		pole_produktu = NULL;
		velikost_pole = 0;
		proud = "";
	}

    if(!vstup_soubor.is_open()) //Pokud soubor nebyl otevren/nalezen nasledny vypis
    {
        chyba();
        cout << "Chyba pri otevreni souboru.\n";
        cout << "Soubor bud neexistuje nebo je v nepovolenem formatu.\n";
        cout << "\nPro opetovne nacteni souboru, stisknete Enter...";
        cin.ignore();
        cin.get();
        return nacteni_produktu(pole_produktu, velikost_pole);
    }
    else //v pripade ze otevreni probehlo v poradku
    {
        for(int i=0; !vstup_soubor.eof(); i++)
        {
            getline(vstup_soubor, proud,';');

            if(proud.length() == 0) break;

            if(velikost_pole == 0) //Alokace pameti
            {
                pole_produktu=(shop*)malloc(sizeof(shop)*(++velikost_pole));
            }
            else
            {
                pole_produktu=(shop*)realloc(pole_produktu,sizeof(shop)*(++velikost_pole));
            }

            pole_produktu[i].id = atoi(proud.c_str()); //Nacteni id
            getline(vstup_soubor, proud,';');
            strcpy(pole_produktu[i].nazev, proud.c_str()); //Nacteni nazvu
            getline(vstup_soubor, proud,';');
            strcpy(pole_produktu[i].popis, proud.c_str()); //Nacteni popisu
            getline(vstup_soubor, proud,';');
            pole_produktu[i].cena = atoi(proud.c_str()); //Nacteni ceny
            getline(vstup_soubor, proud,'\n');
            pole_produktu[i].pocet_kusu = atoi(proud.c_str()); //Nacteni poctu kusu

            proud = "";
        }

        if(velikost_pole == 0) //Pokud pole nebylo ani 1x zvetsene, soubor je nejspise prazdny
        {
            cout << "Soubor '" << file_in << "' je prazdny.\n";
        }
        else
        {
            cout << "Nacteni produktu ze souboru '" << file_in << "' probehlo uspesne.\n";
        }
    }

    vstup_soubor.close();
    return pole_produktu;
}

shop *funkce_pro_setrizeni_produktu_dle_ceny(shop *pole_produktu, int &velikost_pole) //Podpurna setrizovaci funkce (dle cen o nejmensi po nejvetsi)
{
    /** @brief Podpurna setrizovaci funkce, ktera setridi zbozi dle ceny od nejmensi po nejvetsi,
        funkce je pouzita pro dalsi funkce:
        setrizeni_podle_ceny & setrizeni_podle_intervalu & doporuceni_zbozi_podle_castky.
        @param pole_produktu je promenna struktur, ktere obsahuje produkty.
        @param velikost_pole je cislo, ktere udava pocet produktu.
        @return vraci strukturu: pole_produktu_setrizeni
    */
    //Buble sort setrizeni podle ceny
    //Funkce(funkce_pro_setrizeni_produktu_dle_ceny) je pouzita pro funkce: setrizeni_podle_ceny & setrizeni_podle_intervalu & doporuceni_zbozi_podle_castky*/

	shop *pole_produktu_setrizeni = pole_produktu;

	for(int i=0; i<velikost_pole; i++)
    {
        for(int j=0; j<i; j++)
        {
            if(pole_produktu_setrizeni[i].cena < pole_produktu_setrizeni[j].cena)
            {
                long int temp_cena = pole_produktu_setrizeni[i].cena;
                pole_produktu_setrizeni[i].cena = pole_produktu_setrizeni[j].cena;
                pole_produktu_setrizeni[j].cena = temp_cena;

                long int temp_id = pole_produktu_setrizeni[i].id;
                pole_produktu_setrizeni[i].id = pole_produktu_setrizeni[j].id;
                pole_produktu_setrizeni[j].id = temp_id;

                long int temp_pocet_kusu = pole_produktu_setrizeni[i].pocet_kusu;
                pole_produktu_setrizeni[i].pocet_kusu = pole_produktu_setrizeni[j].pocet_kusu;
                pole_produktu_setrizeni[j].pocet_kusu = temp_pocet_kusu;

                char temp_nazev[50];
                strcpy(temp_nazev, pole_produktu_setrizeni[i].nazev);
                strcpy(pole_produktu_setrizeni[i].nazev, pole_produktu_setrizeni[j].nazev);
                strcpy(pole_produktu_setrizeni[j].nazev, temp_nazev);


                char temp_popis[250];
                strcpy(temp_popis, pole_produktu_setrizeni[i].popis);
                strcpy(pole_produktu_setrizeni[i].popis, pole_produktu_setrizeni[j].popis);
                strcpy(pole_produktu_setrizeni[j].popis, temp_popis);
            }
        }
    }
    return pole_produktu_setrizeni;
}

shop *setrizeni_podle_ceny(shop *pole_produktu, int &velikost_pole) //1. bod projektu - vypis produktu do HTML setrizenych dle cen
{
    /** @brief Prvni bod projektu - vypis produktu do HTML setrizenych dle cen, od nejmensi po nejvetsi.
        @param pole_produktu je promenna struktur, ktere obsahuje produkty.
        @param velikost_pole je cislo, ktere udava pocet produktu.
        @return vraci strukturu: pole_produktu
    */

    string pripona = ".html";
    string file_out;
    int o = 0;

    clear_screen();
    cout.width(69);
    cout << "***********************************************************\n";
    cout.width(69);
    cout << "*       Setrizeni produktu podle cen a vypis do HTML      *\n";
    cout.width(70);
    cout << "***********************************************************\n\n";

    if(velikost_pole == NULL) //pokud je seznam prazdny, vypis hlasky
    {
        cout << "Seznam produktu je prazdny.\n";
        return 0;
    }

    funkce_pro_setrizeni_produktu_dle_ceny(pole_produktu, velikost_pole); //volani podpurne funkce pro setrizeni dle cen

    cout << "Zadejte nazev pro vystupni soubor bez pripony html: ";
    cin >> file_out;

    while(file_out.c_str()[o]!='\0') //Osetreni nazvu vystupniho souboru, v pripade chyby, volani funkci a navraceni zpet
	{
		if(!cin.good() || file_out.c_str()[o]=='.' || file_out.c_str()[o]=='/' || file_out.c_str()[o]=='\\'  || file_out.c_str()[o]==':' || file_out.c_str()[o]=='*' || file_out.c_str()[o]=='?' || file_out.c_str()[o]=='"' || file_out.c_str()[o]=='<' || file_out.c_str()[o]=='>' || file_out.c_str()[o]=='|')
		{
			chyba();
            chyba_nepovolene_znaky();
            return setrizeni_podle_ceny(pole_produktu, velikost_pole);
		}
		o++;
	}

    file_out = file_out + pripona;

    ofstream vystup_soubor;
    vystup_soubor.open(file_out.c_str(), ios::out);

    if(!vystup_soubor.is_open()) //Kontrola jestli soubor se otevre ale lze do nej zapisovat
	{
		cout << "Soubor se neotevrel.\n";
		return 0;
	}
	else //Soubor se otevrel, nasleduje vypis do HTML souboru
    {
        vystup_soubor << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">"
                      << "\n<html>" << "\n<head>"
                      << "\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
                      << "\n<title>eShop a zbozi v nem</title>"
                      << "\n</head>\n" << "<body bgcolor=\"#f5f5f5\">"
                      << "\n<h1 align=\"center\"><b>eShop a zbozi v nem</b></h1>"
                      << "\n<h2 align=\"center\">Serazeni podle ceny:</h2>"
                      << "\n<table width=\"850\" align=\"center\" border=\"1\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\" bordercolor=\"black\">"
                      << "\n<tr><th bgcolor=\"black\" width=\"25\"><font size=\"4\" color=\"white\">ID</font></th><th width=\"99\" bgcolor=\"black\">"
                      << "<font size=\"4\" color=\"white\">Nazev produktu</font></th><th width=\"149\" bgcolor=\"black\">"
                      << "<font size=\"4\" color=\"white\">Popis</font></th><th width=\"55\" bgcolor=\"black\">"
                      << "<font size=\"4\" color=\"white\">Cena</font></th><th width=\"55\" bgcolor=\"black\">"
                      << "<font size=\"4\" color=\"white\">Pocet kusu na sklade</font></th></tr>";

        for(int i=0; i<velikost_pole; i++)
        {
            vystup_soubor << "\n<tr><td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].id << "</td>" << "</td > "<< "<td bgcolor=\"white\" align=\"center\">"
                          << pole_produktu[i].nazev << "</td>" << "<td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].popis << "</td>"
                          << "<td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].cena << "</td>" << "<td  bgcolor=\"white\" align=\"center\">"
                          << pole_produktu[i].pocet_kusu << "</td>" << "</tr>\n";
        }

        vystup_soubor << "\n</table>";
        vystup_soubor << "\n</body>";
        vystup_soubor << "\n</html>";
        cout << "Ulozeni do souboru HTML probehlo uspesne.\n";
    }
    vystup_soubor.close();
    return pole_produktu;
}

shop *setrizeni_podle_intervalu(shop *pole_produktu, int &velikost_pole) //2. bod projektu - vypis produktu do HTML setrizenych dle zadaneho intervalu
{
    /** @brief Druhy bod projektu - vypis produktu do HTML setrizenych dle zadaneho intervalu <x, y> .
        @param pole_produktu je promenna struktur, ktere obsahuje produkty.
        @param velikost_pole je cislo, ktere udava pocet produktu.
        @return vraci strukturu: pole_produktu
    */

    long int dolni_mez = 0;
    long int horni_mez = 0;
    string pripona = ".html";
    string file_out;
    int o = 0;
    int pocitadlo = 0;

    clear_screen();
    cout.width(72);
    cout << "*******************************************************************\n";
    cout.width(72);
    cout << "* Vypis produktu nachazejicich se v zadanym intervalu cen do HTML *\n";
    cout.width(73);
    cout << "*******************************************************************\n\n";

    if(velikost_pole == NULL) //Kontrola zda soubor obsahuje produkty
    {
        cout << "Seznam produktu je prazdny.\n";
        return 0;
    }

    funkce_pro_setrizeni_produktu_dle_ceny(pole_produktu, velikost_pole); //Volani podpurne funkce, ktera nam setridi produkty dle ceny

    cout << "\nZadejte interval cen, ktery chcete aby se vypsal do html.\n"; //Dulezite zjisteni intervalu, uzivatele

    cout << "Dolni mez intervalu: ";
    cin >> dolni_mez;
    cout << "Horni mez intervalu: ";
    cin >> horni_mez;

    if(dolni_mez < 0 || horni_mez < 0) //Mez intervalu v zadnem pripade nesmi byt mensi nez 0, pokud ano vypis chyby
    {
        chyba();
        cout << "\nMezni hodnoty intervalu nemohou byt mensi jak 0.\n";
        cout << "\nPro zadani intervalu znova, stisknete Enter...";
        cin.ignore();
        cin.get();
        return setrizeni_podle_intervalu(pole_produktu, velikost_pole);
    }

    if(dolni_mez > horni_mez) //Mez intervalu v zadnem pripade nesmi byt mensi nez 0, pokud ano vypis chyby
    {
        chyba();
        cout << "\nDolni mez intervalu nemuze byt vetsi jak 0.\n";
        cout << "\nPro zadani intervalu znova, stisknete Enter...";
        cin.ignore();
        cin.get();
        return setrizeni_podle_intervalu(pole_produktu, velikost_pole);
    }

    cout << "\nZadejte nazev pro vystupni soubor bez pripony html: ";
    cin >> file_out;

    while(file_out.c_str()[o]!='\0') //Dulezita kontrola nazvu vystupniho souboru
	{
		if(!cin.good() || file_out.c_str()[o]=='.' || file_out.c_str()[o]=='/' || file_out.c_str()[o]=='\\'  || file_out.c_str()[o]==':' || file_out.c_str()[o]=='*' || file_out.c_str()[o]=='?' || file_out.c_str()[o]=='"' || file_out.c_str()[o]=='<' || file_out.c_str()[o]=='>' || file_out.c_str()[o]=='|')
		{
            chyba();
			chyba_nepovolene_znaky();
            return setrizeni_podle_intervalu(pole_produktu, velikost_pole);
        }
		o++;
	}

    file_out = file_out + pripona;

    ofstream vystup_soubor; //Otevirani vystupniho proudu
    vystup_soubor.open(file_out.c_str(), ios::out);

    if(!vystup_soubor.is_open()) //Osetreni v pripade neotevreni souboru
	{
		cout << "Soubor se neotevrel.\n";
		return 0;
	}
	else //V pripade zadne chyby vypis
    {
        vystup_soubor << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">"
                      << "\n<html>" << "\n<head>"
                      << "\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
                      << "\n<title>eShop a zbozi v nem</title>"
                      << "\n</head>\n" << "<body bgcolor=\"#f5f5f5\">"
                      << "\n<h1 align=\"center\"><b>eShop a zbozi v nem</b></h1>"
                      << "\n<h2 align=\"center\">Vypis produktu v zadanem intervalu cen:</h2>"
                      << "\n<h3 align=\"center\">Dolni mez intervalu cen je " << dolni_mez << " Kc a horni mez je " << horni_mez << " Kc" << "</h2>"
                      << "\n<table width=\"850\" align=\"center\" border=\"1\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\" bordercolor=\"black\">"
                      << "\n<tr><th bgcolor=\"black\" width=\"25\"><font size=\"4\" color=\"white\">ID</font></th><th width=\"99\" bgcolor=\"black\">"
                      << "<font size=\"4\" color=\"white\">Nazev produktu</font></th><th width=\"149\" bgcolor=\"black\">"
                      << "<font size=\"4\" color=\"white\">Popis</font></th><th width=\"55\" bgcolor=\"black\">"
                      << "<font size=\"4\" color=\"white\">Cena</font></th><th width=\"55\" bgcolor=\"black\">"
                      << "<font size=\"4\" color=\"white\">Pocet kusu na sklade</font></th></tr>";

        for(int i=0; i<velikost_pole; i++)
        {
            if((pole_produktu[i].cena >= dolni_mez) && (pole_produktu[i].cena <= horni_mez)) //Vypis jen v pripade, ze zbozi odpovida meznim hodnotam intervalu
            {
                vystup_soubor << "\n<tr><td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].id << "</td>" << "</td > "<< "<td bgcolor=\"white\" align=\"center\">"
                              << pole_produktu[i].nazev << "</td>" << "<td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].popis << "</td>"
                              << "<td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].cena << "</td>" << "<td  bgcolor=\"white\" align=\"center\">"
                              << pole_produktu[i].pocet_kusu << "</td>" << "</tr>\n";
                pocitadlo++;
            }
        }

        if(pocitadlo == 0) //Pokud nebyl nalezen zadny produkt, nasledny vypis
        {
            vystup_soubor << "\n<tr><td bgcolor=\"white\" align=\"center\">" << "-" << "</td>" << "</td > "<< "<td bgcolor=\"white\" align=\"center\">"
                          << "-" << "</td>" << "<td bgcolor=\"white\" align=\"center\">" << "-" << "</td>"
                          << "<td bgcolor=\"white\" align=\"center\">" << "-" << "</td>" << "<td  bgcolor=\"white\" align=\"center\">"
                          << "-" << "</td>" << "</tr>\n";
        }

        vystup_soubor << "\n</table>";

        if(pocitadlo == 0)
        {
            vystup_soubor << "\n<h3 align=\"center\"><i>Zadany interval cen, neobsahuje zadne produkty.</i></h3>";
        }
        vystup_soubor << "\n</body>";
        vystup_soubor << "\n</html>";
        cout << "Ulozeni do souboru HTML probehlo uspesne.\n";
    }
    vystup_soubor.close();
    return pole_produktu;
}

shop *doporuceni_zbozi_podle_castky(shop *pole_produktu, int &velikost_pole) //3. bod projektu - vypis produktu do HTML setrizenych dle zadane castky uzivatelem
{
    /** @brief Treti bod projektu - vypis produktu do HTML setrizenych dle zadane castky uzivatelem, rozmezi castky musi byt vetsi jak 0.
        @param pole_produktu je promenna struktur, ktere obsahuje produkty.
        @param velikost_pole je cislo, ktere udava pocet produktu.
        @return vraci strukturu: pole_produktu
    */

    string pripona = ".html";
    string file_out;
    int zbyvajici_castka[velikost_pole];
    int o = 0;
    int doporucena_cena = 0;
    int pocitadlo = 0;

    clear_screen();
    cout << "  ***************************************************************************\n";
    cout << "  *  Doporucene zbozi, ktere lze koupit pro zadanou castku a vypis do HTML  *\n";
    cout << "  ***************************************************************************\n\n";

    if(velikost_pole == NULL) //Pokud velikost pole je mensi jak nula, seznam produktu je prazdny
    {
        cout << "Seznam produktu je prazdny.\n";
        return 0;
    }

    cout << "Zadejte penezni castku, pro kterou chcete doporucit produkty: "; //Cast programu zaznamenavajici castku(kc) pro, kterou doporucime zbozi
    cin >> doporucena_cena;                                                   // + nutne osetreni vstupu
    if(doporucena_cena < 0)
    {
        chyba();
        cout << "\nPenezni castka, kterou chcete zadat nemuze byt mensi jak 0.\n";
        cout << "\nPro zadani penezni castky znova, stisknete Enter...";
        cin.ignore();
        cin.get();
        return doporuceni_zbozi_podle_castky(pole_produktu, velikost_pole);
    }

    funkce_pro_setrizeni_produktu_dle_ceny(pole_produktu, velikost_pole); //Zavolani funkce, ktera setridi produkty dle ceny

    cout << "\nZadejte nazev pro vystupni soubor bez pripony html: ";
    cin >> file_out;

    while(file_out.c_str()[o]!='\0') //Osetreni nazvy vystupniho souboru, v pripade chyby, volani funkci a navraceni zpet
	{
		if(!cin.good() || file_out.c_str()[o]=='.' || file_out.c_str()[o]=='/' || file_out.c_str()[o]=='\\'  || file_out.c_str()[o]==':' || file_out.c_str()[o]=='*' || file_out.c_str()[o]=='?' || file_out.c_str()[o]=='"' || file_out.c_str()[o]=='<' || file_out.c_str()[o]=='>' || file_out.c_str()[o]=='|')
		{
            chyba();
			chyba_nepovolene_znaky();
            return doporuceni_zbozi_podle_castky(pole_produktu, velikost_pole);
        }
		o++;
	}

    file_out = file_out + pripona; //Spojeni nazvu vystupniho souboru s priponou .HTML

    ofstream vystup_soubor; //definice vystupniho soubouboru, nasledovne otevreni a nutne osetreni vstupu v pripade chyby
    vystup_soubor.open(file_out.c_str(), ios::out);

    if(!vystup_soubor.is_open())
	{
		cout << "Soubor se neotevrel.\n";
        return 0;
	}
	else //Vypis zakladni hlavicky a tabulky HTML souboru
    {
        vystup_soubor << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">"
                      << "\n<html>" << "\n<head>"
                      << "\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
                      << "\n<title>eShop a zbozi v nem</title>"
                      << "\n</head>\n" << "<body bgcolor=\"#f5f5f5\">"
                      << "\n<h1 align=\"center\"><b>eShop a zbozi v nem</b></h1>"
                      << "\n<h2 align=\"center\">Doporucene zbozi pro zadanou penezni castku: "<< doporucena_cena << " Kc" << "</h2>"
                      << "\n<table width=\"950\" align=\"center\" border=\"1\" border=\"0\" cellpadding=\"2\" cellspacing=\"0\" bordercolor=\"black\">"
                      << "\n<tr><th bgcolor=\"black\" width=\"25\"><font size=\"4\" color=\"white\">ID</font></th><th width=\"99\" bgcolor=\"black\">"
                      << "<font size=\"4\" color=\"white\">Nazev produktu</font></th><th width=\"149\" bgcolor=\"black\">"
                      << "<font size=\"4\" color=\"white\">Popis</font></th><th width=\"55\" bgcolor=\"black\">"
                      << "<font size=\"4\" color=\"white\">Cena</font></th><th width=\"55\" bgcolor=\"black\">"
                      << "<font size=\"4\" color=\"white\">Pocet kusu na sklade</font></th>"
                      << "<th width=\"70\" bgcolor=\"black\"><font size=\"4\" color=\"white\">Zbyvajici penezni castka</font></th>";

        for(int i=0; i<velikost_pole; i++)
        {
            zbyvajici_castka[i] = doporucena_cena - pole_produktu[i].cena; //Vypocteni penezni castky, ktera zbyva

            if(pole_produktu[i].cena <= doporucena_cena) //Podminka je splnena pokud se nalezne cena mensi nebo rovna cene zadane uzivatelem
            {
                if(pole_produktu[i].pocet_kusu == 0) //Pokud je na sklade 0 kusu vypis v tabulce bude zaznamenan cervene(vystrazne)
                {
                    vystup_soubor << "\n<tr><td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].id << "</td>" << "</td > "<< "<td bgcolor=\"white\" align=\"center\">"
                                  << pole_produktu[i].nazev << "</td>" << "<td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].popis << "</td>"
                                  << "<td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].cena << "</td>" << "<td bgcolor=\"red\" align=\"center\">"
                                  << pole_produktu[i].pocet_kusu << "</td>"
                                  << "<td bgcolor=\"white\" align=\"center\">" << zbyvajici_castka[i] << "</td>" << "</tr>\n";
                }
                else if(pole_produktu[i].pocet_kusu > 0 && pole_produktu[i].pocet_kusu <= 5) //Pokud je zbozi na sklade mene jak 5 a vice jak 0, vypis bude oranzove
                {
                    vystup_soubor << "\n<tr><td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].id << "</td>" << "</td > "<< "<td bgcolor=\"white\" align=\"center\">"
                                  << pole_produktu[i].nazev << "</td>" << "<td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].popis << "</td>"
                                  << "<td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].cena << "</td>" << "<td bgcolor=\"orange\" align=\"center\">"
                                  << pole_produktu[i].pocet_kusu << "</td>"
                                  << "<td bgcolor=\"white\" align=\"center\">" << zbyvajici_castka[i] << "</td>" << "</tr>\n";
                }
                else if(pole_produktu[i].pocet_kusu > 5 && pole_produktu[i].pocet_kusu <= 10) //Pokud je na sklade vice jak 5 a mene jak 10, vypis bude zluty
                {
                    vystup_soubor << "\n<tr><td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].id << "</td>" << "</td > "<< "<td bgcolor=\"white\" align=\"center\">"
                                  << pole_produktu[i].nazev << "</td>" << "<td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].popis << "</td>"
                                  << "<td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].cena << "</td>" << "<td bgcolor=\"yellow\" align=\"center\">"
                                  << pole_produktu[i].pocet_kusu << "</td>"
                                  << "<td bgcolor=\"white\" align=\"center\">" << zbyvajici_castka[i] << "</td>" << "</tr>\n";
                }
                else //Zbozim, ktere je na sklade vice ja 10 kusu, nemusi budit zadnou pozornost, tym padem vypis bude zelene
                {
                    vystup_soubor << "\n<tr><td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].id << "</td>" << "</td > "<< "<td bgcolor=\"white\" align=\"center\">"
                                  << pole_produktu[i].nazev << "</td>" << "<td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].popis << "</td>"
                                  << "<td bgcolor=\"white\" align=\"center\">" << pole_produktu[i].cena << "</td>" << "<td  bgcolor=\"green\" align=\"center\">"
                                  << pole_produktu[i].pocet_kusu << "</td>"
                                  << "<td bgcolor=\"white\" align=\"center\">" << zbyvajici_castka[i] << "</td>" << "</tr>\n";
                }
                pocitadlo++;

            }
        }

        if(pocitadlo == 0) //Pokud nebyla podminka (viz. vyse) splnena ani jedenkrat, nasleduje tento vypis
        {
            vystup_soubor << "\n<tr><td bgcolor=\"white\" align=\"center\">" << "-" << "</td>" << "</td > "<< "<td bgcolor=\"white\" align=\"center\">"
                          << "-" << "</td>" << "<td bgcolor=\"white\" align=\"center\">" << "-" << "</td>"
                          << "<td bgcolor=\"white\" align=\"center\">" << "-" << "</td>" << "<td  bgcolor=\"white\" align=\"center\">"
                          << "-" << "</td>"
                          << "<td bgcolor=\"white\" align=\"center\">" << "-" << "</td>" << "</tr>\n";
        }

        vystup_soubor << "\n</table>";

        if(pocitadlo == 0) //Posledni textovy vypis, urcen jen pro informacni text mimo tabulku
        {
            vystup_soubor << "\n<h3 align=\"center\"><i>Pro zadanou penezni castku neni na sklade zadny produkt.</i></h3>";
        }

        vystup_soubor << "\n</body>";
        vystup_soubor << "\n</html>";
        cout << "Ulozeni do souboru HTML probehlo uspesne.\n";
    }
    vystup_soubor.close();
    return pole_produktu;
}

int main() //Hlavni cast programu, obsahujici hlavni menu a prepinac switch pro hlavni menu
{
    /**
        @brief Hlavni cast programu, obsahujici hlavni menu a prepinac switch pro hlavni menu.
        @return vraci 0
    */

    shop *pole_produktu = NULL;
    int velikost_pole = 0;
    int volba;
    string chyba;

    while(1)
    {
        clear_screen();
        cout.width(69);
        cout << "***********************************************************\n";
        cout.width(69);
        cout << "*    eShop a zbozi v nem, program pro praci s produkty.   *\n";
        cout.width(70);
        cout << "***********************************************************\n\n";

        cout << "Hlavni menu:\n"
             << "1. Nacteni produktu ze souboru\n"
             << "2. Vypis produktu do konzole\n"
             << "3. Vypis produktu do HTML - setrizenych podle cen\n"
             << "4. Vypis produktu do HTML - v zadanem intervalu cen\n"
             << "5. Vypis produktu do HTML - doporucenych pro zadanou penezni castku\n"
             << "6. Autor programu\n"
             << "7. Konec programu\n";

            cout << "Zadej volbu: ";
            cin >> volba;

            if(!cin.good())
            {
                cin.clear();
                cin >> chyba;
            }

            switch(volba)
            {
                case 1: pole_produktu = nacteni_produktu(pole_produktu, velikost_pole);
                        break;

                case 2: vypis_do_konzole(pole_produktu, velikost_pole);
                        break;

                case 3: pole_produktu = setrizeni_podle_ceny(pole_produktu, velikost_pole);
                        break;
                case 4: pole_produktu = setrizeni_podle_intervalu(pole_produktu, velikost_pole);
                        break;

                case 5: pole_produktu = doporuceni_zbozi_podle_castky(pole_produktu, velikost_pole);
                        break;

                case 6: autor();
                        break;
                case 7: konec_programu();
                        free(pole_produktu);
                        return 0;

                default: continue;
            }
            cout << "\nPro pokracovani stisknete Enter...";
            cin.ignore();
            cin.get();
        }

    free(pole_produktu); //Pro jistotu jeste uvolnime pamet
    return 0;
}
