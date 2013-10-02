/* --------------------------------------------------------------------------
Stereovision dense par calcul de correlation
Copyright (C) 2010, 2011  Universite Lille 1

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------
Inclure les fichiers d'entete
-------------------------------------------------------------------------- */
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
using namespace cv;
#include "prenom-nom.hpp"

#include <iostream>

// -----------------------------------------------------------------------
/// \brief Estime la disparite par minimisation du SSD, image gauchee
/// prise comme reference.
///
/// @param psLeftImage: image gauche
/// @param psRightImage: image droite
/// @param psLeftDisparity: carte des disparites calculee
/// @param iMaxDisparity: disparite maximale recherchee
/// @param iWindowHalfSize: demi-taille de la fenetre de correlation
/// @return rien
// -----------------------------------------------------------------------
Mat iviLeftDisparityMap(const Mat& mLeftGray,
                        const Mat& mRightGray,
                        int iMaxDisparity,
                        int iWindowHalfSize) {
// Images pour les resultats intermediaires
Mat mSSD(mLeftGray.size(), CV_64F);
Mat mMinSSD(mLeftGray.size(), CV_64F);
Mat mLeftDisparityMap(mLeftGray.size(), CV_8U);
double dMinSSD, *pdPtr1, *pdPtr2;
unsigned char *pucDisparity;
int iShift, iRow, iCol;

    // Initialisation de l'image du minimum de SSD
    dMinSSD = pow((double)(2 * iWindowHalfSize + 1), 2.0) * 512.0;
    for (iRow = iWindowHalfSize;
        iRow < mMinSSD.size().height - iWindowHalfSize;
        iRow++) {
        // Pointeur sur le debut de la ligne
        pdPtr1 = mMinSSD.ptr<double>(iRow);
        // Sauter la demi fenetre non utilisee
        pdPtr1 += iWindowHalfSize;
        // Remplir le reste de la ligne
        for (iCol = iWindowHalfSize;
            iCol < mMinSSD.size().width - iWindowHalfSize;
            iCol++)
                *pdPtr1++ = dMinSSD;
    }
    // Boucler pour tous les decalages possibles
    for (iShift = 0; iShift < iMaxDisparity; iShift++) {
        // Calculer le cout SSD pour ce decalage
        mSSD = iviComputeLeftSSDCost(mLeftGray, mRightGray,
                                     iShift, iWindowHalfSize);
        // Mettre a jour les valeurs minimales
        for (iRow = iWindowHalfSize;
            iRow < mMinSSD.size().height - iWindowHalfSize;
            iRow++) {
            // Pointeurs vers les debuts des lignes
            pdPtr1 = mMinSSD.ptr<double>(iRow);
            pdPtr2 = mSSD.ptr<double>(iRow);
            pucDisparity = mLeftDisparityMap.ptr<unsigned char>(iRow);
            // Sauter la demi fenetre non utilisee
            pdPtr1 += iWindowHalfSize;
            pdPtr2 += iWindowHalfSize;
            pucDisparity += iWindowHalfSize;
            // Comparer sur le reste de la ligne
            for (iCol = iWindowHalfSize;
                iCol < mMinSSD.size().width - iWindowHalfSize;
                iCol++) {
                // SSD plus faible que le minimum precedent
                if (*pdPtr1 > *pdPtr2) {
                    *pucDisparity = (unsigned char)iShift;
                    *pdPtr1 = *pdPtr2;
                }
                // Pixels suivants
                pdPtr1++; pdPtr2++; pucDisparity++;
            }
        }
    }
    return mLeftDisparityMap;
}

// -----------------------------------------------------------------------
/// \brief Calcule la somme des differences aux carre, image gauche
/// prise comme reference.
///
/// @param psLeftImage: image gauche
/// @param psRightImage: image droite
/// @param psSSD: somme des differences au carre (double 64bits)
/// @param iShift: decalage teste
/// @param iWindowHalfSize: demi-taille de la fenetre de correlation
/// @return rien
// -----------------------------------------------------------------------
Mat iviComputeLeftSSDCost(const Mat& mLeftGray,
                          const Mat& mRightGray,
                          int iShift,
                          int iWindowHalfSize) {
    Mat mLeftSSDCost(mLeftGray.size(), CV_64F);
    for(int i=iWindowHalfSize; i<mLeftGray.rows; i++) {
        for(int j=iWindowHalfSize; j<mLeftGray.cols; j++) {
            for(int wx=i-iWindowHalfSize; wx<i+iWindowHalfSize; wx++) {
                for(int wy=j-iWindowHalfSize; wy<j+iWindowHalfSize; wy++) {
                    mLeftSSDCost.at<double>(i,j)+=std::pow(mLeftGray.at<double>(i+wx,j+wy)-mRightGray.at<double>(i+wx-iShift,j+wy),2);
                }
            }
        }
    }
    return mLeftSSDCost;
}

// -----------------------------------------------------------------------indice
/// \brief Estime la disparite par minimisation du SSD, image droite
/// prise comme reference.
///
/// @param psLeftImage: image gauche
/// @param psRightImage: image droite
/// @param psRightDisparity: carte des disparites calculee
/// @param iMaxDisparity: disparite maximale recherchee
/// @param iWindowHalfSize: demi-taille de la fenetre de correlation
/// @return rien
// -----------------------------------------------------------------------
Mat iviRightDisparityMap(const Mat& mLeftGray,
                         const Mat& mRightGray,
                         int iMaxDisparity,
                         int iWindowHalfSize) {
    // Images pour les resultats intermediaires
    Mat mSSD(mRightGray.size(), CV_64F);
    Mat mMinSSD(mRightGray.size(), CV_64F);
    Mat mRightDisparityMap(mRightGray.size(), CV_8U);
    double dMinSSD, *pdPtr1, *pdPtr2;
    unsigned char *pucDisparity;
    int iShift, iRow, iCol;

        // Initialisation de l'image du minimum de SSD
        dMinSSD = pow((double)(2 * iWindowHalfSize + 1), 2.0) * 512.0;
        for (iRow = iWindowHalfSize;
            iRow < mMinSSD.size().height - iWindowHalfSize;
            iRow++) {
            // Pointeur sur le debut de la ligne
            pdPtr1 = mMinSSD.ptr<double>(iRow);
            // Sauter la demi fenetre non utilisee
            pdPtr1 += iWindowHalfSize;
            // Remplir le reste de la ligne
            for (iCol = iWindowHalfSize;
                iCol < mMinSSD.size().width - iWindowHalfSize;
                iCol++)
                    *pdPtr1++ = dMinSSD;
        }
        // Boucler pour tous les decalages possibles
        for (iShift = 0; iShift < iMaxDisparity; iShift++) {
            // Calculer le cout SSD pour ce decalage
            mSSD = iviComputeRightSSDCost(mLeftGray, mRightGray,
                                         iShift, iWindowHalfSize);
            // Mettre a jour les valeurs minimales
            for (iRow = iWindowHalfSize;
                iRow < mMinSSD.size().height - iWindowHalfSize;
                iRow++) {
                // Pointeurs vers les debuts des lignes
                pdPtr1 = mMinSSD.ptr<double>(iRow);
                pdPtr2 = mSSD.ptr<double>(iRow);
                pucDisparity = mRightDisparityMap.ptr<unsigned char>(iRow);
                // Sauter la demi fenetre non utilisee
                pdPtr1 += iWindowHalfSize;
                pdPtr2 += iWindowHalfSize;
                pucDisparity += iWindowHalfSize;
                // Comparer sur le reste de la ligne
                for (iCol = iWindowHalfSize;
                    iCol < mMinSSD.size().width - iWindowHalfSize;
                    iCol++) {
                    // SSD plus faible que le minimum precedent
                    if (*pdPtr1 > *pdPtr2) {
                        *pucDisparity = (unsigned char)iShift;
                        *pdPtr1 = *pdPtr2;
                    }
                    // Pixels suivants
                    pdPtr1++; pdPtr2++; pucDisparity++;
                }
            }
        }
    return mRightDisparityMap;
}

// -----------------------------------------------------------------------
/// \brief Calcule la somme des differences aux carre, image droite
/// prise comme reference.
///
/// @param psLeftImage: image gauche
/// @param psRightImage: image droite
/// @param psSSD: somme des differences au carre (double 64bits)
/// @param iShift: decalage teste
/// @param iWindowHalfSize: demi-taille de la fenetre de correlation
/// @return rien
// -----------------------------------------------------------------------
Mat iviComputeRightSSDCost(const Mat& mLeftGray,
                           const Mat& mRightGray,
                           int iShift,
                           int iWindowHalfSize) {
    Mat mRightSSDCost(mRightGray.size(), CV_64F);
    for(int i=iWindowHalfSize; i<mRightGray.rows; i++) {
        for(int j=iWindowHalfSize; j<mRightGray.cols; j++) {
            for(int wx=i-iWindowHalfSize; wx<i+iWindowHalfSize; wx++) {
                for(int wy=j-iWindowHalfSize; wy<j+iWindowHalfSize; wy++) {
                    mRightSSDCost.at<double>(i,j)+=std::pow(mRightGray.at<double>(i+wx,j+wy)-mLeftGray.at<double>(i+wx-iShift,j+wy),2);
                }
            }
        }
    }
    return mRightSSDCost;
}

// -----------------------------------------------------------------------
/// \brief Verifie la coherence des cartes gauche et froite.
///
/// @param psLeftDisparity: carte gauche des disparites
/// @param psRightDisparity: carte droite des disparites
/// @param psDisparity: carte des disparites fusionnee
/// @param psValidityMask: carte des disparites valides
/// @return rien
// -----------------------------------------------------------------------
Mat iviLeftRightConsistency(const Mat& mLeftDisparity,
                            const Mat& mRightDisparity,
                            Mat& mValidityMask) {
    Mat mDisparity(mLeftDisparity.size(), CV_8U);
    /*for(int i=0; i<mLeftDisparity.rows; i++) {
        for(int j=0; j<mLeftDisparity.cols; j++) {
            int xr = i-mLeftDisparity.at<double>(i,j);
            std::cout << "i :" << i << std::endl;
            std::cout << "j :" << j << std::endl;
            std::cout << "xr :" << xr << std::endl;
            if(mRightDisparity.at<double>(xr,j)==mLeftDisparity.at<double>(xr+mRightDisparity.at<double>(xr,j),j)) {
                std::cout << "ici" << std::endl;
                mValidityMask.at<double>(i,j)=0;
            }
            else {
                std::cout << "là" << std::endl;
                mValidityMask.at<double>(i,j)=255;
            }
        }
    }*/
    return mDisparity;
}
