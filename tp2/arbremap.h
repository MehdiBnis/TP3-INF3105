/* Structure ArbreMap<K, V>.
 * Reprise du laboratoire 8
 * Mehdi Bennis - BENM80120206
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 */

#if !defined(ARBREMAP_H_)
#define ARBREMAP_H_
#include "arbreavl.h"

template <class K, class V>
class ArbreMap {

  class Entree {
    public:
      Entree(const K& c):cle(c),valeur() {}
      K cle;
      V valeur;
      bool operator < (const Entree& e) const { return cle < e.cle; }
      bool operator > (const Entree& e) const { return cle > e.cle; }
  };
  ArbreAVL<Entree> entrees;

  public:
    bool contient(const K&) const;

    void enlever(const K&);
    void vider();
    bool vide() const;

    const V& operator[] (const K&) const;
    V& operator[] (const K&);

    class Iterateur {
      public:
        Iterateur(ArbreMap& a) : iter(a.entrees.debut()) {}
        Iterateur(typename ArbreAVL<Entree>::Iterateur it) : iter(it) {}
        operator bool() const { return iter.operator bool(); }
        Iterateur&  operator++() { ++iter; return *this; }
        const K& cle() const { return (*iter).cle; }
        V& valeur() { return (V&) (*iter).valeur; }

        private:
          typename ArbreAVL<Entree>::Iterateur iter;
    };

    class IterateurConst {
      public:
        IterateurConst(ArbreMap& a) : iter(a.entrees.debut()) {}
        IterateurConst(typename ArbreAVL<Entree>::Iterateur it) : iter(it) {}
        operator bool() const { return iter.operator bool(); }
        IterateurConst&  operator++() { ++iter; return *this; }
        const K& cle() const { return (*iter).cle; }
        V& valeur() { return (V&) (*iter).valeur; }

        private:
          typename ArbreAVL<Entree>::Iterateur iter;
    };

    Iterateur debut() { return Iterateur(entrees.debut()); }
    IterateurConst debut() const { return IterateurConst(entrees.debut()); }
    Iterateur fin() { return Iterateur(entrees.fin()); }
    IterateurConst fin() const { return IterateurConst(entrees.fin()); }
    Iterateur rechercher(const K& c) { return Iterateur(entrees.rechercher(c)); }
    IterateurConst rechercher(const K& c) const { return IterateurConst(entrees.rechercher(c)); }
    Iterateur rechercherEgalOuSuivant(const K& c) { return Iterateur(entrees.rechercherEgalOuSuivant(c)); }
    IterateurConst rechercherEgalOuSuivant(const K& c) const { return IterateurConst(entrees.rechercherEgalOuSuivant(c)); }
    Iterateur rechercherEgalOuPrecedent(const K& c) { return Iterateur(entrees.rechercherEgalOuPrecedent(c)); }
    IterateurConst rechercherEgalOuPrecedent(const K& c) const { return IterateurConst(entrees.rechercherEgalOuPrecedent(c)); }
};

template <class K, class V>
void ArbreMap<K,V>::vider(){
    return entrees.vider();
}

template <class K, class V>
bool ArbreMap<K,V>::vide() const{
    return entrees.vide();
}

template <class K, class V>
void ArbreMap<K,V>::enlever(const K& c)
{
  Entree entree(c);
  return entrees.enlever(entree);
}

template <class K, class V>
bool ArbreMap<K,V>::contient(const K& c) const
{
  Entree entree(c);
  return entrees.contient(entree);
}

template <class K, class V>
const V& ArbreMap<K,V>::operator[] (const K& c)  const
{
  typename ArbreAVL<Entree>::Iterateur iter=entrees.rechercher(c);
  return entrees[iter].valeur;
}

template <class K, class V>
V& ArbreMap<K,V>::operator[] (const K& c) 
{
  typename ArbreAVL<Entree>::Iterateur iter=entrees.rechercher(Entree(c));
  if(!iter) {
    entrees.inserer(Entree(c));
    iter = entrees.rechercher(c);
  }
  return entrees[iter].valeur;
}



#endif
