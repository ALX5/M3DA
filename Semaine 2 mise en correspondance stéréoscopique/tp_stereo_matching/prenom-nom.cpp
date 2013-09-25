/* --------------------------------------------------------------------------
Mise en correspondance de points d'interet detectes dans deux images
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
using namespace cv;
#include "glue.hpp"
#include "prenom-nom.hpp"

#include <iostream>

#include "QDebug";

// -----------------------------------------------------------------------
/// \brief Detecte les coins.
///
/// @param mImage: pointeur vers la structure image openCV
/// @param iMaxCorners: nombre maximum de coins detectes
/// @return matrice des coins
// -----------------------------------------------------------------------
Mat iviDetectCorners(const Mat& mImage,
                     int iMaxCorners) {
    vector<Point2d> corners;
    cv::goodFeaturesToTrack(mImage,corners,iMaxCorners,0.01,10,Mat(),3,false,0.04);
    double tx = mImage.cols, ty = mImage.rows;
    Mat mCorners = Mat_<double>(3,corners.size());
    for(int i=0; i<corners.size(); i++) {
        mCorners.at<double>(0,i)=corners.at(i).x;
        mCorners.at<double>(1,i)=corners.at(i).y;
        mCorners.at<double>(2,i)=1;
    }
    // Retour de la matrice
    return mCorners;
}

// -----------------------------------------------------------------------
/// \brief Initialise une matrice de produit vectoriel.
///
/// @param v: vecteur colonne (3 coordonnees)
/// @return matrice de produit vectoriel
// -----------------------------------------------------------------------
Mat iviVectorProductMatrix(const Mat& v) {
    Mat mVectorProduct = Mat::eye(3, 3, CV_64F);
    mVectorProduct.at<double>(0,0)= 0;
    mVectorProduct.at<double>(0,1)= -v.at<double>(0,2);
    mVectorProduct.at<double>(0,2)= v.at<double>(0,1);
    mVectorProduct.at<double>(1,0)= v.at<double>(0,2);
    mVectorProduct.at<double>(1,1)= 0;
    mVectorProduct.at<double>(1,2)= -v.at<double>(0,0);
    mVectorProduct.at<double>(2,0)= -v.at<double>(0,1);
    mVectorProduct.at<double>(2,1)= v.at<double>(0,0);
    mVectorProduct.at<double>(2,2)= 0;
    // Retour de la matrice
    return mVectorProduct;
}

// -----------------------------------------------------------------------
/// \brief Initialise et calcule la matrice fondamentale.
///
/// @param mLeftIntrinsic: matrice intrinseque de la camera gauche
/// @param mLeftExtrinsic: matrice extrinseque de la camera gauche
/// @param mRightIntrinsic: matrice intrinseque de la camera droite
/// @param mRightExtrinsic: matrice extrinseque de la camera droite
/// @return matrice fondamentale
// -----------------------------------------------------------------------
Mat iviFundamentalMatrix(const Mat& mLeftIntrinsic,
                         const Mat& mLeftExtrinsic,
                         const Mat& mRightIntrinsic,
                         const Mat& mRightExtrinsic) {
    // Doit utiliser la fonction iviVectorProductMatrix
    Mat mFundamental = Mat::eye(3, 3, CV_64F);

    Mat eye3x4 = Mat::eye(3, 4, CV_64F);

    Mat o = Mat::eye(4, 1, CV_64F);
    o.at<double>(0,0)=0;
    o.at<double>(1,0)=0;
    o.at<double>(2,0)=0;
    o.at<double>(3,0)=1;

    Mat O1 = mLeftExtrinsic.inv()*o;

    Mat P1 = mLeftIntrinsic*eye3x4*mLeftExtrinsic;
    Mat P2 = mRightIntrinsic*eye3x4*mRightExtrinsic;

    mFundamental = iviVectorProductMatrix(P2*O1)*P2*P1.inv(DECOMP_SVD);
    // Retour de la matrice fondamentale
    return mFundamental;
}

// -----------------------------------------------------------------------
/// \brief Initialise et calcule la matrice des distances entres les
/// points de paires candidates a la correspondance.
///
/// @param mLeftCorners: liste des points 2D image gauche
/// @param mRightCorners: liste des points 2D image droite
/// @param mFundamental: matrice fondamentale
/// @return matrice des distances entre points des paires
// -----------------------------------------------------------------------
Mat iviDistancesMatrix(const Mat& m2DLeftCorners,
                       const Mat& m2DRightCorners,
                       const Mat& mFundamental) {
    double distance1 = 0.0; double distance2 = 0.0;
    Mat mDistances = Mat(m2DLeftCorners.cols,m2DRightCorners.cols,CV_64F);
    for(int i=0; i<m2DLeftCorners.cols; i++) {
        Mat m1 = m2DLeftCorners.col(i);
        Mat d2 = mFundamental*m1;
        for(int j=0; j<m2DRightCorners.cols; j++) {
            Mat m2 = m2DRightCorners.col(j);
            Mat d1 = mFundamental.t()*m2;
            distance1 = std::abs(m1.at<double>(0)*d1.at<double>(0)+m1.at<double>(1)*d1.at<double>(1)+d1.at<double>(2))
                    /std::sqrt(std::pow(d1.at<double>(0),2)+std::pow(d1.at<double>(1),2));
            distance2 = std::abs(m2.at<double>(0)*d2.at<double>(0)+m2.at<double>(1)*d2.at<double>(1)+d2.at<double>(2))
                     /std::sqrt(std::pow(d2.at<double>(0),2)+std::pow(d2.at<double>(1),2));
            mDistances.at<double>(i,j)=distance1+distance2;
        }
    }
    //std::cout << mDistances << std::endl;
    // Retour de la matrice fondamentale
    return mDistances;
}

// -----------------------------------------------------------------------
/// \brief Initialise et calcule les indices des points homologues.
///
/// @param mDistances: matrice des distances
/// @param fMaxDistance: distance maximale autorisant une association
/// @param mRightHomologous: liste des correspondants des points gauche
/// @param mLeftHomologous: liste des correspondants des points droite
/// @return rien
// -----------------------------------------------------------------------
void iviMarkAssociations(const Mat& mDistances,
                         double dMaxDistance,
                         Mat& mRightHomologous,
                         Mat& mLeftHomologous) {
    // A modifier !
}
