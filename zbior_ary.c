// Ksawier Krajewski nr. alb. 479524
// Zadanie Zbiory arytmetyczne 

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "zbior_ary.h"
#include <stdio.h>

#define ll long long
#define ull unsigned ll

/*
Plik zbior_ary.h zawiera struct'a zbior_ary (A), ktory reprezentuje zbior ciagow arytmetycznych (A.t) oraz ich ilosc (A.n).
Tablica jest typu (trojka) zatem kazdy (A.t[i]) przechowuje jeden rozlaczny ciag arytmetyczny, gdzie (A.t[i].x) przechowuje
poczatek ciagu, (A.t[i].y) przechowuje koniec ciagu, a (A.t[i].mod) zawiera wartosc element ciagu modulo roznica tego ciagu.

Taka reprezentacja pozwala na zachowanie specyficznej kolejnosci ciagow w tablicy w nastepujacay sposob:
1) ciag (A.t[i]) bedzie przed ciagiem (A.t[j]) <=> (A.t[i].mod) < (A.t[j].mod), dla i, j bedacych indeksami tablicy t
2) jesli dwa ciagi (A.t[i]) oraz (A.t[j]) maja te sama wartosc modulo (A.t[i].mod) = (A.t[j].mod), to ciag (A.t[i]) wystapi 
prze ciagiem (A.t[j]) <=> (A.t[i].y) < (A.t[j].x)
Mozemy pozwolic sobie na taki sposob "sortowania" powolujac sie na to, ze wszystkie ciagi w tablic sa rozlaczne.
*/

static ll glob_q = 0;   // roznica we wszystkich ciagach arytmetyczny przechowywana globalnie

//funkcja sprawdza czy dla dwoch podanych ciagow (A.t[a]) i (B.t[b]) o jednakowym modulo, ciagi te maja puste przeciecie
bool rozlaczne(zbior_ary A, zbior_ary B, ull a, ull b){
    if(A.t[a].y < B.t[b].x || A.t[a].x > B.t[b].y) return 1;
    return 0;
}
//funkcja sprawdza czy dla dwoch podanych ciagow (A.t[a]) i (B.t[b]) o jednakowym modulo, ciagi te maja puste przeciecie
//biorac tez pod uwage blad o glob_q. Np. jesli t[a].y = k oraz t[b].x = l oraz k + glob.q = l ciagi beda uznane za spojne
bool rozlaczne_suma(zbior_ary A, zbior_ary B, ull a, ull b){
    if(A.t[a].y + glob_q < B.t[b].x || A.t[a].x > B.t[b].y + glob_q) return 1;
    return 0;
}
ll maxx(ll a, ll b){
    if(a > b) return a;
    return b;
}
ll minx(ll a, ll b){
    if(a < b) return a;
    return b;
}
//funkcja liczy modulo liczby a, ktory m.in. uzywamy w (A.t[i].mod) do ustalenia kolejnosci elementow w tablicy t
ll policzMod(ll a){
    if(a < 0) return (a - ((a - (glob_q - 1)) / glob_q)*glob_q)% glob_q;
    return a % glob_q;
}

//funkcja laczy dwa zbiory rosnaco po (A.t[i].mod) nastepnie rosnaco po (A.t[i].x) i finalnie rosnaco po (A.t[i].x)
zbior_ary polacz_zbiory_rosnaco(zbior_ary A, zbior_ary B){
    zbior_ary Sup;  
    Sup.n = A.n + B.n;
    Sup.t = (trojka*)malloc(Sup.n * sizeof(trojka));
    ull b = 0, ind = 0;
    for(ull a = 0; a < A.n; a++){
        while(b < B.n && B.t[b].mod < A.t[a].mod)
            Sup.t[ind++] = B.t[b++];
        //laczymy zbiory A i B w kolejnosci rosnacej poczatkami i koncami
        while(b < B.n && B.t[b].mod == A.t[a].mod && B.t[b].x <= A.t[a].x){
            if(A.t[a].x == B.t[b].x && B.t[b].y <= A.t[a].y)
                Sup.t[ind++] = B.t[b++];
            else if(B.t[b].x < A.t[a].x)
                Sup.t[ind++] = B.t[b++];
            else
                break;
        }
        Sup.t[ind++] = A.t[a];
    }
    while(b < B.n)
        Sup.t[ind++] = B.t[b++];
    Sup.n = ind;
    Sup.t = (trojka*)realloc(Sup.t, Sup.n * sizeof(trojka));
    return Sup;
}

// funkcja tworzy zbior z ciagiem arytmetycznym postaci (A.t[0].x = a), (A.t[0].y = b), (A.t[0].mod = policzMod(a));
zbior_ary ciag_arytmetyczny(ll a, ll q, ll b){
    if(glob_q == 0) glob_q = q;
    zbior_ary A;
    A.n = 1;
    A.t = (trojka*)malloc(1 * sizeof(trojka));
    A.t[0].x = a;
    A.t[0].y = b;
    A.t[0].mod = policzMod(a);
    return A;
}
// tutaj dzieje sie to samo co w funkcji ciag arytmetyczny z drobna zmiana, tj. (A.t[0].y = a)
zbior_ary singleton(ll a){
    return ciag_arytmetyczny(a, glob_q, a);
}

zbior_ary suma(zbior_ary A, zbior_ary B){
    zbior_ary C;
    //jesli oba zbiory sa puste to ich suma tez bedzie pusta wiec od razu zwracamy
    if(A.n == 0 && B.n == 0){
        C.n = 0;
        return C;
    }
    //lacze oba zbiory
    zbior_ary Sup = polacz_zbiory_rosnaco(A, B);
    C.t = (trojka*)malloc(Sup.n * sizeof(trojka));
    ull ind = 0;
    //teraz przerzucam je do zbioru C wydluzajac obecny element (C.t[ind]) jesli moge, w przeciwnym przypadku dodaje kolejny element 
    //ze zbioru Sup
    C.t[ind] = Sup.t[0];
    for(ull i = 1; i < Sup.n; i++){
        if(Sup.t[i].mod == C.t[ind].mod){ 
            if(rozlaczne_suma(Sup, C, i, ind)){ // jesli dwa ciagi sa rozlaczne to dodaje nowy
                C.t[++ind] = Sup.t[i];
            }
            else{ // jesli nie to wydluzam obecny do max z ich koncow
                C.t[ind].y = maxx(C.t[ind].y, Sup.t[i].y);
            }
        }
        else{ // ciagi maja rozne modulo, zatem sa rozlaczne, rowniez dodaje nowy element
            C.t[++ind] = Sup.t[i];
        }
    }
    free(Sup.t);
    C.n = ind + 1;
    C.t = (trojka*)realloc(C.t, C.n * sizeof(trojka));
    return C;  
}

zbior_ary iloczyn(zbior_ary A, zbior_ary B){
    zbior_ary C;
    //jesli ktorys zbior jest pusty to ich przeciecie tez jest puste
    if(A.n == 0 || B.n == 0){
        C.n = 0;
        return C;
    }
    zbior_ary Sup;
    //laczyme oba zbiory
    Sup = polacz_zbiory_rosnaco(A, B);
    trojka best;
    C.t = (trojka*)malloc(Sup.n * sizeof(trojka));
    ull ind = 0;
    C.t[ind] = Sup.t[0];
    best = Sup.t[0];
    /*
    teraz biore przeciecie zbiorow w nastepujacy sposob:
    w tablicy C.t trzymam najdalej konczacy sie dotychczas ciag arytmetyczny i jesli moge to biore 
    przeciecie elementu Sup[i] z nim.
    Jesli wystepuje niepuste przeciecie, to niepuste przeciecie zachowuje i dodaje nowy element 
    z najdluzszym koncem, ktory zamierzam potencjalnie przeciac z kolejnym elementem
    */
    for(ull i = 1; i < Sup.n; i++){
        //jesli ich modulo jest jednakowe to
        if(Sup.t[i].mod == C.t[ind].mod){
            if(rozlaczne(Sup, C, i, ind)){//jesli wystapil element z "lepszym" najdluzszym koncem to go nadpisuje
                best = Sup.t[i];
                if(i != Sup.n - 1) C.t[ind] = best;
                else ind--;
            }
            else{//biore wspolny przedzial i dodaje najdluzszy element
                C.t[ind].x = maxx(C.t[ind].x, Sup.t[i].x);
                C.t[ind].y = minx(C.t[ind].y, Sup.t[i].y);
                if(Sup.t[i].y > best.y) best = Sup.t[i];
                if(i < Sup.n - 1) C.t[++ind] = best;
            }
        }
        else{//jesli wystapil element z "lepszym" najdluzszym koncem to go nadpisuje
            best = Sup.t[i];
            if(i != Sup.n - 1) C.t[ind] = best;
            else ind--;
        }   
    }
    free(Sup.t);
    C.n = ind + 1;
    C.t = (trojka*)realloc(C.t, C.n * sizeof(trojka));
    return C;  
}
zbior_ary roznica(zbior_ary A, zbior_ary B){
    zbior_ary Ilo = iloczyn(A, B); //licze przeciecie zbiorow i potem bede bral A - Ilo co da A - B
    zbior_ary C;
    zbior_ary Sup; 
    trojka best; 
    //jesli A jest pusty to roznica tez jest pusta
    if(A.n == 0){
        free(Ilo.t);
        C.n = 0;
        return C;
    }
    //jesli przeciecie A i B jest puste to A - B = A
    if(Ilo.n == 0){
        free(Ilo.t);
        return A;
    }
    Sup.n = A.n + Ilo.n;
    Sup.t = (trojka*)malloc(Sup.n * sizeof(trojka));
    C.t = (trojka*)malloc(Sup.n * sizeof(trojka));
    Sup.t[0].mod = glob_q + 1;
    ull b = 0, ind = 0;
    // lacze zbiory A i Ilo w z nastepujacym priorytetem:
    // 1) (A.t[i].mod) < (Ilo.t[j].mod)  rosnaco po modulo
    // 2) (A.t[i].x) < (Ilo.t[j].x) rosnaco po poczatkach ciagow
    // 3) (A.t[i].y) > (Ilo.t[j].y) malejaco po koncacch ciagow
    for(ull a = 0; a < A.n; a++){
        // Jesli ciag Ilo.t[b] calkowicie pokrywa zbior ciag A.t[a] to nie musimy ich rozpatrywac bo dadza zbior pusty
        if(b < Ilo.n && A.t[a].x == Ilo.t[b].x && A.t[a].y == Ilo.t[b].y && A.t[a].mod == Ilo.t[b].mod){ 
            b++;
            continue;
        }
        Sup.t[ind++] = A.t[a];
        while(b < Ilo.n && Ilo.t[b].mod == A.t[a].mod && Ilo.t[b].y <= A.t[a].y && Ilo.t[b].x >= A.t[a].x){
            Sup.t[ind++] = Ilo.t[b++];
        }
    }
    //jesli otrzymalismy zbior pusty to zwracamy
    if(ind == 0){
        free(Sup.t);
        free(Ilo.t);
        free(C.t); 
        C.n = 0;
        return C;
    }

    Sup.n = ind;
    ind = 0;
    C.t[ind] = Sup.t[0];
    best = Sup.t[0];
    // biore roznice zbiorow A i Ilo
    for(ull i = 1; i < Sup.n; i++){
        //jesli oba ciagi maja te same modulo
        if(Sup.t[i].mod == C.t[ind].mod){
            if(rozlaczne(Sup, C, i, ind)){ 
            // jesli sa rozlaczne to bazujac na naszym sortowaniu elementow 
            // rozpatruje kolejny el wystepujacy pierwotnie w zbiorze A, wiec moge go dodac jako nowy
                C.t[++ind] = Sup.t[i];
                best = Sup.t[i];
            }
            else{
                // lewa strona tego co zostalo jesli cos zostalo
                ll prawa = C.t[ind].y;
                if(C.t[ind].x != Sup.t[i].x){
                    if(C.t[ind].y == Sup.t[i].y)
                        C.t[ind].y = Sup.t[i].x - glob_q;
                    else
                        C.t[ind++].y = Sup.t[i].x - glob_q; 
                }
                // prawa strona tego co zostalo jesli cos zostalo
                if(prawa != Sup.t[i].y){
                    C.t[ind] = best;
                    C.t[ind].x = Sup.t[i].y + glob_q;
                }
            }
        }
        else{
            //jesli modulo sa inne to tez bazujac na naszym sortowaniu elementow 
            // rozpatruje kolejny el wystepujacy pierwotnie w zbiorze A wiec moge dodac go jako nowy 
            C.t[++ind] = Sup.t[i];
            best = Sup.t[i];
        }   
    }
    free(Ilo.t);
    free(Sup.t);
    C.n = ind + 1;
    C.t = (trojka*)realloc(C.t, C.n * sizeof(trojka));
    return C;
}
//binsearch po modulo i elementach ciagu
bool nalezy(zbior_ary A, ll b){
    if(A.n == 0) return false;
    ll modb = policzMod(b);
    ll l = 0, r = (ll)(A.n - 1), MID;
    while(l < r){
        MID = (l + r + 1) / 2;
        if(A.t[MID].mod > modb || (A.t[MID].mod == modb && A.t[MID].x > b))
            r = MID - 1;
        else
            l = MID;
    }
    if(modb == A.t[l].mod && A.t[l].x <= b && b <= A.t[l].y)
        return true;
    return false;
}
//sumuje liczby elementow w kolejnych ciagach
ull moc(zbior_ary A){
    ull res = 0;
    for(ull i = 0; i < A.n; i++){
        res += (ull)(((A.t[i].y - A.t[i].x) / glob_q) + 1);
    }
    return res;
}
ull ary(zbior_ary A){
    return A.n;
}




int porownaj_longlongy(const void *a, const void *b) {
    ll A = *(const ll*)a;
    ll B = *(const ll*)b;

    if (A < B) return -1;
    if (A > B) return 1;
    return 0;
}

void print(zbior_ary A) {
    if (A.n == 0){
        printf("\n");
        return;
    fprintf(stderr,"\n");
    fflush(stdout);
    }
    else{
    // Policz liczbę elementów
    ull count = 0;
    for (unsigned i = 0; i < A.n; i++)
        count += (ull)((A.t[i].y - A.t[i].x) / glob_q + 1);

    // Zbierz wszystkie elementy
ll *tab = (ll*) malloc(count * sizeof(ll));    unsigned idx = 0;
    for (unsigned i = 0; i < A.n; i++) {
        for (ll x = A.t[i].x; x <= A.t[i].y; x += glob_q)
            tab[idx++] = x;
    }

    // Posortuj
    // qsort(tab, count, sizeof(ll), porownaj_longlongy);
 qsort(tab, count, sizeof(ll), porownaj_longlongy);

    // Wypisz w jednej linii
    //prlong longf("{ ");
    for (unsigned i = 0; i < count; i++)
        printf("%lld ", tab[i]);
    // fprlong longf(stderr,"A.n %lld ", A.n);
    printf("\n");
    
    free(tab);
}
}

void wypisz_zbior(zbior_ary Z) {
    printf("Zbiór ma %llu ciągów:\n", Z.n);
    for (ull i = 0; i < Z.n; i++) {
        printf("  [%lld, %lld] mod=%lld\n", Z.t[i].x, Z.t[i].y, Z.t[i].mod);
    }
}
