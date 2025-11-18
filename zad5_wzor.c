#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

const int INF = INT_MAX;

typedef struct motel{
    int typ;
    int odl;
} motel;

typedef struct para{
    int bli;
    int dal;
} para;

int maxx(int a, int b){
    if(a > b) return a;
    return b;
}
int minx(int a, int b){
    if(a < b) return a;
    return b;
}

int sprawdz(int n, motel* t, int idl, int idi, int idp){
    motel l, i, p;
    
    if(idl == n || idp == n) return INF;
    l = t[idl];
    p = t[idp];
    i = t[idi];
    // Sprawdzam czy wszystkie typy sa rozne
    if(l.typ == i.typ || l.typ == p.typ || i.typ == p.typ) return INF;

    // if(maxx(i.odl - l.odl, p.odl - i.odl) == 8)
    // printf("%d, %d, %d \n", idl, idi, idp);

    return maxx(i.odl - l.odl, p.odl - i.odl);
}

void znajdz_lewa(int n, motel* t, motel* lewa){
    if(n == 0) return;

    int ind = 0;
    lewa[ind] = t[0];

    for(int i = 1; i < n; i++){
        if(lewa[ind].typ == t[i].typ) continue;     // Jesli element o tym samym typie t[i].typ 
                                // jest juz w tablicy to nie moge dodac drugi raz tego samego typu
        if(ind == 1 && lewa[ind - 1].typ != t[i].typ){  // sprawdzam czy sa juz dwa elementy
            lewa[++ind] = t[i];
            break;
        }
        else if(ind == 0)
            lewa[++ind] = t[i];
    }    
}

void znajdz_prawa(int n, motel* t, motel* prawa){
    if(n == 0) return;

    int ind = 0;
    prawa[ind] = t[n - 1];

    for(int i = n - 2; i >= 0; i--){
        if(prawa[ind].typ == t[i].typ) continue;

        if(ind == 1 && prawa[ind - 1].typ != t[i].typ){
            prawa[++ind] = t[i];
            break;
        }
        else if(ind == 0)
            prawa[++ind] = t[i];
    }
}


int najdalsze(int n, motel* t, motel* lewa, motel* prawa){
    int res = 0;
    for(int i = 1; i < n; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 3; k++){
                if(lewa[j].typ == prawa[k].typ) continue;
                if(t[i].typ != lewa[j].typ && t[i].typ != prawa[k].typ){ // lm i pm nie musza byc dodatnie (chyba) 
                    int lm = t[i].odl - lewa[j].odl;                            // potem to trzeba udowodnic
                    int pm = prawa[k].odl - t[i].odl;
                    res = maxx(res, minx(pm, lm));
                }
            }
        }
    }
    return res;
}

int najblizsze(int n, motel* t){
    para *l = (para*)malloc((unsigned)n * sizeof(para));
    para *p = (para*)malloc((unsigned)n * sizeof(para));

    l[0].bli = l[0].dal = n;
    p[n - 1].bli = p[n - 1].dal = n;

    for(int i = 1; i < n; i++){
        if(t[i].typ == t[i - 1].typ)
            l[i] = l[i - 1];
        else{
            l[i].dal = l[i - 1].bli;
            l[i].bli = i - 1;
        }
    } 
  
    for(int i = n - 2; i >= 0; i--){
        if(t[i].typ == t[i + 1].typ)
            p[i] = p[i + 1];
        else{
            p[i].dal = p[i + 1].bli;
            p[i].bli = i + 1;
        }
    }  

    int res = INF;

    for(int i = 1; i < n - 1; i++){
        res = minx(res, sprawdz(n, t, l[i].bli, i, p[i].bli));
        res = minx(res, sprawdz(n, t, l[i].dal, i, p[i].dal));
        res = minx(res, sprawdz(n, t, l[i].bli, i, p[i].dal));
        res = minx(res, sprawdz(n, t, l[i].dal, i, p[i].bli));
    }
    free(p);
    free(l);
    return res;
}

int main(){
    int n, res1, res2;
    motel *l = (motel*)calloc(3, sizeof(motel));  // czy da sie zrobic tablice globlana i przekazywac referencja???
    motel *p = (motel*)calloc(3, sizeof(motel));  // upewnic sie ze typ moteli nie moze byc 0
    if(!scanf("%d", &n)) return 1;
    motel *t = (motel*)malloc((unsigned)n * sizeof(motel));
    for(int i = 0; i < n; i++){
        if(!scanf("%d %d", &t[i].typ, &t[i].odl)) return 1;
    }
    znajdz_lewa(n, t, l);
    znajdz_prawa(n, t, p);
    if(p[2].typ == 0)
        res1 = res2 = 0;
    else{
        res1 = najdalsze(n, t, l, p);
        res2 = najblizsze(n, t);
    }
    free(t);
    printf("%d %d\n", res2, res1);
    return 0;
}
