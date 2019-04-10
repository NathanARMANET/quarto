#include <utility>

#include <iostream>
#include "Pion.h"

Pion::Pion() {
    this->couleur = false;
    this->forme = false;
    this->taille = false;
    this->trou = false;
    this->lien_image = "";
}

Pion::Pion(bool taille, bool forme, bool couleur, bool trou, std::string str) {
    this->taille = taille;
    this->forme = forme;
    this->couleur = couleur;
    this->trou = trou;
    this->lien_image = std::move(str);
}

/**
 *
 * @param pion Prend en paramètre un pion
 * @return vrai si le pion courant (this) est égal au pion comapré (en paramètre)
 */
bool Pion::equals(Pion * pion)
{
    if(this != nullptr && pion != nullptr)
    {
        // bool a = this->couleur == pion->couleur;
        // bool b = this->forme == pion->forme;
        // bool c = this->taille == pion->taille;
        // bool d = this->trou == pion->trou;

        return( (this->couleur == pion->couleur)
                || (this->forme == pion->forme)
                || (this->taille == pion->taille)
                || (this->trou == pion->trou) );
    }
    return false;
}

bool Pion::operator==(Pion *pion) {
    if(this != nullptr && pion != nullptr)
    {
        // bool a = this->couleur == pion->couleur;
        // bool b = this->forme == pion->forme;
        // bool c = this->taille == pion->taille;
        // bool d = this->trou == pion->trou;

        return( (this->couleur == pion->couleur)
                && (this->forme == pion->forme)
                && (this->taille == pion->taille)
                && (this->trou == pion->trou) );
    }
    return false;
}

void Pion::read() {
    std::cout<<couleur<<" | "<<forme<<" | "<<taille<<" | "<<trou<<std::endl;
    std::cout<<lien_image<<std::endl;
}

const std::string &Pion::getLien_image() const {
    return lien_image;
}

std::string Pion::toString()
{
   std::string res;
   if(this->couleur)
   {
       res += "Bleu, ";
   }
   else
   {
       res += "Rouge, ";
   }

   if(this->forme)
   {
       res += "Carré, ";
   }
   else
   {
       res += "Rond, ";
   }

   if(this->taille)
   {
       res += "Grand, ";
   }
   else
   {
       res += "Petit, ";
   }

   if(this->trou)
   {
       res += "Troué.";
   }
   else
   {
       res += "Non troué.";
   }
   return res;
}

bool Pion::getForme() const
{
    if(this != nullptr)
    {
        return this->forme;
    }
}

bool Pion::getTaille() const
{
    if(this != nullptr)
    {
        return this->taille;
    }
}

bool Pion::getCouleur() const
{
    if(this != nullptr)
    {
        return this->couleur;
    }
}

bool Pion::getTrou() const
{
    if(this != nullptr)
    {
        return this->trou;
    }
}

