// -----------------------------------------------------------------------
/// \brief Calcule un terme de contrainte a partir d'une homographie.
///
/// \param H: matrice 3*3 définissant l'homographie.
/// \param i: premiere colonne.
/// \param j: deuxieme colonne.
/// \return vecteur definissant le terme de contrainte.
// -----------------------------------------------------------------------
function v = ZhangConstraintTerm(H, i, j)
  v1 = H(i,1)*H(j,1);
  v2 = H(i,1)*H(j,2)+H(i,2)*H(j,1);
  v3 = H(i,2)*H(j,2);
  v4 = H(i,3)*H(j,1)+H(i,1)*H(j,3);
  v5 = H(i,3)*H(j,2)+H(i,2)*H(j,3);
  v6 = H(i,3)*H(j,3);
  v = [v1,v2,v3,v4,v5,v6]';
  //v = rand(1, 6);
endfunction

// -----------------------------------------------------------------------
/// \brief Calcule deux equations de contrainte a partir d'une homographie
///
/// \param H: matrice 3*3 définissant l'homographie.
/// \return matrice 2*6 definissant les deux contraintes.
// -----------------------------------------------------------------------
function v = ZhangConstraints(H)
  v = [ZhangConstraintTerm(H, 1, 2); ...
    ZhangConstraintTerm(H, 1, 1) - ZhangConstraintTerm(H, 2, 2)];
endfunction

// -----------------------------------------------------------------------
/// \brief Calcule la matrice des parametres intrinseques.
///
/// \param b: vecteur resultant de l'optimisation de Zhang.
/// \return matrice 3*3 des parametres intrinseques.
// -----------------------------------------------------------------------
function A = IntrinsicMatrix(b)
  v0 = (b(1,2)*b(1,3)-b(1,1)*b(2,3))/(b(1,1)*b(2,2)-b(1,2)^2);
  lambda = b(3,3)-(b(1,3)^2+v0((b(1,2)*b(1,3))-(b(1,1)*b(2,3))))/b(1,1);
  alpha = sqrt(lambda/b(1,1));
  beta = sqrt(lambda*b(1,1)/(b(1,1)*b(2,2)-b(1,2)^2));
  gamma = -b(1,2)*alpha^2*beta/lambda;
  u0 = gamma*v0/beta-b(1,3)*alpha^2/lambda;
  A=[alpha,gamma,u0;0,beta,v0;0,0,1];
  //A = rand(3, 3);
endfunction

// -----------------------------------------------------------------------
/// \brief Calcule la matrice des parametres extrinseques.
///
/// \param iA: inverse de la matrice intrinseque.
/// \param H: matrice 3*3 definissant l'homographie.
/// \return matrice 3*4 des parametres extrinseques.
// -----------------------------------------------------------------------
function E = ExtrinsicMatrix(iA, H)
  // A modifier!
  E = rand(3, 4);
endfunction

