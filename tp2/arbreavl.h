/* Structure d'arbre AVL
 * Reprise des laboratoires 6 et 7
 * Mehdi Bennis - BENM80120206
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 */

#if !defined(ARBREAVL_H_)
#define ARBREAVL_H_
#include <cassert>
#include "pile.h"

template <class T>
T max(const T& a, const T& b) {
  return a > b ? a : b;
}

template <class T>
T min(const T& a, const T& b) {
  return a < b ? a : b;
}

template <class T>
class ArbreAVL {
  public:
    ArbreAVL();
    ~ArbreAVL();

    bool vide() const;
    bool contient(const T&) const;
    void inserer(const T&);
    void vider();

    void enlever(const T&);

    ArbreAVL& operator = (const ArbreAVL&);
    ArbreAVL(const ArbreAVL&);

    int taille() const;
    int hauteur() const;

    class Iterateur;
    
    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const T&) const;
    Iterateur rechercherEgalOuSuivant(const T&) const;
    Iterateur rechercherEgalOuPrecedent(const T&) const;

    const T& operator[](const Iterateur&) const;
    T& operator[](const Iterateur&);

  private:
    struct Noeud{
        Noeud(const T&);
        T contenu;
        Noeud *gauche,
              *droite;
        int hauteur;
    };
    Noeud* racine;

    void inserer(Noeud*&, const T&);
    void enlever(Noeud*&, const T&);
    void rotationGauche(Noeud*&);
    void rotationDroite(Noeud*&);
    void rotationGaucheDroite(Noeud*&);
    void rotationDroiteGauche(Noeud*&);
    void equilibrer(Noeud*&);
    void vider(Noeud*&);

    void copier(const Noeud*, Noeud*&) const;

    int hauteur(const Noeud*) const;
    int equilibre(const Noeud*) const;
    int compter(const Noeud*) const;

  public:
    class Iterateur{
      public:
        Iterateur(const ArbreAVL& a);
        Iterateur(const Iterateur& a);
        Iterateur(const ArbreAVL& a, Noeud* c);

        operator bool() const;
        bool operator!() const;
        bool operator==(const Iterateur&) const;
        bool operator!=(const Iterateur&) const;
        
        const T& operator*() const;

        Iterateur& operator++();
        Iterateur operator++(int);
        Iterateur& operator = (const Iterateur&);
      private:
        const ArbreAVL& arbre_associe;
        Noeud* courant;
        Pile<Noeud*> chemin;
        
      friend class ArbreAVL;
    };
};

template <class T>
ArbreAVL<T>::Noeud::Noeud(const T& c)
 : contenu(c), gauche(nullptr), droite(nullptr), hauteur(1)
{
}

template <class T>
ArbreAVL<T>::ArbreAVL() 
 : racine(nullptr)
{
}

template <class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL<T>& autre)
 : racine(nullptr)
{
    copier(autre.racine, racine);
}

template <class T>
ArbreAVL<T>::~ArbreAVL()
{
    vider(racine);
}

template <class T>
bool ArbreAVL<T>::contient(const T& element) const
{
  Noeud *courant = racine;
  while (courant != nullptr) {
    if (element < courant->contenu) {
      courant = courant->gauche;
    } else if (element > courant->contenu) {
      courant = courant->droite;
    } else {
      return true;
    }
  }
  return false;
}

template <class T>
void ArbreAVL<T>::inserer(const T& element)
{
    inserer(racine, element);
}

template <class T>
void ArbreAVL<T>::inserer(Noeud*& noeud, const T& element)
{
  if (!noeud) {
    noeud = new Noeud(element);
    return;
  }
  if (element < noeud->contenu) {
    inserer(noeud->gauche, element);
  } else if (element > noeud->contenu) {
    inserer(noeud->droite, element);
  }
  equilibrer(noeud);
}

template <class T>
void ArbreAVL<T>::enlever(const T& element)
{
    enlever(racine, element);
}

template <class T>
void ArbreAVL<T>::enlever(Noeud*& noeud, const T& element)
{
  if (!noeud) return;

  if (element < noeud->contenu) {
    enlever(noeud->gauche, element);
  } else if (element > noeud->contenu) {
    enlever(noeud->droite, element);
  } else {
    if (!noeud->gauche && !noeud->droite) {
      delete noeud;
      noeud = nullptr;
    } else if (!noeud->gauche) {
      Noeud* temp = noeud;
      noeud = noeud->droite;
      delete temp;
    } else if (!noeud->droite) {
      Noeud* temp = noeud;
      noeud = noeud->gauche;
      delete temp;
    } else {
      Noeud* suivant = noeud->droite;
      while (suivant->gauche) {
        suivant = suivant->gauche;
      }
      noeud->contenu = suivant->contenu;
      enlever(noeud->droite, suivant->contenu);
    }
  }
  if (noeud) {
    equilibrer(noeud);
  }
}

template <class T>
void ArbreAVL<T>::rotationGauche(Noeud*& noeud)
{
  Noeud* temp = noeud->droite;
  noeud->droite = temp->gauche;
  temp->gauche = noeud;

  noeud->hauteur = 1 + max(hauteur(noeud->gauche), hauteur(noeud->droite));
  temp->hauteur = 1 + max(hauteur(temp->gauche), hauteur(temp->droite));

  noeud = temp;
}

template <class T>
void ArbreAVL<T>::rotationDroite(Noeud*& noeud)
{
  Noeud* temp = noeud->gauche;
  noeud->gauche = temp->droite;
  temp->droite = noeud;

  noeud->hauteur = 1 + max(hauteur(noeud->gauche), hauteur(noeud->droite));
  temp->hauteur = 1 + max(hauteur(temp->gauche), hauteur(temp->droite));

  noeud = temp;
}

template <class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud*& noeud)
{
  rotationGauche(noeud->gauche);
  rotationDroite(noeud);
}

template <class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud*& noeud)
{
  rotationDroite(noeud->droite);
  rotationGauche(noeud);
}

template <class T>
void ArbreAVL<T>::equilibrer(Noeud*& noeud)
{
  if (!noeud) return;

  if (equilibre(noeud) > 1) {
    if (equilibre(noeud->gauche) < 0) {
      rotationGaucheDroite(noeud);
    } else {
      rotationDroite(noeud);
    }
  } else if (equilibre(noeud) < -1) {
    if (equilibre(noeud->droite) > 0) {
      rotationDroiteGauche(noeud);
    } else {
      rotationGauche(noeud);
    }
  }
  noeud->hauteur = 1 + max(hauteur(noeud->gauche), hauteur(noeud->droite));
}

template <class T>
bool ArbreAVL<T>::vide() const { return racine == nullptr; }

template <class T>
void ArbreAVL<T>::vider(){
  vider(racine);
}

template <class T>
void ArbreAVL<T>::vider(Noeud*& noeud)
{
    if (!noeud) return;
    vider(noeud->gauche);
    vider(noeud->droite);
    delete noeud;
    noeud = nullptr;
}

template <class T>
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) const
{
  if (source) {
    noeud = new Noeud(source->contenu);
    copier(source->gauche, noeud->gauche);
    copier(source->droite, noeud->droite);
    noeud->hauteur = source->hauteur;
  }
}

template <class T>
int  ArbreAVL<T>::hauteur() const{ return hauteur(racine); }

template <class T>
int  ArbreAVL<T>::taille() const{
    return compter(racine);
}

template <class T>
int  ArbreAVL<T>::hauteur(const Noeud* n) const{ return n ? n->hauteur : 0; }

template <class T>
int  ArbreAVL<T>::equilibre(const Noeud* n) const{ return hauteur(n->gauche) - hauteur(n->droite); }

template <class T>
int ArbreAVL<T>::compter(const Noeud* n) const{
    if(n==nullptr) return 0;
    return 1 + compter(n->gauche) + compter(n->droite);
}

template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
    if(this==&autre) return *this;
    vider();
    copier(autre.racine, racine);
    return *this;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const
{
    Iterateur iter(*this);
    iter.courant = racine;
    if(iter.courant != nullptr)
      while(iter.courant->gauche != nullptr) {
        iter.chemin.empiler(iter.courant);
        iter.courant = iter.courant->gauche;
      }
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const
{
  return Iterateur(*this);
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher(const T& e) const
{
  Noeud* courant = racine;
  Iterateur iter(*this);
  while (courant) {
    if (e < courant->contenu) {
      iter.chemin.empiler(courant);
      courant = courant->gauche;
    } else if (e > courant->contenu) {
      courant = courant->droite;
    } else {
      iter.courant = courant;
      return iter;
    }
  }

  iter.courant = nullptr;
  return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant(const T& e) const
{
  Noeud *n = racine, *dernier = nullptr;
  while(n) {
    if (e < n->contenu) {
      dernier = n;
      n = n->gauche;
    } else if (n->contenu < e) {
      n = n->droite;
    } else {
      return rechercher(e);
    }
  }
  if(dernier) return rechercher(dernier->contenu);
  return Iterateur(*this);
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent(const T& e) const
{
  Noeud *n = racine, *dernier = nullptr;
  while(n) {
    if (e < n->contenu) {
      n = n->gauche;
    } else if (n->contenu < e) {
      dernier = n;
      n = n->droite;
    } else {
      return rechercher(e);
    }
  }
  if(dernier) return rechercher(dernier->contenu);
  return Iterateur(*this);
}

template <class T>
const T& ArbreAVL<T>::operator[](const Iterateur& iterateur) const
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
T& ArbreAVL<T>::operator[](const Iterateur& iterateur)
{
    assert(&iterateur.arbre_associe == this);

    if (!iterateur.courant) {
      std::cerr << "Erreur : Itérateur invalide dans operator[]" << std::endl;
    }
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL& a)
 : arbre_associe(a), courant(nullptr)
{
}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur& a)
: arbre_associe(a.arbre_associe)
{
    courant = a.courant;
    chemin = a.chemin;
}

template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator++()
{
  assert(courant);
  Noeud *suivant = courant->droite;
  while (suivant) {
    chemin.empiler(suivant);
    suivant = suivant->gauche;
  }
  if(!chemin.vide())
    courant = chemin.depiler();
  else
    courant = nullptr;
  return *this;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int)
{
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class T>
ArbreAVL<T>::Iterateur::operator bool() const
{
    return courant!=nullptr;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!() const{
    return courant==nullptr;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant==o.courant;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant!=o.courant;
}

template <class T>
const T& ArbreAVL<T>::Iterateur::operator*() const
{
    assert(courant!=nullptr);
    return courant->contenu;
}

template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator = (const Iterateur& autre){
    assert(&arbre_associe==&autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}

#endif
