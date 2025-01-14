/** @file abstracttoilets.h
 *  @brief Abstract class for reader-writers in the form of toilets sharing
 *
 *  This class is the super class of all other toilets sharing classes.
 *  It offers the 4 standard methods.
 *
 *  @author Yann Thoma
 *  @date 25.01.2022
 *  @bug No known bug
 */

#ifndef ABSTRACTTOILETS_H
#define ABSTRACTTOILETS_H

class AbstractToilet {
public:
    AbstractToilet(int nbSeats) : nbSeats(nbSeats) {};
    virtual void manAccessing() = 0;
    virtual void manLeaving() = 0;
    virtual void womanAccessing() = 0;
    virtual void womanLeaving() = 0;
protected:
    int nbSeats;
};

#endif // ABSTRACTTOILETS_H
