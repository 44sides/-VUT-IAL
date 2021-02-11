
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                              Úpravy: Kamil Jeřábek, září 2020
**                               Implementace: Vladyslav Tverdokhlib, říjen 2020
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    
L->Act = NULL; // ukazuje na zadnou adresu
L->First = NULL; // ukazuje na zadnou adresu
L->Last = NULL; // ukazuje na zadnou adresu
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	
L->Act = NULL;
L->Last = NULL;

while(L->First != NULL) // opakovani, dokud se nezrusi vsechny prvky
{
    tDLElemPtr tmp = L->First->rptr; // zachovani nasledujiciho
    free(L->First); // uvolneni
    L->First = tmp; // zmena na nasledujici
}
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
if(tmp != NULL){
    tmp->data = val; // vlozime data
    tmp->rptr = L->First; // pravy prvek vlozeneho bude First
    tmp->lptr = NULL; // levy prvek nebude zadny
    L->First = tmp; // vlozeni
    if(L->First->rptr != NULL)
        L->First->rptr->lptr = L->First;
    else // kdyz ve fronte se projevil jeden prvek
        L->Last = L->First;
}
else DLError();
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	
tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
if(tmp != NULL){
    tmp->data = val; // vlozeni data
    tmp->lptr = L->Last; // levy - Last
    tmp->rptr = NULL; // pravy - nic
    L->Last = tmp;
    if(L->Last->lptr != NULL)
        L->Last->lptr->rptr = L->Last;
    else // kdyz ve fronte se projevil jeden prvek
        L->First = L->Last;
    
}
else DLError();
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
L->Act = L->First; // nastaveni aktivity na prvin prvek
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
L->Act= L->Last; // nastaveni na Last
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

if(L->First != NULL){
    *val = L->First->data; // vraceni pomoci pointeru
}
else DLError();
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	
if(L->First != NULL){
    *val = L->Last->data; // vraceni pomoci pointeru
}
else DLError();
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	
if(L->First != NULL){ // kontrola podminky
    if(L->Act == L->First) L->Act = NULL;

    tDLElemPtr tmp = L->First->rptr; // zachovani nasledujiciho
    tmp->lptr = NULL;

    if(L->First == L->Last) // kdyz se projevil jden prvek ve fronte
        L->Last = NULL;
        
    free(L->First);
    L->First = tmp;
}
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L.
** Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/ 
	
if(L->First != NULL){
    if(L->Act == L->Last) L->Act = NULL;

    tDLElemPtr tmp = L->Last->lptr; // zachovani nasledujiciho
    tmp->rptr = NULL;

    if(L->First == L->Last) // kdyz se projevil jden prvek ve fronte
        L->First = NULL;

    free(L->Last);
    L->Last = tmp;
}
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	
if(L->Act != NULL && L->Act != L->Last){
    tDLElemPtr tmp = L->Act->rptr; // ukazatel na prvek ktery chceme zrusit
    L->Act->rptr = tmp->rptr;

    if(tmp->rptr != NULL) // kdyz je ruseny prvek posledni
        tmp->rptr->lptr = L->Act; // nastaveni leveho prvku, protoze se objevil novy prvek
    else
        L->Last = L->Act;
    
    free(tmp);
}
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	
if(L->Act != NULL && L->Act != L->First){
    tDLElemPtr tmp = L->Act->lptr; // ukazatel na prvek ktery chceme zrusit
    L->Act->lptr = tmp->lptr;

    if(tmp->lptr != NULL)// kdyz je ruseny prvek prvni
        tmp->lptr->rptr = L->Act; // nastaveni praveho prvku, protoze se objevil novy prvek
    else
        L->First = L->Act;

    free(tmp);
}
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
if(L->Act != NULL){
    tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
    if(tmp != NULL){
        tmp->data = val; // vlozeni data
        tmp->rptr = L->Act->rptr; // do praveho vlozime pravy aktivniho
        tmp->lptr = L->Act; // do leveho vlozime ukazatel na aktivni
        L->Act->rptr = tmp;
    if(L->Act == L->Last) // kdyz je aktivni posledni prvek, tak se vlozi za neho
        L->Last = tmp;
    else
        tmp->rptr->lptr = tmp;
    }
    else DLError();
}
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
if(L->Act != NULL){
    tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
    if(tmp != NULL){
        tmp->data = val; // vlozeni data
        tmp->rptr = L->Act; // do praveho vlozime ukazatel na aktivni
        tmp->lptr = L->Act->lptr; // do leveho vlozime levy aktivniho
        L->Act->lptr = tmp;
    if(L->Act == L->First)// kdyz je aktivni prvni prvek, tak se vlozi za neho
        L->First = tmp;
    else
        tmp->lptr->rptr = tmp;
    }
    else DLError();
}
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
		
if(L->Act != NULL){
    *val = L->Act->data;
}
else DLError();
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	
if(L->Act != NULL){
    L->Act->data = val;
}
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	
if(L->Act != NULL){
    L->Act = L->Act->rptr;
}
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	
if(L->Act != NULL){
    L->Act = L->Act->lptr;
}
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	
return (L->Act != NULL ? 1 : 0); // Ternary operator
}

/* Konec c206.c*/
